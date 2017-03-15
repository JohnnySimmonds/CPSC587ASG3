#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

class Mass 
{

public:
	float getMass();
	float getVelocity();
	vec3 getPosition();
	vec3 getForce();
	vec3 getFixedPoint();
	void setVelocity(float v);
	void setPosition(vec3 p);
	void setMass(float m);
	void setForce(vec3 f);
	void setFixedPoint(vec3 fix);

private:
	float mass;
	vec3 pos;
	float velocity;
	vec3 force;
	vec3 fixedPoint;

};
