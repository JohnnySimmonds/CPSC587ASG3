#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

class Mass 
{

public:
	void resolveForces(float k);
	float getVelocity();
	void setVelocity(float v);
	float getMass();
	void setMass(float m);
	vec3 getPosition();
	void setPosition(vec3 p);
	vec3 getForce();
	void setForce(vec3 f);
	vec3 getFixedPoint();
	void setFixedPoint(vec3 fix);
	vec3 getPrevPosition();
	void setPrevPosition(vec3 p);

private:
	float mass;
	vec3 pos;
	vec3 prevPos;
	float velocity;
	vec3 force;
	vec3 fixedPoint;

};
