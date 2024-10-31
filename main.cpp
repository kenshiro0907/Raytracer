#include <iostream>
#include <sstream>
#include <ctime>
#include "Image.hpp"
#include "src/object/Camera.hpp"

using namespace std;

int main() {
    clock_t before = clock();
    const int width = 1920;
    const int height = 1080;
    float aspectRatio = static_cast<float>(width) / height;
    Image image(width, height, Color(0.0f, 0.0f, 0.0f));

    vec3 cameraPosition(0.7f, 0.0f, 1.0f);
    vec3 cameraTarget(1.0f, 1.0f, -1.5f);
    vec3 cameraUp(0.0f, 3.5f, 0.0f);
    float fov = M_PI / 3.0f;
    
    Camera camera(cameraPosition, cameraTarget, cameraUp, fov, aspectRatio);


    std::vector<Triangle> triangles = {
        Triangle(vec3(-1.0f, 0.0f, -1.5f), vec3(1.0f, 0.0f, -1.5f), vec3(0.0f, 1.0f, -1.5f), Color(1.0f, 1.0f, 0.0f))
    };

    std::vector<Sphere> spheres = {
        Sphere(1.0f, vec3(0.0f, 1.5f, -4.0f), Color(1.0f, 0.0f, 0.0f)), // Sphère rouge, légèrement en bas
        Sphere(1.0f, vec3(2.0f, 1.0f, -4.0f), Color(0.0f, 1.0f, 0.0f)), // Sphère verte, légèrement en haut
        Sphere(1.0f, vec3(-2.0f, 1.0f, -4.0f), Color(0.0f, 0.0f, 1.0f)) // Sphère bleue, centrée
    };

    std::vector<Plane> planes = {
        Plane(vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.50f, 0.0f), Color(0.8f, 0.8f, 0.8f)), // Sol gris, sous les sphères
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

            Ray ray = camera.getRay(u, v);

            Raytracer raytracer(ray, spheres, planes, lights, triangles);

            Color pixelColor = raytracer.ray_color(maxDepth);

            image.SetPixel(x, y, pixelColor);
        }
    }

    image.WriteFile("output.png");
    to_export.exportMaterials("scene.mtl");
    to_export.exportToObj("scene.obj", "scene.mtl");
    std::cout << "Raytracing terminé! Image sauvegardée sous 'output.png'.\n";
    clock_t duration = clock() - before;
    std::cout << "Duration: " << (float)duration / CLOCKS_PER_SEC << " seconds" << std::endl;

    return 0;
}