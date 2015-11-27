//
//  Grammar.cpp
//  FirstFollowsGenerator
//
//  Created by Matt on 11/24/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#include "Grammar.h"

// Parameterized Constructor
Grammar::Grammar(std::vector<Grammar::Rule>& rules, std::map<Grammar::parseKey_t, Grammar::Rule>& table) : rules(rules), parseTable(table) {
}

// Stream operator overload for printing TerminalEnums
std::ostream& operator<<(std::ostream& stream, Grammar::TerminalEnums t) {
    size_t idx = static_cast<size_t>(t);
    stream << TerminalStrings[idx];
    return stream;
}

std::vector<Grammar::Rule> Grammar::getRules() {
    return this->rules;
}

std::map<Grammar::parseKey_t, Grammar::Rule> Grammar::getParseTable() {
    return this->parseTable;
}