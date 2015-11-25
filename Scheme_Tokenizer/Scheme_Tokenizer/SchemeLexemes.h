//
//  SchemeLexemes.h
//  Scheme_Tokenizer
//
//  Created by Matt on 10/1/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#ifndef Scheme_Tokenizer_SchemeLexemes_h
#define Scheme_Tokenizer_SchemeLexemes_h

#include <string>

enum class SchemeLexemes {
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
    CDDR_T = 13,
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
    LEXICAL_ERROR = 34
    // 35 = Unclosed String Literal
};

static const std::string lexemeStrings[35] = {
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
"CDDR_T",
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
"LERROR_T"
};

#endif