// Termm--Fall 2020

#include <glm/ext.hpp>

#include "A4.hpp"
#include "GeometryNode.hpp"
#include "cs488-framework/MathUtils.hpp"
#include <vector>
// #define FAST_MODE

using namespace glm;
using namespace std;

float AIR_REFRACTIVE_IDX = 1.0f;
float GLASS_REFRACTIVE_IDX = 1.4f;
float WATER_REFRACTIVE_IDX = 1.3f;

int duration = 5;
int fps = 24;
int sample = 18;
int state = 0;
int maxHitTimes = 2;

float d[] = {10.0f, 2.0f, 5.0f, 5.0f};
float lastTime = 0;
float radius = 80.0f;

float aperture = 15.0f;

string ballNames[] = {"redball", "blueball", "metalball", "glassball"};
vector<GeometryNode*> ballNodes;

static vec3 backgroundColorRB = vec3(0.45f, 0.28f, 0.60f);
static vec3 backgroundColorLU = vec3(0.09f, 0.42f, 0.75f);
static vec3 backgroundStar = vec3(1.00f, 1.00f, 0.75f);

vec3 getTexture(Image *texture, float u, float v) {
	float w = texture->width();
	float h = texture->height();
	float di = (w - 1) * u;
	float dj = (h - 1) * v;
	float i = int(di);
	float j = int(dj);
	float up = di - i;
	float vp = dj - j;
	vec3 C00 = texture->getColor(i, j);
	vec3 C01 = texture->getColor(i, j + 1);
	vec3 C10 = texture->getColor(i + 1, j);
	vec3 C11 = texture->getColor(i + 1, j + 1);
	return C00 * (1 - up) * (1 - vp) + C01 * (1 - up) * vp + C10 * (1 - vp) * up + C11 * up * vp;
}

vec3 getBackgroundColor(vec3 bgrDir, Image *backgroundImage) {
	// return vec3();
	float pitch = atan(bgrDir.z / bgrDir.x);
	if (bgrDir.x < 0) {
		pitch += PI;

	} else if (bgrDir.z < 0) pitch += 2 * PI;
	float u = (pitch ) / (PI  * 2 );
	float xz = sqrt(pow(bgrDir.x, 2) + pow(bgrDir.z, 2));
	float v = 1.0f - (atan(bgrDir.y / xz) + PI / 2) / PI;
	// if (y == ny / 2 && x == nx / 2) cout << "u " << u << " v " << v << " dir " << radiansToDegrees(pitch) << " x " << bgrDir.x << " z " << bgrDir.z << endl;
	// cout << "u " << u << " v " << v << endl;
	return  getTexture(backgroundImage, u, v);
}

