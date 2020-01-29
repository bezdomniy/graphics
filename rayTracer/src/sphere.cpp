#include "sphere.h"

Sphere::Sphere()
    : Shape()
{
}

Sphere::~Sphere() {}

void Sphere::intersectRay(Ray& ray, std::vector<Geometry::Intersection<Shape>>& intersections) {
    Ray transformedRay = transformRay(ray);

    glm::vec4 sphereToRay = transformedRay.origin - glm::vec4(0.f, 0.f, 0.f, 1.f);
    float a = glm::dot(transformedRay.direction, transformedRay.direction);
    float b = 2 * glm::dot(transformedRay.direction, sphereToRay);
    float c =
        glm::dot(sphereToRay, sphereToRay) - 1;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
        return;

    float t1 = (-b - std::sqrt(discriminant)) / (2 * a);
    float t2 = (-b + std::sqrt(discriminant)) / (2 * a);

    intersections.push_back(Geometry::Intersection<Shape>{t1, this});
    intersections.push_back(Geometry::Intersection<Shape>{t2, this});

}

glm::vec4 Sphere::normalAt(glm::vec4 point)
{
  // glm::mat4 transformInverse(glm::affineInverse(this->transform));
  glm::vec4 objectPoint = this->inverseTransform * point;
  glm::vec4 objectNormal = objectPoint - glm::vec4(0.f, 0.f, 0.f, 1.f);
  glm::vec4 worldNormal = glm::transpose(this->inverseTransform) * objectNormal;
  worldNormal.w = 0.f;

  return glm::normalize(worldNormal);
}

std::string Sphere::type()
{
  return "Sphere";
}