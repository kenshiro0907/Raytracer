#include "Export.hpp"

Export::Export(const std::vector<Sphere>& spheres, const std::vector<Plane>& planes, const std::vector<Triangle>& triangles)
            : spheres(spheres), planes(planes), triangles(triangles) {}

Export::~Export() {}

void Export::exportMaterials(const std::string& mtlFilename) {
    std::ofstream mtlFile(mtlFilename);
    if (!mtlFile.is_open()) {
        std::cerr << "Impossible d'ouvrir le fichier " << mtlFilename << " pour l'export.\n";
        return;
    }

    for (size_t i = 0; i < spheres.size(); ++i) {
        mtlFile << "newmtl material_sphere_" << i << "\n";
        mtlFile << "Kd " << spheres[i].color.R() << " " << spheres[i].color.G() << " " << spheres[i].color.B() << "\n"; // Couleur de chacune des sphères
        mtlFile << "Ka 0.1 0.1 0.1\n"; // Ambiante
        mtlFile << "Ks 0.5 0.5 0.5\n"; // Spéculaire
        mtlFile << "Ns 50.0\n\n";     // Brillance
    }

    for (size_t i = 0; i < planes.size(); ++i) {
        mtlFile << "newmtl material_plane_\n";
        mtlFile << "Kd " << planes[i].color.R() << " " << planes[i].color.G() << " " << planes[i].color.B() << "\n"; // Couleur de chacun des plans
        mtlFile << "Ka 0.1 0.1 0.1\n"; // Ambiante
        mtlFile << "Ks 0.3 0.3 0.3\n"; // Spéculaire
        mtlFile << "Ns 10.0\n\n";         // Brillance
    }

    for (size_t i = 0; i < triangles.size(); ++i) {
        mtlFile << "newmtl material_triangle_\n";
        mtlFile << "Kd " << triangles[i].color.R() << " " << triangles[i].color.G() << " " << triangles[i].color.B() << "\n"; // Couleur de chacun des triangles
        mtlFile << "Ka 0.1 0.1 0.1\n"; // Ambiante
        mtlFile << "Ks 0.5 0.5 0.5\n"; // Spéculaire
        mtlFile << "Ns 50.0\n\n";         // Brillance
    }

    mtlFile.close();
}

void Export::exportToObj(const std::string& objFilename, const std::string& mtlFilename) {
    std::ofstream objFile(objFilename);
    if (!objFile.is_open()) {
        std::cerr << "Impossible d'ouvrir le fichier " << objFilename << " pour l'export.\n";
        return;
    }

    // Lien vers le fichier .mtl
    objFile << "mtllib " << mtlFilename << "\n";
    int vertexOffset = 1;

    for (size_t i = 0; i < spheres.size(); ++i) {
        objFile << "usemtl material_sphere_" << i << "\n";

        const int latitudeBands = 20;
        const int longitudeBands = 20;
        std::vector<vec3> vertices;

        for (int latNumber = 0; latNumber <= latitudeBands; ++latNumber) {
            float theta = latNumber * M_PI / latitudeBands;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            for (int longNumber = 0; longNumber <= longitudeBands; ++longNumber) {
                float phi = longNumber * 2 * M_PI / longitudeBands;
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);

                vec3 vertex(
                    spheres[i].center.x + spheres[i].radius * cosPhi * sinTheta,
                    spheres[i].center.y + spheres[i].radius * cosTheta,
                    spheres[i].center.z + spheres[i].radius * sinPhi * sinTheta
                );
                vertices.push_back(vertex);
                objFile << "v " << vertex.x << " " << vertex.y << " " << vertex.z << "\n";
            }
        }

        for (int latNumber = 0; latNumber < latitudeBands; ++latNumber) {
            for (int longNumber = 0; longNumber < longitudeBands; ++longNumber) {
                int first = (latNumber * (longitudeBands + 1)) + longNumber + vertexOffset;
                int second = first + longitudeBands + 1;

                objFile << "f " << first << " " << second << " " << first + 1 << "\n";
                objFile << "f " << second << " " << second + 1 << " " << first + 1 << "\n";
            }
        }

        vertexOffset += vertices.size();
    }

    for (size_t i = 0; i < planes.size(); ++i) {
        objFile << "usemtl material_plane_\n";
        vec3 p1 = planes[i].position + vec3(-10, 0, -10);
        vec3 p2 = planes[i].position + vec3(10, 0, -10);
        vec3 p3 = planes[i].position + vec3(10, 0, 10);
        vec3 p4 = planes[i].position + vec3(-10, 0, 10);

        objFile << "v " << p1.x << " " << p1.y << " " << p1.z << "\n";
        objFile << "v " << p2.x << " " << p2.y << " " << p2.z << "\n";
        objFile << "v " << p3.x << " " << p3.y << " " << p3.z << "\n";
        objFile << "v " << p4.x << " " << p4.y << " " << p4.z << "\n";

        objFile << "f " << vertexOffset << " " << vertexOffset + 1 << " " << vertexOffset + 2 << "\n";
        objFile << "f " << vertexOffset << " " << vertexOffset + 2 << " " << vertexOffset + 3 << "\n";

        vertexOffset += 4;
    }

    for (size_t i = 0; i < triangles.size(); ++i) {
        objFile << "usemtl material_triangle_" << i << "\n";
        
        vec3 v0 = triangles[i].v0;
        vec3 v1 = triangles[i].v1;
        vec3 v2 = triangles[i].v2;

        objFile << "v " << v0.x << " " << v0.y << " " << v0.z << "\n";
        objFile << "v " << v1.x << " " << v1.y << " " << v1.z << "\n";
        objFile << "v " << v2.x << " " << v2.y << " " << v2.z << "\n";

        objFile << "f " << vertexOffset << " " << vertexOffset + 1 << " " << vertexOffset + 2 << "\n";
        
        vertexOffset += 3;
    }

    objFile.close();
    std::cout << "Exportation terminée! Fichier sauvegardé sous " << objFilename << ".\n";
}