vec3 rayTrace(Ray &ray, SceneNode *root, Image *backgroundImage, vec3 ambient, const list<Light *> lights, int maxHit, bool refracted = false) {
	// if (ray.time != 0) cout << " time2: " << ray.time << endl;
	Intersection inter = root->intersect(ray);
	vec3 backgroundColor = getBackgroundColor(ray.direction, backgroundImage);
	vec3 color = backgroundColor;

	if (inter.hit) {
		vec3 matColor = inter.mat->m_kd;

		if (inter.mat->m_texture != nullptr) {
			matColor = getTexture(inter.mat->m_texture, inter.u, inter.v);
			// cout << "i: " << i << " j: " << j << endl;
			//
			// cout << "w: " << inter.mat->m_texture->width() << " h: " << inter.mat->m_texture->height() << endl;
			// cout << to_string(C00) << endl;
		}
		color = matColor * ambient;
		// if (sqrt(dot(matColor, matColor)) < 0.01f) cout << "black dot: pos " << to_string(inter.position) << " maxhit " << maxHit << endl;

		for(const Light * light : lights) {
			vec3 lightColor = vec3(0);
			// shadow ray
			vec3 lightDir = normalize(light->position - inter.position);

		  Ray shadowRay = Ray(inter.position, lightDir, ray.time);
			Intersection shadowInter = root->intersect(shadowRay);
		//	std::cout << "shadowInter.hit" << shadowInter.hit << '\n';
			if (!shadowInter.hit) {
				// light contribute to colour
				// diffuse

				float n_dot_l = dot(inter.normal, lightDir);
				if (n_dot_l < 0) n_dot_l = 0;

				vec3 diffuse = matColor * n_dot_l;
				lightColor += light->colour * diffuse;

				// Specular
				float n_dot_h = dot(inter.normal * normalize(lightDir - ray.direction), -ray.direction);
				if (n_dot_h == 0) cout << "pos: " << to_string(inter.position) << " normal: " << to_string(inter.normal) << endl;
				if (n_dot_h < 0) n_dot_h = 0;
				float pf = pow(n_dot_h, inter.mat->m_shininess);
				vec3 specular = inter.mat->m_ks * pf;
				lightColor += light->colour * specular;
			}
			float c1 = 1000;
			float c2 = 100;
			float c3 = 1;
			float lightIntensity = 300000;
			float r = distance(light->position, inter.position);
			color += lightColor * std::min(lightIntensity / (c1 + c2 * r + c3 * pow(r, 2)), 1.0d);
		}
		// Reflect
		#ifndef FAST_MODE

		if (maxHit > 0) {
			float FR = 1.0f;
			vec3 reflection = vec3(0);
			vec3 refraction = vec3(0);
			vec3 offsetPos = inter.position;
			if (inter.mat->m_reflectivity != 0 ) {
				// Reflection
				vec3 reflectDir = normalize(ray.direction - 2 * inter.normal * dot(ray.direction, inter.normal));
				// float fudge = 0.00f;
				Ray reflectRay = Ray(offsetPos, reflectDir);
				reflection = inter.mat->m_reflectivity * rayTrace(reflectRay, root, backgroundImage, ambient, lights, maxHit - 1);
				// FR = ;
			}
			if (inter.mat->m_refraction != 0) {
				// Refraction
				vec3 i = ray.direction;
				vec3 n = inter.normal;
				float etai = AIR_REFRACTIVE_IDX;
				float etat = inter.mat->m_refraction;
				if (refracted) std::swap(etai, etat);
				float eta = etai / etat;
				float internalRefraction = 1 - pow(eta, 2) * (1 - pow(dot(i, n), 2));

				if (internalRefraction >= 0) {
					vec3 t = normalize((-eta * dot(i, n) - sqrt(internalRefraction)) * n + eta * i);
					offsetPos = inter.position;
					Ray refractRay = Ray(offsetPos, t);
					int nextMaxHit = maxHit;
					if (refracted) nextMaxHit -= 1;
					refraction = rayTrace(refractRay, root, backgroundImage, ambient, lights, nextMaxHit, !refracted);

					float thetai = dot(i, n);
					float thetat = asin(sin(thetai) * eta);
					float etai2 = pow(etai, 2);
					float etat2 = pow(etat, 2);
					float cit = cos(thetai) * cos(thetat);
					float nit = etai * etat;
					FR = pow((etai2 - etat2), 2) * pow(cit, 2) + pow(cos(pow(thetai, 2)) - cos(pow(thetat, 2)), 2) * pow(nit, 2) /
										pow(cit * (etai2 + etat2) + nit * (cos(pow(thetai, 2)) + cos(pow(thetat, 2))), 2);
				}
			}

			float FT = 1 - FR;
			if (inter.mat->m_reflectivity == 0) FT = 1.0;
			color += inter.mat->m_ks * (reflection * abs(FR) + refraction * abs(FT));
			// if (sqrt(dot(color, color)) < 0.05f) cout << "black dot: FT " << FT << " FR " << FR << " refle " << to_string(reflection) << " refra " << to_string(refraction) <<endl;
		}
		#endif
	}

	return color;
}

SceneNode* getNode(SceneNode& root, string name){
	if(root.m_name == name){
		return &root;
	}
	for(SceneNode * child : root.children){
		SceneNode * res = getNode(*child, name);
		if(res!= NULL){
			return res;
		}
	}
	return NULL;
}

vec3 transformVec(mat4 trans, vec3 vector) {
	return vec3(trans * vec4(vector, 1.0f));
}

