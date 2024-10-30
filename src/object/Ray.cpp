#include "Ray.hpp"

Ray::Ray(const vec3& origin, const vec3& direction)
            : origin(origin), direction(direction.normalize()) {}

Ray::~Ray() {}

std::optional<vec3> Ray::intersectSphere(const Sphere& sphere) const {
        vec3 oc = origin - sphere.center;
        float a = direction.dot(direction);
        float b = 2.0f * oc.dot(direction);
        float c = oc.dot(oc) - sphere.radius * sphere.radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0) {
            return std::nullopt;
        } else {
            float t = (-b - std::sqrt(discriminant)) / (2.0f * a);
            return origin + direction * t;
        }
}

bool Ray::isShadowed(const vec3& point, const Light& light, const std::vector<Sphere>& spheres) const {
    vec3 lightDir = (light.position - point).normalize();
    Ray shadowRay(point, lightDir);
    
    for (const Sphere& sphere : spheres) {
        if (shadowRay.intersectSphere(sphere)) {
            return true; // Un objet bloque la lumière
        }
    }
    return false; // Pas d'objet entre le point et la lumière
}

std::optional<vec3> Ray::reflect(const vec3& normal, const vec3& incoming) const {
    return incoming - normal * 2.0f * normal.dot(incoming);
}