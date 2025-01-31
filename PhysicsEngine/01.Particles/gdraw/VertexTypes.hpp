//
// Created by Gaëtan Blaise-Cazalet on 15/01/2025.
//

#ifndef GDRAW_VERTEXTYPES_HPP
#define GDRAW_VERTEXTYPES_HPP

struct PositionTextureVertex
{
    float x, y, z;
    float u, v;
};

struct PositionNormalVertex
{
    float x, y, z;
    float nx, ny, nz;
};

typedef struct FragMultiplyUniform
{
    float r, g, b, a;
} fragMultiplyUniform;


#endif //GDRAW_VERTEXTYPES_HPP
