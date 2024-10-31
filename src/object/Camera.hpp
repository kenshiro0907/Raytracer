#include "Raytracer.hpp"

class Camera {
public:
    vec3 position;
    vec3 direction;
    vec3 up;
    float fov;
    float aspectRatio;

    Camera(const vec3& pos, const vec3& target, const vec3& upDir, float fieldOfView, float aspect);

    ~Camera();

    Ray getRay(float u, float v) const;
};