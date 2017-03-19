#include "Spring.h"

float vA = 1.0f; //velocity of a
float vB = 0.5f; //velocity of b
float kA = 0.5f; //stiffness of spring
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
	printVec3(bANorm);
	cout << "Rest Length : " << rL << endl;
	cout << "Spring Stiffness : " << getStiffness() << endl;	
			
	vec3 force = -1.0f * k * rL * bANorm;

	zeroForce(massA,massB);
	
	force = dt * (force + f);
	massA.setForce(-force);
	massB.setForce(force);
	
	massA.resolveForces(k);
	massB.resolveForces(k);
	
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


