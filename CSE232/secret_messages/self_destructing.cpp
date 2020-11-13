#define CAST static_cast
#define SDM SelfDestructingMessage


#include <iostream>
#include <cstdlib>
#include <cstring>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "redact.h"
#include "self_destructing.h"

using std::cout;
using std::istream;
using std::ostream;
using std::string;
using std::vector; 

const int Sptr = sizeof(string*);
const int Long = sizeof(long);
// for array sizing

istream& operator>>(istream& Input, SDM& sdm)
{
	sdm.setsz(sdm.size() + 1);
	string** message = CAST<string**>(realloc(sdm.getmess(), sdm.size() * Sptr));
	mcheck(message);
	long* view = CAST<long*>(realloc(sdm.getview(), (sdm.size() + 1) * Long));
	mcheck(view);    // resizing and checking
	view[sdm.size()] = 0;  // default setting =0
	string temp;
	char chr;
	while ((chr = Input.get()) != EOF) { temp = temp + chr; }  // creating new string
	string* temptr = new string;
	*temptr = temp; 
	message[sdm.size() - 1] = temptr;  // putting in array
	view[sdm.size()] = 0;
	sdm.setmess(message);
	sdm.setview(view);   // setting

	return Input;
}

ostream& operator << (ostream& Output, SDM& sdm)
{
	int mx = sdm.size();
	for (int count4 = 0; count4 < mx; count4++)
	{
		if (sdm.number_of_views_remaining(count4) < 10) { Output << "0"; } // two digits
		Output << sdm.number_of_views_remaining(count4) << ": ";
		Output << Redact(*(sdm.getmess()[count4]), 1) << "\n";   // redacting the values
	}
	
	return Output;

}
SDM::SDM(vector <string> Message, long View)
{
	Size = CAST<int>(Message.size());   // setting size 
	messages = CAST<string**>(malloc(Size * Sptr));  
	if (Size) { mcheck(messages); }
	// memory and checking memeory
	for (int count = 0; count < Size; count++)
	{	// transfering values
		string* temp = new string;
		*temp = Message[count];
		messages[count] = temp;

	}
	views = CAST<long*>(calloc(Size + 1, Long));  // everything default
	mcheck(views); 
	views[0] = View;   // first value is original amount of views allowed
}

SDM::SDM(SDM& sdm)
{
	Size = sdm.Size;
	messages = CAST<string**>(malloc(Size * Sptr));
	if (Size) { mcheck(messages); }
	views = CAST<long*>(calloc(Size + 1, Long));
	mcheck(views);
	for (int count2 = 0; count2 < Size; count2++)
	{   // transfering strings
		string* temp = new string;
		(*temp) = *(sdm.messages[count2]);
		messages[count2] = temp;
	}
	memcpy(views, sdm.views, (Size + 1) * Long);  // copying view info
	sdm.views[0] = 0; // setting default of old to 0
}

const SDM& SDM:: operator=(SDM& old)
{
	if (&old == this)
	{
		cout << "Silly user detected, attempting self-assignment"; return *this;
	}
	else
	{
		Size = old.Size;
		messages = CAST<string**>(realloc(messages, Size * Sptr)); // realloc prevents leakage
		mcheck(messages);
		for (int count1 = 0; count1 < Size; count1++)
		{	// transfering strings
			string* temp = new string;
			(*temp) = *(old.messages[count1]);
			messages[count1] = temp;
		}
		views = CAST<long*>(realloc(views, (Size + 1) * Long));
		mcheck(views);
		memcpy(views, old.views, Long * (Size + 1)); // copy view info
		old.views[0] = 0;
		return *this;
	}
}


string& SDM::operator[](int index)
{
	if (index < 0 || index >= Size)
	{
		throw
			std::out_of_range("Wrong index, what kind of agent are you?\n");
	}

	else if (views[index + 1] < 0 || !(views[0]))
	{
		throw
			std::invalid_argument("Acesss denied.\n");
	}
	if (views[index + 1] == 1) { views[index + 1] = -1; }
	else if (!(views[index + 1])) { views[index + 1] = views[0] - 1; }
	else { views[index + 1] = views[index + 1] - 1; }
	// adjusting view data
	return *(messages[index]);
}

vector <string> SDM::get_redacted(void)
{
	vector <string> Out;
	for (int count1 = 0; count1 < Size; count1++)
	{
		Out.push_back(Redact(*(messages[count1]), 1));
	}
	return Out;
}


int SDM::number_of_views_remaining(int index)
{
	if (views[0] && views[index + 1] > 0) { return views[index + 1]; } // check for default greater than 0
	else if (views[index + 1] < 0) { return 0; }  // - value is == 0
	else { return views[0]; }  // if 0, return default
}

void SDM::add_array_of_lines(string* array, long siz)
{
	messages = CAST<string**>(realloc(messages, (Size + siz) * Sptr));
	mcheck(messages);
	for (int count3 = 0; count3 < siz; count3++)
	{	// transfering strings 
		string* temp = new string;
		*temp = array[count3];
		messages[count3 + Size] = temp;
	}

	views = CAST<long*>(realloc(views, (siz + Size + 1) * Long));
	mcheck(views);
	memset(views + Size + 1, 0, Long * siz);  // default view setting
	Size = siz + Size;

}

SDM::~SDM()
{
	for (int fcount = 0; fcount < Size; fcount++)
	{	// delete strings first
		delete messages[fcount];
	}
	free(messages); messages = 0; // delete arrays
	free(views); views = 0;   // set to zero to prevent dereference
}
