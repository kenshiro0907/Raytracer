#include "Plane.hpp"
#include <optional>

class Ray {
    public:
        vec3 origin;
        vec3 direction;

        Ray(const vec3& origin, const vec3& direction);
        ~Ray();

        std::optional<vec3> intersectSphere(const Sphere& sphere) const;
        bool isShadowed(const vec3& point, const Light& light, const std::vector<Sphere>& spheres) const;
        std::optional<vec3> reflect(const vec3& normal, const vec3& incoming) const;
        std::optional<vec3> intersectPlane(const Plane& plane) const;
};