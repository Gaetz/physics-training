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

typedef struct FragMultiplyUniform
{
    float r, g, b, a;
} fragMultiplyUniform;


#endif //GDRAW_VERTEXTYPES_HPP
