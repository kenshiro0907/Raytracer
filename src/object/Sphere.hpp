#pragma once

#include <iostream>
#include <vector>
#include "Vec3.hpp"

class Sphere
{
    private:
        float radius;
        vec3 center; 
    public:
        Sphere(float radius, const vec3& center);
        ~Sphere();  
};