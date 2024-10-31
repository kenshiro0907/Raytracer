#include <iostream>
#include <sstream>
#include <ctime>
#include "src/utility/json.hpp"
#include "Image.hpp"
#include "src/object/Camera.hpp"

using namespace std;
using json = nlohmann::json;

void loadSceneFromJson(const std::string& filename,
                       std::vector<Sphere>& spheres,
                       std::vector<Triangle>& triangles,
                       std::vector<Plane>& planes,
                       std::vector<Light>& lights) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir le fichier " << filename << "\n";
        return;
    }

    json sceneConfig;
    file >> sceneConfig;

    for (const auto& sphereData : sceneConfig["spheres"]) {
        float radius = sphereData["radius"];
        vec3 position(sphereData["position"][0], sphereData["position"][1], sphereData["position"][2]);
        Color color(sphereData["color"][0], sphereData["color"][1], sphereData["color"][2]);
        spheres.emplace_back(radius, position, color);
    }

    for (const auto& triangleData : sceneConfig["triangles"]) {
        vec3 v1(triangleData["vertices"][0][0], triangleData["vertices"][0][1], triangleData["vertices"][0][2]);
        vec3 v2(triangleData["vertices"][1][0], triangleData["vertices"][1][1], triangleData["vertices"][1][2]);
        vec3 v3(triangleData["vertices"][2][0], triangleData["vertices"][2][1], triangleData["vertices"][2][2]);
        Color color(triangleData["color"][0], triangleData["color"][1], triangleData["color"][2]);
        triangles.emplace_back(v1, v2, v3, color);
    }

    for (const auto& planeData : sceneConfig["planes"]) {
        vec3 normal(planeData["normal"][0], planeData["normal"][1], planeData["normal"][2]);
        vec3 point(planeData["point"][0], planeData["point"][1], planeData["point"][2]);
        Color color(planeData["color"][0], planeData["color"][1], planeData["color"][2]);
        planes.emplace_back(normal, point, color);
    }

    for (const auto& lightData : sceneConfig["lights"]) {
        vec3 position(lightData["position"][0], lightData["position"][1], lightData["position"][2]);
        float intensity = lightData["intensity"];
        lights.emplace_back(position, intensity);
    }

    file.close();
}

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

    std::vector<Sphere> spheres;
    std::vector<Triangle> triangles;
    std::vector<Plane> planes;
    std::vector<Light> lights;

    loadSceneFromJson("E:/Projets/Persos/C++ Projects/Raytracer/scene.json", spheres, triangles, planes, lights);

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