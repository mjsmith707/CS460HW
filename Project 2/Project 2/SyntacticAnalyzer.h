//
//  SyntacticAnalyzer.h
//  Project 2
//
//  Created by Matt on 11/24/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#ifndef Project_2_SyntacticAnalyzer_h
#define Project_2_SyntacticAnalyzer_h

#include <fstream>
#include <stack>
#include <memory>
#include <iostream>

template <typename G, typename T> class SyntacticAnalyzer {
    // Grammar Type
    std::shared_ptr<G> grammar;
    
    // Tokenizer
    std::shared_ptr<T> tokenizer;
    
    // File streams
    std::fstream& inputStream;
    std::fstream& lstStream;
    std::fstream& dbgStream;
    
    // Grammar rules with Start at 0
    std::vector<Grammar::Rule> rulesList;
    
    // Parse table
    std::map<Grammar::parseKey_t, Grammar::Rule> parseTable;
    
    // Global error count
    size_t errorCount = 0;
    
    // Recursive descent entry point
    void parse(std::shared_ptr<Grammar::Symbol>& start) {
        // Initialize Stack
        std::stack<std::shared_ptr<Grammar::Symbol>> stk;
        
        // Push start symbol
        stk.push(start);
        
        // Get first symbol
        auto token = tokenizer->get_token();
        
        while (!stk.empty()) {
            // Get top of stack
            auto& top = stk.top();
            dbgStream << "Top: " << top->name << std::endl;
            dbgStream << "Token: " << token << std::endl;
            // If not terminal
            if (!top->terminal) {
                auto itr = parseTable.find(Grammar::parseKey_t(top, token));
                // If table rule is found
                if (itr != parseTable.end()) {
                    dbgStream << "Accepted Nonterminal: " << itr->second.left->name << " <-> " << token << std::endl;
                    // Pop stack
                    stk.pop();
                    // Push righthand side of production to stack
                    for (long long i=itr->second.right.size()-1; i>=0; i--) {
                        auto& symbol = itr->second.right.at(i);
                        // Lambda is not a real symbol so don't include it
                        if (!symbol->lambda) {
                            stk.push(symbol);
                        }
                    }
                }
                else {
                    dbgStream << "Parse Error: " << top->name << " <-> " << token << std::endl;
                    reportError(top->name, token, top->follow[0]->name);
                    dbgStream << "== Stack Trace == " << std::endl;
                    while (!stk.empty()) {
                        auto& temp = stk.top();
                        stk.pop();
                        dbgStream << temp->name << std::endl;
                    }
                    dbgStream << "== End Stack == " << std::endl;
                    // Repush start symbol
                    stk.push(start);
                    // Get next token
                    token = tokenizer->get_token();
                }
            }
            else if (token == top->tokenName) {
                // Match terminal symbol in input
                dbgStream << "Accepted Terminal: " << top->tokenName << std::endl;
                stk.pop();
                
                // Get next token
                token = tokenizer->get_token();
            }
            else {
                dbgStream << "Parse Error2: " << top->name << " <-> " << token << std::endl;
                reportError(top->name, token, top->first[0]->name);
                dbgStream << "== Stack Trace == " << std::endl;
                while (!stk.empty()) {
                    auto& temp = stk.top();
                    stk.pop();
                    dbgStream << temp->name << std::endl;
                }
                dbgStream << "== End Stack == " << std::endl;
                // Repush start symbol
                stk.push(start);
                // Get next token
                token = tokenizer->get_token();
            }
        }
    }
    
    void reportError(std::string productionLeft, Grammar::TerminalEnums found, std::string expected) {
        lstStream << "In production: " << productionLeft << ". Found: '" << found << "' but expected '" << expected << "'" << std::endl;
        dbgStream << "In production: " << productionLeft << ". Found: '" << found << "' but expected '" << expected << "'" << std::endl;
        errorCount++;
    }
    
    public:
        SyntacticAnalyzer(std::shared_ptr<G>& inGrammar, std::shared_ptr<T>& inTokenizer, std::fstream& inputFile, std::fstream& lststream, std::fstream& dbgstream)
            : grammar(inGrammar), tokenizer(inTokenizer), inputStream(inputFile), lstStream(lststream), dbgStream(dbgstream) {
            
            rulesList = inGrammar->getRules();
            parseTable = inGrammar->getParseTable();
        }
    
        // Parse the token stream and return success or failure
        bool parseCode() {
            // Assuming start symbol is first in list
            auto& start = rulesList[0].left;
            parse(start);
            return errorCount == 0;
        }

        size_t getErrorCount() {
            return errorCount;
        }
};

#endif
