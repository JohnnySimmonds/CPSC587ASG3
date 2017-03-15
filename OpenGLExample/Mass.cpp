#include "Mass.h"

void Mass::setVelocity(float v)
{
	velocity = v;
}

void Mass::setPosition(vec3 p)
{
	pos = p;
}

void Mass::setMass(float m)
{
	mass = m;
}

void Mass::setForce(vec3 f)
{
	force = f;
}

void Mass::setFixedPoint(vec3 fix)
{
	fixedPoint = fix;
}

float Mass::getVelocity()
{
	return velocity;
}

vec3 Mass::getPosition()
{
	return pos;
}

float Mass::getMass()
{
	return mass;
}

vec3 Mass::getForce()
{
	return force;
}

vec3 Mass::getFixedPoint()
{
	return fixedPoint;
}
