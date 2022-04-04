#ifndef SECURITIES_H
#define SECURITIES_H
#include <iostream>
#include <string>

using namespace std;

class Securities //abstract class
{
	protected:
		string compName;
		float pValue; //We use float variables instead of double for monetary variables since there won't be too many numbers after the floating point and we use less memory this way
		int quantity;
		int acqYear;
	public:
		Securities(string cN,float pV,int qu,int aY);
		virtual void display()=0; //for some reason during our early builds if we had display prototyped below yearcost in this class the bonds didn't display, but later on the issue got resolved on its own
		virtual float yearCost()=0; //calculates cost in each security (stock yearcost and bond yearcost is calculated differently)
};

#endif
