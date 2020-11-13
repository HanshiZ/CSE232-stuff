/*********************************
		    Project 3
Creating a mini-library to process 
financial insitution commands
Uses simple math and string 
manipulation.

***********************************/
// to prevent redeclaration
#ifndef BANK
#define BANK

#include <cmath>

#include <string>

using std::string;
using std::stoi;
using std::stod;
using std::to_string;


/* a function from project one that takes the desired 
   piece of string based on starting index and length*/
string Slice(const string& String, int start, int len)
{
	string Out = {};
	for (int count = 0; count < len; count++)
	{
		Out = Out + String[start + count];
	}
	return (Out);
}
/* function that takes a double and returns string
   with requested decimals up to 6 				   */

string two_deci(const double& doub, int places = 2)
{
	string Out = {};
	string full = to_string(doub);
	int len = static_cast<int>(full.size());
	Out = Slice(full, 0, len + places - 6);   // subtracting 6 because of default
	return Out;
}

// mandatory functions below, self made above

// depositing
inline void deposit(double& account, double amount) { account = account + amount; }

// withdrawing and checking for overdraft
inline bool withdraw(double& account, double amount)
{
	if (account > amount) { account = account - amount; return 1; } return 0;
}
// overdraft account modification
inline void overdraft(double& account, double amount) { account = account - amount - 35; }

// calculating interst and rounding to appropriate amount and precision
double interest_for_month(double& account, const double &apr)
{
	double interest = account*100 * (apr / (12*100));
	
	return (floor(interest) / 100.0);
}
// taking a string date and assinging values to pointers
inline void string_date_to_int_ptrs(const string& date, int* year, int* month, int* day)
{
	*year = stoi(Slice(date, 0, 4));
	*month = stoi(Slice(date, 5, 2));
	*day = stoi(Slice(date, 8, 2));
}
//same as above but with references
inline void string_date_to_ints(const string& date, int& year, int& month, int& day)
{
	year = stoi(Slice(date, 0, 4));
	month = stoi(Slice(date, 5, 2));
	day = stoi(Slice(date, 8, 2));
}

// calculating the number of interest compoudings needed 
int number_of_first_of_months(const string& date1,const string& date2)
{
	int num = 0; // number of months
	int year1, month1, day1;
	int year2, month2, day2;
	string_date_to_ints(date1, year1, month1, day1);
	string_date_to_ints(date2, year2, month2, day2);

	if (day1 == day2 && month1 == month2 && year1 == year2) { return 0; }
	// return if same day
	num = num + 12 * (year2 - year1);  // year is twelve months
	num = num + month2 - month1;

	if (day1 == 1) { num = num + 1; } // if deposit on first then interest

	return num;
}


double interest_earned(const double& account, const double& apr, const string& date1, const string& date2)
{
	double temp = account;  // temp to hold account info so no changes to account
	int compound = number_of_first_of_months(date1, date2);
	for (int i = 0; i < compound; i++)
	{	// adding in interest by month
		temp = temp + interest_for_month(temp, apr);
	}
	
	/***************************************************************
		double total = account * pow((1 + apr / (1200.0)), compound);
	***************************************************************/
	// actual formula but not work well with mimir rounding
	return (round((temp - account) * 100) / 100.0);
}

//Processing the commands piece by piece and updating the date

string process_command(const string& order, string& date, double& account, const double& apr)
{
	string today = Slice(order, 0, 10);

	int len = static_cast<int>(order.size()); // geting length to help slice

	string Output = "On ";  //starting output string
	Output = Output + today + ": Instructed to perform \"" + Slice(order, 11, len - 11) + "\"\n"; //concatenating
	if (date != "")
	{	// for non-empty date calculate interest
		int compound = number_of_first_of_months(date, today);
		if (compound > 0)
		{
			Output = Output + "Since " + date + ", interest has accrued "
				+ to_string(compound) + " times.\n$";
			double interest;
			account > 0 ? interest = interest_earned(account, apr, date, today) : interest = 0;
			// interest for those in the black else 0
			Output = Output + two_deci(interest) + " interest has been earned.\n";
			account = account + interest;
		}
	}
	if (order[11] == 'D') // deposit starts with D
	{
		account = account + stod(Slice(order, 20, len - 20));
	}
	else if (order[11] == 'W') // W for Withdraw
	{
		account = account - stod(Slice(order, 21, len - 21));
	}

	else { std::cout << "Ooops! Josh tricked me :("; } // this is just for error checking
	if (account < 0) // overdraft fee
	{
		account = account - 35;
		Output = Output + "Overdraft!\nBalance: -" + two_deci(-1 * account) + "\n";
	}
	else { Output = Output + "Balance: " + two_deci(account) + "\n"; } 
	// printing out balance with two deicmals
	date = today;  // updating today
	return Output;
}


/* taking the process command and iterating through all of the
	commands needed*/

string process_commands(const string& orders, const double apr)
{
	double account = 0;
	string day = {};
	string Out = {};  // initialization 
	int Size = static_cast<int>(orders.size());
	int newline = 0;  // position of newlines to help with slice
	for (int count3 = 0; count3 < Size; count3++) // loop through to look for newline
	{
		if (orders[count3] == '\n') 
		{	// we slice out string before newline and use that for process commands
			string order = Slice(orders, newline, count3 - newline);
			Out = Out + process_command(order, day, account, apr);
			newline = count3 + 1;
		}
	}
	return Out;
}
#endif
