 // Termm--Fall 2020

#include "Primitive.hpp"
#include "Mesh.hpp"
#include <iostream>
#include "cs488-framework/MathUtils.hpp"
using namespace std;



Intersection Primitive::intersect(Ray ray) {
	Intersection intersection = Intersection();
	return intersection;
}



Intersection Sphere::intersect(Ray ray) {
	float m_radius = 1.0f;
	vec3 m_pos = vec3();
	// mat4 invTrans = inverse(this->trans);
	// ray.source = vec3(invTrans * vec4(ray.source, 1.0f));
	// ray.direction = mat3(invTrans) * ray.direction;

	vec3 ac = ray.source - m_pos;
	float A = dot(ray.direction, ray.direction);
	float B = 2 * dot(ray.direction, ac);
	float C = dot(ac, ac) - pow(m_radius, 2);
	// float delta = pow(B, 2) - 4 * A * C;
	Intersection inter = Intersection();
	double roots[2];
	size_t nofRoots = quadraticRoots(A, B, C, roots);

	if (nofRoots <= 0) {
		inter.hit = false;
		return inter;
	} else {
		inter.t = roots[0];
		inter.hit = true;
	}
	if (nofRoots == 2 &&roots[1] < roots[0]) inter.t = roots[1];


	inter.position = ray.source + inter.t * ray.direction;
	inter.normal = normalize(inter.position - m_pos);

	return inter;
}



Intersection NonhierSphere::intersect(Ray ray) {
	// cout << ray.direction.x <<ray.direction.y << ray.direction.z<< endl;

	// cout << ray.direction.x <<ray.direction.y << ray.direction.z<< endl;
	ray.source -= m_pos;
  vec3 ac = ray.source - m_pos;
  float A = dot(ray.direction, ray.direction);
  float B = 2 * dot(ray.direction, ac);
  float C = dot(ac, ac) - pow(m_radius, 2);
  // float delta = pow(B, 2) - 4 * A * C;
  Intersection inter = Intersection();
	double roots[2];
	size_t nofRoots = quadraticRoots(A, B, C, roots);
	// if (nofRoots > 0) cout << nofRoots << endl;
  if (nofRoots <= 0) {
    inter.hit = false;
    return inter;
  } else {
		inter.t = roots[0];
		inter.hit = true;
	}
	if (nofRoots == 2 &&roots[1] < roots[0]) inter.t = roots[1];

  // inter.t = (- B + sqrt(delta)) / 2 * A;
  inter.position = ray.source + inter.t * ray.direction;
  inter.normal = normalize(inter.position - m_pos);
	inter.u = (atan(inter.normal.x / inter.normal.z) + PI) / (2 * PI);
	float xz = sqrt(pow(inter.normal.x, 2) + pow(inter.normal.z, 2));
	inter.v = (atan(inter.normal.y / xz) + PI / 2) / PI;
	if (inter.u > 1.0f) inter.u -= 1;
	if (inter.v > 1.0f) inter.v -= 1;
  // if (delta > 0) {
  //   float t2 = (- B - sqrt(delta)) / 2 * A;
  //   if (abs(t2) < abs(inter.t)) {
  //     inter.t = t2;
  //     inter.position = ray.source + t2 * ray.direction;
  //     inter.norm = normalize(inter.position - m_pos);
  //   }
	// cout << inter.hit << endl;
  return inter;
}

NonhierBox::NonhierBox(const glm::vec3& pos, double size, Mesh* mesh)
	: m_pos(pos), m_size(size), m_mesh(mesh)
{

	// applyTrans(translate(pos) * scale((float)size * vec3(1)));
	mat4 transform = translate(pos) * scale((float)size * vec3(1));
	this->trans = transform;
	this->invtrans = inverse(transform);
	this->m_mesh->applyTrans(transform);
}

Intersection NonhierBox::intersect(Ray ray) {
	// mat4 invTrans = inverse(this->trans);
	// ray.source = vec3(invTrans * vec4(ray.source, 1.0f));
	// ray.direction = mat3(invTrans) * ray.direction;
	return this->m_mesh->intersect(ray);
}

Intersection Cube::intersect(Ray ray) {
	return this->m_mesh->intersect(ray);
}

void Primitive::applyTrans(mat4 transform) {
	this->trans = transform;
	this->invtrans = inverse(transform);
}
void NonhierBox::applyTrans(mat4 transform) {

}
void Cube::applyTrans(mat4 transform) {
	this->trans = transform;
	this->invtrans = inverse(transform);
	this->m_mesh->applyTrans(transform);
}

Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

Cube::~Cube()
{
	delete m_mesh;
}

NonhierSphere::~NonhierSphere()
{
}

NonhierBox::~NonhierBox()
{
	delete m_mesh;
}
ostream& operator<<(ostream& out, const Primitive& Primitive)
{
  out << "premitive {";
  out << "}";
  return out;
}
