#include "Mass.h"


#include <iostream>
using namespace std;


Mass::Mass()
{
	
}
Mass::~Mass()
{
}
void Mass::resolveForces(float dt)
{
	if(!isFixed)
	{
		vec3 accel = (force/mass);
		vec3 v = velocity + (accel * dt);
		velocity = v;
		pos = pos + (v * dt);
	}
	printVec3(pos);
}
void Mass::printVec3(vec3 toPrint)
{
	cout << "X: " << toPrint.x << endl;
	cout << "Y: " << toPrint.y << endl;
	cout << "Z: " << toPrint.z << endl;
}
void Mass::setIsFixed(bool isFix)
{
	isFixed = isFix;
}
bool Mass::getIsFixed()
{
	return isFixed;
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

vec3 Mass::getVelocity()
{
	return velocity;
}
void Mass::setVelocity(vec3 v)
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


