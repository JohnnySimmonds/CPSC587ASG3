#include "Spring.h"

float vA = 1.0f;
float vB = 0.5f;
float k = 1.0f;
float mass =  1.0f;
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
void Spring::applyForce(vec3 f)
{
	zeroForce(a,b);
	
	b->setForce(b->getForce() + f);
	b->setPosition(b->getForce() + b->getPosition());
	
	
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
