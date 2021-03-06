// Termm--Fall 2020

#include "GeometryNode.hpp"
#include "PhongMaterial.hpp"

#include <iostream>

using namespace std;

static float TMIN = 0.008;
//---------------------------------------------------------------------------------------
GeometryNode::GeometryNode(
	const std::string & name, Primitive *prim, Material *mat )
	: SceneNode( name )
	, m_material( mat )
	, m_primitive( prim )
	, velocity( vec3(0) )
{
	m_nodeType = NodeType::GeometryNode;
	cout << "GeometryNode: " << name << endl;
}

void GeometryNode::setMaterial( Material *mat )
{
	// Obviously, there's a potential memory leak here.  A good solution
	// would be to use some kind of reference counting, as in the
	// C++ shared_ptr.  But I'm going to punt on that problem here.
	// Why?  Two reasons:
	// (a) In practice we expect the scene to be constructed exactly
	//     once.  There's no reason to believe that materials will be
	//     repeatedly overwritten in a GeometryNode.
	// (b) A ray tracer is a program in which you compute once, and
	//     throw away all your data.  A memory leak won't build up and
	//     crash the program.

	m_material = mat;
}

vec3 transferVec(mat4 trans, vec3 vector) {
	return vec3(trans * vec4(vector, 1.0f));
}

void GeometryNode::applyTrans(mat4 transform) {
	transform = transform * this->trans;
	 this->invtrans = inverse(this->trans);
	this->m_primitive->applyTrans(transform);
	for (SceneNode * child : this->children) {
		child->applyTrans(transform);
	}
}

Intersection GeometryNode::intersect(Ray ray) {
	// motion blur
	vec3 offset = ray.time * velocity;
	ray.source -= offset;

	Ray transRay;
  transRay.time = ray.time;
	transRay.source = transferVec(invtrans, ray.source);
	transRay.direction = normalize(transferVec(invtrans, ray.source + ray.direction) - transferVec(invtrans, ray.source));

	Intersection nearestInter = Intersection();
	Intersection inter = this->m_primitive->intersect(transRay);
	float limit = numeric_limits<float>::max();

	if (inter.hit && inter.t > TMIN) {
		if (inter.mat == nullptr) inter.mat = dynamic_cast<PhongMaterial*>(this->m_material);

		nearestInter = inter;
		limit = inter.t;
	}

	for (SceneNode * child : this->children) {
		Intersection childInter = child->intersect(transRay);
		if (childInter.hit && childInter.t > TMIN && childInter.t < limit) {
			nearestInter = childInter;
			limit = childInter.t;
		}
	}
	nearestInter.normal = normalize(transferVec(trans, nearestInter.normal + nearestInter.position) - transferVec(trans, nearestInter.position));
	nearestInter.position = transferVec(trans, nearestInter.position);
	nearestInter.t = distance(nearestInter.position, ray.source);

	return nearestInter;
}
