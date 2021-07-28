// Termm--Fall 2020

#pragma once

#include "SceneNode.hpp"
#include "PhongMaterial.hpp"

class GeometryNode : public SceneNode {
public:
	GeometryNode( const std::string & name, Primitive *prim,
		Material *mat = nullptr );

	void setMaterial( Material *material );
	void applyTrans(mat4 transform);
	Intersection intersect(Ray ray);
	Material *m_material;
	Primitive *m_primitive;
};
