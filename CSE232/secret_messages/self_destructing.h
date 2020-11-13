#ifndef SELFDESTRUCTING
#define SELFDESTRUCTING

#define CAST static_cast
#define SDM SelfDestructingMessage
// make my life easier 

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "redact.h"

using std::cout;
using std::istream;
using std::ostream;
using std::string;
using std::vector;

template <class ptr>
inline void mcheck(ptr* mem) { if (!(mem)) { std::cout << "No memory"; exit(1); } }
// checking memory allocation
class SDM
{
	friend istream& operator>>(istream& Input, SDM& sdm);
	friend ostream& operator<<(ostream& Output, SDM& sdm);

public:
	SDM(vector <string> Message = {}, long View = 0); // default + custom in one
	SDM(SDM& sdm);  // constructors

	const SDM& operator= (SDM& old);
	string& operator[](int index);   // overloaded operators

	int size(void) { return Size; }  // getters
	string** getmess(void) { return messages; }
	long* getview(void) { return views; }

	void setsz(int siz) { Size = siz; }  // setters
	void setmess(string** m) { messages = m; }
	void setview(long* v) { views = v; }

	vector <string> get_redacted(void);  // required
	int number_of_views_remaining(int index = 0);
	void add_array_of_lines(string* array, long Size);

	~SDM();  // destructor

private:
	string** messages = 0;  // prevent dereferencing
	long* views = 0;
	int Size = 0;

};

#endif