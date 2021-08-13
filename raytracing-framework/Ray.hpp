#include <glm/glm.hpp>
#include "PhongMaterial.hpp"

using namespace glm;

class Ray {
public:
  Ray(vec3 source, vec3 direction) : source(source), direction(direction) {}
  Ray(vec3 source, vec3 direction, float time) : source(source), direction(direction), time(time) {}
  Ray() {}
  vec3 source = vec3();
  vec3 direction = vec3();
  float time = 0;
};

class Intersection {
public:
  bool hit = false;
  vec3 position = vec3();
  vec3 normal = vec3();
  float t = 0;
  float u = 0; // for texture mapping
  float v = 0; // for texture mapping
  PhongMaterial* mat;
};
