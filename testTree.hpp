#include <iostream>
#include "node.hpp"
#include <list>
using namespace std;

class BinarySearchTree{

    public:
    Node* root;   

    Node* newNode(string, list<token>);
    void buildTree();
    void printInorder(Node *currentNode, int depth, string);
    void printPreorder(Node *currentNode, int depth, string);
    void printPostorder(Node *currentNode, int depth, string);

    BinarySearchTree(){
        root = NULL;
    }

    Node* insert(Node** parentNode, string name, list<token> newToken);

};