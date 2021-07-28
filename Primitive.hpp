// Termm--Fall 2020

#pragma once

#include <glm/glm.hpp>
#include "Ray.hpp"
#include "polyroots.hpp"
#include <glm/gtx/transform.hpp>

using namespace glm;


class Primitive;
class Mesh;

class Primitive {
private:

public:
  mat4 trans = mat4(1);
  mat4 invtrans = mat4(1);
  virtual ~Primitive();
  virtual Intersection intersect(Ray ray);
  // vec3 transferVec(mat4 trans, vec3 vector);
  virtual void applyTrans(mat4 transform);
};

class Sphere : public Primitive {
public:
  Intersection intersect(Ray ray);
  virtual ~Sphere();
};

class Cube : public Primitive {
public:
  Cube(Mesh* mesh)
  : m_mesh(mesh)
  {
  }
  Intersection intersect(Ray ray);
  virtual ~Cube();
  void applyTrans(mat4 transform);
private:
  Mesh* m_mesh;
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const glm::vec3& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  Intersection intersect(Ray ray);
  virtual ~NonhierSphere();

  glm::vec3 m_pos;
  double m_radius;

};

class NonhierBox : public Primitive {
public:

  NonhierBox(const glm::vec3& pos, double size, Mesh* mesh);
  Intersection intersect(Ray ray);
  virtual ~NonhierBox();
  void applyTrans(mat4 transform);
private:
  glm::vec3 m_pos;
  double m_size;
  Mesh* m_mesh;
};
