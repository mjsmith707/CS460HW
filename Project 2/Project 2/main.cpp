//
//  main.cpp
//  FirstFollowsGenerator
//
//  Created by Matt on 11/18/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <functional>
#include <memory>
#include "Grammar.h"
#include "FirstFollowGenerator.h"
#include "LexicalAnalyzer.h"
#include "SyntacticAnalyzer.h"

void runInterpreter(std::string& input, std::fstream& inputstream, std::fstream& lststream, std::fstream& dbgstream);
void initSymbols(std::shared_ptr<FirstFollowGenerator>& ffgen, std::fstream& dbgstream);

int main(int argc, const char* argv[]) {
    // File stream objects
    std::fstream inputstream;
    std::fstream lststream;
    std::fstream dbgstream;
    
    // Interpreter Objects
    // Write to file
    if (argc == 2) {
        inputstream.open(argv[1]);
        if (!inputstream.is_open()) {
            std::cerr << "Error: Failed to open file: " << argv[1] << std::endl;
            return -2;
        }
        
        // Find output file names
        std::string fileoutlst = argv[1];
        std::string fileoutdbg = argv[1];
        size_t dotpos = fileoutlst.find_last_of(".", fileoutlst.size());
        if (dotpos == fileoutlst.npos) {
            fileoutlst.append(".lst");
            fileoutdbg.append(".dbg");
        }
        else {
            fileoutlst.replace(dotpos, fileoutlst.npos, ".lst");
            fileoutdbg.replace(dotpos, fileoutdbg.npos, ".dbg");
        }
        
        // Open output streams
        lststream.open(fileoutlst, std::fstream::out | std::fstream::trunc);
        dbgstream.open(fileoutdbg, std::fstream::out | std::fstream::trunc);
        
        if (!lststream.is_open()) {
            std::cerr << "Error: Failed to open " << fileoutlst << " for writing." << std::endl;
            exit(-2);
        }
        else if (!dbgstream.is_open()) {
            std::cerr << "Error: Failed to open " << fileoutdbg << " for writing." << std::endl;
            exit(-3);
        }
    }
    else {
        std::cerr << "Usage: ./a.out filename" << std::endl;
        return -1;
    }
    
    // Run the interpreter
    std::string input = argv[1];
    runInterpreter(input, inputstream, lststream, dbgstream);
    
    return 0;
}

void runInterpreter(std::string& input, std::fstream& inputstream, std::fstream& lststream, std::fstream& dbgstream) {
    // Initialize Language Generator
    std::shared_ptr<FirstFollowGenerator> ffgen(new FirstFollowGenerator());
    initSymbols(ffgen, dbgstream);
    std::shared_ptr<Grammar> grammar(new Grammar(ffgen->getRules(), ffgen->getParseTable()));
    
    // Initialize Tokenizer
    std::shared_ptr<LexicalAnalyzer> tokenizer(new LexicalAnalyzer(input, inputstream, lststream, dbgstream));
    
    // Initialize Syntactic Analyzer
    std::shared_ptr<SyntacticAnalyzer<Grammar, LexicalAnalyzer>> analyzer(new SyntacticAnalyzer<Grammar, LexicalAnalyzer>(grammar, tokenizer, inputstream, lststream, dbgstream));
    
    // Run Syntactic Analyzer
    bool success = analyzer->parseCode();
    
    if (success) {
        lststream << "Successfully parsed input file!" << std::endl;
    }
    else {
        lststream << "There were " << analyzer->getErrorCount() << " errors found." << std::endl;
    }
}

// Initialize Language Symbols
void initSymbols(std::shared_ptr<FirstFollowGenerator>& ffgen, std::fstream& dbgstream) {
    for (auto& i : TerminalStrings) {
        ffgen->addSymbol(i, true);
    }
    
    for (auto& i : NonterminalStrings) {
        ffgen->addSymbol(i, false);
    }
    
    ffgen->addRules(RulesStrings);
    ffgen->printRules(dbgstream);
    ffgen->generateSets();
    dbgstream << std::endl;
    ffgen->printFirsts(dbgstream);
    dbgstream << std::endl;
    ffgen->printFollows(dbgstream);
    dbgstream << std::endl;
    ffgen->printPredicts(dbgstream);
    dbgstream << std::endl;
}