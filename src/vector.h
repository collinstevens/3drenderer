#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float x;
    float y;
} v2;

typedef struct {
    float x;
    float y;
    float z;
} v3;

v3 v3_rotate_x(v3 v, float angle);
v3 v3_rotate_y(v3 v, float angle);
v3 v3_rotate_z(v3 v, float angle);

#endif