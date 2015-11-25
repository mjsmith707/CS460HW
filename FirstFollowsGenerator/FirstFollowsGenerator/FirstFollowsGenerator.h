//
//  FirstFollowsGenerator.h
//  FirstFollowsGenerator
//
//  Created by Matt on 11/18/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#ifndef __FirstFollowsGenerator__FirstFollowsGenerator__
#define __FirstFollowsGenerator__FirstFollowsGenerator__

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>

class FirstFollowsGenerator {
    public:
        // Symbol struct
        typedef struct Symbol {
            std::string name;
            bool terminal;
            bool lambda;
            std::vector<std::shared_ptr<Symbol>> firsts;
            std::vector<std::shared_ptr<Symbol>> follows;
            
            // Symbol comparator
            //friend bool operator<(const Symbol& a, const Symbol& b) {
            //    return a.name < b.name;
            //}
        } Symbol;
    
        // Rule struct
        typedef struct Rule {
            std::shared_ptr<Symbol> left;
            std::vector<std::shared_ptr<Symbol>> right;
        } Rule;
    
    
    private:
        // List of terminal symbols
        std::vector<std::shared_ptr<Symbol>> terminals;
    
        // List of nonterminal symbols
        std::vector<std::shared_ptr<Symbol>> nonterminals;
    
        // List of rules
        std::vector<Rule> rules;
    
        // For adding grammar rules
        // Left is a symbol name, right is a list of symbol names
        void addRule(std::string& left, std::vector<std::string>& right);
    
        bool contains(std::shared_ptr<Symbol>& symbol, std::vector<std::shared_ptr<Symbol>>& set);
    
    public:
        FirstFollowsGenerator();
    
        // For adding symbols
        // Symbol name and whether it is a terminal or nonterminal
        std::shared_ptr<Symbol> addSymbol(std::string name, bool isTerminal);
    
        void addRules(std::vector<std::string>& productions);
    
        // Generate a FF Set
        void generateSets();
    
        void printRules();
    
        void printFirsts();
        void printFollows();
    
};
#endif /* defined(__FirstFollowsGenerator__FirstFollowsGenerator__) */
