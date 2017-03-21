#include "Spring.h"

vec3 vA = vec3(0.0f,0.0f,0.0f); //velocity of a
vec3 vB = vec3(0.0f,0.0f,0.0f); //velocity of b
float kA = 1.0f; //stiffness of spring
float mass =  1.0f; //mass
#include <math.h>
Spring::Spring(vec3 massAPos, vec3 massBPos, bool setFixedA, bool setFixedB)
{
	
	massA = new Mass();
	massB = new Mass();
	
	massA->setPosition(massAPos);
	massB->setPosition(massBPos);
	
	massA->setFixedPoint(massAPos);
	massB->setFixedPoint(massBPos);
	
	massA->setMass(mass);
	massB->setMass(mass);
	
	massA->setVelocity(vA);
	massB->setVelocity(vB);
	
	massA->setIsFixed(setFixedA);
	massB->setIsFixed(setFixedB);
	restLength = glm::length(massBPos - massAPos);
	
	setStiffness(kA);
	dampingCo = 5.0f;
	
}
Spring::~Spring()
{
	delete massA;
	delete massB;
}
void Spring::zeroForce(Mass *mA, Mass *mB)
{
	vec3 zero = vec3(0.0f,0.0f,0.0f);
	mA->setForce(zero);
	mB->setForce(zero);
}
/*TODO*/
void Spring::applyForce(vec3 f, float dt)
{	

	vec3 bANorm = (massB->getPosition()-massA->getPosition()) / glm::length((massB->getPosition()-massA->getPosition())); //B-A normalized
	float rL = (glm::length((massB->getPosition()-massA->getPosition())) - restLength);
	float amp = 0.85f;
	float k = getStiffness();
	vec3 forceA;
	vec3 forceB;

	dampingCo =  amp * exp(-dt) * cos(2.0f * 3.14159f * dt);
	if(rL < -1.0f)
	{
		cout << "Rest Length : " << rL << endl;
	}
		
	
	forceA = (-k * rL * bANorm) - (dampingCo * (massA->getVelocity()));
	
	forceB = (-k * rL * bANorm) - (dampingCo * (massB->getVelocity()));

	zeroForce(massA,massB);
	
	forceB = forceB + (f*massB->getMass());
	forceA = forceA + (f*massA->getMass());
	massA->setForce(forceA);
	massB->setForce(forceB);
	
	massA->resolveForces(dt);
	massB->resolveForces(dt);
	
}

void Spring::unCalced()
{
	massA->setCalced(false);
	massB->setCalced(false);
}
void Spring::printVec3(vec3 toPrint)
{
	cout << "X: " << toPrint.x << endl;
	cout << "Y: " << toPrint.y << endl;
	cout << "Z: " << toPrint.z << endl;
}
/*Getters and setters*/

void Spring::setStiffness(float stiff)
{
	k = stiff;
}
float Spring::getStiffness()
{
	return k;
}

void Spring::setRestLength(float restLen)
{
	restLength = restLen;
}
float Spring::getRestLength()
{
	return restLength;
}
void Spring::setDampingCo(float damp)
{
	dampingCo = damp;
}
float Spring::getDampingCo()
{
	return dampingCo;
}
Mass* Spring::getMassA()
{
	return massA;
}
Mass* Spring::getMassB()
{
	return massB;
}	
void Spring::setMassA(Mass *mA)
{
	massA = mA;
}
void Spring::setMassB(Mass *mB)
{
	massB = mB;
}

