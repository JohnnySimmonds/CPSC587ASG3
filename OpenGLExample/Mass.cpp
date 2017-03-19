#include "Mass.h"
float PI = 3.14159;
void Mass::resolveForces(float k)
{
	//pos = sin(2.0f * PI * sqrtf(mass/k)) + pos;
	pos =  ((1.0f / mass) * force) + pos;
	//pos = (1.0f / (2.0f * PI)) * sqrtf(k/mass)*force + pos;
	
}
/*Getters and Setters*/
void Mass::setFixedPoint(vec3 fix)
{
	fixedPoint = fix;
}

vec3 Mass::getFixedPoint()
{
	return fixedPoint;
}

float Mass::getVelocity()
{
	return velocity;
}
void Mass::setVelocity(float v)
{
	velocity = v;
}

vec3 Mass::getPosition()
{
	return pos;
}

void Mass::setPosition(vec3 p)
{
	pos = p;
}

vec3 Mass::getPrevPosition()
{
	return prevPos;
}

void Mass::setPrevPosition(vec3 p)
{
	prevPos = p;
}


float Mass::getMass()
{
	return mass;
}

void Mass::setMass(float m)
{
	mass = m;
}

vec3 Mass::getForce()
{
	return force;
}

void Mass::setForce(vec3 f)
{
	force = f;
}


