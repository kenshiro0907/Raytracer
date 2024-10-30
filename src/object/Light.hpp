#include "Vec3.hpp"

class Light {
public:
    vec3 position;
    float intensity;

    Light(const vec3& pos, float intens) : position(pos), intensity(intens) {}
};