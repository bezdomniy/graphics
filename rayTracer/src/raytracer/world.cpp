#include "world.h"

World::World() {}

World::~World() {}

void World::addLight(std::shared_ptr<PointLight> &light)
{
  this->lights.push_back(light);
}

void World::addShape(std::shared_ptr<Shape> &shape_ptr)
{
  this->shapes.push_back(shape_ptr);
}

std::vector<Geometry::Intersection<Shape>> World::intersectRay(Ray &ray)
{
  std::vector<Geometry::Intersection<Shape>> ret;
  ret.reserve(this->shapes.size() * 2);

  // TODO - take out ray transform here and do on CUDA then have another for loop do the rest on cpu
  for (auto &shape : this->shapes)
  {
    shape->intersectRay(ray, ret);
  }

  std::sort(ret.begin(), ret.end(), Geometry::compareIntersection<Shape>);

  return ret;
}

std::vector<Geometry::Intersection<Shape>> World::intersectRayShadow(Ray &ray)
{
  std::vector<Geometry::Intersection<Shape>> ret;
  ret.reserve(this->shapes.size() * 2);

  for (auto &shape : this->shapes)
  {
    if (shape->material->shadow)
      shape->intersectRay(ray, ret);
  }

  std::sort(ret.begin(), ret.end(), Geometry::compareIntersection<Shape>);

  return ret;
}