/*
 * General.cpp
 *
 *  Created on: May 24, 2013
 *      Author: stratj2
 */

#include "General.h"

General::General () {
}

General::~General() {
}

std::vector<std::string> General::GetFunctions ( ) {
	std::vector<std::string> all_functions;
	all_functions.push_back ( std::string("A") );

	return all_functions;
}
