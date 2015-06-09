#include "sphere.h"

#include "platform.h"

void Sphere::draw()
{
    scene_node::draw();

    vector3f color = this->material.getKd();
    glColor3f(color[0], color[1], color[2]);

    glutSolidSphere(this->radius, 15, 15);
}

hit_result Sphere::hit(Ray ray [[gnu::unused]])
{
    hit_result result;
    float a, b, c, discriminant;

    a = ray.direction.dot(ray.direction);
    b = 2 * ray.direction.dot(ray.origin);
    c = ray.origin.dot(ray.origin) - radius * radius;

    discriminant = b * b - 4 * a * c;
    if (discriminant < 0.f)
        return result;

    // We have a hit
    result.hit = true;
    result.node = shared_from_this();

    // Calculate closest distance
    result.depth = (-b + sqrtf(discriminant)) / 2 * a;

    if(result.depth < 0 && discriminant > 0.f) {
        float t;

        t = (-b - sqrtf(discriminant)) / 2 * a;
        if(t < result.depth)
            result.depth = t;
    }

    return result;
}

color3 Sphere::apply(unsigned int level [[gnu::unused]], hit_result hit_info [[gnu::unused]])
{
    return color3(material.getKd());
}