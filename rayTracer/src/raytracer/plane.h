#pragma once

#include "shape.h"
#include "geometry.h"
#include "types.h"
// #include <vector>

class Plane : public Shape
{
public:
    Plane();
    ~Plane();

    virtual void intersectRay(Ray &ray, std::vector<Geometry::Intersection<Shape>> &intersections) override;
    virtual Vec4 normalAt(const Vec4 &point) override;
    virtual Vec4 normalAt(const Vec4 &point, const Vec2 &uv) override;
    virtual std::pair<Vec4, Vec4> bounds() const override;
    virtual std::string type() override;

private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(cereal::virtual_base_class<Shape>(this));
    }
};

CEREAL_REGISTER_TYPE(Plane);