void updateMovement(SceneNode * root, int frame, glm::vec3 & eye, glm::vec3 & view, glm::vec3 & up) {
	float frameFactor = fps * d[state];
  // 	cout << "frame: " << frame << " lastTime: " << lastTime << " state: " << state << " hit " << hit1 << " hit2 " << hit2 << endl;
	if (state == 0) {
		view = transformVec(translate(vec3(-1, 0, 0) * 400.0f / frameFactor), view);
	}
	// else if (state == 1) {
	// 	eye = transformVec(translate(vec3(-3, -2.5, 0) * 300.0f / frameFactor), eye);
	// 	view = transformVec(translate(vec3(0, 1, 0) * 200.0f / frameFactor), view);
	// } else if (state == 2) {
	// 	eye = transformVec(translate(vec3(-1, 0, 0) * 350.0f / frameFactor), eye);
	// 	view = transformVec(translate(vec3(-1, 0, 0) * 350.0f / frameFactor), view);
	// } else if (state == 3) {
	// 	eye = transformVec(translate(vec3(-1, 0, 0) * 350.0f / frameFactor), eye);
	// 	view = transformVec(translate(vec3(-1, 0, 0) * 350.0f / frameFactor), view);
	// }

	if (frame / fps - lastTime >= d[state]) {

		lastTime += d[state];
		state++;

	}

	// check collision
	for (int i = 0; i < ballNodes.size(); i++) {
		for (int j = i + 1; j < ballNodes.size(); j++) {
			vec3 iPos = vec3(ballNodes[i]->trans * vec4(0.0f, 0.0f, 0.0f, 1.0f));
			vec3 jPos = vec3(ballNodes[j]->trans * vec4(0.0f, 0.0f, 0.0f, 1.0f));
			if (distance(iPos, jPos) < 2 * radius) {
				GeometryNode *balli = ballNodes[i];
				GeometryNode *ballj = ballNodes[j];
				if (balli->velocity == vec3(0)) swap(balli ,ballj);
				vec3 n = normalize(iPos - jPos);
				vec3 iDir = normalize(balli->velocity);
				float viRatio = cos(dot(iDir, n));
				float vjRatio = sin(dot(iDir, n));
				// cout << "n " << to_string(n) << endl;
				// cout << "ballNodes[i]->velocity " << to_string(ballNodes[i]->velocity) << endl;
				// cout << "iDir " << to_string(iDir) << endl;
				// cout << "dot(iDir, n) " << dot(iDir, n) << endl;
				// cout << ballNodes[i]->m_name << viRatio << endl;
				// cout << ballNodes[j]->m_name << vjRatio << endl;
				vec3 jDir = n;
				ballj->velocity = jDir * distance(balli->velocity, vec3(0)) * vjRatio;
				// iDir = normalize(iDir + (PI / 2 - dot(iDir, n)));
				balli->velocity = balli->velocity - ballj->velocity;

				// balli->velocity = iDir * balli->velocity * viRatio;


				// cout << ballNodes[j]->m_name << to_string(ballNodes[j]->velocity) << endl;
			}
		}
	}

	// move by velocity
	for (int i = 0; i < ballNodes.size(); i++) {
		cout << ballNodes[i]->m_name << to_string(ballNodes[i]->velocity) << endl;
		ballNodes[i]->translate(ballNodes[i]->velocity);
		ballNodes[i]->rotate('z', (float) radiansToDegrees(-ballNodes[i]->velocity.x / radius));
		ballNodes[i]->rotate('x', (float) radiansToDegrees(ballNodes[i]->velocity.z /radius));
	}
}

void transTree(SceneNode *root) {
	for(SceneNode *child: root->children) {
		child->trans = root->trans * child->trans;
		child->invtrans = inverse(child->trans);
		transTree(child);
	}
}

double random(double start,double end)
{
 return start + (end - start) * rand() / (RAND_MAX + 1.0f);
}

