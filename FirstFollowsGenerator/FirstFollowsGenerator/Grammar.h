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

class Grammar {
    public:
        // Symbol struct
        typedef struct Symbol {
            std::string name;
            bool terminal;
            bool lambda;
            std::vector<std::shared_ptr<Symbol>> first;
            std::vector<std::shared_ptr<Symbol>> follow;
        } Symbol;

        // Rule struct
        typedef struct Rule {
            std::shared_ptr<Symbol> left;
            std::vector<std::shared_ptr<Symbol>> right;
        } Rule;
    
        static const std::vector<std::string> terminal_strings;
        static const std::vector<std::string> nonterminal_strings;
        static const std::vector<std::string> rules_strings;
    
        Grammar();
};

// List of scheme terminal symbols
const std::vector<std::string> terminal_strings = {"EOF_T", "SYMBOL_T", "NUMLIT_T", "CAR_T", "CDR_T", "CONS_T", "IF_T", "AND_T", "OR_T", "NOT_T", "DEFINE_T", "NUMBERP_T", "SYMBOLP_T", "LISTP_T",
                                            "ZEROP_T", "NULLP_T", "CHARP_T", "STRINGP_T", "PLUS_T", "MINUS_T", "DIV_T", "MULT_T", "EQUALTO_T", "GT_T", "LT_T", "GTE_T", "LTE_T", "LPAREN_T",
                                            "RPAREN_T", "QUOTE_T", "ERROR_T", "LISTOP_T", "LAMBDA"};

// List of scheme nonterminal symbols
const std::vector<std::string> nonterminal_strings = {"<program>", "<stmt>", "<stmt_list>", "<literal>", "<quoted_lit>", "<action>", "<param_list>", "<else_part>"};

// List of scheme production rules
const std::vector<std::string> rules_strings = {
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
    "<action> -> DEFINE_T LPAREN_T SYMBOL_T <param_list> RPAREN_T <stmt>",
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
