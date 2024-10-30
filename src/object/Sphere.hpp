#pragma once

#include <iostream>
#include <vector>
#include "../raymath/Color.hpp"
#include "Light.hpp"

class Sphere
{
    public:
        float radius;
        vec3 center;
        Color color;

        Sphere(float radius, const vec3& center, const Color& col);
        ~Sphere();
};