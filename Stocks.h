#ifndef STOCKS_H
#define STOCKS_H
#include <iostream>
#include "Securities.h"

using namespace std;

class Stocks:public Securities
{
	private:
		float buyValue;  //We use float variables instead of double for monetary variables since there won't be too many numbers after the floating point and we use less memory this way
	public:
		Stocks(string cN,float pV,int qu,int aY,float bV);
		float yearCost();
		void display();
};

#endif
