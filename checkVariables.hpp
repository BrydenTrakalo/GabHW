#ifndef CHECKVARIABLES_HPP_
#define CHECKVARIABLES_HPP_

#include "node.hpp"


void checkVariables(Node* root);
void traversePreorder(Node *currentNode);
void checkCurrentNode(string name, list<token> data);


#endif