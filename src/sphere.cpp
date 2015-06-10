#include "sphere.h"

#include "platform.h"
#include "raytracing.h"

void Sphere::draw()
{
    SceneNode::draw();

    Vector3f color = this->material.getKd();
    glColor3f(color[0], color[1], color[2]);

    glutSolidSphere(this->radius, 15, 15);
}

void Sphere::createBoundingBox() {
    boundingBox.min = Vector3f(-radius,-radius,-radius);
    boundingBox.max = Vector3f(+radius,+radius,+radius);
}

hit_result Sphere::hit(Ray ray, shared_ptr<SceneNode> skip [[gnu::unused]])
{
    hit_result result;
    float a, b, c, discriminant;

    // HACK
    ray.origin += translation;
    // END

    a = ray.direction.dot(ray.direction);
    b = 2 * ray.direction.dot(ray.origin - translation);
    c = translation.dot(translation) + ray.origin.dot(ray.origin) - 2 * translation.dot(ray.origin) - radius * radius;

    discriminant = b * b - 4 * a * c;
    if (discriminant < 0.f)
        return result;

    // We have a hit
    result.hit = true;
    result.node = shared_from_this();

    // Calculate closest distance
    result.depth = (-b + sqrtf(discriminant)) / 2 * a;

    if(discriminant > 0.f) {
        float t;

        t = (-b - sqrtf(discriminant)) / 2 * a;
        if(t < result.depth)
            result.depth = t;
    }

    result.hitPosition = ray.origin + result.depth * ray.direction;

    result.normal = result.hitPosition - translation;
    result.normal.normalize();

    return result;
}

Vector3f Sphere::apply(unsigned int level [[gnu::unused]], hit_result hit_info)
{
    Vector3f color;
    auto& light = g_scene.lights[0];


    Vector3f ls = light->position - hit_info.hitPosition;
    Vector3f l = ls / ls.length();

    color = light->ambient * material.getKa() + l.dot(hit_info.normal) * light->diffuse * material.getKd();

    // Check for shadows
    Ray shadowRay(hit_info.hitPosition, light->position);
    hit_result shadowRes = g_scene.hit(shadowRay, shared_from_this());

    if(shadowRes.is_hit())
        color = light->ambient * material.getKa();

    return color;
}