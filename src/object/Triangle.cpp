#include "Triangle.hpp"

Triangle::Triangle(const vec3& v0, const vec3& v1, const vec3& v2, const Color& color)
        : v0(v0), v1(v1), v2(v2), color(color) {}

Triangle::~Triangle() {}