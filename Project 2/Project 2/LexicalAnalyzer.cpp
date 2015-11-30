#include <iomanip>
#include <cstdlib>
#include "LexicalAnalyzer.h"

using namespace std;

static string token_names[] = {	"SYMBOL_T", "NUMLIT_T", "CAR_T", "CDR_T", "CONS_T", "IF_T",
				"LISTOP_T", "AND_T", "OR_T", "NOT_T", "DEFINE_T", "NUMBERP_T",
				"SYMBOLP_T", "LISTP_T", "ZEROP_T", "NULLP_T", "CHARP_T",
				"STRINGP_T", "PLUS_T", "MINUS_T", "DIV_T", "MULT_T", "EQUALTO_T",
				"GT_T", "LT_T", "GTE_T", "LTE_T", "LPAREN_T", "RPAREN_T",
				"QUOTE_T", "ERROR_T", "EOF_T"}; 
static int table [][19] = 
//	  a   c   d   r   b   0   .   (   )   +   -   *   /   '   =   <   >   ?  other
       {{32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},  // 0 not used
	{ 8,  9,  8,  8,  8,  2,  3, 24, 24,  5,  6, 24, 24, 24, 24,  7,  7, 32, 24},  // 1 starting
	{22, 22, 22, 22, 22,  2,  4, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},  // 2 number
	{31, 31, 31, 31, 31,  4, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31},  // 3 period
	{22, 22, 22, 22, 22,  4, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22},  // 4 number after decimal
	{23, 23, 23, 23, 23,  2,  3, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23},  // 5 plus
	{23, 23, 23, 23, 23,  2,  3, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23},  // 6 minus
	{23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 24, 23, 23, 23, 23},  // 7 less than
	{ 8,  8,  8,  8,  8,  8, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 24, 21},  // 8 letter other than c
	{10,  8, 11,  8,  8,  8, 21, 21, 21, 24, 21, 10, 21, 21, 21, 21, 21, 21, 21},  // 9 letter c
	{ 8,  8, 11, 20,  8,  8, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21},  // 10 letter a after c
	{ 8,  8, 11, 20,  8,  8, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21}}; // 11 letter d after c, ca, or cd...

LexicalAnalyzer::LexicalAnalyzer (std::string filename, std::fstream& input, std::fstream& listing, std::fstream& debug) : input(input), listing(listing), debug(debug)//(char * filename)
{
	listing << "Input file: " << filename << endl;
	debug << "Input file: " << filename << endl;
	line = " ";
	linenum = 0;
	pos = 0;
	lexeme = "";
	errors = 0;
}

LexicalAnalyzer::~LexicalAnalyzer ()
{
	listing << errors << " errors found in input file\n";
	debug << errors << " errors found in input file\n";
}

