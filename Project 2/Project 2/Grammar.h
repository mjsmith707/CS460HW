//
//  Grammar.h
//  FirstFollowsGenerator
//
//  Created by Matt on 11/24/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#ifndef FirstFollowsGenerator_Grammar_h
#define FirstFollowsGenerator_Grammar_h

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <map>

// Class that holds Grammar definitions
class Grammar {
    public:
        // Public structure definitions
        enum class TerminalEnums {
            PLUS_T = 0,
            MINUS_T = 1,
            DIV_T = 2,
            MULT_T = 3,
            LPAREN_T = 4,
            RPAREN_T = 5,
            QUOTE_T = 6,
            COMMENT_T =7,
            GTE_T = 8,
            GT_T = 9,
            LTE_T = 10,
            LT_T = 11,
            CAR_T = 12,
            CDR_T = 13,
            CONS_T = 14,
            CHARP_T = 15,
            IF_T = 16,
            ELSE_T = 17,
            AND_T = 18,
            OR_T = 19,
            NOT_T = 20,
            NULLP_T = 21,
            NUMBERP_T = 22,
            SYMBOLP_T = 23,
            STRINGP_T = 24,
            LISTP_T = 25,
            ZEROP_T = 26,
            DEFINE_T = 27,
            EQUALTO_T = 28,
            SYMBOL_T = 29,
            NUMLIT_T = 30,
            LAMBDA_T = 31,
            STRING_T = 32,
            ERROR_T = 33,
            EOF_T = 34,
            LAMBDA=35,
            LEXICAL_ERROR = 36,
            // 37 = Unclosed String Literal?
            LISTOP_T = 37,
            NONTERMINAL_T
        };
        
        // Symbol struct
        typedef struct Symbol {
            std::string name;
            TerminalEnums tokenName;
            bool terminal;
            bool lambda;
            std::vector<std::shared_ptr<Symbol>> first;
            std::vector<std::shared_ptr<Symbol>> follow;
        } Symbol;

        // Rule struct
        typedef struct Rule {
            std::shared_ptr<Symbol> left;
            std::vector<std::shared_ptr<Symbol>> right;
            std::vector<std::shared_ptr<Symbol>> predict;
        } Rule;
    
        // Parse table key type
        typedef std::pair<std::shared_ptr<Symbol>, TerminalEnums> parseKey_t;
    
        // Language symbols and productions
        static const std::vector<std::string> TerminalStrings;
        static const std::vector<std::string> NonterminalStrings;
        static const std::vector<std::string> RulesStrings;
    
    private:
        // Vector of generated rules
        std::vector<Rule> rules;
    
        // Parse Table
        std::map<Grammar::parseKey_t, Grammar::Rule> parseTable;
    
    public:
        // Constructor
        Grammar(std::vector<Grammar::Rule>& rules, std::map<Grammar::parseKey_t, Grammar::Rule>& table);
    
        std::vector<Grammar::Rule> getRules();
    
        std::map<Grammar::parseKey_t, Grammar::Rule> getParseTable();
    
        // Enum stream operator
        friend std::ostream& operator<<(std::ostream& stream, TerminalEnums t);
    
        // parseTable comparator
        friend bool operator< (const Grammar::parseKey_t& left, const Grammar::parseKey_t& right) {
            auto& lptr = std::get<0>(left);
            auto& rptr = std::get<0>(right);
            if (lptr == rptr) {
                auto& ltok = std::get<1>(left);
                auto& rtok = std::get<1>(right);
                return ltok < rtok;
            }
            else {
                return lptr < rptr;
            }
        }
};

// List of Terminal Strings, Aligned with TerminalEnums
const std::vector<std::string> TerminalStrings = {
    "PLUS_T",
    "MINUS_T",
    "DIV_T",
    "MULT_T",
    "LPAREN_T",
    "RPAREN_T",
    "QUOTE_T",
    "COMMENT_T",
    "GTE_T",
    "GT_T",
    "LTE_T",
    "LT_T",
    "CAR_T",
    "CDR_T",
    "CONS_T",
    "CHARP_T",
    "IF_T",
    "ELSE_T",
    "AND_T",
    "OR_T",
    "NOT_T",
    "NULLP_T",
    "NUMBERP_T",
    "SYMBOLP_T",
    "STRINGP_T",
    "LISTP_T",
    "ZEROP_T",
    "DEFINE_T",
    "EQUALTO_T",
    "SYMBOL_T",
    "NUMLIT_T",
    "LAMBDA_T",
    "STRING_T",
    "ERROR_T",
    "EOF_T",
    "LAMBDA",
    "LERROR_T",
    "LISTOP_T",
    "NONTERMINAL_T"
};

// List of scheme nonterminal symbols
const std::vector<std::string> NonterminalStrings = {"<program>", "<stmt>", "<stmt_list>", "<literal>", "<quoted_lit>", "<action>", "<param_list>", "<else_part>"};

// List of scheme production rules
const std::vector<std::string> RulesStrings = {
    "<program> -> <stmt> <stmt_list> EOF_T",
    "<stmt_list> -> <stmt> <stmt_list>",
    "<stmt_list> -> LAMBDA",
    "<stmt> -> <literal>",
    "<stmt> -> LPAREN_T <action> RPAREN_T",
    "<literal> -> NUMLIT_T",
    "<literal> -> SYMBOL_T",
    "<literal> -> QUOTE_T <quoted_lit>",
    "<quoted_lit> -> SYMBOL_T",
    "<quoted_lit> -> NUMLIT_T",
    "<quoted_lit> -> LPAREN_T <stmt_list> RPAREN_T",
    //"<action> -> DEFINE_T LPAREN_T SYMBOL_T <param_list> RPAREN_T <stmt>",
    "<action> -> DEFINE_T SYMBOL_T <param_list> <stmt>",    // Modified
    "<param_list> -> SYMBOL_T <param_list>",
    "<param_list> -> LAMBDA",
    "<action> -> IF_T <stmt> <stmt> <else_part>",
    "<else_part> -> <stmt>",
    "<else_part> -> LAMBDA",
    "<action> -> CAR_T <stmt>",
    "<action> -> CDR_T <stmt>",
    "<action> -> CONS_T <stmt> <stmt>",
    "<action> -> AND_T <stmt_list>",
    "<action> -> OR_T <stmt_list>",
    "<action> -> NOT_T <stmt>",
    "<action> -> LISTOP_T <stmt>",
    "<action> -> NUMBERP_T <stmt>",
    "<action> -> SYMBOLP_T <stmt>",
    "<action> -> LISTP_T <stmt>",
    "<action> -> ZEROP_T <stmt>",
    "<action> -> NULLP_T <stmt>",
    "<action> -> CHARP_T <stmt>",
    "<action> -> STRINGP_T <stmt>",
    "<action> -> PLUS_T <stmt_list>",
    "<action> -> MINUS_T <stmt> <stmt_list>",
    "<action> -> DIV_T <stmt> <stmt_list>",
    "<action> -> MULT_T <stmt_list>",
    "<action> -> EQUALTO_T <stmt_list>",
    "<action> -> GT_T <stmt_list>",
    "<action> -> LT_T <stmt_list>",
    "<action> -> GTE_T <stmt_list>",
    "<action> -> LTE_T <stmt_list>",
    "<action> -> SYMBOL_T <stmt_list>"
};

#endif
