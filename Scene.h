/*
 * Scene.h
 *	Class for storing objects in a scene
 *  Created on: Sep 10, 2018
 *      Author: cb-ha
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Geometry.h"
#include "Camera.h"

using namespace glm;

//Forward declaration of classes
//(note this is necessary because these are pointers and it allows the #include to appear in the .cpp file)
class RenderingEngine;

class Scene {
public:
	Scene(RenderingEngine* renderer);
	virtual ~Scene();

	//Send geometry to the renderer
	void displayScene();

	void updateCamera(float phi, float theta, float radius);

	Camera* camera;
	Geometry* sun;
	Geometry* earth;
	Geometry* moon;
	Geometry* stars;
	Geometry* mars;

	mat4 view;
	mat4 perspective;

	void updateScene();

	void initEarth();

	void initSun();

	void initMoon();

	void initStars();

	void initMars();

	void resetScene();

private:
	RenderingEngine* renderer;

	//Geometry rectangle;

	//list of objects in the scene
	std::vector<Geometry> objects;

	float earth_rotate;
	float earth_orbit;

	float moon_rotate;
	float moon_orbit;

	float mars_rotate;
	float mars_orbit;

	float sun_rotate;

	void  parseFile(FILE * filename, Geometry* g);



	void updateEarth(float e_rotate, float eo_rotate);

	void updateMoon(float m_rotate, float mo_rotate);

	void updateSun(float s_rotate);

	void updateMars(float mr_rotate, float mr_orbit);


};

#endif /* SCENE_H_ */
