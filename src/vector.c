#include <math.h>
#include "vector.h"

v3 v3_rotate_x(v3 v, float angle) {
    v3 rotation = {
        .x = v.x,
        .y = v.y * cos(angle) - v.z * sin(angle),
        .z = v.y * sin(angle) + v.z * cos(angle)
    };

    return rotation;
}

v3 v3_rotate_y(v3 v, float angle) {
    v3 rotation = {
        .x = v.x * cos(angle) - v.z * sin(angle),
        .y = v.y,
        .z = v.x * sin(angle) + v.z * cos(angle)
    };

    return rotation;
}

v3 v3_rotate_z(v3 v, float angle) {
    v3 rotation = {
        .x = v.x * cos(angle) - v.y * sin(angle),
        .y = v.x * sin(angle) + v.y * cos(angle),
        .z = v.z
    };

    return rotation;
}