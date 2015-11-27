//
//  main.cpp
//  FirstFollowsGenerator
//
//  Created by Matt on 11/18/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#include <iostream>
#include <vector>
#include "FirstFollowGenerator.h"

int main(int argc, const char * argv[]) {
    FirstFollowGenerator ffgen;
    
    
    for (auto& i : terminal_strings) {
        ffgen.addSymbol(i, true);
    }
    
    for (auto& i : nonterminal_strings) {
        ffgen.addSymbol(i, false);
    }
    
    
    ffgen.addRules(rules_strings);
    
    ffgen.printRules();
    
    ffgen.generateSets();
    
    std::cout << std::endl;
    ffgen.printFirsts();
    std::cout << std::endl;
    ffgen.printFollows();
    
    return 0;
}
