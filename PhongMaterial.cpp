// Termm--Fall 2020

#include "PhongMaterial.hpp"
#include <iostream>
#include "string.h"
using namespace std;

PhongMaterial::PhongMaterial(
	const glm::vec3& kd, const glm::vec3& ks, double shininess, double reflectivity, double refractionIndex, const char* texture)
	: m_kd(kd)
	, m_ks(ks)
	, m_shininess(shininess)
	, m_reflectivity(reflectivity)
	, m_refraction(refractionIndex)
	, m_texture(nullptr)
{
	// cout << texture << endl;
	if (strlen(texture) != 0) {
		m_texture = new Image();
		m_texture->loadPng(texture);
	}
}

PhongMaterial::~PhongMaterial()
{}
