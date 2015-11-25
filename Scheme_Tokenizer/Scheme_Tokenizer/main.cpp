//
//  main.cpp
//  Scheme_Tokenizer
//
//  Created by Matt on 10/1/15.
//  Copyright (c) 2015 Matt. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <iomanip>
#include <functional>
#include "LexicalAnalyzer.h"
#include "SchemeLexemes.h"

static std::ostream& operator<<(std::ostream& stream, SchemeLexemes t) {
    size_t idx = static_cast<size_t>(t);
    stream << lexemeStrings[idx];
    return stream;
}

void runScanner(std::ostream&, std::ostream&, std::vector<std::string>);

int main(int argc, const char* argv[]) {
    std::vector<std::string> file;
    std::string temp;
    if (argc == 1) {
        // Read from stdin
        while (std::getline(std::cin, temp)) {
            file.push_back(temp);
        }
    }
    // Read from file input
    else if (argc == 2) {
        std::fstream stream;
        stream.open(argv[1]);
        if (!stream.is_open()) {
            std::cerr << "Error: Failed to open file: " << argv[1] << std::endl;
            return -2;
        }
        while (std::getline(stream, temp)) {
            file.push_back(temp);
        }
    }
    else {
        std::cerr << "Error: Invalid number of arguments. Expected 0 for stdin, 1 for file input" << std::endl;
        return -1;
    }
    
    // Write to file
    if (argc == 2) {
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
        std::fstream lststream;
        std::fstream dbgstream;
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
        
        // Run Tokenizer
        runScanner(lststream, dbgstream, file);
    }
    else {
        // Write to stdout
        runScanner(std::cout, std::cout, file);
    }
    
    return 0;
}

// Run the tokenizer
void runScanner(std::ostream& lststream, std::ostream& dbgstream, std::vector<std::string> file) {
    // Who knew streams didn't have builtin equality tests
    bool sameStream = &lststream == &dbgstream;
    
    LexicalAnalyzer lexer;
    
    // Tokenize line by line
    // Yes this will be changed alittle for project 2
    // although internally I plan to still tokenize per line.
    size_t lineNum = 1;
    for (auto& line : file) {
        // Tokenize the line and store in res
        auto res = lexer.getTokens(line);
        
        // Print out line number + line of code
        if (sameStream) {
            lststream << lineNum << ". " << line << std::endl;
        }
        else {
            lststream << lineNum << ". " << line << std::endl;
            dbgstream << lineNum << ". " << line << std::endl;
        }
        
        // Print out Lexeme list
        for (auto& i : res) {
            dbgstream << std::left << std::setw(10) << i.lexeme << i.value << std::endl;
        }
        
        // Print out error list
        for (auto& i : lexer.getErrors()) {
            if (sameStream) {
                lststream << i << std::endl;
            }
            else {
                lststream << i << std::endl;
                dbgstream << i << std::endl;
            }
        }
        // Reset errors list
        // Could've been replaced by a
        // std::tuple return instead
        lexer.resetErrors();
        lineNum++;
    }
    
    // Print out error count
    if (sameStream) {
        lststream << lexer.getErrorCount() << " errors found." << std::endl;
    }
    else {
        lststream << lexer.getErrorCount() << " errors found." << std::endl;
        dbgstream << lexer.getErrorCount() << " errors found." << std::endl;
    }
}