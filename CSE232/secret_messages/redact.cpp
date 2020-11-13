#include <iostream>
#include <cstdlib>
#include <cctype>
#include <string>
#include <vector>
#include "redact.h"

using std::string;
using std::vector;

string redact_all_chars(const string& str) {return Redact(str);}
// this usage of Redact helps eliminate duplication
string redact_alphabet_digits(const string& str) { return Redact(str, 1); }  


string redact_words(const string& s, const vector<string>& words)
{
	int wsiz = static_cast<int>(words.size());
	char* bad = static_cast<char*>(calloc(1, s.size()));
	/* creating a "mirror" image of size equal to string set to zero
	   and changing to one if need #, this helps if an word like
	   "it" and doesnot change university if university in vector   */
	for (int count = 0; count < (int)words.size(); count++)
	{
		findreplace(words[count], s, bad);
	}
	string out;
	for (int count1 = 0; count1 < static_cast<int>(s.size()); count1++)
	{	//actually replacing
		if (bad[count1]) out = out + "#";
		else { out = out + s[count1]; }
	}
	free(bad);
	return out;
}

//redacting alnum and everything
string Redact(const string& secret, int option)
{	
	string Out = secret;
	for (int i = 0; i < static_cast<int>(secret.size()); i++)
	{
		if (!(option) || isalnum(secret[i])) { Out[i] = '#'; }
	}
	return Out;
}


void findreplace(const string& word, const string& out, char* bad)
{
	int wsiz = static_cast<int>(word.size());
	int	osiz = static_cast<int>(out.size());

	int pos = 0;
	for (int count = 0; count < osiz; count++)
	{	//checking letter by letter and moving pos indicator accordingly
		if (tolower(out[count]) == tolower(word[pos])) {pos = pos + 1; }
		else if (tolower(out[count]) == tolower(word[0])) { pos = 1; }
		else { pos = 0; }
		if (pos == wsiz)
		{
			for (int rcount = 0; rcount < wsiz; rcount++)
			{
				bad[count - rcount] = 1;
			}
			pos = 0;
		}
	}
}
