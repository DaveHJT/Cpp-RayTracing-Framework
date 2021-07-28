// Termm--Fall 2020

#pragma once

#include <glm/glm.hpp>
#include "Image.hpp"
#include "Material.hpp"

class PhongMaterial : public Material {
public:
  PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess, double reflectivity, double refraction, const char *texture);
  virtual ~PhongMaterial();


  glm::vec3 m_kd;
  glm::vec3 m_ks;

  double m_shininess;
  double m_reflectivity;
  double m_refraction;
  Image *m_texture;
};
