#ifndef NODE_HPP_
#define NODE_HPP_


#include <iostream>
#include <string>
#include <list>
#include "token.hpp"
using namespace std;

struct Node{ 
    string name;
    list<token> data;
    //int size;


    Node* rightChild;
    Node* leftChild;
};

#endif