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

std::optional<vec3> Ray::intersectPlane(const Plane& plane) const {
    float denom = direction.dot(plane.normal);
    if (fabs(denom) > 1e-6) {
        vec3 p0l0 = plane.position - origin;
        float t = p0l0.dot(plane.normal) / denom;
        if (t >= 0) {
            return origin + direction * t;
        }
    }
    return std::nullopt;
}

std::optional<vec3> Ray::intersectTriangle(const Triangle& triangle) const {
    const float EPSILON = 0.0000001f;
    vec3 edge1 = triangle.v1 - triangle.v0;
    vec3 edge2 = triangle.v2 - triangle.v0;
    vec3 h = direction.cross(edge2);
    float a = edge1.dot(h);

    if (a > -EPSILON && a < EPSILON) return std::nullopt; // Le rayon est parallèle au triangle

    float f = 1.0f / a;
    vec3 s = origin - triangle.v0;
    float u = f * s.dot(h);
    if (u < 0.0f || u > 1.0f) return std::nullopt;

    vec3 q = s.cross(edge1);
    float v = f * direction.dot(q);
    if (v < 0.0f || u + v > 1.0f) return std::nullopt;

    float t = f * edge2.dot(q);
    if (t > EPSILON) {
        return origin + direction * t; // Intersection
    } else {
        return std::nullopt;
    } 
}

bool Ray::isShadowed(const vec3& point, const Light& light, const std::vector<Sphere>& spheres) const {
    vec3 lightDir = (light.position - point).normalize();
    Ray shadowRay(point, lightDir);
    
    for (const Sphere& sphere : spheres) {
        if (shadowRay.intersectSphere(sphere)) {
            return true;
        }
    }
    return false;
}