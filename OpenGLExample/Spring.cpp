#include "Spring.h"


Spring::Spring(vec3 massAPos, vec3 massBPos)
{
	
	
	setMassA(&massA);
	setMassB(&massB);
	
	a->setPosition(massAPos);
	b->setPosition(massBPos);
	
	float mass =  1.0f;
	a->setMass(mass);
	b->setMass(mass);
	
	a->setVelocity(1.0f);
	b->setVelocity(1.0f);
	
	setStiffness(1.0f);
	
	
}
Spring::~Spring()
{
	
}
void Spring::applyForce()
{
	
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
