#include <cmath>

struct vec3 {
    float x, y, z;

    vec3() : x(0.0f), y(0.0f), z(0.0f) {}

    vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    vec3 operator+(const vec3& other) const {
        return vec3(x + other.x, y + other.y, z + other.z);
    }

    vec3 operator-(const vec3& other) const {
        return vec3(x - other.x, y - other.y, z - other.z);
    }

    vec3 operator*(float scalar) const {
        return vec3(x * scalar, y * scalar, z * scalar);
    }

    float dot(const vec3& other) const {
        return (x * other.x) + (y * other.y) + (z * other.z);
    }

    vec3 normalize() const {
        float mag = std::sqrt(x * x + y * y + z * z);
        return mag == 0 ? vec3(0, 0, 0) : vec3(x / mag, y / mag, z / mag);
    }
};