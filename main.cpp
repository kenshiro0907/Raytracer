#include <iostream>
#include "Image.hpp"
#include "src/object/Raytracer.hpp"

using namespace std;

int main() {
    const int width = 800;
    const int height = 600;
    Image image(width, height, Color(0.0f, 0.0f, 0.0f));

    std::vector<Sphere> spheres = {
        Sphere(1.0f, vec3(0.0f, 1.5f, -4.0f), Color(1.0f, 0.0f, 0.0f)), // Sphère rouge, légèrement en bas
        Sphere(1.0f, vec3(2.0f, 1.0f, -4.0f), Color(0.0f, 1.0f, 0.0f)), // Sphère verte, légèrement en haut
        Sphere(1.0f, vec3(-2.0f, 1.0f, -4.0f), Color(0.0f, 0.0f, 1.0f)) // Sphère bleue, centrée
    };

    std::vector<Plane> planes = {
        Plane(vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.50f, 0.0f), Color(0.8f, 0.8f, 0.8f)), // Sol gris, sous les sphères
    };

    std::vector<Triangle> triangles = {
        Triangle(vec3(-1.0f, 0.0f, -1.5f), vec3(1.0f, 0.0f, -1.5f), vec3(0.0f, 1.0f, -1.5f), Color(1.0f, 1.0f, 0.0f))
    };

    std::vector<Light> lights = {
        //Light(vec3(5.0f, 5.0f, 0.0f), 1.0f), // Lumière supérieure droite
        Light(vec3(-5.0f, 5.0f, 4.0f), 1.0f), // Lumière supérieure gauche
        //Light(vec3(-5.0f, -5.0f, 0.0f), 2.0f) // Lumière inférieure gauche
    };

    Export to_export(spheres, planes, triangles);

    const int maxDepth = 5;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float u = (2.0f * x) / width - 1.0f;
            float v = 1.0f - (2.0f * y) / height;
            Ray ray(vec3(0, 0, 0), vec3(u, v, -1.0f).normalize());
            Raytracer raytracer(ray, spheres, planes, lights, triangles);
            Color pixelColor = raytracer.ray_color(maxDepth);
            image.SetPixel(x, y, pixelColor);
        }
    }

    image.WriteFile("output.png");
    to_export.exportMaterials("scene.mtl");
    to_export.exportToObj("scene.obj", "scene.mtl");
    std::cout << "Raytracing terminé! Image sauvegardée sous 'output.png'.\n";

    return 0;
}