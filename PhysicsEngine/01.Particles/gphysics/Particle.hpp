//
// Created by Gaëtan Blaise-Cazalet on 14/01/2025.
//

#ifndef GPHYSICS_PARTICLE_HPP
#define GPHYSICS_PARTICLE_HPP

#include "Vec3.hpp"
#include "Precision.hpp"

using gmath::Vec3;

namespace gphysics
{
    class Particle
    {
    protected:
        Vec3 position{0, 0, 0};
        Vec3 velocity{0, 0, 0};
        Vec3 acceleration{0, 0, 0};

        /**
         * Holds the amount of damping applied to linear
         * motion. Damping is required to remove energy added
         * through numerical instability in the integrator.
         */
        real damping{1.0f};

        /**
         * Holds the inverse of the mass of the particle. It
         * is more useful to hold the inverse mass because
         * integration is simpler, and because in real-time
         * simulation it is more useful to have objects with
         * infinite mass (immovable) than zero mass
         * (completely unstable in numerical simulation).
         */
        real inverseMass{0};

        /**
         * Holds the accumulated force to be applied at the next
         * simulation iteration only. This value is zeroed at each
         * integration step.
         */
        Vec3 forceAccum{0, 0, 0};

        /**
         * Clears the forces applied to the particle. This will be
         * called automatically after each integration step.
         */
        void ClearAccumulator();

        /**
         * Integrates the particle forward in time by the given amount.
         * This function uses a Newton-Euler integration method, which is a
         * linear approximation to the correct integral. For this reason it
         * may be inaccurate in some cases.
         */
        void Integrate(real duration);


    public:
        [[nodiscard]] Vec3 GetPosition() const { return position; }
        void SetPosition(const Vec3& position_) { position = position_; }
        [[nodiscard]] Vec3 GetVelocity() const { return velocity; }
        void SetVelocity(const Vec3& velocity_) { velocity = velocity_; }
        [[nodiscard]] Vec3 GetAcceleration() const { return acceleration; }
        void SetAcceleration(const Vec3& acceleration_) { acceleration = acceleration_; }
        [[nodiscard]] real GetDamping() const { return damping; }
        void SetDamping(real damping_) { damping = damping_; }
        [[nodiscard]] real GetInverseMass() const { return inverseMass; }
        void SetInverseMass(real inverseMass_) { inverseMass = inverseMass_; }

        [[nodiscard]] bool HasFiniteMass() const { return inverseMass >= 0.0f; }
    };
} // gphysics

#endif //GPHYSICS_PARTICLE_HPP
