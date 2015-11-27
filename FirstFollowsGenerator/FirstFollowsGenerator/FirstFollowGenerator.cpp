//
//  FirstfollowGenerator.cpp
//  FirstfollowGenerator
//
//  Created by Matt on 11/18/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#include "FirstFollowGenerator.h"

FirstFollowGenerator::FirstFollowGenerator() {

}

// For adding symbols
// Symbol name and whether it is a terminal or nonterminal
std::shared_ptr<Grammar::Symbol> FirstFollowGenerator::addSymbol(std::string name, bool isTerminal) {
    std::shared_ptr<Grammar::Symbol> newSymbol(new Grammar::Symbol());
    newSymbol->name = name;
    newSymbol->terminal = isTerminal;
    newSymbol->lambda = false;
    if (isTerminal) {
        if (name.compare("LAMBDA") == 0) {
            newSymbol->lambda = true;
        }
        terminals.push_back(newSymbol);
    }
    else {
        nonterminals.push_back(newSymbol);
    }
    return newSymbol;
}

// For adding a vector of strings containing BNF rules in the form:
// symbol -> symbol ...
// E.x:
// <program> -> <stmt> <stmt_list> EOF_T
void FirstFollowGenerator::addRules(const std::vector<std::string>& productions) {
    for (auto& line : productions) {
        // Find left side
        std::string left = "";
        size_t delim = line.find("->");
        if (delim == line.npos) {
            std::string error = "Invalid production rule. Missing -> delimiter: " + line;
            throw std::runtime_error(error);
        }
        left = line.substr(0, delim-1);
        
        std::vector<std::string> right;
        std::string temp = "";
        // Right side
        for (size_t i=delim+3; i<=line.size(); i++) {
            if (i == line.size()) {
                right.push_back(temp);
            }
            else if (line.at(i) == ' ') {
                right.push_back(temp);
                temp = "";
            }
            else {
                temp += line.at(i);
            }
        }
        
        addRule(left, right);
    }
}

// For adding grammar rules
// Left is a symbol name, right is a list of symbol names
void FirstFollowGenerator::addRule(std::string& left, std::vector<std::string>& right) {
    Grammar::Rule newRule;
    
    bool foundnt = false;
    std::shared_ptr<Grammar::Symbol> leftsymbol;
    std::vector<std::shared_ptr<Grammar::Symbol>> rightsymbols;
    for (auto& i : nonterminals) {
        if (i->name == left) {
            leftsymbol = i;
            foundnt = true;
            break;
        }
    }
    
    if (!foundnt) {
        std::string error = "Failed to find nonterminal symbol: " + left;
        throw std::runtime_error(error);
    }
    
    for (auto& name : right) {
        if (name.size() == 0) {
            break;
        }
        bool foundt = false;
        for (auto& i : nonterminals) {
            if (i->name == name) {
                foundt = true;
                rightsymbols.push_back(i);
                break;
            }
        }
        if (!foundt) {
            for (auto& i : terminals) {
                if (i->name == name) {
                    foundt = true;
                    rightsymbols.push_back(i);
                    break;
                }
            }
            if (!foundt) {
                std::string error = "Failed to find symbol: " + name;
                throw std::runtime_error(error);
            }
        }
    }
    
    newRule.left = leftsymbol;
    newRule.right = rightsymbols;
    
    rules.push_back(newRule);
}

