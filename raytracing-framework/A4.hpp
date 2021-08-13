// Termm--Fall 2020

#pragma once

#include <glm/glm.hpp>

#include "Image.hpp"
#include "Light.hpp"
#include "SceneNode.hpp"


vec3 rayTrace(Ray &ray, SceneNode *root, Image backgroundImage, vec3 ambient, const std::list<Light *> lights, int maxHit, bool refracted);

void A4_Render(
		// What to render
		SceneNode * root,

		// Image to write to, set to a given width and height
		Image & image,

		// Viewing parameters
		glm::vec3 & eye,
		glm::vec3 & view,
		glm::vec3 & up,
		double fovy,

		// Lighting parameters
		const glm::vec3 & ambient,
		const std::list<Light *> & lights
);
