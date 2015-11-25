/* Assignment : Project 1
 File : LexicalAnalyzer.cpp
 Author : Forrest Rosetti
 Professor : Dr. Tia Watts
 Date : 2/19/15
 Development time: Between 8 and 10 hours, some reason this went quick.
 Description : Just the main code for the lex analyzer
 */

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "LexicalAnalyzer.h"

using namespace std;

//static string token_names[] = { };

LexicalAnalyzer::LexicalAnalyzer(char * filename) {
	//Basic set up, use input to open the file
	input.open(filename);

	//Strip filename so we can use it for .1st and .dbg
	stringstream ss(filename);
	string outfilename;
	getline(ss, outfilename, '.');
	string firstfilename = outfilename + ".1st";
	string debugfilename = outfilename + ".dbg";

	first.open(firstfilename.c_str());
	debug.open(debugfilename.c_str());

	//Set my class variables up
	pos = 0;
	linecount = 1;
	errorcount = 0;
	eof = false;

	if (input.fail()) {
		cout << "failed to open file, add exit1 to this" << endl;
	}
	getline(input, line); // I grab one line from here.

	//Output file prints and cout too console
	first << "Input file: " << filename << endl;
	debug << "Input file: " << filename << endl;
	cout << "Input file: " << filename << endl;

	//Write the first line to the files
	first << "   " << linecount << ": " << line << endl;
	debug << "   " << linecount << ": " << line << endl;

	//this loads my default state table
	loadDefaults();

}

LexicalAnalyzer::~LexicalAnalyzer() {
	first.close();
	debug.close();
	input.close();
}

