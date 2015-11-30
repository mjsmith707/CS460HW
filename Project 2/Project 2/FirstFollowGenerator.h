//
//  FirstFollowsGenerator.h
//  FirstFollowsGenerator
//
//  Created by Matt on 11/18/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#ifndef __FirstFollowsGenerator__FirstFollowsGenerator__
#define __FirstFollowsGenerator__FirstFollowsGenerator__

#include "Grammar.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <map>

// Generates the First, Follow, and Predict sets for a given Grammar (Grammar.h)
class FirstFollowGenerator {
    private:
        // List of terminal symbols
        std::vector<std::shared_ptr<Grammar::Symbol>> terminals;
    
        // List of nonterminal symbols
        std::vector<std::shared_ptr<Grammar::Symbol>> nonterminals;
    
        // List of rules
        std::vector<Grammar::Rule> rules;
    
        // Parse Table
        std::map<Grammar::parseKey_t, Grammar::Rule> parseTable;
    
        // For adding grammar rules
        // Left is a symbol name, right is a list of symbol names
        void addRule(std::string& left, std::vector<std::string>& right);
    
        void generateFirsts();
    
        void generateFollows();
    
        void generatePredicts();
    
        void generateParseTable();
    
        // For attempting a unique vector insert
        bool tryInsert(std::shared_ptr<Grammar::Symbol>& symbol, std::vector<std::shared_ptr<Grammar::Symbol>>& set);
    
        // Converts a name to a Grammar token
        Grammar::TerminalEnums getTokenName(std::string& name);
    
    public:
        FirstFollowGenerator();
    
        // For adding symbols
        // Symbol name and whether it is a terminal or nonterminal
        std::shared_ptr<Grammar::Symbol> addSymbol(std::string name, bool isTerminal);
    
        void addRules(const std::vector<std::string>& productions);
    
        // Generate First, Follow and Predict set for all symbols/rules
        void generateSets();
    
        // Return the rules list
        std::vector<Grammar::Rule>& getRules();
    
        // Return the parse table
        std::map<Grammar::parseKey_t, Grammar::Rule>& getParseTable();
    
        void printRules(std::fstream&);
        void printFirsts(std::fstream&);
        void printFollows(std::fstream&);
        void printPredicts(std::fstream&);
    
};
#endif /* defined(__FirstFollowsGenerator__FirstFollowsGenerator__) */
