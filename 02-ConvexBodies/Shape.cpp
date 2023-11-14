#include "Shape.h"
#include "code/Math/Matrix.h"

Mat3 ShapeSphere::InertiaTensor() const
{
	Mat3 tensor;
	tensor.Zero();
	tensor.rows[0][0] = 2.0f * radius * radius / 5.0f;
	tensor.rows[1][1] = 2.0f * radius * radius / 5.0f;
	tensor.rows[2][2] = 2.0f * radius * radius / 5.0f;
	return tensor;
}

Bounds ShapeSphere::GetBounds(const Vec3& pos, const Quat& orient) const
{
	Bounds tmp;
	tmp.mins = Vec3(-radius) + pos;
	tmp.maxs = Vec3(radius) + pos;
	return tmp;
}

Bounds ShapeSphere::GetBounds() const
{
	Bounds tmp;
	tmp.mins = Vec3(-radius);
	tmp.maxs = Vec3(radius);
	return tmp;
}

Vec3 ShapeSphere::Support(const Vec3& dir, const Vec3& pos, const Quat& orient, const float bias)
{
	return pos + dir * (radius + bias);
}

Mat3 ShapeBox::InertiaTensor() const
{
	// Inertia tensor for box centered around zero
	const float dx = bounds.maxs.x - bounds.mins.x;
	const float dy = bounds.maxs.y - bounds.mins.y;
	const float dz = bounds.maxs.z - bounds.mins.z;

	Mat3 tensor;
	tensor.Zero();
	tensor.rows[0][0] = (dy * dy + dz * dz) / 12.0f;
	tensor.rows[1][1] = (dx * dx + dz * dz) / 12.0f;
	tensor.rows[2][2] = (dx * dx + dy * dy) / 12.0f;

	// Now we use the parallel axis theorem to get the inertia
	// tensor for a box that is not centered around the origin

	Vec3 cm;
	cm.x = (bounds.maxs.x + bounds.mins.x) * 0.5f;
	cm.y = (bounds.maxs.y + bounds.mins.y) * 0.5f;
	cm.z = (bounds.maxs.z + bounds.mins.z) * 0.5f;

	// Displacement of the center of mass from origin
	const Vec3 R = Vec3{ 0, 0, 0 } - cm;
	const float R2 = R.GetLengthSqr();

	Mat3 patTensor;
	patTensor.rows[0] = Vec3{ R2 - R.x * R.x, R.x * R.y, R.x * R.z };
	patTensor.rows[1] = Vec3{ R.y * R.x, R2 - R.y * R.y, R.y * R.z };
	patTensor.rows[2] = Vec3{ R.z * R.x, R.z * R.y, R2 - R.z * R.z };

	// Now we need to add the center of mass tensor and the parallel axis
	// theorem tensor together:
	tensor += patTensor;
	return tensor;
}

Bounds ShapeBox::GetBounds(const Vec3& pos, const Quat& orient) const
{
	Vec3 corners[8];
	corners[0] = Vec3{ bounds.mins.x, bounds.mins.y, bounds.mins.z };
	corners[1] = Vec3{ bounds.mins.x, bounds.mins.y, bounds.maxs.z };
	corners[2] = Vec3{ bounds.mins.x, bounds.maxs.y, bounds.mins.z };
	corners[3] = Vec3{ bounds.maxs.x, bounds.mins.y, bounds.mins.z };

	corners[4] = Vec3{ bounds.maxs.x, bounds.maxs.y, bounds.maxs.z };
	corners[5] = Vec3{ bounds.maxs.x, bounds.maxs.y, bounds.mins.z };
	corners[6] = Vec3{ bounds.maxs.x, bounds.mins.y, bounds.maxs.z };
	corners[7] = Vec3{ bounds.mins.x, bounds.maxs.y, bounds.maxs.z };

	Bounds expandedBounds;
	for (int i = 0; i < 8; ++i) {
		corners[i] = orient.RotatePoint(corners[i]) + pos;
		expandedBounds.Expand(corners[i]);
	}
	return expandedBounds;
}

Bounds ShapeBox::GetBounds() const
{
	return bounds;
}

void ShapeBox::Build(const Vec3* pts, const int num)
{
	for (int i = 0; i < num; ++i)
	{
		bounds.Expand(points[i]);
	}

	points.clear();
	points.push_back(Vec3{ bounds.mins.x, bounds.mins.y, bounds.mins.z });
	points.push_back(Vec3{ bounds.maxs.x, bounds.mins.y, bounds.mins.z });
	points.push_back(Vec3{ bounds.mins.x, bounds.maxs.y, bounds.mins.z });
	points.push_back(Vec3{ bounds.mins.x, bounds.mins.y, bounds.maxs.z });

	points.push_back(Vec3{ bounds.maxs.x, bounds.maxs.y, bounds.maxs.z });
	points.push_back(Vec3{ bounds.mins.x, bounds.maxs.y, bounds.maxs.z });
	points.push_back(Vec3{ bounds.maxs.x, bounds.mins.y, bounds.maxs.z });
	points.push_back(Vec3{ bounds.maxs.x, bounds.maxs.y, bounds.mins.z });

	centerOfMass = (bounds.maxs + bounds.mins) * 0.5f;
}

Vec3 ShapeBox::Support(const Vec3& dir, const Vec3& pos, const Quat& orient, const float bias)
{
	// Find the point in furthest direction
	Vec3 maxPt = orient.RotatePoint(points[0]) + pos;
	float maxDist = dir.Dot(maxPt);
	for (int i = 1; i < points.size(); i++) {
		const Vec3 pt = orient.RotatePoint(points[i]) + pos;
		const float dist = dir.Dot(pt);

		if (dist > maxDist) {
			maxDist = dist;
			maxPt = pt;
		}
	}

	Vec3 norm = dir;
	norm.Normalize();
	norm *= bias;
	return maxPt + norm;
}

float ShapeBox::FastestLinearSpeed(const Vec3& angularVelocity, const Vec3& dir) const
{
	float maxSpeed{ 0 };
	for (int i = 1; i < points.size(); i++) {
		Vec3 r = points[i] - centerOfMass;
		Vec3 linearVelocity = angularVelocity.Cross(r);
		float speed = dir.Dot(linearVelocity);
		if (speed > maxSpeed) {
			maxSpeed = speed;
		}
	}
	return maxSpeed;
}