token_type LexicalAnalyzer::get_token() {
	//Calls skipspace function that bypasses spaces and tabs, moves onto new lines automatically, checks for EOF too.
	skipspace();
	// This eof gets set in skipspace, if you move along to end of file it reports it before we continue
	if (eof == true) {
		first << errorcount << " errors found in input file";
		debug << errorcount << " errors found in input file";
		cout << errorcount << " errors found in input file";
		return EOF_T;
	}
	lexeme.clear();
	int state = 0;
	token_type t = (token_type) -1;
	while (t == -1) {
		char c = line[pos];
		lexeme += c;
		int findit = 99; // I just set this to 99, could have been any thing but worse case I get a error on case 99 if it goes through
		if (isalpha(c)) {
			findit = 0;
		} else if (isdigit(c)) {
			findit = 2;
		} else {
			//This getChar is just a big if else statement for analyzing the char and returns the int value for my table
			findit = GetChar(c);
		}
		// This is your state table idea and switch with it.
		state = table[state][findit];
		switch (state) {
		case 99:
			++errorcount;
			++pos;
			first << "Error at " << linecount << ',' << pos << ": Invalid character found: " << c << endl;
			debug << "Error at " << linecount << ',' << pos << ": Invalid character found: " << c << endl;
			first << "        " << setw(16) << left << "ERROR_T" << lexeme << endl;
			//This tokenMap is a map of token_types and strings for print out
			//I had originally had them all like tokenMap[ERROR_T], now I realize its a waste for now.
			//Keeping the old version of the lines now just in case. Long story short, I over engineered haha
			//first << "        " << setw(16) << left << tokenMap[ERROR_T] << lexeme << endl;
			t = ERROR_T;
			break;
		case 100:
			++errorcount;
			lexeme.erase(lexeme.end() - 1);
//			string temp;
//			temp +="Error at ";
//			temp +=linecount;
//			temp +=',';
//			temp +=pos;
//			temp += ": Invalid character found: ";
//			temp +=lexeme;
//			report_error(temp);
			// I didnt use the report_error function. I could have with the above code BUT this below looks cleaner
			first << "Error at " << linecount << ',' << pos << ": Invalid character found: " << lexeme << endl;
			debug << "Error at " << linecount << ',' << pos << ": Invalid character found: " << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[ERROR_T] << lexeme << endl;
			first << "        " << setw(16) << left << "ERROR_T" << lexeme << endl;
			t = ERROR_T;
			break;
		case 101:
			//I am not sure if this was wise but I minus out the last char from lexeme string.
			//I suspect I can do this a better way but it does work.
			lexeme.erase(lexeme.end() - 1);
			// this IdentOrKeyWord returns a token type based on the class variable lexeme string.
			// So it analyzes if the lexeme string is a int or variable temp2020.
			//This was also the only place that tokenMap paid off, deduce what type of token the lexeme
			//is and print out the string
			first << "        " << setw(16) << left << tokenMap[IdentOrKeyWord()] << lexeme << endl;
			return IdentOrKeyWord();
			break;
		case 102:
			lexeme.erase(lexeme.end() - 1);
			first << "        " << setw(16) << left << "NUMLIT_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[NUMLIT_T] << lexeme << endl;
			t = NUMLIT_T;
			break;
		case 103:
			++pos;
			first << "        " << setw(16) << left << "SEMI_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[SEMI_T] << lexeme << endl;
			t = SEMI_T;
			break;
		case 104:
			++pos;
			first << "        " << setw(16) << left << "PLUSPLUS_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[PLUSPLUS_T] << lexeme << endl;
			t = PLUSPLUS_T;
			break;
		case 105:
			lexeme.erase(lexeme.end() - 1);
			first << "        " << setw(16) << left << "PLUS_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[PLUS_T] << lexeme << endl;
			t = PLUS_T;
			break;

		case 106:
			++pos;
			first << "        " << setw(16) << left << tokenMap[DIV_T] << lexeme << endl;
			t = DIV_T;
			break;
		case 107:
			++pos;
			first << "        " << setw(16) << left << "MULT_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[MULT_T] << lexeme << endl;
			t = MULT_T;
			break;
		case 108:
			++pos;
			first << "        " << setw(16) << left << "MOD_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[MOD_T] << lexeme << endl;
			t = MOD_T;
			break;
		case 109:
			++pos;
			first << "        " << setw(16) << left << "XOR_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[XOR_T] << lexeme << endl;
			t = XOR_T;
			break;
		case 110:
			++pos;
			first << "        " << setw(16) << left << "TILDA_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[TILDA_T] << lexeme << endl;
			t = TILDA_T;
			break;
		case 111:
			++pos;
			first << "        " << setw(16) << left << "LPAREN_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[LPAREN_T] << lexeme << endl;
			t = LPAREN_T;
			break;
		case 112:
			++pos;
			first << "        " << setw(16) << left << "RPAREN_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[RPAREN_T] << lexeme << endl;
			t = RPAREN_T;
			break;
		case 113:
			++pos;
			first << "        " << setw(16) << left << "QUEST_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[QUEST_T] << lexeme << endl;
			t = QUEST_T;
			break;
		case 114:
			++pos;
			first << "        " << setw(16) << left << "COLON_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[COLON_T] << lexeme << endl;
			t = COLON_T;
			break;

		case 115:
			++pos;
			first << "        " << setw(16) << left << "COMMA_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[COMMA_T] << lexeme << endl;
			t = COMMA_T;
			break;
		case 116:
			lexeme.erase(lexeme.end() - 1);
			first << "        " << setw(16) << left << "MINUS_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[MINUS_T] << lexeme << endl;
			t = MINUS_T;
			break;
		case 117:
			++pos;
			first << "        " << setw(16) << left << "MINUSMINUS_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[MINUSMINUS_T] << lexeme << endl;
			t = MINUSMINUS_T;
			break;
		case 119:
			lexeme.erase(lexeme.end() - 1);
			first << "        " << setw(16) << left << "ASSIGN_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[ASSIGN_T] << lexeme << endl;
			t = ASSIGN_T;
			break;
		case 118:
			++pos;
			first << "        " << setw(16) << left << "EQUALTO_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[EQUALTO_T] << lexeme << endl;
			t = EQUALTO_T;
			break;
		case 120:
			lexeme.erase(lexeme.end() - 1);
			first << "        " << setw(16) << left << "NOT_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[NOT_T] << lexeme << endl;
			t = NOT_T;
			break;
		case 121:
			++pos;
			first << "        " << setw(16) << left << "NOTEQ_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[NOTEQ_T] << lexeme << endl;
			t = NOTEQ_T;
			break;
		case 122:
			lexeme.erase(lexeme.end() - 1);
			first << "        " << setw(16) << left << "LT_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[LT_T] << lexeme << endl;
			t = LT_T;
			break;
		case 123:
			++pos;
			first << "        " << setw(16) << left << "SHIFTL_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[SHIFTL_T] << lexeme << endl;
			t = SHIFTL_T;
			break;
		case 124:
			++pos;
			first << "        " << setw(16) << left << "LTE_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[LTE_T] << lexeme << endl;
			t = LTE_T;
			break;
		case 125:
			lexeme.erase(lexeme.end() - 1);
			first << "        " << setw(16) << left << "GT_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[GT_T] << lexeme << endl;
			t = GT_T;
			break;
		case 126:
			++pos;
			first << "        " << setw(16) << left << "SHIFTR_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[SHIFTR_T] << lexeme << endl;
			t = SHIFTR_T;
			break;
		case 127:
			++pos;
			first << "        " << setw(16) << left << "GTE_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[GTE_T] << lexeme << endl;
			t = GTE_T;
			break;
		case 128:
			lexeme.erase(lexeme.end() - 1);
			first << "        " << setw(16) << left << "AND_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[AND_T] << lexeme << endl;
			t = AND_T;
			break;
		case 129:
			++pos;
			first << "        " << setw(16) << left << "LOGAND_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[LOGAND_T] << lexeme << endl;
			t = LOGAND_T;
			break;
		case 130:
			lexeme.erase(lexeme.end() - 1);
			first << "        " << setw(16) << left << "OR_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[OR_T] << lexeme << endl;
			t = OR_T;
			break;
		case 131:
			++pos;
			first << "        " << setw(16) << left << "LOGOR_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[LOGOR_T] << lexeme << endl;
			t = LOGOR_T;
			break;
		case 132:
			++pos;
			first << "        " << setw(16) << left << "LBRACE_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[LBRACE_T] << lexeme << endl;
			t = LBRACE_T;
			break;
		case 133:
			++pos;
			first << "        " << setw(16) << left << "RBRACE_T" << lexeme << endl;
			//first << "        " << setw(16) << left << tokenMap[RBRACE_T] << lexeme << endl;
			t = RBRACE_T;
			break;
		default:
			++pos;
			break;

		}

	}

	return t;

}

