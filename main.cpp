#include <iostream>
#include <fstream>
#include "Image.hpp"
#include "src/object/Ray.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

void exportMaterials(const std::vector<Sphere>& spheres, const Plane& plane, const std::string& mtlFilename) {
    std::ofstream mtlFile(mtlFilename);
    if (!mtlFile.is_open()) {
        std::cerr << "Impossible d'ouvrir le fichier " << mtlFilename << " pour l'export.\n";
        return;
    }

    // Matériaux pour chaque sphère
    for (size_t i = 0; i < spheres.size(); ++i) {
        mtlFile << "newmtl material_sphere_" << i << "\n";
        mtlFile << "Kd " << spheres[i].color.R() << " " << spheres[i].color.G() << " " << spheres[i].color.B() << "\n"; // Couleur diffuse
        mtlFile << "Ka 0.2 0.2 0.2\n"; // Ambiante
        mtlFile << "Ks 0.5 0.5 0.5\n"; // Spéculaire
        mtlFile << "Ns 100.0\n\n";     // Brillance
    }

    // Matériau pour le plan
    mtlFile << "newmtl material_plane\n";
    mtlFile << "Kd 0.8 0.8 0.8\n"; // Couleur gris clair pour le plan
    mtlFile << "Ka 0.1 0.1 0.1\n"; // Ambiante
    mtlFile << "Ks 0.3 0.3 0.3\n"; // Spéculaire
    mtlFile << "Ns 50.0\n";         // Brillance
    mtlFile.close();
}

// Fonction pour exporter la scène en .obj
void exportToObj(const std::vector<Sphere>& spheres, const Plane& plane, const std::string& objFilename, const std::string& mtlFilename) {
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
        objFile << "usemtl material_sphere_" << i << "\n"; // Utilisation du matériau

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
    objFile << "usemtl material_plane\n"; // Utilisation du matériau du plan
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

Color ray_color(const Ray& r, const std::vector<Sphere>& spheres, const std::vector<Plane>& planes, const std::vector<Light>& lights, int depth) {
    if (depth <= 0) {
        return Color(0.0f, 0.0f, 0.0f); // Retourne noir si la profondeur maximale est atteinte
    }

    std::optional<vec3> closestHitPoint = std::nullopt;
    const Sphere* hitSphere = nullptr;
    const Plane* hitPlane = nullptr;
    vec3 normalAtHitPoint;
    vec3 hitPoint;
    Color hitColor;

    // Vérifie les intersections avec les sphères
    for (const auto& sphere : spheres) {
        std::optional<vec3> hit = r.intersectSphere(sphere);
        if (hit && (!closestHitPoint || (closestHitPoint && (hit->z < closestHitPoint->z)))) {
            closestHitPoint = hit;
            hitSphere = &sphere;
            hitPlane = nullptr; // Pas de plan touché
            hitPoint = *hit;
            normalAtHitPoint = (hitPoint - sphere.center).normalize();
            hitColor = sphere.color;
        }
    }

    // Vérifie les intersections avec les plans
    for (const auto& plane : planes) {
        std::optional<vec3> hit = r.intersectPlane(plane);
        if (hit && (!closestHitPoint || (closestHitPoint && (hit->z < closestHitPoint->z)))) {
            closestHitPoint = hit;
            hitSphere = nullptr; // Pas de sphère touchée
            hitPlane = &plane;
            hitPoint = *hit;
            normalAtHitPoint = plane.normal;
            hitColor = plane.color;
        }
    }

    // Pas d'intersection, on retourne la couleur de fond
    if (!hitSphere && !hitPlane) {
        return Color(0.5f, 0.7f, 1.0f); // Couleur du fond
    }

    Color pixelColor = Color(0.0f, 0.0f, 0.0f); // Couleur initiale

    // Calcul de l'ombrage
    for (const auto& light : lights) {
        vec3 lightDir = (light.position - hitPoint).normalize();
        float lightDistance = (light.position - hitPoint).length();
        float diffuseStrength = std::max(0.0f, normalAtHitPoint.dot(lightDir));

        // Vérifie l'ombre
        Ray shadowRay(hitPoint + normalAtHitPoint * 0.001f, lightDir);
        bool inShadow = false;

        // Vérifie les ombres pour les sphères
        for (const auto& sphere : spheres) {
            if (&sphere != hitSphere && shadowRay.intersectSphere(sphere) && (shadowRay.intersectSphere(sphere)->length() < lightDistance)) {
                inShadow = true;
                break;
            }
        }

        // Vérifie les ombres pour les plans
        for (const auto& plane : planes) {
            if (&plane != hitPlane && shadowRay.intersectPlane(plane) && (shadowRay.intersectPlane(plane)->length() < lightDistance)) {
                inShadow = true;
                break;
            }
        }

        if (!inShadow) {
            pixelColor = pixelColor + (hitColor * diffuseStrength * light.intensity);
        }
    }

    // Gestion des réflexions
    vec3 reflectionDir = r.direction - normalAtHitPoint * 2.0f * r.direction.dot(normalAtHitPoint);
    Ray reflectionRay(hitPoint + normalAtHitPoint * 0.001f, reflectionDir);
    Color reflectedColor = ray_color(reflectionRay, spheres, planes, lights, depth - 1);
    pixelColor = pixelColor + reflectedColor * 0.5f; // Contribue à la couleur finale

    return pixelColor;
}

int main() {
    const int width = 800;
    const int height = 600;
    Image image(width, height, Color(0.0f, 0.0f, 0.0f));

    // Création des sphères
    std::vector<Sphere> spheres = {
        Sphere(1.0f, vec3(0.0f, 1.5f, -4.0f), Color(1.0f, 0.0f, 0.0f)), // Sphère rouge, légèrement en bas
        Sphere(1.0f, vec3(2.0f, 1.0f, -4.0f), Color(0.0f, 1.0f, 0.0f)), // Sphère verte, légèrement en haut
        Sphere(1.0f, vec3(-2.0f, 1.0f, -4.0f), Color(0.0f, 0.0f, 1.0f)) // Sphère bleue, centrée
    };

    // Création des plans
    std::vector<Plane> planes = {
        Plane(vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.50f, 0.0f), Color(0.8f, 0.8f, 0.8f)), // Sol gris, sous les sphères
    };

    // Création des lumières
    std::vector<Light> lights = {
        //Light(vec3(5.0f, 5.0f, 0.0f), 1.0f), // Lumière supérieure droite
        Light(vec3(-5.0f, 5.0f, 4.0f), 1.0f), // Lumière supérieure gauche
        //Light(vec3(-5.0f, -5.0f, 0.0f), 2.0f) // Lumière inférieure gauche
    };

    const int maxDepth = 5; // Limite de profondeur pour les réflexions

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float u = (2.0f * x) / width - 1.0f;
            float v = 1.0f - (2.0f * y) / height;
            Ray ray(vec3(0, 0, 0), vec3(u, v, -1.0f).normalize());
            Color pixelColor = ray_color(ray, spheres, planes, lights, maxDepth);
            image.SetPixel(x, y, pixelColor);
        }
    }

    image.WriteFile("output.png");
    exportMaterials(spheres, planes.front(), "scene.mtl");
    exportToObj(spheres, planes.front(), "scene.obj", "scene.mtl");
    std::cout << "Raytracing terminé! Image sauvegardée sous 'output.png'.\n";

    return 0;
}