Grammar::TerminalEnums LexicalAnalyzer::get_token ()
{
	static string valid = "acdrb0.()+-*/'=<>?";
	while (isspace(line[pos]))
		if (++pos >= line.length())
		{
			getline (input, line);
			if (input.fail())
				return Grammar::TerminalEnums::EOF_T;
			linenum++;
			listing << setw(4) << right << linenum << ": " << line << endl;	
			debug << setw(4) << right << linenum << ": " << line << endl;	
			line += ' ';
			pos = 0;
		}
	lexeme = "";
	int state = 1;
	Grammar::TerminalEnums token = Grammar::TerminalEnums::NONE;
	while (token == Grammar::TerminalEnums::NONE)
	{
		char c = line[pos++];
		lexeme += c;
		if (isalpha(c) && (c != 'a' && c != 'c' && c != 'd' && c != 'r')) 
			c = 'b';
		else if (isdigit(c))
			c = '0';
		int col = 0;
		while (col < valid.length() && valid[col] != c)
			col++;
		state = table[state][col];
		switch (state)
		{
		    case 20: // car, cdr, cadr, cddr, and other list operators
			if (lexeme == "car") token = Grammar::TerminalEnums::CAR_T;
			else if (lexeme == "cdr") token = Grammar::TerminalEnums::CDR_T;
			else token = Grammar::TerminalEnums::LISTOP_T;
			break;
		    case 21: // symbol or keyword
			pos--;
			lexeme = lexeme.erase(lexeme.length()-1,1);
			if (lexeme == "cons") token = Grammar::TerminalEnums::CONS_T;
			else if (lexeme == "if") token = Grammar::TerminalEnums::IF_T;
			else if (lexeme == "and") token = Grammar::TerminalEnums::AND_T;
			else if (lexeme == "or") token = Grammar::TerminalEnums::OR_T;
			else if (lexeme == "not") token = Grammar::TerminalEnums::NOT_T;
			else if (lexeme == "define") token = Grammar::TerminalEnums::DEFINE_T;
			else token = Grammar::TerminalEnums::SYMBOL_T;
			break;
		    case 22: // numeric literal
			pos--;
			lexeme = lexeme.erase(lexeme.length()-1,1);
			token = Grammar::TerminalEnums::NUMLIT_T;
			break;
		    case 23: // operator/symbol with backup
			pos--;
			lexeme = lexeme.erase(lexeme.length()-1,1);
		    case 24: // operator/symbol without backup
			if (lexeme == "number?") token = Grammar::TerminalEnums::NUMBERP_T;
			else if (lexeme == "symbol?") token = Grammar::TerminalEnums::SYMBOLP_T;
			else if (lexeme == "list?") token = Grammar::TerminalEnums::LISTP_T;
			else if (lexeme == "zero?") token = Grammar::TerminalEnums::ZEROP_T;
			else if (lexeme == "null?") token = Grammar::TerminalEnums::NULLP_T;
			else if (lexeme == "char?") token = Grammar::TerminalEnums::CHARP_T;
			else if (lexeme == "string?") token = Grammar::TerminalEnums::STRINGP_T;
			else if (lexeme == "+") token = Grammar::TerminalEnums::PLUS_T;
			else if (lexeme == "-") token = Grammar::TerminalEnums::MINUS_T;
			else if (lexeme == "/") token = Grammar::TerminalEnums::DIV_T;
			else if (lexeme == "*") token = Grammar::TerminalEnums::MULT_T;
			else if (lexeme == "=") token = Grammar::TerminalEnums::EQUALTO_T;
			else if (lexeme == "<") token = Grammar::TerminalEnums::LT_T;
			else if (lexeme == ">") token = Grammar::TerminalEnums::GT_T;
			else if (lexeme == "(") token = Grammar::TerminalEnums::LPAREN_T;
			else if (lexeme == ")") token = Grammar::TerminalEnums::RPAREN_T;
			else if (lexeme == "'") token = Grammar::TerminalEnums::QUOTE_T;
			else if (lexeme == "<=") token = Grammar::TerminalEnums::LTE_T;
			else if (lexeme == ">=") token = Grammar::TerminalEnums::GTE_T;
			else if (lexeme[lexeme.length()-1] == '?') token = Grammar::TerminalEnums::SYMBOL_T;
			else token = Grammar::TerminalEnums::ERROR_T;
			break;
		    case 31: // error with backup
			pos--;
			lexeme = lexeme.erase(lexeme.length()-1,1);
		    case 32: // error
			report_error (string("Invalid character found: ") + lexeme);
			errors++;
			token = Grammar::TerminalEnums::ERROR_T;
		}
	}
	debug << '\t' << setw(16) << left << token << lexeme << endl;
	return token;
}

string LexicalAnalyzer::get_token_name (Grammar::TerminalEnums t) const
{
	return TerminalStrings[static_cast<int>(t)];
}

string LexicalAnalyzer::get_lexeme () const
{
	return lexeme;
}

void LexicalAnalyzer::report_error (const string & msg)
{
	listing << "Error at " << linenum << ',' << pos << ": " << msg << endl;
	debug << "Error at " << linenum << ',' << pos << ": " << msg << endl;
}
