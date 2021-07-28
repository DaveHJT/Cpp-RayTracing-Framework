// Termm--Fall 2020

#include <iostream>
#include <fstream>

#include <glm/ext.hpp>

#include "Mesh.hpp"

#include "Primitive.hpp"
using namespace std;
using namespace glm;

Intersection traingleIntersect(Ray ray, vec3 P0, vec3 P1, vec3 P2, float limit) {
		Intersection inter = Intersection();

    vec3 a = ray.source;
    vec3 b = ray.direction;
		vec3 B1 = P1 - P0;
		vec3 B2 = P2 - P0;
		// cout << "mesh0" << endl;
		// cout << "a: " << a << endl;
		// cout << "b: " << b << endl;
		// cout << "B1: " << b << endl;
		mat3 M = {B1, B2, -b};
		mat3 M1 = {a - P0, B2, -b};
		mat3 M2 = {B1, a - P0, -b};
		mat3 M3 = {B1, B2, a - P0};

		float D = determinant(M);
		float D1 = determinant(M1);
		float D2 = determinant(M2);
		float D3 = determinant(M3);
		// cout << "mesh1" << endl;
		if (D == 0) return inter;
		float beta = D1 / D;
		float gamma = D2 / D;
		float t = D3 / D;
		// cout << "mesh2" << endl;
		// cout << beta << endl;
	if(t < limit && beta >= 0 && gamma >= 0 && beta + gamma <= 1){
		inter.hit = true;
		inter.t = t;
		inter.position = ray.source + inter.t * ray.direction;
	  inter.normal = normalize(cross(B1,P2 - P1));
	}
	return inter;
}

float ratio(float a, float b) {
	while (a > b) a -= b;
	return a / b;
}

Intersection Mesh::intersect(Ray ray) {
	// Intersection interBV = boundingV->intersect(ray);
	//
	// if (!interBV.hit) return Intersection();
	// cout << *this << endl;
	vector<vec3> vertices = m_vertices;
	vector<Triangle> faces = m_faces;
	float limit = numeric_limits<float>::max();
	Intersection inter = Intersection();
	#ifdef RENDER_BOUNDING_VOLUMES
	interBV.mat = this->mat;
		if (interBV.hit) {
			limit = interBV.t;
			inter = interBV;
		}
	#endif
	// cout << "mesh" << endl;
	for (Triangle triangle : faces) {
		Intersection newInter = traingleIntersect(ray, vertices[triangle.v1], vertices[triangle.v2], vertices[triangle.v3], limit);
		if (newInter.hit) {
			limit = newInter.t;
			inter = newInter;
		}
	}
	int repeat = 25;
	float lenX = (max.x - min.x) / repeat;
	float lenY = (max.y - min.y) / repeat;
	float lenZ = (max.z - min.z) / repeat;
	if (inter.normal.y != 0) {
		inter.u = ratio(inter.position.x - min.x, lenX);
		inter.v = ratio(inter.position.z - min.z, lenZ);
	} else {
		if (inter.normal.x != 0) {
			inter.u = ratio(inter.position.z - min.z, lenZ);
		} else {
			inter.u = ratio(inter.position.x - min.x,  lenX);
		}
		inter.v = ratio(inter.position.y - min.y,  lenY);
	}
	// cout << inter.u << " " << inter.v << endl;
	return inter;
}
// vec3 transferVec(mat4 trans, vec3 vector) {
// 	return vec3(trans * vec4(vector, 1.0f));
// }
// Intersection Mesh::intersect(Ray ray) {
// 	Ray transRay;
//
// 	transRay.source = transferVec(invtrans, ray.source);
// 	transRay.direction = normalize(transferVec(invtrans, ray.source + ray.direction) - transferVec(invtrans, ray.source));
// 	cout << *this << endl;
// 	vector<vec3> vertices = m_vertices;
// 	vector<Triangle> faces = m_faces;
// 	float limit = numeric_limits<float>::max();
// 	Intersection inter = Intersection();
// 	// cout << "mesh" << endl;
// 	for (Triangle triangle : faces) {
// 		Intersection newInter = traingleIntersect(transRay, vertices[triangle.v1], vertices[triangle.v2], vertices[triangle.v3], limit);
// 		if (newInter.hit) {
// 			limit = newInter.t;
// 			inter = newInter;
// 		}
// 	}
//
// 	inter.position = transferVec(trans, transRay.source + inter.t * transRay.direction);
// 	inter.t = distance(inter.position, ray.source);
// 	inter.normal = normalize(transferVec(transpose(invtrans), inter.normal));
// 	return inter;
//
// 	return inter;
// }

void Mesh::applyTrans(glm::mat4 trans) {
	this->trans *= trans;
	this->invtrans = inverse(this->trans);
	for (int i = 0; i < m_vertices.size(); i++) {
		vec4 verts = trans * vec4(m_vertices[i], 1);
		m_vertices[i] = vec3(verts);
	}

}



Mesh::Mesh( const string& fname )
	: m_vertices()
	, m_faces()
{
	string code;
	double vx, vy, vz;

	min = vec3(numeric_limits<float>::max());
	max = vec3(numeric_limits<float>::min());
	size_t s1, s2, s3;

	ifstream ifs( fname.c_str() );
	while( ifs >> code ) {
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			min.x = min.x < vx ? min.x : vx;
			min.y = min.y < vy ? min.y : vy;
			min.z = min.z < vz ? min.z : vz;
			max.x = max.x > vx ? max.x : vx;
			max.y = max.y > vy ? max.y : vy;
			max.z = max.z > vz ? max.z : vz;
			m_vertices.push_back( vec3( vx, vy, vz ) );
		} else if( code == "f") {
			ifs >> s1 >> s2 >> s3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
		}
	}

	vec3 pos = (max + min / 2);
	float radius = distance(max, pos) > distance(min, pos) ? distance(max, pos) : distance(min, pos);

	mat = new PhongMaterial(vec3(0.9, 0.9, 0.9), vec3(0.9, 0.9, 0.9), 100, 0.0, 1.0, "");
	boundingV = new NonhierSphere(pos, radius);

	// cout << "mesh: " << fname << *this << endl;
}

ostream& operator<<(ostream& out, const Mesh& mesh)
{
  out << "mesh {";
	out << "nofverts: " << mesh.m_vertices.size();
	out << " noffaces: " << mesh.m_vertices.size();
  // for( size_t idx = 0; idx < mesh.m_vertices.size(); ++idx ) {
  // 	out << to_string( mesh.m_vertices[idx] );
	// }
	out << to_string( mesh.m_vertices[0] );
	out << to_string( mesh.trans );
  out << "}";
  return out;
}
