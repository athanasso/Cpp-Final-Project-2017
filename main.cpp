//!!! Mr. Meletiou allowed us to use a couple of C++ 11 functions, devc++ is by default set to the 98 version of the compiler, to enable it go to\
  Tools -> Compiler Options -> Programs and ADD " -std=c++11" (with the space at the start, without the quotation marks) at the end of the gcc and g++ textboxes !!!
#include <iostream>
#include <fstream>
#include <string> 
#include <vector>
#include <exception> //used for stoi and stof exceptions
#include <typeinfo> //used to get exception type of stoi and stof exceptions
#include <cstring> //used to use strcmp to determine what kind of exception is thrown by stoi and stof and print the corresponding message
#include <algorithm> //used for sorting our vectors
#include <limits> //used for cin.ignore()
#include <cstdlib> //used for exit()

#include "Portfolio.h"   //
#include "Securities.h" //including our class files
#include "Stocks.h"    //
#include "Bonds.h"    //

using namespace std;

//definitions for colour coding and formatting screen outputs

#define UNDERON "\x1b[4m" //underlined text ON
#define UNDEROFF "\x1b[24m" //underlined text OFF
#define WHITE "\x1b[97m" //bright white text color
#define GREEN "\x1b[92m" //green text color
#define YELLOW "\x1b[93m" //yellow text
#define RED "\x1b[91m" //red text
#define CYAN "\x1b[96m" //cyan text
#define C_Reset "\x1b[0m" //reset screen output to default

//quality of life defines

#define horLine(n) for (int i=0;i<n;i++) cout<<(char)205; //prints a horizontal double line of n length
#define FIX_INPUT cin.clear();cin.ignore(numeric_limits<streamsize>::max(),'\n'); //clears error flag and the input in case of input failure, not worth to be put in a function so just defined here
#define CURYEAR 2018 //current year
#define WIPE_SCR system("cls"); //shorter version of system("cls");, you can change cls to clear here for it to work on linux

//function prototypes indented to show where each function is used with proper indentations

void menu(); //this is the main menu() function
	void portData(vector<Portfolio>&); //option 1 of main menu() (create a new portfolio and input that into the vector)
		vector<Securities*> subMenu(); //called by portData() to choose from inputting Security data from the keyboard or a file
			vector<Securities*> secInput(); //option 1 of subMenu() (start process of inputting Security data from keyboard)
				//inputStock() used here //option 1 of secInput()
				//inputBond() used here //option 2 of secInput()
			vector<Securities*> fileInput(); //option 2 of subMenu() (start process of inputting Security data from file)
				//fileOpen() used here //calls getFileName() and opens the file with the given name for reading
				//fileStock() used here //"grabs" Stock data from the given file
				//fileBond() used here //"grabs" Bond data from the given file
	void portChoice(vector<Portfolio>&,int); //option 2 of main menu() (edit data of an existing portfolio)
		void editMenu(Portfolio&); //lets user choose what kind of data altering they want to do to the portfolio
			void infoMenu(Portfolio&); //option 1 of editMenu() (change basic info of a Portfolio's holder)
				string newInfo(int); //just adding more detail to the basic info alteration process from above
			void addSecurity(Portfolio &,int); //both option 2 and 3 of editMenu()
				//inputStock() used here //option 2 of editMenu() (add Securities from the keyboard) and option 1 of addSecurity() (add a Stock)
				//inputBond() used here //option 2 of editMenu() (add Securities from the keyboard) and option 2 of addSecurity() (add a Bond)
				void fileCheck(vector<Securities*>&,int); //option 3 of editMenu() (add Securities from a file)
					//fileOpen() used here //calls getFileName() and opens the file with the given name for reading
					//fileStock() used here //"grabs" Stock data from the given file
					//fileBond() used here //"grabs" Bond data from the given file
			void removeSec(Portfolio&); //option 4 of editMenu()
	//portChoice() is also used here //option 3 of main menu() (cancel a portfolio so it's not taken into consideration for option 4 of main menu
	void statsMenu(vector<Portfolio>&); //option 4 of main menu() (display statistics)
		void portCostCount(vector<Portfolio>&); //option 1 of statsMenu(), displays the amount of portfolios that have a higher cost than the user-given X variable
		void secCostCount(vector<Portfolio>&); //option 2 of statsMenu(), displays the amount of portfolios that have atleast 1 security with a cost higher than the user-given X variable
		void topPorts(vector<Portfolio>); //option 3 of statsMenu(), displays the info of the top 10 most expensive portfolios in the vector of portfolios
			bool sortDescending(Portfolio&,Portfolio&); //function to be used by sort() to sort in descending order ( by calculateCost() )
			void displayTop(Portfolio&); //displays full data of a portfolio
	void exitMsg(); //displays final message before exiting the program

int countCanceled(vector<Portfolio>&); //"basic" function. Counts all canceled portfolios.
float getX(); //"basic" function. Asks the user for the X ( called by 4th option of main menu() )
void inputStock(vector<Securities*>&); //"basic" function used a few times in the program for inputting Stock data from the keyboard
void inputBond(vector<Securities*>&); //"basic" function used a few times in the program for inputting Bond data from the keyboard
void fileOpen(ifstream&); //"basic" function. Opens a file we want to access for reading ( name given by the function called in fileOpen(), called getFileName() )
	string getFileName(); //"basic" function. Asks the user for the name of the file they want to access ( called by fileOpen() )
void fileStock(vector<Securities*>&,ifstream&); //"basic" function used a few times in the program for inputting Stock data from a file
void fileBond(vector<Securities*>&,ifstream&); //"basic" function used a few times in the program for inputting Bond data from a file

//main

