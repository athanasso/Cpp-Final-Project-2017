#include "Portfolio.h"

Portfolio::Portfolio(string nS,string ad,string tl,string afm,vector<Securities *> s)
{
	nameSur=nS;
	address=ad;
	tel=tl;
	AFM=afm;
	S=s;
	canceled=false;
}

bool Portfolio::operator > (Portfolio& other)
{
	return calculateCost()>other.calculateCost();
}

void Portfolio::setNameSur(string nS)
{
	nameSur=nS;
}

void Portfolio::setAddress(string ad)
{
	address=ad;
}

void Portfolio::setTel(string tl)
{
	tel=tl;
}

void Portfolio::setAFM(string afm)
{
	AFM=afm;
}

void Portfolio::setCanceled()
{
	canceled=true;
}

string Portfolio::getNameSur()
{
	return nameSur;
}

string Portfolio::getAddress()
{
	return address;
}

string Portfolio::getTel()
{
	return tel;
}

string Portfolio::getAFM()
{
	return AFM;
}

bool Portfolio::getCanceled()
{
	return canceled;
}

vector<Securities*>& Portfolio::getS()
{
	return S;
}

float Portfolio::calculateCost() //total cost of a portfolio based on individual security costs
{
	float totalCost=0;

	if (S.empty())
		return 0;
	else
		for (int i=0;i<S.size();i++)
			totalCost+=S[i]->yearCost();

	return totalCost;
}
