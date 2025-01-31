cbuffer UBO : register(b0, space1)
{
    float4x4 transform : packoffset(c0);
    float4x4 view : packoffset(c4);
    float4x4 projection : packoffset(c8);
};

struct Input
{
    float3 Position : TEXCOORD0;
    float4 Color : TEXCOORD1;
    float4 Normal : TEXCOORD2;
};

struct Output
{
    float4 Color : TEXCOORD0;
    float4 Position : SV_Position;
    float4 Normal : TEXCOORD1;
};

Output main(Input input)
{
    Output output;
    output.Color = input.Color;
    float4 worldPosition = mul(transform, float4(input.Position, 1.0f));
    float4 viewPosition = mul(view, worldPosition);
    output.Position = mul(projection, viewPosition);
    output.Normal = input.Normal;
    return output;
}
