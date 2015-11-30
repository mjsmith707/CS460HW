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
    newSymbol->tokenName = getTokenName(name);
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

// Converts a string name to TerminalEnum
Grammar::TerminalEnums FirstFollowGenerator::getTokenName(std::string& name) {
    for (size_t i=0; i<TerminalStrings.size(); i++) {
        if (TerminalStrings.at(i).compare(name) == 0) {
            return static_cast<Grammar::TerminalEnums>(i);
        }
    }
    return Grammar::TerminalEnums::NONTERMINAL_T;
}

// For adding a vector of strings containing rules in the form:
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
    // Map left string  to a left symbol
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
    
    // Map each right string to a right symbol
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
    generateFirsts();
    generateFollows();
    generatePredicts();
    generateParseTable();
}

// Generate first sets
void FirstFollowGenerator::generateFirsts() {
    bool changed = true;
    while (changed) {
        changed = false;
        
        // Rule 1
        for (size_t i=0; i<rules.size(); i++) {
            Grammar::Rule& rule = rules.at(i);
            if ((!rule.left->terminal) && (rule.right.size() != 0)) {
                // Rule 1
                if ((rule.right.at(0)->terminal) ) {
                    changed |= tryInsert(rule.right.at(0), rule.left->first);
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
                        changed |= tryInsert(tk, rule.left->first);
                    }
                }
                
            }
        }
    }
    
    // Add each terminal as a first of itself
    for (auto& terminal : terminals) {
        tryInsert(terminal, terminal->first);
    }
}

// Generate follow sets
void FirstFollowGenerator::generateFollows() {
    bool changed = true;
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
                        changed |= tryInsert(tk, nti->first);
                    }
                }
            }
        }
        
        // Rule 4
        // Don't ask why this loop is backwards
        for (long long i=rules.size()-1; i>=0; i--) {
            Grammar::Rule& rule = rules.at(i);
            if (rule.right.size() > 1) {
                for (size_t j=1; j<rule.right.size()-1; j++) {
                    auto& nti = rule.right.at(j);
                    auto& tk = rule.right.at(j+1);
                    if ((!nti->terminal) && (tk->terminal) && (!tk->lambda)) {
                        changed |= tryInsert(tk, nti->follow);
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
                            if ((tk->terminal) && (!tk->lambda)) {
                                changed |= tryInsert(tk, nti->follow);
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
                            changed |= tryInsert(tk, ntj->follow);
                        }
                    }
                }
            }
        }
    }
    
    // Add EOF_T to start symbol (assumed first lhs)
    for (auto& i : terminals) {
        if (i->name.compare("EOF_T") == 0) {
            rules[0].left->follow.push_back(i);
            break;
        }
    }
}

// Generate predict sets
void FirstFollowGenerator::generatePredicts() {
    // For each production rule
    for (size_t i=0; i<rules.size(); i++) {
        auto& rule = rules.at(i);
        auto& right = rule.right[0];
        if (right->lambda) {
            for (auto& follow : rule.left->follow) {
                if (!follow->lambda) {
                    tryInsert(follow, rule.predict);
                }
            }
        }
        else {
            for (auto& first : right->first) {
                tryInsert(first, rule.predict);
            }
        }
    }
}

// Generate a parse table from the predict set
void FirstFollowGenerator::generateParseTable() {
    parseTable.clear();
    
    // For each rule
    for (size_t ruleidx=0; ruleidx<rules.size(); ruleidx++) {
        auto& rule = rules.at(ruleidx);
        // For each terminal
        for (size_t termidx=0; termidx<terminals.size(); termidx++) {
            auto& terminal = terminals.at(termidx);
            if (!terminal->lambda) {
                // Check if in predict of rule
                bool found = false;
                for (auto& predict : rule.predict) {
                    if (terminal == predict) {
                        found = true;
                        break;
                    }
                }
                if (found) {
                    for (auto& predict : rule.predict) {
                        if (terminal == predict) {
                            parseTable.emplace(Grammar::parseKey_t(rule.left, predict->tokenName), rule);
                        }
                    }
                }
            }
        }
    }
}

// Return the parse table
std::map<Grammar::parseKey_t, Grammar::Rule>& FirstFollowGenerator::getParseTable() {
    return this->parseTable;
}

// Return the production rules
std::vector<Grammar::Rule>& FirstFollowGenerator::getRules() {
    return this->rules;
}

// Comparator to keep std::vector as a unique set
bool FirstFollowGenerator::tryInsert(std::shared_ptr<Grammar::Symbol>& symbol, std::vector<std::shared_ptr<Grammar::Symbol>>& set) {
    for (auto& i : set) {
        if (symbol->name.compare(i->name) == 0) {
            return false;
        }
    }
    set.push_back(symbol);
    return true;
}

// For debugging
void FirstFollowGenerator::printRules(std::fstream& stream) {
    stream << "==Production Rules==" << std::endl;
    for (auto& rule : rules) {
        stream << rule.left->name << " -> ";
        for (auto& right : rule.right) {
            stream << right->name << " ";
        }
        stream << std::endl;
    }
}

void FirstFollowGenerator::printFirsts(std::fstream& stream) {
    stream << "==Firsts==" << std::endl;
    for (auto& nt : nonterminals) {
        stream << nt->name << "\t = { ";
        for (auto& first : nt->first) {
            stream << first->name << ", ";
        }
        stream << "}" << std::endl;
    }
}

void FirstFollowGenerator::printFollows(std::fstream& stream) {
    stream << "==Follows==" << std::endl;
    for (auto& nt : nonterminals) {
        stream << nt->name << "\t = { ";
        for (auto& follow : nt->follow) {
            stream << follow->name << ", ";
        }
        stream << "}" << std::endl;
    }
}

void FirstFollowGenerator::printPredicts(std::fstream& stream) {
    stream << "==Predicts==" << std::endl;
    for (auto& rule : rules) {
        stream << rule.left->name << " -> ";
        for (auto& right : rule.right) {
            stream << right->name << " ";
        }
        stream << "\t = { ";
        for (auto& predict : rule.predict) {
            stream << predict->name << ", ";
        }
        stream << "}" << std::endl;
    }
}