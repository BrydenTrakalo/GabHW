#ifndef SCANNER_HPP_
#define SCANNER_HPP_

#include "token.hpp"
#include <list>
#include <fstream>

std::list<token> scan(std::string);

token driver(char, int, std::ifstream&);

int getNextCharNum(char);

token makeToken(tokenID, std::string, int, std::string content);

#endif