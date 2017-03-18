#pragma once
#include "Mass.h"
#include <iostream>
using namespace std;
class Spring
{
	
public:

	Spring(vec3 massAPos, vec3 massBPos);
	~Spring();
	
	void applyForce(vec3 f);
	Mass* getMassA();
	Mass* getMassB();
	void setMassA(Mass *massA);
	void setMassB(Mass *massB);
	void setStiffness(float stiff);
	float getStiffness();
	void setRestLength(float restLen);
	float getRestLength();
	void setDampingCo(float damp);
	float getDampingCo();
	void zeroForce(Mass *mA, Mass *mB);
	void printVec3(vec3 toPrint);

private:
	Mass massA;
	Mass massB;
	Mass *a, *b;
	float k; //stiffness of the spring
	float restLength;
	float dampingCo;

};
