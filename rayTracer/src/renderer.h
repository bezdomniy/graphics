#pragma once

#include "camera.h"
#include "canvas.h"

#include "shape.h"

#include "geometry.h"
#include "pointLight.h"
#include "ray.h"
// #include "sphere.h"
#include "world.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <memory>

class Renderer
{
private:
  std::shared_ptr<Camera> camera;

public:
  Renderer();
  Renderer(std::shared_ptr<Camera> &c);
  ~Renderer();

  glm::vec3 colourAt(Ray &ray, World &world, short remaining);
  glm::vec3 reflectColour(Geometry::Intersection<Shape> *hit, World &world, short remaining);
  glm::vec3 lighting(std::shared_ptr<Shape> &shape,
                     std::shared_ptr<PointLight> &light, glm::vec4 point,
                     glm::vec4 eyev, glm::vec4 normalv, bool inShadow);
  glm::vec3 shadeHit(Geometry::Intersection<Shape> *hit, World &world, short remaining);
  bool isShadowed(glm::vec4 point, World &world);
  Canvas canvas;

  void render(World &world);
};
