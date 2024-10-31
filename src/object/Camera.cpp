#include "Camera.hpp"

Camera::Camera(const vec3& pos, const vec3& target, const vec3& upDir, float fieldOfView, float aspect)
        : position(pos), fov(fieldOfView), aspectRatio(aspect) {
    direction = (target - pos).normalize();
    up = upDir.normalize();
}

Camera::~Camera() {}

Ray Camera::getRay(float u, float v) const {
    float viewportHeight = 2.0f * tan(fov / 2.0f);
    float viewportWidth = aspectRatio * viewportHeight;

    vec3 horizontal = direction.cross(up).normalize() * viewportWidth;
    vec3 vertical = up * viewportHeight;

    vec3 lowerLeftCorner = position + direction - horizontal / 2.0f - vertical / 2.0f;

    vec3 rayDirection = (lowerLeftCorner + horizontal * u + vertical * v - position).normalize();
    return Ray(position, rayDirection);
}