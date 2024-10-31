#include "Export.hpp"

Export::Export(const std::vector<Sphere>& spheres, const Plane& plane)
            : spheres(spheres), plane(plane) {}

Export::~Export() {}

void Export::exportMaterials(const std::string& mtlFilename) {
    std::ofstream mtlFile(mtlFilename);
    if (!mtlFile.is_open()) {
        std::cerr << "Impossible d'ouvrir le fichier " << mtlFilename << " pour l'export.\n";
        return;
    }

    for (size_t i = 0; i < spheres.size(); ++i) {
        mtlFile << "newmtl material_sphere_" << i << "\n";
        mtlFile << "Kd " << spheres[i].color.R() << " " << spheres[i].color.G() << " " << spheres[i].color.B() << "\n"; // Couleur diffuse
        mtlFile << "Ka 0.2 0.2 0.2\n"; // Ambiante
        mtlFile << "Ks 0.5 0.5 0.5\n"; // Spéculaire
        mtlFile << "Ns 100.0\n\n";     // Brillance
    }

    mtlFile << "newmtl material_plane\n";
    mtlFile << "Kd 0.8 0.8 0.8\n"; // Couleur gris clair pour le plan
    mtlFile << "Ka 0.1 0.1 0.1\n"; // Ambiante
    mtlFile << "Ks 0.3 0.3 0.3\n"; // Spéculaire
    mtlFile << "Ns 50.0\n";         // Brillance
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

    // Exportation des sphères
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

    // Exportation du plan
    objFile << "usemtl material_plane\n";
    vec3 p1 = plane.position + vec3(-10, 0, -10);
    vec3 p2 = plane.position + vec3(10, 0, -10);
    vec3 p3 = plane.position + vec3(10, 0, 10);
    vec3 p4 = plane.position + vec3(-10, 0, 10);

    objFile << "v " << p1.x << " " << p1.y << " " << p1.z << "\n";
    objFile << "v " << p2.x << " " << p2.y << " " << p2.z << "\n";
    objFile << "v " << p3.x << " " << p3.y << " " << p3.z << "\n";
    objFile << "v " << p4.x << " " << p4.y << " " << p4.z << "\n";

    objFile << "f " << vertexOffset << " " << vertexOffset + 1 << " " << vertexOffset + 2 << "\n";
    objFile << "f " << vertexOffset << " " << vertexOffset + 2 << " " << vertexOffset + 3 << "\n";

    objFile.close();
    std::cout << "Exportation terminée! Fichier sauvegardé sous " << objFilename << ".\n";
}