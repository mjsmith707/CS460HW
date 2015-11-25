/* Assignment : Project 1
 File : SyntacticalAnalyzer.h
 Author : Forrest Rosetti
 Professor : Dr. Tia Watts
 Date : 2/19/15
 Development time: N/A.
 Description : Header for SyntacticalAnalyzer
*/

#ifndef SYN_H
#define SYN_H

#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"

using namespace std;

class SyntacticalAnalyzer 
{
    public:
	SyntacticalAnalyzer (char * filename);
	~SyntacticalAnalyzer ();
    private:
	LexicalAnalyzer * lex;
	token_type token;
};
	
#endif