string LexicalAnalyzer::get_lexeme() const {
	return lexeme;
}

void LexicalAnalyzer::report_error(const string & msg) {
	first << msg << endl;
	debug << msg << endl;

}
token_type LexicalAnalyzer::IdentOrKeyWord() {
	//Very simple, do a == check and return which one else return a IDENT_T
	if (lexeme == "int") {
		return INTTYPE_T;
	} else if (lexeme == "float") {
		return DBLTYPE_T;
	} else if (lexeme == "main") {
		return MAIN_T;
	} else if (lexeme == "if") {
		return IF_T;
	} else if (lexeme == "else") {
		return ELSE_T;
	} else if (lexeme == "while") {
		return WHILE_T;
	} else if (lexeme == "void") {
		return VOID_T;
	} else {
		return IDENT_T;
	}
}
int LexicalAnalyzer::GetChar(char c) {
	//Very simple, take the char and check what it is
	// Based on what it is, return the proper state table location
	// else return 24 for other
	if (c == '_') {
		return 1;
	} else if (c == '.') {
		return 3;
	} else if (c == ';') {
		return 4;
	} else if (c == '+') {
		return 5;
	} else if (c == '-') {
		return 6;
	} else if (c == '/') {
		return 7;
	} else if (c == '*') {
		return 8;
	} else if (c == '%') {
		return 9;
	} else if (c == '=') {
		return 10;
	} else if (c == '<') {
		return 11;
	} else if (c == '>') {
		return 12;
	} else if (c == '&') {
		return 13;
	} else if (c == '^') {
		return 14;
	} else if (c == '|') {
		return 15;
	} else if (c == '!') {
		return 16;
	} else if (c == '~') {
		return 17;
	} else if (c == '(') {
		return 18;
	} else if (c == ')') {
		return 19;
	} else if (c == ';') {
		return 20;
	} else if (c == '?') {
		return 21;
	} else if (c == ':') {
		return 22;
	} else if (c == ',') {
		return 23;
	} else if (c == '{') {
		return 25;
	} else if (c == '}') {
		return 26;
	} else {
		return 24;
	}
}
void LexicalAnalyzer::skipspace() {
	// THis is case one, we are going to burn out any spaces or tabs from where ever we start
	// Just keeps going until you dont see the space or tab any more
	if ((line[pos] == ' ') || (line[pos] == '\t')) {
		while ((line[pos] == ' ') || (line[pos] == '\t')) {
			++pos;
		}
	}
	// Then you come to case two, did you eat up all the spaces and tabs and end at a end of line symbol for string?
	// If you did check to see if you can get a new line from input
	// If you can do the normal behavior and recursively call skipspace() again to skip any spaces on the new line
	// I got the recursive call idea because what if some one made a bunch of spaces and lines, have to burn through them all
	// If getline fails we are at end of line, set my bool eof flag too true, so next time you call get_token
	// You will use skipspace, then check if eof = true? it is okay return EOF_T and get out of get_token.
	if ((line[pos] == '\0')) {
		if (getline(input, line)) { // make sure we even have a new line to get
			pos = 0; // reset our position
			++linecount; // increase our line number
			//print the new lines again.
			first << setw(3) << ' ' << linecount << ": " << line << endl;
			debug << setw(3) << ' ' << linecount << ": " << line << endl;
			skipspace();
		} else {
			eof = true;
		}

	}

}

