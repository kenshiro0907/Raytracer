#include "Export.hpp"

class Raytracer {
    public:
        Ray& r;
        const std::vector<Sphere>& spheres;
        const std::vector<Plane>& planes;
        const std::vector<Light>& lights;

        Raytracer(Ray& r, const std::vector<Sphere>& spheres, const std::vector<Plane>& planes, const std::vector<Light>& lights);
        ~Raytracer();
        Color ray_color(int depth);
};