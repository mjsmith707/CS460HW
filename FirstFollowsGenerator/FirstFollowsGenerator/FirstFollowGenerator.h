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
#include <vector>
#include <string>
#include <algorithm>
#include <memory>

class FirstFollowGenerator {
    private:
        // List of terminal symbols
        std::vector<std::shared_ptr<Grammar::Symbol>> terminals;
    
        // List of nonterminal symbols
        std::vector<std::shared_ptr<Grammar::Symbol>> nonterminals;
    
        // List of rules
        std::vector<Grammar::Rule> rules;
    
        // For adding grammar rules
        // Left is a symbol name, right is a list of symbol names
        void addRule(std::string& left, std::vector<std::string>& right);
    
        // For testing if a vector already contains a symbol
        bool contains(std::shared_ptr<Grammar::Symbol>& symbol, std::vector<std::shared_ptr<Grammar::Symbol>>& set);
    
    public:
        FirstFollowGenerator();
    
        // For adding symbols
        // Symbol name and whether it is a terminal or nonterminal
        std::shared_ptr<Grammar::Symbol> addSymbol(std::string name, bool isTerminal);
    
        void addRules(const std::vector<std::string>& productions);
    
        // Generate First Follow set for all symbols
        void generateSets();
    
        // Return the rules list
        std::vector<Grammar::Rule>& getRules();
    
        void printRules();
        void printFirsts();
        void printFollows();
    
};
#endif /* defined(__FirstFollowsGenerator__FirstFollowsGenerator__) */
