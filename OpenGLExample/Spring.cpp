#include "Spring.h"

float vA = 1.0f; //velocity of a
float vB = 0.5f; //velocity of b
float k = 0.5f; //stiffness of spring
float mass =  1.0f; //mass
float rL = 20.0f; //rest length
Spring::Spring(vec3 massAPos, vec3 massBPos)
{
	
	
	setMassA(&massA);
	setMassB(&massB);
	
	a->setPosition(massAPos);
	b->setPosition(massBPos);
	
	a->setFixedPoint(massAPos);
	b->setFixedPoint(massBPos);
	
	a->setMass(mass);
	b->setMass(mass);
	
	a->setVelocity(vA);
	b->setVelocity(vB);
	restLength = rL;
	
	setStiffness(k);
	
	
}
Spring::~Spring()
{
	
}
void Spring::zeroForce(Mass *mA, Mass *mB)
{
	vec3 zero = vec3(0.0f,0.0f,0.0f);
	mA->setForce(zero);
	mB->setForce(zero);
}
void Spring::applyForce(vec3 f, float dt)
{
	vec3 posA = a->getPosition();
	vec3 posB = b->getPosition();
	vec3 L = posB-posA;
	vec3 force = -k * (glm::length(L) - restLength) * (L / glm::length(L));
	zeroForce(a,b);
	force = dt*(force + f);
	//force = force;
	printVec3(force);
	b->setForce(force);
	//b->setPrevPosition(b->getPosition());
	b->setPosition(force + posB);
	
	
	//printVec3(a->getForce());
	//printVec3(b->getForce());
	
	
}

void Spring::printVec3(vec3 toPrint)
{
	cout << "X: " << toPrint.x << endl;
	cout << "Y: " << toPrint.y << endl;
	cout << "Z: " << toPrint.z << endl;
}
/*Getters and setters*/
Mass* Spring::getMassA()
{
	return a;
}
Mass* Spring::getMassB()
{
	return b;
}

void Spring::setMassA(Mass *massA)
{
	a = massA;
}
void Spring::setMassB(Mass *massB)
{
	b = massB;
}
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