inline vec3 getRandomInDisk() {
    vec3 pos;
    do {
        pos = vec3(random(-1,1), random(-1,1), 0);
    } while (dot(pos ,pos) >= 1.0f);
    return pos;
}

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
) {

  for (string name : ballNames) {
		ballNodes.push_back(static_cast<GeometryNode*>(getNode(*root, name)));
		if (ballNodes.back()->m_name == "glassball") ballNodes.back()->velocity = vec3(-400, 0, 0) / fps;
		// if (ballNodes.back()->m_name == "redball") ballNodes.back()->velocity = vec3(0, 0, -400) / fps;
		// if (ballNodes.back()->m_name == "lens") ballNodes.back()->velocity = vec3(30, 0, 0) / fps;
	}

  std::cout << "F20: Calling A4_Render(\n" <<
		  "\t" << *root <<
          "\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
          "\t" << "eye:  " << glm::to_string(eye) << std::endl <<
		  "\t" << "view: " << glm::to_string(view) << std::endl <<
		  "\t" << "up:   " << glm::to_string(up) << std::endl <<
		  "\t" << "fovy: " << fovy << std::endl <<
          "\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
		  "\t" << "lights{" << std::endl;

	for(const Light * light : lights) {
		std::cout << "\t\t" <<  *light << std::endl;
	}
	std::cout << "\t}" << std::endl;
	std:: cout <<")" << std::endl;

	transTree(root);
	//root->applyTrans(mat4(1));
	// transform to world
	int ny = image.height();
	int nx = image.width();
	#ifdef FAST_MODE
	ny /= 5;
	nx /= 5;
	#endif



	std::cout << "Rendering: " << endl;
	int loading = 0;

	Image *backgroundImage = new Image();
	backgroundImage->loadPng("Assets/textures/castle2k.png");

	// render frames


	Image im(nx, ny);
	int nofFrames = fps * duration;
	string path =  "render/frame";
	string suffix =  ".png";
	for (uint frame = 0; frame < nofFrames; frame++) {
		float lensRadius = aperture / 2.0f;
		float focus = distance(eye, view);
		// cout << "focus " << focus << endl;
		float height = 2.0f * tan(radians(fovy) / 2.0f);
		// cout << "height " << height << endl;
		float width = nx/ny * height;

		vec3 w = normalize(eye - view);
		vec3 u = normalize(cross(up, w));
		vec3 v = cross(w, u);


		vec3 horizontal = width * focus * u;
		// cout << "horizontal " << to_string(horizontal) << endl;
		vec3 vertical = height * focus * v;
		// cout << "vertical " << to_string(vertical) << endl;
		vec3 corner = eye - (horizontal / 2) - (vertical / 2) - focus * w;
		// cout << "corner " << to_string(corner) << endl;
		if (frame - loading > nofFrames / 10) {
			std::cout << "█ " << (double) loading / nofFrames * 100 << "%" << endl;
			loading = frame;
		}
		for (uint y = 0; y < ny; ++y) {
			if (nofFrames == 1 && frame - loading > ny / 10) {
				std::cout << "█ " << (double) loading / ny * 100 << "%" << endl;
				loading = y;
			}
			for (uint x = 0; x < nx; ++x) {
				Ray ray = Ray();
				vec3 color = vec3(0);
				 // cout << "calculation" << endl;

				 // cout << "ray.source " << to_string(ray.source) << endl;

				 for(float i = 0; i < sample; i++){
					 vec3 random = lensRadius * getRandomInDisk();
					//  cout << "lensRadius " << lensRadius << endl;
					 // cout << "getRandomInDisk " << to_string(getRandomInDisk()) << endl;
					 // cout << "random " << to_string(random) << endl;
					 vec3 offset = u * random.x + v * random.y;
					 ray.source = eye + offset;
						ray.time = rand() % 100 / 100.0f;
						// cout << " time1: " << ray.time << endl;
						ray.direction = normalize(corner + (float) x / nx * horizontal + (float) y / ny * vertical - ray.source);
						// cout << "direction " << to_string(ray.direction) << endl;

	 					color += rayTrace(ray, root, backgroundImage, ambient, lights, maxHitTimes);
		 			}
					color /= (float) sample;

					// flip
					y = ny - y - 1;
					// Red:
					im(x, y, 0) = color.x;
					// Green:
					im(x, y, 1) = color.y;
					// Blue:
					im(x, y, 2) = color.z;
			}
		}
		string index = "";
		int digits = frame == 0? 0 : log10(frame);
		for (int i = 0; i < 3 - digits; i++) {
			index += "0";
		}
		cout << path + index + to_string(frame) << endl;
		im.savePng(path + index + to_string(frame) + suffix);
		updateMovement(root, frame, eye, view, up);
		// cout << to_string(view) << endl;
	}

	std::cout << "█ 100.0% DONE" << std::endl;
}
