/* Assignment : Project 1
 File : rosettiP1.cpp
 Author : Forrest Rosetti
 Professor : Dr. Tia Watts
 Date : 2/19/15
 Development time: N/A.
 Description : driver program for a Compiler
*/

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "SetLimits.h"
#include "SyntacticalAnalyzer.h"

int main(int argc, char * argv[]) {
	if (argc < 2) {
		printf("format: proj1 <filename>\n");
		exit(1);
	}
	SetLimits();

	SyntacticalAnalyzer parse(argv[1]);

	return 0;
}
