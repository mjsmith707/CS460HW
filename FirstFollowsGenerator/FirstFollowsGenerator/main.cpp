//
//  main.cpp
//  FirstFollowsGenerator
//
//  Created by Matt on 11/18/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#include <iostream>
#include <vector>
#include "FirstFollowsGenerator.h"

int main(int argc, const char * argv[]) {
    FirstFollowsGenerator ffgen;
    
    std::vector<std::string> terminals = {"EOF_T", "SYMBOL_T", "NUMLIT_T", "CAR_T", "CDR_T", "CONS_T", "IF_T", "AND_T", "OR_T", "NOT_T", "DEFINE_T", "NUMBERP_T", "SYMBOLP_T", "LISTP_T",
                                            "ZEROP_T", "NULLP_T", "CHARP_T", "STRINGP_T", "PLUS_T", "MINUS_T", "DIV_T", "MULT_T", "EQUALTO_T", "GT_T", "LT_T", "GTE_T", "LTE_T", "LPAREN_T",
                                            "RPAREN_T", "QUOTE_T", "ERROR_T", "LISTOP_T", "LAMBDA"};
    
    std::vector<std::string> nonterminals = {"<program>", "<stmt>", "<stmt_list>", "<literal>", "<quoted_lit>", "<action>", "<param_list>", "<else_part>"};
    
    for (auto& i : terminals) {
        ffgen.addSymbol(i, true);
    }
    
    for (auto& i : nonterminals) {
        ffgen.addSymbol(i, false);
    }
    
    std::vector<std::string> rules = {
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
    
    ffgen.addRules(rules);
    
    ffgen.printRules();
    
    ffgen.generateSets();
    
    ffgen.printFirsts();
    ffgen.printFollows();
    
    return 0;
}
