#ifndef VECTOR_H
#define VECTOR_H

#include "typedefs.h"

typedef struct {
    f32 x;
    f32 y;
} v2;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} v3;

v3 v3_rotate_x(v3 v, f32 angle);
v3 v3_rotate_y(v3 v, f32 angle);
v3 v3_rotate_z(v3 v, f32 angle);

#endif