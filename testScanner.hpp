#ifndef TESTSCANNER_HPP_
#define TESTSCANNER_HPP_

#include <list>

#include "token.hpp"

void testScanner(std::list<token>);
std::string tokenToString(std::list<token>::iterator i);

#endif