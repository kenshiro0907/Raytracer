#include "Export.hpp"

class Raytracer {
    public:
        Ray& r;
        const std::vector<Sphere>& spheres;
        const std::vector<Plane>& planes;
        const std::vector<Light>& lights;
        const std::vector<Triangle>& triangles;

        Raytracer(Ray& r, const std::vector<Sphere>& spheres, const std::vector<Plane>& planes, const std::vector<Light>& lights, const std::vector<Triangle>& triangles);
        ~Raytracer();
        Color ray_color(int depth);
};