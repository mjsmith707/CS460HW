//
//  LexicalAnalyzer.h
//  Scheme_Tokenizer
//
//  Created by Matt on 10/1/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#ifndef __Scheme_Tokenizer__LexicalAnalyzer__
#define __Scheme_Tokenizer__LexicalAnalyzer__

#include <vector>
#include <string>
#include "SchemeLexemes.h"

class LexicalAnalyzer {
    public:
        // Token struct
        typedef struct Token {
            SchemeLexemes lexeme;
            std::string value;
            size_t lineNum;
        } Token;
    
    private:
        // The massive state table
        static const unsigned char statetbl[87][40];
    
        // Some relation numbers for the state table
        static const unsigned char TERMINAL_STATE = 99;
        static const unsigned char ERROR_STATE = 99;
        static const unsigned char UNCLOSED_LIT_ERROR_STATE = 135;
        static const unsigned char PUTBACK_STATE = 108;
        static const unsigned char STATE_OFFSET = 100;
    
        // Variable for current DFA state
        unsigned char state;
    
        // Line position
        size_t pos;
    
        // Current char being read
        char curr;
    
        // Line Number
        size_t lineNum;
    
        // Error Count
        size_t errorCount;
        
        // Errors List
        std::vector<std::string> errors;
    
        // Inline functions for generating tokens
        inline void generateError(std::vector<Token>& result, const std::string& line, std::string& tokenstr);
        inline void generateToken(std::vector<Token>& result, const std::string& line, std::string& tokenstr);
    
        // Char to state table column mapping function
        inline unsigned char charToCol();
    
        // Casting function for int to enum
        inline SchemeLexemes terminalToLexeme();
        
    public:
        // CTOR
        LexicalAnalyzer();
    
        // Tokenizing function
        std::vector<Token> getTokens(std::string& newline);
    
        // Return last errors
        std::vector<std::string> getErrors();
    
        // Return error count
        size_t getErrorCount();
    
        // Clear errors vector
        void resetErrors();
    
        // Zero out errors count
        void resetErrorCount();
};

#endif /* defined(__Scheme_Tokenizer__LexicalAnalyzer__) */