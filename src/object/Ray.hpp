#include "Triangle.hpp"
#include <optional>

class Ray {
    public:
        vec3 origin;
        vec3 direction;

        Ray(const vec3& origin, const vec3& direction);
        ~Ray();

        std::optional<vec3> intersectSphere(const Sphere& sphere) const;
        std::optional<vec3> intersectPlane(const Plane& plane) const;
        std::optional<vec3> intersectTriangle(const Triangle& triangle) const;
};