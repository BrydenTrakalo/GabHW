#include "checkVariables.hpp"

#include <iostream>
#include <stack>

using namespace std;

	//Starts the program to check if the variables in the tree are valid
void checkVariables(Node* root){
	//Your code here, you can being the preorder traversal with the root node of the tree

}



//Preorder Traversal - You may want to enter code here or may not
void traversePreorder(Node *currentNode){
    if(currentNode == NULL){
        return;
    }
    
	//Check if there is an error with the the current node
    checkCurrentNode(currentNode->name, currentNode->data);
    
    traversePreorder(currentNode->leftChild);

    traversePreorder(currentNode->rightChild);

    return;
}

//Valiates if the variables on the current node are valid
void checkCurrentNode(string name, list<token> data){
	//Your code here


}