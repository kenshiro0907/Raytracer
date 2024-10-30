#include "Sphere.hpp"

Sphere::~Sphere() {}

Sphere::Sphere(float radius, const vec3& center, const Color& col) : radius(radius), center(center), color(col) {
}