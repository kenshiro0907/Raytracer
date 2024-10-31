#include "Raytracer.hpp"

Raytracer::Raytracer(Ray& r, const std::vector<Sphere>& spheres, const std::vector<Plane>& planes, const std::vector<Light>& lights) 
            : r(r), spheres(spheres), planes(planes), lights(lights) {}

Raytracer::~Raytracer() {}

Color Raytracer::ray_color(int depth) {
    if (depth <= 0) {
        return Color(0.0f, 0.0f, 0.0f);
    }

    std::optional<vec3> closestHitPoint = std::nullopt;
    const Sphere* hitSphere = nullptr;
    const Plane* hitPlane = nullptr;
    vec3 normalAtHitPoint;
    vec3 hitPoint;
    Color hitColor;

    for (const auto& sphere : spheres) {
        std::optional<vec3> hit = r.intersectSphere(sphere);
        if (hit && (!closestHitPoint || (closestHitPoint && (hit->z < closestHitPoint->z)))) {
            closestHitPoint = hit;
            hitSphere = &sphere;
            hitPlane = nullptr;
            hitPoint = *hit;
            normalAtHitPoint = (hitPoint - sphere.center).normalize();
            hitColor = sphere.color;
        }
    }

    for (const auto& plane : planes) {
        std::optional<vec3> hit = r.intersectPlane(plane);
        if (hit && (!closestHitPoint || (closestHitPoint && (hit->z < closestHitPoint->z)))) {
            closestHitPoint = hit;
            hitSphere = nullptr;
            hitPlane = &plane;
            hitPoint = *hit;
            normalAtHitPoint = plane.normal;
            hitColor = plane.color;
        }
    }

    if (!hitSphere && !hitPlane) {
        return Color(0.5f, 0.7f, 1.0f);
    }

    Color pixelColor = Color(0.0f, 0.0f, 0.0f);

    for (const auto& light : lights) {
        vec3 lightDir = (light.position - hitPoint).normalize();
        float lightDistance = (light.position - hitPoint).length();
        float diffuseStrength = std::max(0.0f, normalAtHitPoint.dot(lightDir));

        Ray shadowRay(hitPoint + normalAtHitPoint * 0.001f, lightDir);
        bool inShadow = false;

        for (const auto& sphere : spheres) {
            if (&sphere != hitSphere && shadowRay.intersectSphere(sphere) && (shadowRay.intersectSphere(sphere)->length() < lightDistance)) {
                inShadow = true;
                break;
            }
        }

        for (const auto& plane : planes) {
            if (&plane != hitPlane && shadowRay.intersectPlane(plane) && (shadowRay.intersectPlane(plane)->length() < lightDistance)) {
                inShadow = true;
                break;
            }
        }

        if (!inShadow) {
            pixelColor = pixelColor + (hitColor * diffuseStrength * light.intensity);
        }
    }

    vec3 reflectionDir = r.direction - normalAtHitPoint * 2.0f * r.direction.dot(normalAtHitPoint);
    Ray reflectionRay(hitPoint + normalAtHitPoint * 0.001f, reflectionDir);
    r = reflectionRay;
    Color reflectedColor = ray_color(depth - 1);
    pixelColor = pixelColor + reflectedColor * 0.5f;

    return pixelColor;
}