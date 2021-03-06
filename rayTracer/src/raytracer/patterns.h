#pragma once

// #include "../include/SimplexNoise.h"
#include "../simplexNoise/SimplexNoise.h"
#include "pattern.h"
#include "textureMap.h"
#include "uvTextures.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <memory>

class StripedPattern : public ColourPattern
{
private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(cereal::virtual_base_class<ColourPattern>(this));
  }

  // template <class Archive>
  // static void load_and_construct(Archive &archive, cereal::construct<StripedPattern> &construct)
  // {
  //   Vec3 colourA;
  //   Vec3 colourB;

  //   archive(colourA, colourB);
  //   construct(colourA, colourB);
  // }

public:
  StripedPattern() {}
  StripedPattern(Vec3 colourA, Vec3 colourB);
  ~StripedPattern();
  virtual Vec3 patternAt(const Vec4 &point) override;
};

class GradientPattern : public ColourPattern
{
private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(cereal::virtual_base_class<ColourPattern>(this));
  }

  // template <class Archive>
  // static void load_and_construct(Archive &archive, cereal::construct<GradientPattern> &construct)
  // {
  //   Vec3 colourA;
  //   Vec3 colourB;

  //   archive(colourA, colourB);
  //   construct(colourA, colourB);
  // }

public:
  GradientPattern() {}
  GradientPattern(Vec3 colourA, Vec3 colourB);
  ~GradientPattern();
  virtual Vec3 patternAt(const Vec4 &point) override;
};

class RingPattern : public ColourPattern
{
private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(cereal::virtual_base_class<ColourPattern>(this));
  }

  // template <class Archive>
  // static void load_and_construct(Archive &archive, cereal::construct<RingPattern> &construct)
  // {
  //   Vec3 colourA;
  //   Vec3 colourB;

  //   archive(colourA, colourB);
  //   construct(colourA, colourB);
  // }

public:
  RingPattern() {}
  RingPattern(Vec3 colourA, Vec3 colourB);
  ~RingPattern();
  virtual Vec3 patternAt(const Vec4 &point) override;
};

class CheckedPattern : public ColourPattern
{
private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(cereal::virtual_base_class<ColourPattern>(this));
  }

  // template <class Archive>
  // static void load_and_construct(Archive &archive, cereal::construct<CheckedPattern> &construct)
  // {
  //   Vec3 colourA;
  //   Vec3 colourB;

  //   archive(colourA, colourB);
  //   construct(colourA, colourB);
  // }

public:
  CheckedPattern() {}
  CheckedPattern(Vec3 colourA, Vec3 colourB);
  ~CheckedPattern();
  virtual Vec3 patternAt(const Vec4 &point) override;
};

class BlendedPattern : public Pattern
{
private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(cereal::virtual_base_class<Pattern>(this), patternA, patternB);
  }

  // template <class Archive>
  // static void load_and_construct(Archive &archive, cereal::construct<BlendedPattern> &construct)
  // {
  //   std::shared_ptr<Pattern> patternA;
  //   std::shared_ptr<Pattern> patternB;

  //   archive(patternA, patternB);
  //   construct(patternA, patternB);
  // }

public:
  BlendedPattern() {}
  BlendedPattern(std::shared_ptr<Pattern> &patternA,
                 std::shared_ptr<Pattern> &patternB);
  BlendedPattern(const BlendedPattern &blendedPattern);
  ~BlendedPattern();

  virtual Vec3 patternAt(const Vec4 &point) override;

  std::shared_ptr<Pattern> patternA;
  std::shared_ptr<Pattern> patternB;
};

class PerturbedPattern : public Pattern
{
private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(cereal::virtual_base_class<Pattern>(this), pattern, perturbedCoeff);
  }

  // template <class Archive>
  // static void load_and_construct(Archive &archive, cereal::construct<PerturbedPattern> &construct)
  // {
  //   std::shared_ptr<Pattern> pattern;
  //   Float perturbedCoeff;

  //   archive(pattern, perturbedCoeff);
  //   construct(pattern, perturbedCoeff);
  // }

public:
  PerturbedPattern() {}
  PerturbedPattern(std::shared_ptr<Pattern> &pattern, Float perturbedCoeff);
  PerturbedPattern(const PerturbedPattern &perturbedPattern);
  ~PerturbedPattern();

  virtual Vec3 patternAt(const Vec4 &point) override;

  std::shared_ptr<Pattern> pattern;
  Float perturbedCoeff;
};

class MappedPattern : public Pattern
{
private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive &archive)
  {
    archive(cereal::virtual_base_class<Pattern>(this), uvTexture, textureMap);
  }

  // template <class Archive>
  // static void load_and_construct(Archive &archive, cereal::construct<MappedPattern> &construct)
  // {
  //   std::shared_ptr<UVTexture> uvTexture;
  //   std::shared_ptr<TextureMap> textureMap;

  //   archive(uvTexture, textureMap);
  //   construct(uvTexture, textureMap);
  // }

public:
  MappedPattern() {}
  MappedPattern(std::shared_ptr<UVTexture> &uvTexture, std::shared_ptr<TextureMap> &textureMap);
  MappedPattern(const MappedPattern &mappedPattern);
  ~MappedPattern();

  virtual Vec3 patternAt(const Vec4 &point) override;

  std::shared_ptr<UVTexture> uvTexture;
  std::shared_ptr<TextureMap> textureMap;
};

CEREAL_REGISTER_TYPE(StripedPattern);
CEREAL_REGISTER_TYPE(GradientPattern);
CEREAL_REGISTER_TYPE(RingPattern);
CEREAL_REGISTER_TYPE(CheckedPattern);
CEREAL_REGISTER_TYPE(BlendedPattern);
CEREAL_REGISTER_TYPE(PerturbedPattern);
CEREAL_REGISTER_TYPE(MappedPattern);