int main(int argc, char* argv[])
{
	WIPE_SCR //for colors to work
	menu(); //no need to create a vector of Portfolios in main() since its not used anywhere else other than the menu itself \
			(if there would be a need for it to be created in main then we can just pass it as an argument of the menu() function)
	
	system("pause");
	exit(0);
}

//unique functions

void menu()
{
	cout<<WHITE; //THIS PROGRAM'S SCREEN OUTPUTS HEAVILY RELY ON TABLE CHARACTERS AND COLOURED DISPLAY that we DON'T know if they work on LINUX
	vector<Portfolio> P; //no need to create a vector of Portfolios in main() since its not used anywhere else other than the menu itself
    int choice,i=-1;

    while(true)
    {
		cout<<"\tMain Menu"<<endl;

		horLine(28)
		cout<<endl;

        cout<<"["<<GREEN<<"1"<<WHITE<<"]Create portfolio."<<endl; //question number 1 of final project
        if (P.empty()) //grey out unusable options
        {
			cout<<C_Reset<<"[2]Edit portfolio."<<endl; //question number 2 of final project
        	cout<<"[3]Cancel portfolio."<<endl; //question number 3 of final project
			cout<<"[4]Display statistical data."<<WHITE<<endl; //question number 4 of final project
        }
        else if (!(P.empty()) && (countCanceled(P)==P.size())) //grey out unusable options
        {
			cout<<"["<<GREEN<<"2"<<WHITE<<"]Edit portfolio."<<endl; //question number 2 of final project
			cout<<"["<<GREEN<<"3"<<WHITE<<"]Cancel portfolio."<<endl; //question number 3 of final project
			cout<<C_Reset<<"[4]Display statistical data."<<WHITE<<endl; //question number 4 of final project
        }        	
		else
        {
			cout<<"["<<GREEN<<"2"<<WHITE<<"]Edit portfolio."<<endl; //question number 2 of final project
			cout<<"["<<GREEN<<"3"<<WHITE<<"]Cancel portfolio."<<endl; //question number 3 of final project
			cout<<"["<<GREEN<<"4"<<WHITE<<"]Display statistical data."<<endl; //question number 4 of final project
        }
		cout<<"["<<RED<<"5"<<WHITE<<"]EXIT."<<endl; //exit/close/(finish) the program

		horLine(28)
		cout<<endl;

		cout<<"Give choice from 1 to 5: ";
        cin>>choice;
        cout<<endl;
		WIPE_SCR

		FIX_INPUT

        switch(choice)
        {
            case 1:
            	portData(P); //question number 1 of final project
            	i++;
            	WIPE_SCR
				break;
            case 2:
				portChoice(P,choice); //question number 2 of final project
				break;
            case 3:
				portChoice(P,choice); //question number 3 of final project
				break;
            case 4: 
				statsMenu(P); //question number 4 of final project
				break;
			case 5:
				exitMsg();
				return;
			default:
				cout<<YELLOW<<"*Please choose an option from 1 to 5.*"<<WHITE<<endl<<endl;
        }
    }
}

void portData(vector<Portfolio>& P)
{
    string nS,ad,tl,afm; //give portfolio's owner basic info

	cout<<"Give full name: ";
	getline(cin,nS);
	cout<<endl;	

	cout<<"Give address: ";
	getline(cin,ad);
	cout<<endl;	

	cout<<"Give phone number: ";
	getline(cin,tl);
	cout<<endl;
	while(tl.length()>15)
	{
		cout<<YELLOW<<"Give a correct phone number (must be under 15 characters): "<<WHITE;
		getline(cin,tl);
		cout<<endl;
	}

	cout<<"Give A.F.M.: ";
	getline(cin,afm);
	cout<<endl;	
	while(afm.length()!=9)
	{
		cout<<YELLOW<<"Give a correct A.F.M. (must be 9 characters): "<<WHITE;
		getline(cin,afm);
		cout<<endl;
	}

	WIPE_SCR

	P.push_back(Portfolio(nS,ad,tl,afm,subMenu())); //creates the portfolio in the vector of portfolios

	/*We had an idea for better memory managment, to make subMenu(), secInput() and fileInput() 
	return void but take a reference of a tempS, that is created inside portData(),as a paremeter
	and then we gave the Portfolio constructor that tempS instead of subMenu() like we do here 
	but then, AFTER the consturctor got called (which theoretically means there is a Portfolio object
	with the data provided by the user, inside the vector<Portfolio> P now) delete what is inside 
	the tempS and then clear it since everything in it was created using "new". Supposedly now there is
	a Portfolio object in the vector AND there is no memory leak, HOWEVER, if we did this (or create a
	destructor for Portfolio which deletes everything inside its vector<Securities*> S) then we had major
	problems, whenever we tried to use either a Stock's or a Bond's display (for example in removeSec() or any 
	of the statsMenu() options it said pure virtual method called (insinuating we called the abstract 
	class Securities display) and we had no idea why so we stuck with this*/
}

