#ifndef PORTFOLIO_H
#define PORTFOLIO_H
#include <iostream>
#include <string>
#include <vector>
#include "Securities.h"

using namespace std;

class Portfolio
{
	private:
		string nameSur;
		string address;
		string tel;
		string AFM;
		vector<Securities*> S;
		bool canceled;
	public:
		Portfolio(string nS,string ad,string tl,string afm,vector<Securities *> s);
		bool operator > (Portfolio&);
		void setNameSur(string nS);
		void setAddress(string ad);
		void setTel(string tl);
		void setAFM(string afm);
		void setCanceled();
		string getNameSur();
		string getAddress();
		string getTel();
		string getAFM();
		bool getCanceled();
		vector<Securities*>& getS(); //returns REFERENCE
		float calculateCost(); //total cost of a portfolio based on individual security costs
};

#endif
