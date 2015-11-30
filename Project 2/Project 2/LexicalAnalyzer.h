#ifndef LEX_H
#define LEX_H

// Your LexicalAnalyzer, slightly modified
// While mine worked reasonably well, this is one
// less point of failure before project 3.
#include <iostream>
#include <fstream>
#include "Grammar.h"

using namespace std;

class LexicalAnalyzer 
{
    public:
	LexicalAnalyzer (std::string, std::fstream&, std::fstream&, std::fstream&);
	~LexicalAnalyzer ();
	Grammar::TerminalEnums get_token ();
	string get_token_name (Grammar::TerminalEnums t) const;
	string get_lexeme () const;
	void report_error (const string & msg);
    private:
	std::fstream& input;
	std::fstream& listing;
	std::fstream& debug;
	string line;
	int linenum;
	int pos;
	string lexeme;
	int errors;
};
	
#endif