vector<Securities*> subMenu()
{
    int choice;

    while(true) //choose which way you'll input the security data
    {
    	cout<<"Input securities to the portfolio:"<<endl;

		horLine(21)
		cout<<endl;

		cout<<"["<<GREEN<<"1"<<WHITE<<"]From the keyboard."<<endl;
        cout<<"["<<GREEN<<"2"<<WHITE<<"]From a file."<<endl;

    	horLine(21)
		cout<<endl;

        cout<<"Give choice from 1 to 2: ";
        cin>>choice;
        cout<<endl;
		WIPE_SCR

		FIX_INPUT

        switch(choice)
        {
            case 1:
                return secInput(); //inputs security data from keyboard
            case 2:
            	try
            	{
                	return fileInput(); //inputs security data from file
            	}
				catch(int x)
			    {
			    	cout<<RED<<UNDERON;
			        if (x==1)
			            cout<<"*Incorrect par value was read from the file, action terminated, please choose input method again.*"<<endl<<endl;
			        else if (x==2)
			            cout<<"*Incorrect quantity was read from the file, action terminated, please choose input method again.*"<<endl<<endl;
			        else if (x==3)
			            cout<<"*Incorrect acqusition year was read from the file, action terminated, please choose input method again.*"<<endl<<endl;
			        else if (x==4)
			            cout<<"*Incorrect buy value was read from the file, action terminated, please choose input method again.*"<<endl<<endl;
					else if (x==5)
			            cout<<"*Incorrect expiration year was read from the file, action terminated, please choose input method again.*"<<endl<<endl;
			        else if (x==6)
			            cout<<"*Incorrect interest rate was read from the file, action terminated, please choose input method again.*"<<endl<<endl;
			        else if (x==7)
			        	cout<<"*File is empty(or nothing was read due to incorrect formatting), action terminated, please choose input method again.*"<<endl<<endl;
			    	cout<<WHITE<<UNDEROFF;
				}
			    catch(exception& e)
			    {
			    	cout<<RED<<UNDERON;
			    	if ((strcmp(typeid(e).name(),"St16invalid_argument"))==0) //if stoi/stof throw exception for invalid conversion argument
						cout<<"*There is a character in place of a number in your file, action terminated, please choose input method again.*"<<endl<<endl;
					else if ((strcmp(typeid(e).name(),"St12out_of_range"))==0) //if stoi/stof throw exception that the item to be converted is out of range for the corresponding variable type (int for stoi, float for stof)
						cout<<"*One of the numbers in the file is abnormaly large, action terminated, please choose input method again.*"<<endl<<endl;
					cout<<WHITE<<UNDEROFF;
				}
				break;		
            default:
                cout<<YELLOW<<"*Please choose an option from 1 to 2.*"<<WHITE<<endl<<endl;
                break;
        }
    }
}

vector<Securities*> secInput()
{
	int choice;

	vector<Securities*> tempS;

	while(true) //choose which kind of security you'd like to input from the keyboard (option 3 finishes the input process and returns the vector made up of securities)
	{
		horLine(60)
		cout<<endl;

		cout<<"["<<GREEN<<"1"<<WHITE<<"]Input stock data."<<endl;
		cout<<"["<<GREEN<<"2"<<WHITE<<"]Input bond data."<<endl;
		cout<<"["<<YELLOW<<"3"<<WHITE<<"](I am done inputting security data) Go back to main menu."<<endl;

		horLine(60)
		cout<<endl;

		cout<<"Give choice from 1 to 3: ";
        cin>>choice;
        cout<<endl;
		WIPE_SCR

		FIX_INPUT

        if (choice==3)
        	return tempS;

		switch(choice)
		{
			case 1:
				inputStock(tempS); //input a stock
				WIPE_SCR
				break;
			case 2:
				inputBond(tempS); //input a bond
				WIPE_SCR
				break;
			default:
				cout<<YELLOW<<"*Please choose an option from 1 to 3.*"<<WHITE<<endl<<endl;
				break;
		}
	}
	return tempS;
}

