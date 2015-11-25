/* Assignment : Project 1
 File : SyntaticalAnalyzer.cpp
 Author : Forrest Rosetti
 Professor : Dr. Tia Watts
 Date : 2/19/15
 Development time: N/A.
 Description : Syntactical Analyzer not finished yet
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer(char * filename) {
	lex = new LexicalAnalyzer(filename);
	token_type t;
	//while ((t = lex->get_token()) != EOF_T);
	while ((t = lex->get_token()) != EOF_T) {
		//cout << lex->get_lexeme() << endl;
	}
}

SyntacticalAnalyzer::~SyntacticalAnalyzer() {
	delete lex;
}
