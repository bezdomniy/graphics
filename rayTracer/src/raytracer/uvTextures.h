#pragma once

#include "types.h"

#include <string>
#include <cmath>
#include <vector>
#include <memory>

#include <iostream>

// #include <cereal/archives/binary.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>

#include "serialisation.h"
// #include <bit>

class UVTexture
{
private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &archive)
    {
        // archive();
    }

public:
    UVTexture(/* args */);
    virtual ~UVTexture() = 0;

    virtual void loadRight(std::string const &path) = 0;
    virtual void loadLeft(std::string const &path) = 0;
    virtual void loadUp(std::string const &path) = 0;
    virtual void loadDown(std::string const &path) = 0;
    virtual void loadFront(std::string const &path) = 0;
    virtual void loadBack(std::string const &path) = 0;

    virtual Vec3 patternAt(Vec2 uv, int faceIndex = 0) = 0;
};

class CheckeredTexture : public UVTexture
{
private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(cereal::virtual_base_class<UVTexture>(this), colourA, colourB, width, height);
    }

    virtual void loadRight(std::string const &path) override;
    virtual void loadLeft(std::string const &path) override;
    virtual void loadUp(std::string const &path) override;
    virtual void loadDown(std::string const &path) override;
    virtual void loadFront(std::string const &path) override;
    virtual void loadBack(std::string const &path) override;

public:
    CheckeredTexture();
    CheckeredTexture(Vec3 colourA, Vec3 colourB, int width, int height);
    ~CheckeredTexture();
    virtual Vec3 patternAt(Vec2 uv, int faceIndex = 0) override;

    Vec3 colourA;
    Vec3 colourB;
    int width;
    int height;
};

struct Surface
{
private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(rgb, w, h, bpp);
    }

public:
    Surface() {}
    Surface(int w, int h, int bpp)
    {
        this->w = w;
        this->h = h;
        this->bpp = bpp;
    }

    std::vector<unsigned char> rgb;
    int w;
    int h;
    int bpp;
};

class ImageTexture : public UVTexture
{
private:
    friend class cereal::access;
    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(cereal::virtual_base_class<UVTexture>(this), textures, width, height);
    }

    Vec3 rgbFromSurface(std::unique_ptr<Surface> &surface, int x, int y);

public:
    ImageTexture();
    ImageTexture(std::string const &path);
    ~ImageTexture();

    virtual void loadRight(std::string const &path) override;
    virtual void loadLeft(std::string const &path) override;
    virtual void loadUp(std::string const &path) override;
    virtual void loadDown(std::string const &path) override;
    virtual void loadFront(std::string const &path) override;
    virtual void loadBack(std::string const &path) override;

    virtual Vec3 patternAt(Vec2 uv, int faceIndex = 0) override;

    std::vector<std::unique_ptr<Surface>> textures;
    int width;
    int height;
};

CEREAL_REGISTER_TYPE(CheckeredTexture);
CEREAL_REGISTER_TYPE(ImageTexture);

CEREAL_REGISTER_POLYMORPHIC_RELATION(UVTexture, CheckeredTexture)
CEREAL_REGISTER_POLYMORPHIC_RELATION(UVTexture, ImageTexture)