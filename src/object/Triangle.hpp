#include "Plane.hpp"

class Triangle {
public:
    vec3 v0, v1, v2;
    Color color;

    Triangle(const vec3& v0, const vec3& v1, const vec3& v2, const Color& color);
    ~Triangle();
};