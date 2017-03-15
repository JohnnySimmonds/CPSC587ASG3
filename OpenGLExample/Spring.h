#pragma once
#include "Mass.h"
class Spring
{
	
public:
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
	


private:
	Mass *a,*b;
	float k; //stiffness of the spring
	float restLength;
	float dampingCo;

};