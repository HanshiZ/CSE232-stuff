#ifndef REDACT
#define REDACT

#include <iostream>
#include <cctype>
#include <string>
#include <vector>

using std::string;
using std::vector;

string Redact(const string& secret, int option = 0);
void findreplace(const string& word, const string& out, char* bad);

string redact_alphabet_digits(const string& str);
string redact_all_chars(const string& str);

string redact_words(const string &s, const vector<string>& words);

#endif 

