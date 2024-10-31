#include <fstream>
#include "Ray.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Export {
    public:
        const std::vector<Sphere>& spheres;
        const std::vector<Plane>& planes;
        const std::vector<Triangle>& triangles;

        Export(const std::vector<Sphere>& spheres, const std::vector<Plane>& planes, const std::vector<Triangle>& triangles);
        ~Export();
        void exportMaterials(const std::string& mtlFilename);
        void exportToObj(const std::string& objFilename, const std::string& mtlFilename);
};
