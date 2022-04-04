#include "Stocks.h"

Stocks::Stocks(string cN,float pV,int qu,int aY,float bV):Securities(cN,pV,qu,aY)
{
	buyValue=bV;
}

float Stocks::yearCost()
{
	return (quantity*(0.02*buyValue));
}

void Stocks::display()
{
	cout<<"Stock|"<<compName<<"|"<<pValue<<"|"<<quantity<<"|"<<acqYear<<"|"<<buyValue<<"|"<<yearCost();
}
