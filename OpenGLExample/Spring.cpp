#include "Spring.h"

vec3 vA = vec3(0.0f,0.0f,0.0f); //velocity of a
vec3 vB = vec3(0.0f,0.0f,0.0f); //velocity of b
float kA = 1.0f; //stiffness of spring
float mass =  1.0f; //mass
//float rL = -5.0f; //rest length
Spring::Spring(vec3 massAPos, vec3 massBPos)
{
	
	
	massA.setPosition(massAPos);
	massB.setPosition(massBPos);
	
	massA.setFixedPoint(massAPos);
	massB.setFixedPoint(massBPos);
	
	massA.setMass(mass);
	massB.setMass(mass);
	
	massA.setVelocity(vA);
	massB.setVelocity(vB);
	restLength = glm::length(massBPos - massAPos);
	
	setStiffness(kA);
	dampingCo = 5.0f;
	
}
Spring::~Spring()
{
	
}
void Spring::zeroForce(Mass mA, Mass mB)
{
	vec3 zero = vec3(0.0f,0.0f,0.0f);
	mA.setForce(zero);
	mB.setForce(zero);
}
/*TODO*/
void Spring::applyForce(vec3 f, float dt)
{	

	vec3 bANorm = (massB.getPosition()-massA.getPosition()) / glm::length((massB.getPosition()-massA.getPosition())); //B-A normalized
	float mB = massB.getMass();
	float rL = (glm::length((massB.getPosition()-massA.getPosition())) - restLength);
	float k = getStiffness();
	float test = (1.0f - ((dampingCo * dampingCo) / (4.0f * massB.getMass() * k)));
	/*
	printVec3(bANorm);
	cout << "Rest Length : " << rL << endl;
	cout << "Spring Stiffness : " << getStiffness() << endl;		
	*/
	//dampingCo = -1.0f * sqrtf((k/massB.getMass())) * sqrtf(test);
	//cout << "DampingCo : "<< dampingCo << endl;
	vec3 force = (-k * rL * bANorm);// - (dampingCo * massB.getVelocity());
	
	printVec3(force);
	zeroForce(massA,massB);
	
	force = force + f;
	//massA.setForce(-force);
	massB.setForce(force);
	
	//massA.resolveForces(dt);
	massB.resolveForces(dt);
	
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
Mass Spring::getMassA()
{
	return massA;
}
Mass Spring::getMassB()
{
	return massB;
}	


