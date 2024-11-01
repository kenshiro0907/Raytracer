#pragma once

#include "Sphere.hpp"
#include "../raymath/Color.hpp"

class Plane {
public:
    vec3 position;
    vec3 normal;
    Color color;

    Plane(const vec3& pos, const vec3& norm, const Color& col) 
        : position(pos), normal(norm.normalize()), color(col) {}
};