// Generate the first and follow sets
// There be dragons in here
void FirstFollowGenerator::generateSets() {
    // Generate first sets
    bool changed = true;
    while (changed) {
        changed = false;
        // Rule 1
        for (size_t i=0; i<rules.size(); i++) {
            Grammar::Rule& rule = rules.at(i);
            if ((!rule.left->terminal) && (rule.right.size() != 0)) {
                // Rule 1
                if ((rule.right.at(0)->terminal) ) {
                    if (!contains(rule.right.at(0), rule.left->first)) {
                        rule.left->first.push_back(rule.right.at(0));
                        changed = true;
                    }
                }
            }
        }
        
        // Rule 2
        for (size_t i=0; i<rules.size(); i++) {
            Grammar::Rule& rule = rules.at(i);
            if ((!rule.left->terminal) && (rule.right.size() != 0)) {
                // Rule 2
                if ((!rule.right.at(0)->terminal) ) {
                    for (auto& tk : rule.right.at(0)->first) {
                        if (!contains(tk, rule.left->first)) {
                            rule.left->first.push_back(tk);
                            changed = true;
                        }
                    }
                }
                
            }
        }
    }
        
    // Generate follow sets
    changed = true;
    while (changed) {
        changed = false;
        // Rule 3
        for (size_t i=0; i<rules.size(); i++) {
            Grammar::Rule& rule = rules.at(i);
            if ((!rule.left->terminal) && (rule.right.size() == 1)) {
                auto& nti = rule.left;
                auto& lamb = rule.right.at(0);
                if (lamb->lambda) {
                    for (auto& tk : nti->follow) {
                        if ((!contains(tk, nti->first))) {
                            nti->first.push_back(tk);
                            changed = true;
                        }
                    }
                }
            }
        }
        
        // Rule 4
        for (long long i=rules.size()-1; i>=0; i--) {
            Grammar::Rule& rule = rules.at(i);
            if (rule.right.size() > 1) {
                for (size_t j=1; j<rule.right.size()-1; j++) {
                    auto& nti = rule.right.at(j);
                    auto& tk = rule.right.at(j+1);
                    if ((!nti->terminal) && (tk->terminal) && (!tk->lambda)) {
                        if (!contains(tk, nti->follow)) {
                            nti->follow.push_back(tk);
                            changed = true;
                        }
                    }
                }
            }
        }
        
        // Rule 5
        for (size_t i=0; i<rules.size(); i++) {
            Grammar::Rule& rule = rules.at(i);
            if (rule.right.size() > 1) {
                for (size_t j=1; j<rule.right.size()-1; j++) {
                    auto& nti = rule.right.at(j);
                    auto& ntj = rule.right.at(j+1);
                    if ((!nti->terminal) && (!ntj->terminal)) {
                        for (auto& tk : ntj->first) {
                            if ((tk->terminal) && (!tk->lambda) && (!contains(tk, nti->follow))) {
                                nti->follow.push_back(tk);
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
        
        // Rule 6
        for (size_t i=0; i<rules.size(); i++) {
            Grammar::Rule& rule = rules.at(i);
            auto& nti = rule.left;
            if ((!nti->terminal) && (rule.right.size() > 0)) {
                auto& ntj = rule.right.at(rule.right.size()-1);
                if (!ntj->terminal) {
                    for (auto& tk : nti->follow) {
                        if ((tk->terminal) && (!tk->lambda)) {
                            if (!contains(tk, ntj->follow)) {
                                ntj->follow.push_back(tk);
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
    }
}

// Comparator to keep std::vector as a unique set
bool FirstFollowGenerator::contains(std::shared_ptr<Grammar::Symbol>& symbol, std::vector<std::shared_ptr<Grammar::Symbol>>& set) {
    for (auto& i : set) {
        if (symbol->name.compare(i->name) == 0) {
            return true;
        }
    }
    return false;
}

// For debugging
void FirstFollowGenerator::printRules() {
    for (auto& rule : rules) {
        std::cout << rule.left->name << " -> ";
        for (auto& right : rule.right) {
            std::cout << right->name << " ";
        }
        std::cout << std::endl;
    }
}

void FirstFollowGenerator::printFirsts() {
    std::cout << "==Firsts==" << std::endl;
    for (auto& nt : nonterminals) {
        std::cout << nt->name << " = { ";
        for (auto& first : nt->first) {
            std::cout << first->name << ", ";
        }
        std::cout << "}" << std::endl;
    }
}

void FirstFollowGenerator::printFollows() {
    std::cout << "==Follows==" << std::endl;
    for (auto& nt : nonterminals) {
        std::cout << nt->name << " = { ";
        for (auto& follow : nt->follow) {
            std::cout << follow->name << ", ";
        }
        std::cout << "}" << std::endl;
    }
}