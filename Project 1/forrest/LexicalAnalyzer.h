/* Assignment : Project 1
 File : LexicalAnalyzer.h
 Author : Forrest Rosetti
 Professor : Dr. Tia Watts
 Date : 2/19/15
 Development time: Between 8 and 10 hours, some reason this went quick.
 Description : Just header file for lex analyzer
*/

#ifndef LEX_H
#define LEX_H

#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include <iomanip> // for setw
#include <map>
#include <sstream>


using namespace std;

enum token_type {
	IDENT_T, NUMLIT_T,
	INTTYPE_T, DBLTYPE_T, MAIN_T, IF_T, ELSE_T, WHILE_T, VOID_T,
	PLUS_T, MINUS_T, DIV_T, MULT_T, MOD_T, MINUSMINUS_T, PLUSPLUS_T,
	ASSIGN_T, EQUALTO_T, NOTEQ_T, GT_T, LT_T, GTE_T, LTE_T, LOGAND_T, LOGOR_T, NOT_T,
	AND_T, OR_T, XOR_T, SHIFTL_T, SHIFTR_T, TILDA_T,
	LPAREN_T, RPAREN_T, LBRACE_T, RBRACE_T, SEMI_T, COMMA_T, ERROR_T, EOF_T,
	QUEST_T, COLON_T};



class LexicalAnalyzer 
{
    public:
	LexicalAnalyzer (char * filename);
	~LexicalAnalyzer ();
	token_type get_token ();
	string get_lexeme () const;
	void report_error (const string & msg);

    private:
	int table[13][27];
	string lexeme;
	ifstream input;
	ofstream first;
	ofstream debug;
	string line;
	int pos;
	int linecount;
	int errorcount;
	bool eof;

	map<token_type, string> tokenMap;
	int GetChar(char c);
	token_type IdentOrKeyWord();


	void skipspace();
	void loadDefaults();

};
	

#endif
