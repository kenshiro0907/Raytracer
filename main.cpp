#include <iostream>
#include "Image.hpp"
#include "src/object/Ray.hpp"

using namespace std;

/*bool intersectSphere(const Sphere& sphere, const Ray& r) {
        vec3 OC = sphere.getCenter() - r.origin;
        float dotProd = OC.dot(r.direction);
        vec3 OP = r.direction * dotProd;
        vec3 P = r.origin + OP;
        vec3 CP = P - sphere.getCenter();
        float lenCP = CP.dot(CP);

        if (lenCP > sphere.getRadius() * sphere.getRadius()) {
            return false;
        }

        float a = std::sqrt(sphere.getRadius() * sphere.getRadius() - lenCP);
        vec3 intersectionPoint = P - (r.direction * a);
        return true;
}

Color ray_color(const Ray& r) {
    Sphere sphere(5.0f, vec3(0.0f, 0.0f, 0.0f));

    if (intersectSphere(sphere, r)) {
      return Color(1.0f, 0.0f, 0.0f);
    }

    Color startValue(1.0f, 1.0f, 1.0f);
    Color endValue(0.5f, 0.7f, 1.0f);
    vec3 unit_direction = (r.direction.normalize());
    auto a = 0.5f*(unit_direction.y + 1.0f);
    return startValue * (1.0f-a) + endValue * a;
}*/


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
        Light(vec3(-5.0f, 8.0f, 4.0f), 0.5f), // Lumière supérieure gauche
        //Light(vec3(-5.0f, -5.0f, 0.0f), 2.0f) // Lumière inférieure gauche
    };

    const int maxDepth = 10; // Limite de profondeur pour les réflexions

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float u = (2.0f * x) / width - 1.0f;
            float v = 1.0f - (2.0f * y) / height;
            Ray ray(vec3(0, 0, 0), vec3(u, v, -1).normalize());
            Color pixelColor = ray_color(ray, spheres, planes, lights, maxDepth);
            image.SetPixel(x, y, pixelColor);
        }
    }

    image.WriteFile("output.png");
    std::cout << "Raytracing terminé! Image sauvegardée sous 'output.png'.\n";

    return 0;
}