vector<Securities*> fileInput() 
{
	ifstream file;	
	fileOpen(file); //opens file

	char c; //variable used to check whether we are reading a Stock or a Bond from the file \
			(the final project specified that the file will always have them in 1 specific and correct format
	vector<Securities*> tempS;

	while(file.good())
	{
		file.get(c); //gets the Security defining character
		file.get(); //discards the next line
		if (c=='M' || c=='Ì') //if a Stock is being read (English M or Greek M), then grab its data
			fileStock(tempS,file);
		if (c=='O' || c=='Ï') //if a Bond is being read (English O or Greek O), then grab its data
			fileBond(tempS,file);
	}

    if (tempS.empty())
        throw 7;

	file.close(); //closes the file since we don't need it anymore
	return tempS;
}

void portChoice(vector<Portfolio>& P,int cho)
{
	while(true)
	{
		int Psz=P.size(); //for ease
		int choice,i;
	
		if (P.empty()) //if there are no portfolios
		{
			cout<<YELLOW<<"*There are no portfolios to choose from, make some first and then try to choose this option.*"<<WHITE<<endl<<endl;
			return;
		}
		else
		{
			horLine(15)
			cout<<endl;
	
			cout<<YELLOW<<"0"<<WHITE<<") Go back."<<endl<<"Or choose one of the following portfolios."<<endl;
			for (i=0;i<P.size();i++)
			{
				if (P[i].getCanceled())
					cout<<C_Reset<<i+1<<") "<<P[i].getNameSur()<<"'s portfolio"<<endl;
				else
					cout<<GREEN<<i+1<<WHITE<<") "<<P[i].getNameSur()<<"'s portfolio"<<endl;
			}
	
			horLine(15)
			cout<<endl;

			cout<<GREEN<<"(Please choose the number that is displayed beside the portfolio you want to manage)"<<WHITE<<endl;
			cout<<YELLOW<<"Canceled Portfolios are greyed out."<<WHITE<<endl;

			horLine(15)
			cout<<endl;
		}
	
		cout<<"Give choice from 0 to "<<i<<": ";
		cin>>choice;
		cout<<endl;		
		while(cin.fail() || (choice>P.size()) || (choice<0)) //input needs to be correct (numeral) and inside the boundaries
		{
			cout<<YELLOW<<"Give correct choice from 0 to "<<i<<": "<<WHITE;
			FIX_INPUT
			cin>>choice;
	       	cout<<endl;
	    }
	
		WIPE_SCR
	
		if (choice==0) //go back
			return;
	
		if (cho==2) //question number 2 of final project
			editMenu(P[choice-1]);
		if (cho==3) //question number 3 of final project
		{
			if (P[choice-1].getCanceled())
				cout<<YELLOW<<"Chosen portfolio is already canceled, please choose another one."<<WHITE<<endl;
			else
				P[choice-1].setCanceled();
		}
	}
}

void editMenu(Portfolio &port)
{
	int choice;

	while(true) //editing menu
	{
		horLine(36)
		cout<<endl;

		cout<<"["<<GREEN<<"1"<<WHITE<<"]Change person's information."<<endl;
		cout<<"["<<GREEN<<"2"<<WHITE<<"]Add securities from the keyboard."<<endl;
		cout<<"["<<GREEN<<"3"<<WHITE<<"]Add securities from a file."<<endl;
		if (port.getS().empty())
			cout<<C_Reset<<"[4]Remove stock or bond."<<WHITE<<endl;
		else
			cout<<"["<<GREEN<<"4"<<WHITE<<"]Remove stock or bond."<<endl;
		cout<<"["<<YELLOW<<"5"<<WHITE<<"]Go back."<<endl;

		horLine(36)
		cout<<endl;

		cout<<"Give choice from 1 to 5: ";
        cin>>choice;
        cout<<endl;
		WIPE_SCR

		FIX_INPUT

		switch(choice)
		{
			case 1:
				infoMenu(port); //change the portfolio owner's info
				break;
			case 2:
				addSecurity(port,choice); //will call addSecurity() to add from keyboard
				break;
			case 3:
				addSecurity(port,choice+1); //will call addSecurity() to add from file
				break;
			case 4:
				removeSec(port); //remove a security
				break;
			case 5:
				return;
			default:
				cout<<YELLOW<<"*Please choose an option from 1 to 5.*"<<WHITE<<endl<<endl;
				break;
		}
	}
}

void infoMenu(Portfolio& port)
{
	int choice; //continuation of changing portfolio's owner info

	while(true)
	{
		horLine(27)
		cout<<endl;

		cout<<"["<<GREEN<<"1"<<WHITE<<"]Change name and surname."<<endl;
		cout<<"["<<GREEN<<"2"<<WHITE<<"]Change address."<<endl;
		cout<<"["<<GREEN<<"3"<<WHITE<<"]Change telephone."<<endl;
		cout<<"["<<GREEN<<"4"<<WHITE<<"]Change A.F.M.."<<endl;
		cout<<"["<<YELLOW<<"5"<<WHITE<<"]Go back."<<endl;

		horLine(27)
		cout<<endl;

		cout<<"Give choice from 1 to 5: ";
        cin>>choice;
        cout<<endl;
		WIPE_SCR

		FIX_INPUT
        
		switch(choice)
		{
			case 1:
				port.setNameSur(newInfo(choice)); //change name and surname
				break;
			case 2:
				port.setAddress(newInfo(choice)); //change address
				break;
			case 3:
				port.setTel(newInfo(choice)); //change phone number
				while(port.getTel().length()>15) //if the check would have been done inside new info i wouldn't need getters but i'd have to use the getline in that function in every case which looked ugly and seemed less practical.
				{
					port.setTel(newInfo(choice+2));
				}
				break;
			case 4:
				port.setAFM(newInfo(choice)); //change AFM
				while(port.getAFM().length()!=9) //same as the above while
				{
					port.setAFM(newInfo(choice+2));
				}
				break;
			case 5:
				return;
			default:
				cout<<YELLOW<<"*Please choose an option from 1 to 5.*"<<WHITE<<endl<<endl;
				break;
		}
	}
}

string newInfo(int cho)
{
	string temp;

	switch(cho) //gives correct message for changing info
	{
		case 1:
			cout<<"Give new name and surname: ";
			break;
		case 2:
			cout<<"Give new address: ";
			break;
		case 3:
			cout<<"Give new telephone: ";	
			break;	
		case 4:
			cout<<"Give new A.F.M.: ";
			break;
		case 5:
			cout<<YELLOW<<"Give a correct new telephone (must be under 15 characters): "<<WHITE;
			break;
		case 6:
			cout<<YELLOW<<"Give a correct new A.F.M. (must 9 characters): "<<WHITE;
			break;
	}
	getline(cin,temp);
	WIPE_SCR

	return temp;
}

void addSecurity(Portfolio& port,int cho)
{
	vector<Securities*>& PSV=port.getS();
	int choice;

	while(true)
	{
		horLine(16)
		cout<<endl;
		
		cout<<"["<<GREEN<<"1"<<WHITE<<"]Add stock(s)."<<endl;
		cout<<"["<<GREEN<<"2"<<WHITE<<"]Add bond(s)."<<endl;
		cout<<"["<<YELLOW<<"3"<<WHITE<<"]Go back."<<endl;

		horLine(16)
		cout<<endl;

		cout<<"Give choice from 1 to 3: ";
        cin>>choice;
        cout<<endl;
		WIPE_SCR

		FIX_INPUT

		if (choice==0) //for added security (used manly for if the user chooses "add from file" in editMenu()and inputs a character instead of a number \
						(we found that after the user inputs a character when the program asks for a numeric value and after we call cin.clear() and cin.ignore() the value is set to 0)
        {
			cout<<YELLOW<<"*Please choose an option from 1 to 3.*"<<WHITE<<endl<<endl;
            continue;
        }

        if (choice==3) //go back
        	return;

		switch(choice+cho)
		{
			case 3:
				inputStock(PSV); //add stock from keyboard
				WIPE_SCR
				break;	
			case 4:
				inputBond(PSV); //add bond from keyboard
				WIPE_SCR
				break;
			case 5:
			case 6:
				try
				{
					fileCheck(PSV,choice+cho); //add stock or bond from file
					WIPE_SCR
				}
				catch(int x)
			    {
			    	cout<<RED<<UNDERON;
			        if (x==1)
			            cout<<"*Incorrect par value was read from the file, action terminated, please choose security type to add again.*"<<endl<<endl;
			        else if (x==2)
			            cout<<"*Incorrect quantity was read from the file, action terminated, please choose security type to add again.*"<<endl<<endl;
			        else if (x==3)
			            cout<<"*Incorrect acqusition year was read from the file, action terminated, please choose security type to add again.*"<<endl<<endl;
			        else if (x==4)
			            cout<<"*Incorrect buy value was read from the file, action terminated, please choose security type to add again.*"<<endl<<endl;
					else if (x==5)
			            cout<<"*Incorrect expiration year was read from the file, action terminated, please choose security type to add again.*"<<endl<<endl;
			        else if (x==6)
			            cout<<"*Incorrect interest rate was read from the file, action terminated, please choose security type to add again.*"<<endl<<endl;
			        else if (x==7)
			        	cout<<"*File is completely empty(or nothing was read due to incorrect formatting), action terminated, please choose security type to add again.*"<<endl<<endl;
			        else if (x==8)
			        	cout<<"*File didn't have any entries corresponding to the security type you chose to insert into the portfolio, action terminated, please choose security type to add again.*"<<endl<<endl;
					cout<<WHITE<<UNDEROFF;
				}
			    catch(exception& e)
			    {
			    	cout<<RED<<UNDERON;
			    	if ((strcmp(typeid(e).name(),"St16invalid_argument"))==0) //if stoi/stof throw exception for invalid conversion argument
						cout<<"*There is a character in place of a number in your file, action terminated, please choose security type to add again.*"<<endl<<endl;
					else if ((strcmp(typeid(e).name(),"St12out_of_range"))==0) //if stoi/stof throw exception that the item to be converted is out of range for the corresponding variable type (int for stoi, float for stof)
						cout<<"*One of the numbers in the file is abnormaly large, action terminated, please choose security type to add again.*"<<endl<<endl;
					cout<<RED<<UNDEROFF;
				}
				break;
			default:
				cout<<YELLOW<<"*Please choose an option from 1 to 3.*"<<WHITE<<endl<<endl;
				break;
		}
	}
}

void fileCheck(vector<Securities*>& S,int cho)
{
	ifstream file;
	fileOpen(file); //opens file

	char c;
	int femp=S.size();
	vector<Securities*> dump;

	while(file.good())
	{
		file.get(c);
		file.get();

		if (c=='M' || c=='Ì')
		{
			if (cho==5) //if a stock, and user has chosen add stock then it adds it
				fileStock(S,file);
			else if (cho==6) ////if a stock, and user has chosen add bond then it discards it
				fileStock(dump,file);
		}
		else if (c=='O' || c=='Ï')
		{
			if (cho==6) //if a bond, and user has chosen add bond then it adds it
				fileBond(S,file);
			else if (cho==5) //if a bond, and user has chosen add stock then it discards it
				fileBond(dump,file);
		}
	}

	if ((S.size()==femp) && (dump.empty()))
		throw 7;
	if ((S.size()==femp) && !(dump.empty()))
		throw 8;

	for (int i=0;i<dump.size();i++)
		delete dump[i];
	dump.clear();

	file.close(); //closes file
}

void removeSec(Portfolio& port)
{
	vector<Securities*>& PSV=port.getS(); //for ease
	int Ssize=PSV.size(); //for ease
	int choice,i;

	if (PSV.empty()) //if there are no securities in this portfolio
	{
		cout<<YELLOW<<"*This portfolio has no securities in it.*"<<WHITE<<endl<<endl;
		return;
	}
	else
	{
		cout<<C_Reset<<"Securities are displayed as: "<<endl;
		cout<<"\tStock|Company name|Par value|Quantity|Acquisition year|Purchase value|Cost"<<endl;
		cout<<"\tBond|Company name|Par value|Quantity|Acquisition year|Expiration year(in % percentage)|Interest rate|Cost"<<WHITE<<endl;

		horLine(15)
		cout<<endl;

		cout<<YELLOW<<"0"<<WHITE<<") Go back."<<endl<<"Or choose one of the following securities to remove from the portfolio."<<endl;
		for (i=0;i<Ssize;i++)
		{
			cout<<GREEN<<i+1<<WHITE<<") ";
			PSV[i]->display();
			cout<<endl;
		}
		cout<<RED<<i+1<<WHITE<<") Remove "<<RED<<"ALL"<<WHITE<<"."<<endl;

		horLine(15)
		cout<<endl;

		cout<<GREEN<<"(Please choose the number that is displayed beside the data of the security you want to remove.)"<<WHITE<<endl;

		horLine(15)
		cout<<endl;
	}

	cout<<"Give choice from 0 to "<<i+1<<": ";
	cin>>choice;
    cout<<endl;
	while(cin.fail() || (choice>Ssize+1) || (choice<0)) //input needs to be correct (numeral) and inside the boundaries
	{
		cout<<YELLOW<<"Give correct choice from 0 to "<<i+1<<": "<<WHITE;
		FIX_INPUT
		cin>>choice;
		cout<<endl;
    }
	WIPE_SCR

	if (choice==0)
		return;

	if (choice==i+1)
	{
		PSV.clear();
		return;
	}

	PSV.erase(PSV.begin()+(choice-1)); //deletes the selected security
}

void statsMenu(vector<Portfolio>& P) //reference so no extra memory is allocated (that would happen if we passed by value) (tried using const but erorrs came out even if we didn't change any member variables),\
									   the vector of Portfolios here is given by value not by reference so the order doesn't get disrupted during sorting, plus there are no changes needed to be made here
{
	int choice;

	if (P.empty()) //if there are no portfolios
	{
		cout<<YELLOW<<"*There are no portfolios to display, make some first and then try to choose this option.*"<<WHITE<<endl<<endl;
		return;
	}

	if (countCanceled(P)==P.size()) //if all portfolios are cancelled
	{
		cout<<YELLOW<<"*There are no uncanceled portfolios to display.*"<<WHITE<<endl<<endl;
		return;
	}

	while(true)
	{
		horLine(116)
		cout<<endl;

		cout<<"["<<GREEN<<"1"<<WHITE<<"]Display the amount of portfolios whose cost is higher than X (X is input by you)."<<endl;
		cout<<"["<<GREEN<<"2"<<WHITE<<"]Display the amount of portfolios that have at least one security whose cost is higher than X (X is input by you)."<<endl;
		cout<<"["<<GREEN<<"3"<<WHITE<<"]Display the data of the 10 most expensive portfolios."<<endl;
		cout<<"["<<YELLOW<<"4"<<WHITE<<"]Go back."<<endl;

		horLine(116)
		cout<<endl;

		cout<<GREEN<<"*Cancelled portfolios are not taken into consideration for these displays*"<<WHITE<<endl; //as per final project request, also the final project did NOT ask us to have an option to un-cancel a portfolio

		horLine(116)
		cout<<endl;

		cout<<"Give choice from 1 to 4: ";
		cin>>choice;
		cout<<endl;
		WIPE_SCR

		FIX_INPUT

		switch(choice)
		{
			case 1:
				portCostCount(P); //portfolios with total cost higher than X
				break;
			case 2:
				secCostCount(P); //portfolios with securities with cost higher than X
				break;
			case 3:
				topPorts(P); //top 10 portfolios
				break;
			case 4:
				return;
			default:
				cout<<YELLOW<<"*Please choose an option from 1 to 4.*"<<WHITE<<endl<<endl;
		}
	}
}

void portCostCount(vector<Portfolio>& P) //passed by reference and not by value even if we don't modify the vector so we use less memory
{
	int k=0;
	float x=getX();

	for (int i=0;i<P.size();i++)	
		if (P[i].calculateCost()>x && !(P[i].getCanceled()))
			k++;

	cout<<GREEN<<"The amount of portfolio objects whose cost is higher than "<<x<<" is: "<<RED<<k<<WHITE<<endl;
}

void secCostCount(vector<Portfolio>& P) //passed by reference and not by value even if we don't modify the vector so we use less memory
{
	int k=0;
	float x=getX();

	for (int i=0;i<P.size();i++)
		for (int j=0;j<P[i].getS().size();j++)
			if ((P[i].getS()[j]->yearCost())>x && !(P[i].getCanceled()))
			{
				k++;
				break;
			}

	cout<<GREEN<<"The amount of portfolio objects which have atleast 1 security with a cost higher than "<<x<<" is: "<<RED<<k<<WHITE<<endl;
}

void topPorts(vector<Portfolio> P) //we weren't sure if the sorting was supposed to happen in the original vector so we just passed it by value here even though this uses abit more memory
{
	int n=10,j=1; //final project wants top 10 portfolios to be displayed, however the value of the n variable can be changed in order to display the top n portfolios

	sort(P.begin(),P.end(),sortDescending); //overloaded > makes this work

	if (P.size()<n)
    	cout<<GREEN<<"There were "<<YELLOW<<"less"<<GREEN<<" than "<<RED<<n<<GREEN<<" portfolios so here they are in descending order (from most expensive to least expensive):"<<WHITE<<endl;
    else
    	cout<<GREEN<<"Here are the "<<RED<<n<<GREEN<<" most expensive portfolios in descending order based on price: "<<WHITE<<endl;

	horLine(116)
	cout<<endl;

    for (int i=0;i<P.size() && i!=n;++i) //if there are less than 10 portfolios it just prints them in descending order based on total cost
    {
    	if (!(P[i].getCanceled()))
    	{
    		cout<<GREEN<<j<<WHITE<<")";
			displayTop(P[i]);
			j++;
			if  (!(i==P.size()-1))
				cout<<endl;
		}
    	else
    		n++;//if cancelled just go to the next one
	}
}

bool sortDescending(Portfolio& A,Portfolio& B) //yes, it could be done without overloading any operator but we just wanted to have atleast one overload in this program
{
    return A>B;
}

void displayTop(Portfolio& port) //passed by reference and not by value even if we don't modify the vector so we use less memory
{
	cout<<GREEN<<port.getNameSur()<<WHITE<<"'s Portfolio with a cost of "<<RED<<port.calculateCost()<<WHITE<<endl;
	cout<<"Further information of the owner: "<<endl;
	cout<<UNDERON<<"Address"<<UNDEROFF<<": "<<port.getAddress()<<endl;
	cout<<UNDERON<<"Telephone"<<UNDEROFF<<": "<<port.getTel()<<endl;
	cout<<UNDERON<<"A.F.M."<<UNDEROFF<<": "<<port.getAFM()<<endl;
}

void exitMsg() //we are not sure if this (the table line characters in particular) works on linux
{
	//all table line characters (they are double lined unless specified otherwise)

	char topLCorner=201; //top left corner
	char topRCorner=187; //top rigth corner
	char botLCorner=200; //bottom left corner
	char botRCorner=188; //bottom right corner
	char vertLine=186; //vertical line
	char lSinConVertLine=199; //left vertical connecting line (Double with Single)
	char rSinConVertLine=182; //right vertical connecting line (Double with Single)
	char lConVertLine=204; //left vertical connecting line
	char rConVertLine=185; //right vertical connecting line
	char sinHorLine=196; //horizontal line (Single)

	WIPE_SCR

	cout<<WHITE;
	cout<<endl<<endl;

	cout<<"\t\t"<<topLCorner;
	horLine(56)
	cout<<topRCorner<<endl;

    cout<<"\t\t"<<vertLine<<"      Copyright (C) 2018 "<<GREEN<<"Timothy Leonard Stefaniak"<<WHITE<<"      "<<vertLine<<endl;
    cout<<"\t\t"<<vertLine<<RED<<"[timosste@yahoo.co.uk]              [cs171012@teiath.gr]"<<WHITE<<vertLine<<endl;

	cout<<"\t\t"<<lSinConVertLine;
	for (int i=0;i<56;i++)
		cout<<sinHorLine;
	cout<<rSinConVertLine<<endl;

    cout<<"\t\t"<<vertLine<<"      Copyright (C) 2018 "<<GREEN<<"Emmanouil  Athanasopoulos"<<WHITE<<"      "<<vertLine<<endl;
	cout<<"\t\t"<<vertLine<<RED<<"[manos.athanasopoulos@yahoo.gr]     [cs171049@teiath.gr]"<<WHITE<<vertLine<<endl;

	cout<<"\t\t"<<lConVertLine;
	horLine(56)
	cout<<rConVertLine<<endl;

    cout<<"\t\t"<<vertLine<<CYAN<<"                        Uni.W.A.                        "<<WHITE<<vertLine<<endl;
	cout<<"\t\t"<<vertLine<<CYAN<<"                 Faculty of Engineering                 "<<WHITE<<vertLine<<endl;
	cout<<"\t\t"<<vertLine<<CYAN<<"            Computer Science and Engineering            "<<WHITE<<vertLine<<endl;

	cout<<"\t\t"<<lSinConVertLine;
	for (int i=0;i<56;i++)
		cout<<sinHorLine;
	cout<<rSinConVertLine<<endl;

	cout<<"\t\t"<<vertLine<<CYAN<<"C++ || Object Oriented Programming Final Project of 2018"<<WHITE<<vertLine<<endl;

	cout<<"\t\t"<<botLCorner;
	horLine(56)
	cout<<botRCorner<<endl;

	cout<<endl<<endl;
	cout<<C_Reset;
}

//multi-use functions

int countCanceled(vector<Portfolio>& P)
{
	int j=0;

	for (int i=0;i<P.size();i++) //counts all cancelled portfolios
		if (P[i].getCanceled())
			j++;

	return j;
}

float getX()
{
	float x;

	cout<<"Give the value of X: ";
	cin>>x;
	cout<<endl;		
	while(cin.fail() || x<0)
	{
		cout<<YELLOW<<"Give correct value of X (must be a number from 0 and up): "<<WHITE;
		FIX_INPUT
		cin>>x;
		cout<<endl;
	}
	WIPE_SCR

	return x;
}

void inputStock(vector<Securities*>& S) //our general input stock from keyboard function, there are logical boundaries for most variables
{
	string cN;
	float pV;
	int qu;
	int aY;
	float bV;

	cout<<"Give company name: ";
	getline(cin,cN);
	cout<<endl;

	cout<<"Give par value: ";
	cin>>pV;
	cout<<endl;		
	while(cin.fail() || pV<=0)
	{
		cout<<YELLOW<<"Give correct par value (must be a number above 0): "<<WHITE;
		FIX_INPUT
		cin>>pV;
		cout<<endl;
	}

	cout<<"Give quantity: ";
	cin>>qu;
	cout<<endl;		
	while(cin.fail() || qu<0)
	{
		cout<<YELLOW<<"Give correct quantity (must be a number from 0 and up): "<<WHITE;
		FIX_INPUT
		cin>>qu;
		cout<<endl;
	}

	cout<<"Give acquisition year: ";
	cin>>aY;
	cout<<endl;
	while(cin.fail() || aY>CURYEAR)
	{
		cout<<YELLOW<<"Give correct acquisition year (must be 2018 or below): "<<WHITE;
		FIX_INPUT
		cin>>aY;
		cout<<endl;
	}

	cout<<"Give buy value: ";
	cin>>bV;
	cout<<endl;		
	while(cin.fail() || bV<=0)
	{
		cout<<YELLOW<<"Give correct buy value (must be a number above 0): "<<WHITE;
		FIX_INPUT
		cin>>bV;
		cout<<endl;
	}

	S.push_back(new Stocks(cN,pV,qu,aY,bV));
}

void inputBond(vector<Securities*>& S) //our general input bond from keyboard function, there are logical boundaries for most variables
{
	string cN;
	float pV;
	int qu;
	int aY;
	int eY,in;

	cout<<"Give company name: ";
	getline(cin,cN);
	cout<<endl;

	cout<<"Give par value: ";
	cin>>pV;
	cout<<endl;		
	while(cin.fail() || pV<=0)
	{
		cout<<YELLOW<<"Give correct par value (must be a number above 0): "<<WHITE;
		FIX_INPUT
		cin>>pV;
		cout<<endl;
	}

	cout<<"Give quantity: ";
	cin>>qu;
	cout<<endl;		
	while(cin.fail() || qu<0)
	{
		cout<<YELLOW<<"Give correct quantity (must be a number from 0 and up): "<<WHITE;
		FIX_INPUT
		cin>>qu;
		cout<<endl;
	}

	cout<<"Give acquisition year: ";
	cin>>aY;
	cout<<endl;
	while(cin.fail() || aY>CURYEAR)
	{
		cout<<YELLOW<<"Give correct acquisition year (must be 2018 or below): "<<WHITE;
		FIX_INPUT
		cin>>aY;
		cout<<endl;
	}

	cout<<"Give expiration year: ";
	cin>>eY;
	cout<<endl;		
	while(cin.fail() || eY<CURYEAR)
	{
		cout<<YELLOW<<"Give correct expiration year (must be 2018 or above): "<<WHITE;
		FIX_INPUT
		cin>>eY;
		cout<<endl;
	}

	cout<<"Give interest rate (percentage %): ";
	cin>>in;
	cout<<endl;		
	while(cin.fail() || in<0)
	{
		cout<<YELLOW<<"Give correct interest rate (percentage %) (must be a number from 0 and up): "<<WHITE;
		FIX_INPUT
		cin>>in;
		cout<<endl;
	}

	S.push_back(new Bonds(cN,pV,qu,aY,eY,in));
}

void fileOpen(ifstream& file) //opens file of given name
{
	file.open(getFileName()+".txt",ios::in); //works in c++ 11, insturctions on how to enable it in devc++ are on the first line of the code, otherwise you would need #include <cstring> and \
											   make getFileName() return a cstring (or turn the return value into a cstring here) and then use strcpy to add ".txt" to the end of the string and THEN put it in the file.open method
	while(file.fail())
	{
		cout<<RED<<UNDERON<<"*File doesn't exist.*"<<WHITE<<UNDEROFF<<endl<<endl;
		file.open(getFileName()+".txt",ios::in);
	}
}

string getFileName() //gets file name to open from user
{
	string fileName;

	cout<<"Give the name of the file that you'll input the securities from: ";
	getline(cin,fileName);
	WIPE_SCR

	return fileName;
}

void fileStock(vector<Securities*>& S,ifstream& file) //our general input stock from file function, there are logical boundaries for most variables
{
	string buff;

	string cN;
	float pV;
	int qu;
	int aY;
	float bV;

	getline(file,cN,'|');

	getline(file,buff,'|');
	pV=stof(buff); //c++11 needed here again, stoi/stof/stod/atoi/atof/atod are not supported by c++98, insturctions on how to enable it in devc++ are on the first line of the code
	if (pV<=0)
		throw 1;

	getline(file,buff,'|');
	qu=stoi(buff); //c++11 needed here again, stoi/stof/stod/atoi/atof/atod are not supported by c++98, insturctions on how to enable it in devc++ are on the first line of the code
	if (qu<0)
		throw 2;

	getline(file,buff,'|');
	aY=stoi(buff); //c++11 needed here again, stoi/stof/stod/atoi/atof/atod are not supported by c++98, insturctions on how to enable it in devc++ are on the first line of the code
	if (aY>CURYEAR)
		throw 3;

	getline(file,buff);
	bV=stof(buff); //c++11 needed here again, stoi/stof/stod/atoi/atof/atod are not supported by c++98, insturctions on how to enable it in devc++ are on the first line of the code
	if (bV<=0)
		throw 4;

	S.push_back(new Stocks(cN,pV,qu,aY,bV));
}

void fileBond(vector<Securities*>& S,ifstream& file) //our general input bond from file function, there are logical boundaries for most variables
{	
	string buff;

	string cN;
	float pV;	
	int qu;
	int aY;
	int eY,in;

	getline(file,cN,'|');

	getline(file,buff,'|');
	pV=stof(buff); //c++11 needed here again, stoi/stof/stod/atoi/atof/atod are not supported by c++98, insturctions on how to enable it in devc++ are on the first line of the code
	if (pV<=0)
		throw 1;

	getline(file,buff,'|');
	qu=stoi(buff); //c++11 needed here again, stoi/stof/stod/atoi/atof/atod are not supported by c++98, insturctions on how to enable it in devc++ are on the first line of the code
	if (qu<0)
		throw 2;

	getline(file,buff,'|');
	aY=stoi(buff); //c++11 needed here again, stoi/stof/stod/atoi/atof/atod are not supported by c++98, insturctions on how to enable it in devc++ are on the first line of the code
	if (aY>CURYEAR)
		throw 3;

	getline(file,buff,'|');
	eY=stoi(buff); //c++11 needed here again, stoi/stof/stod/atoi/atof/atod are not supported by c++98, insturctions on how to enable it in devc++ are on the first line of the code
	if (eY<CURYEAR)
		throw 5;

	getline(file,buff);
	in=stoi(buff); //c++11 needed here again, stoi/stof/stod/atoi/atof/atod are not supported by c++98, insturctions on how to enable it in devc++ are on the first line of the code
	if (in<0)
		throw 6;

	S.push_back(new Bonds(cN,pV,qu,aY,eY,in));
}

/*
	·========================================================·
	|      Copyright (C) 2018 Timothy Leonard Stefaniak      |
    |[timosste@yahoo.co.uk]              [cs171012@teiath.gr]|
	|--------------------------------------------------------|
    |      Copyright (C) 2018 Emmanouil  Athanasopoulos      |
	|[manos.athanasopoulos@yahoo.gr]     [cs171049@teiath.gr]|
	|========================================================|
    |                        Uni.W.A.                        |
	|                 Faculty of Engineering                 |
	|            Computer Science and Engineering            |
	|--------------------------------------------------------|
	|C++ || Object Oriented Programming Final Project of 2018|
	·========================================================·
*/
