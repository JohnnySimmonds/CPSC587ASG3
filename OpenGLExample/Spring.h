#pragma once
#include "Mass.h"
#include <iostream>
using namespace std;
class Spring
{
	
public:

	Spring(vec3 massAPos, vec3 massBPos, bool setFixedA, bool setFixedB);
	~Spring();
	
	void applyForce(vec3 f, float dt);
	Mass* getMassA();
	Mass* getMassB();
	void setMassA(Mass *mA);
	void setMassB(Mass *mB);
	void setStiffness(float stiff);
	float getStiffness();
	void setRestLength(float restLen);
	float getRestLength();
	void setDampingCo(float damp);
	float getDampingCo();
	void zeroForce(Mass *mA, Mass *mB);
	void printVec3(vec3 toPrint);

private:

	Mass *massA;
	Mass *massB;
	float k; //stiffness of the spring
	float restLength;
	float dampingCo;

};
