#include <iostream>
#include <fstream>
#include "testTree.hpp"
using namespace std;

string tokenToStringTree(list<token>::iterator i);

    //Creates a new node to be added to the tree
Node* BinarySearchTree::newNode(string name, list<token> newToken){
    Node* dynamicNode = new Node;
    dynamicNode->data = newToken;
    dynamicNode->name = name;
    dynamicNode->rightChild = NULL;
    dynamicNode->leftChild = NULL;

    return dynamicNode;
}

    //INserts a new node onto a parent node
Node* BinarySearchTree::insert(Node** parentNode, string name, list<token> newToken){
    if(*parentNode == NULL){
        *parentNode = newNode(name, newToken);
        return *parentNode;
    }

    Node* currentNode = newNode(name, newToken);
    if((*parentNode)->leftChild == NULL){
        (*parentNode)->leftChild = currentNode;
    }

    else if((*parentNode)->rightChild == NULL){
        (*parentNode)->rightChild = currentNode;
    }
    else {
       cout << "ERROR, PARENT NODE IS ALL FULL. PArent ndoe is " << (*parentNode)->name << " node ot insert is " << name << endl;
    }
    return currentNode;
}

//writes and formats the input for each traversal
void printToFileCurrentNode(int depth, string name, list<token> data, string fileName, string fileExtension){ 
    fileName +=  fileExtension;
    ofstream ofile(fileName.c_str(), ios::app);
    list<token>::iterator i;

    //find the depth of the node in the tree for formatting and insert it to file
    for(int i = 0; i<depth; i++){
        ofile << "  ";
    }
    ofile << name << ": ";
    for (i = data.begin(); i != data.end(); ++i){
        ofile << tokenToStringTree(i) << " ";
    }
    ofile << endl;

    ofile.close();
}

//Inorder Travresal
void BinarySearchTree::printInorder(Node *currentNode, int depth, string fileName){
    if(currentNode == NULL){
        return;
    }

    printInorder(currentNode->leftChild, depth+1, fileName);

    printToFileCurrentNode(depth, currentNode->name, currentNode->data, fileName, ".inorder");

    printInorder(currentNode->rightChild, depth+1, fileName);

    return;
}

//Postorder Traversal
void BinarySearchTree::printPostorder(Node *currentNode, int depth, string fileName){
    if(currentNode == NULL){
        return;
    }

    printPostorder(currentNode->leftChild, depth+1, fileName);

    printPostorder(currentNode->rightChild, depth+1, fileName);

    printToFileCurrentNode(depth, currentNode->name, currentNode->data, fileName, ".postorder");

    return;
}

//Preorder Traversal
void BinarySearchTree::printPreorder(Node *currentNode, int depth, string fileName){
    if(currentNode == NULL){
        return;
    }
    
    printToFileCurrentNode(depth, currentNode->name, currentNode->data, fileName, ".preorder");
    
    printPreorder(currentNode->leftChild, depth+1, fileName);

    printPreorder(currentNode->rightChild, depth+1, fileName);

    

    return;
}

string tokenToStringTree(list<token>::iterator i){
  string tokenString = "";
  switch(i->TID){
      case IDENT_tk:
        tokenString = tokenString + "ID: ";
      break;
      case KW_tk:
      //tokenString = tokenString + "TID: " + "KW_tk";
      break;
      case OP_tk:
      //tokenString = tokenString  + "TID: " + "OP_tk";
      break;
      case DEL_tk:
      //tokenString = tokenString  + "TID: " + "DEL_tk";
      break;
      case INT_tk:
      tokenString = tokenString  + "# ";
      break;
      case COM_tk:
      //tokenString = tokenString  + "TID: " + "COM_tk";
      break;
      case EOF_tk:
      //tokenString = tokenString  + "TID: " + "EOF_tk";
      break;
      case ERROR_tk:
      //tokenString = tokenString  + "TID: " + "ERROR_tk";
      break;
    }

    //string lineNum = to_string(i->lineNum);
    //Print the token ID name and line number
    tokenString = tokenString + i->content;

    return tokenString;
}