void LexicalAnalyzer::loadDefaults() {
// This might have looked like it took a long time but it did not.
// I used excel and kept using find/replace table[0] with table[1]
// You get the idea, not much typing on my part just copy and paste.
// Then replacing each number the same way got easier once I got past state 2

//Row 0
	table[0][0] = 1;
	table[0][1] = 1;
	table[0][2] = 2;
	table[0][3] = 3;
	table[0][4] = 103;
	table[0][5] = 5;
	table[0][6] = 6;
	table[0][7] = 106;
	table[0][8] = 107;
	table[0][9] = 108;
	table[0][10] = 7;
	table[0][11] = 9;
	table[0][12] = 10;
	table[0][13] = 11;
	table[0][14] = 109;
	table[0][15] = 12;
	table[0][16] = 8;
	table[0][17] = 110;
	table[0][18] = 111;
	table[0][19] = 112;
	table[0][20] = 103;
	table[0][21] = 113;
	table[0][22] = 114;
	table[0][23] = 115;
	table[0][24] = 99;
	table[0][25] = 132;
	table[0][26] = 133;

// Row 1
	table[1][0] = 1;
	table[1][1] = 1;
	table[1][2] = 1;
	table[1][3] = 101;
	table[1][4] = 101;
	table[1][5] = 101;
	table[1][6] = 101;
	table[1][7] = 101;
	table[1][8] = 101;
	table[1][9] = 101;
	table[1][10] = 101;
	table[1][11] = 101;
	table[1][12] = 101;
	table[1][13] = 101;
	table[1][14] = 101;
	table[1][15] = 101;
	table[1][16] = 101;
	table[1][17] = 101;
	table[1][18] = 101;
	table[1][19] = 101;
	table[1][20] = 101;
	table[1][21] = 101;
	table[1][22] = 101;
	table[1][23] = 101;
	table[1][24] = 101;
	table[1][25] = 101;
	table[1][26] = 101;

// Row 2
	table[2][0] = 102;
	table[2][1] = 102;
	table[2][2] = 2;
	table[2][3] = 4;
	table[2][4] = 102;
	table[2][5] = 102;
	table[2][6] = 102;
	table[2][7] = 102;
	table[2][8] = 102;
	table[2][9] = 102;
	table[2][10] = 102;
	table[2][11] = 102;
	table[2][12] = 102;
	table[2][13] = 102;
	table[2][14] = 102;
	table[2][15] = 102;
	table[2][16] = 102;
	table[2][17] = 102;
	table[2][18] = 102;
	table[2][19] = 102;
	table[2][20] = 102;
	table[2][21] = 102;
	table[2][22] = 102;
	table[2][23] = 102;
	table[2][24] = 102;
	table[2][25] = 102;
	table[2][26] = 102;

// Row 3
	table[3][0] = 100;
	table[3][1] = 100;
	table[3][2] = 4;
	table[3][3] = 100;
	table[3][4] = 100;
	table[3][5] = 100;
	table[3][6] = 100;
	table[3][7] = 100;
	table[3][8] = 100;
	table[3][9] = 100;
	table[3][10] = 100;
	table[3][11] = 100;
	table[3][12] = 100;
	table[3][13] = 100;
	table[3][14] = 100;
	table[3][15] = 100;
	table[3][16] = 100;
	table[3][17] = 100;
	table[3][18] = 100;
	table[3][19] = 100;
	table[3][20] = 100;
	table[3][21] = 100;
	table[3][22] = 100;
	table[3][23] = 100;
	table[3][24] = 100;
	table[3][25] = 100;
	table[3][26] = 100;

// Row 4
	table[4][0] = 102;
	table[4][1] = 102;
	table[4][2] = 4;
	table[4][3] = 102;
	table[4][4] = 102;
	table[4][5] = 102;
	table[4][6] = 102;
	table[4][7] = 102;
	table[4][8] = 102;
	table[4][9] = 102;
	table[4][10] = 102;
	table[4][11] = 102;
	table[4][12] = 102;
	table[4][13] = 102;
	table[4][14] = 102;
	table[4][15] = 102;
	table[4][16] = 102;
	table[4][17] = 102;
	table[4][18] = 102;
	table[4][19] = 102;
	table[4][20] = 102;
	table[4][21] = 102;
	table[4][22] = 102;
	table[4][23] = 102;
	table[4][24] = 102;
	table[4][25] = 102;
	table[4][26] = 102;

// Row 5
	table[5][0] = 105;
	table[5][1] = 105;
	table[5][2] = 105;
	table[5][3] = 105;
	table[5][4] = 105;
	table[5][5] = 104;
	table[5][6] = 105;
	table[5][7] = 105;
	table[5][8] = 105;
	table[5][9] = 105;
	table[5][10] = 105;
	table[5][11] = 105;
	table[5][12] = 105;
	table[5][13] = 105;
	table[5][14] = 105;
	table[5][15] = 105;
	table[5][16] = 105;
	table[5][17] = 105;
	table[5][18] = 105;
	table[5][19] = 105;
	table[5][20] = 105;
	table[5][21] = 105;
	table[5][22] = 105;
	table[5][23] = 105;
	table[5][24] = 105;
	table[5][25] = 105;
	table[5][26] = 105;

// Row 6
	table[6][0] = 116;
	table[6][1] = 116;
	table[6][2] = 116;
	table[6][3] = 116;
	table[6][4] = 116;
	table[6][5] = 116;
	table[6][6] = 117;
	table[6][7] = 116;
	table[6][8] = 116;
	table[6][9] = 116;
	table[6][10] = 116;
	table[6][11] = 116;
	table[6][12] = 116;
	table[6][13] = 116;
	table[6][14] = 116;
	table[6][15] = 116;
	table[6][16] = 116;
	table[6][17] = 116;
	table[6][18] = 116;
	table[6][19] = 116;
	table[6][20] = 116;
	table[6][21] = 116;
	table[6][22] = 116;
	table[6][23] = 116;
	table[6][24] = 116;
	table[6][25] = 116;
	table[6][26] = 116;

// Row 7
	table[7][0] = 119;
	table[7][1] = 119;
	table[7][2] = 119;
	table[7][3] = 119;
	table[7][4] = 119;
	table[7][5] = 119;
	table[7][6] = 119;
	table[7][7] = 119;
	table[7][8] = 119;
	table[7][9] = 119;
	table[7][10] = 118;
	table[7][11] = 119;
	table[7][12] = 119;
	table[7][13] = 119;
	table[7][14] = 119;
	table[7][15] = 119;
	table[7][16] = 119;
	table[7][17] = 119;
	table[7][18] = 119;
	table[7][19] = 119;
	table[7][20] = 119;
	table[7][21] = 119;
	table[7][22] = 119;
	table[7][23] = 119;
	table[7][24] = 119;
	table[7][25] = 119;
	table[7][26] = 119;

// Row 8
	table[8][0] = 120;
	table[8][1] = 120;
	table[8][2] = 120;
	table[8][3] = 120;
	table[8][4] = 120;
	table[8][5] = 120;
	table[8][6] = 120;
	table[8][7] = 120;
	table[8][8] = 120;
	table[8][9] = 120;
	table[8][10] = 121;
	table[8][11] = 120;
	table[8][12] = 120;
	table[8][13] = 120;
	table[8][14] = 120;
	table[8][15] = 120;
	table[8][16] = 120;
	table[8][17] = 120;
	table[8][18] = 120;
	table[8][19] = 120;
	table[8][20] = 120;
	table[8][21] = 120;
	table[8][22] = 120;
	table[8][23] = 120;
	table[8][24] = 120;
	table[8][25] = 120;
	table[8][26] = 120;

// Row 9
	table[9][0] = 122;
	table[9][1] = 122;
	table[9][2] = 122;
	table[9][3] = 122;
	table[9][4] = 122;
	table[9][5] = 122;
	table[9][6] = 122;
	table[9][7] = 122;
	table[9][8] = 122;
	table[9][9] = 122;
	table[9][10] = 124;
	table[9][11] = 123;
	table[9][12] = 122;
	table[9][13] = 122;
	table[9][14] = 122;
	table[9][15] = 122;
	table[9][16] = 122;
	table[9][17] = 122;
	table[9][18] = 122;
	table[9][19] = 122;
	table[9][20] = 122;
	table[9][21] = 122;
	table[9][22] = 122;
	table[9][23] = 122;
	table[9][24] = 122;
	table[9][25] = 122;
	table[9][26] = 122;

// Row 10
	table[10][0] = 125;
	table[10][1] = 125;
	table[10][2] = 125;
	table[10][3] = 125;
	table[10][4] = 125;
	table[10][5] = 125;
	table[10][6] = 125;
	table[10][7] = 125;
	table[10][8] = 125;
	table[10][9] = 125;
	table[10][10] = 127;
	table[10][11] = 125;
	table[10][12] = 126;
	table[10][13] = 125;
	table[10][14] = 125;
	table[10][15] = 125;
	table[10][16] = 125;
	table[10][17] = 125;
	table[10][18] = 125;
	table[10][19] = 125;
	table[10][20] = 125;
	table[10][21] = 125;
	table[10][22] = 125;
	table[10][23] = 125;
	table[10][24] = 125;
	table[10][25] = 125;
	table[10][26] = 125;

// Row 11
	table[11][0] = 128;
	table[11][1] = 128;
	table[11][2] = 128;
	table[11][3] = 128;
	table[11][4] = 128;
	table[11][5] = 128;
	table[11][6] = 128;
	table[11][7] = 128;
	table[11][8] = 128;
	table[11][9] = 128;
	table[11][10] = 128;
	table[11][11] = 128;
	table[11][12] = 128;
	table[11][13] = 129;
	table[11][14] = 128;
	table[11][15] = 128;
	table[11][16] = 128;
	table[11][17] = 128;
	table[11][18] = 128;
	table[11][19] = 128;
	table[11][20] = 128;
	table[11][21] = 128;
	table[11][22] = 128;
	table[11][23] = 128;
	table[11][24] = 128;
	table[11][25] = 128;
	table[11][26] = 128;

// Row 12
	table[12][0] = 130;
	table[12][1] = 130;
	table[12][2] = 130;
	table[12][3] = 130;
	table[12][4] = 130;
	table[12][5] = 130;
	table[12][6] = 130;
	table[12][7] = 130;
	table[12][8] = 130;
	table[12][9] = 130;
	table[12][10] = 130;
	table[12][11] = 130;
	table[12][12] = 130;
	table[12][13] = 130;
	table[12][14] = 130;
	table[12][15] = 131;
	table[12][16] = 130;
	table[12][17] = 130;
	table[12][18] = 130;
	table[12][19] = 130;
	table[12][20] = 130;
	table[12][21] = 130;
	table[12][22] = 130;
	table[12][23] = 130;
	table[12][24] = 130;
	table[12][25] = 130;
	table[12][26] = 130;

	//This token map has lil to much in it at this point.
	// I am keeping it for now but writing note now that most can be removed
	// Just need the indent, numlit, inttype, dbl etc.
	tokenMap[IDENT_T] = "IDENT_T";
	tokenMap[NUMLIT_T] = "NUMLIT_T";
	tokenMap[INTTYPE_T] = "INTTYPE_T";
	tokenMap[DBLTYPE_T] = "DBLTYPE_T";
	tokenMap[MAIN_T] = "MAIN_T";
	tokenMap[IF_T] = "IF_T";
	tokenMap[ELSE_T] = "ELSE_T";
	tokenMap[WHILE_T] = "WHILE_T";
	tokenMap[VOID_T] = "VOID_T";
	tokenMap[PLUS_T] = "PLUS_T";
	tokenMap[MINUS_T] = "MINUS_T";
	tokenMap[DIV_T] = "DIV_T";
	tokenMap[MULT_T] = "MULT_T";
	tokenMap[MOD_T] = "MOD_T";
	tokenMap[MINUSMINUS_T] = "MINUSMINUS_T";
	tokenMap[PLUSPLUS_T] = "PLUSPLUS_T";
	tokenMap[ASSIGN_T] = "ASSIGN_T";
	tokenMap[EQUALTO_T] = "EQUALTO_T";
	tokenMap[NOTEQ_T] = "NOTEQ_T";
	tokenMap[GT_T] = "GT_T";
	tokenMap[LT_T] = "LT_T";
	tokenMap[GTE_T] = "GTE_T";
	tokenMap[LTE_T] = "LTE_T";
	tokenMap[LOGAND_T] = "LOGAND_T";
	tokenMap[LOGOR_T] = "LOGOR_T";
	tokenMap[NOT_T] = "NOT_T";
	tokenMap[AND_T] = "AND_T";
	tokenMap[OR_T] = "OR_T";
	tokenMap[XOR_T] = "XOR_T";
	tokenMap[SHIFTL_T] = "SHIFTL_T";
	tokenMap[SHIFTR_T] = "SHIFTR_T";
	tokenMap[TILDA_T] = "TILDA_T";
	tokenMap[LPAREN_T] = "LPAREN_T";
	tokenMap[RPAREN_T] = "RPAREN_T";
	tokenMap[LBRACE_T] = "LBRACE_T";
	tokenMap[RBRACE_T] = "RBRACE_T";
	tokenMap[SEMI_T] = "SEMI_T";
	tokenMap[COMMA_T] = "COMMA_T";
	tokenMap[ERROR_T] = "ERROR_T";
	tokenMap[EOF_T] = "EOF_T";
	tokenMap[QUEST_T] = "QUEST_T";
	tokenMap[COLON_T] = "COLON_T";

}

