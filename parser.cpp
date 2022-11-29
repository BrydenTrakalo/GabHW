#include "parser.hpp"
#include "token.hpp"
#include <iostream>
#include <list>
#include "testTree.hpp"
#include "testScanner.hpp"
#include <stdlib.h>

using namespace std;

list<token> tokens;
list<token>::iterator currentToken;
BinarySearchTree BST;

token recreateToken(list<token>::iterator i);
void updateToken(Node** nodeToUpdate, list<token> tokensToAdd);

bool checkIfError(list<token>::iterator currentToken, tokenID, string);
bool checkIfError(list<token>::iterator currentToken, tokenID, tokenID);
bool checkIfError(list<token>::iterator currentToken, string, string);
bool checkIfError(list<token>::iterator currentToken, string, tokenID);
bool checkIfError(list<token>::iterator currentToken, string);
bool checkIfError(list<token>::iterator currentToken, tokenID);

Node* Parser(list<token> tokensFromMain){      
    tokens = tokensFromMain;
    if(tokens.empty()){
        cout << "error no tokens made" << endl;
        exit(0);
    }
    
    currentToken = tokens.begin();  //Get the first token from the list
    Program();              // call the first nonterminal
    if (currentToken->TID == EOF_tk){
        return BST.root;     // continue, parse was ok
    }
    
    else {
        cout << " error(“tk received, EOFTk expected”);" << endl;  // will exit, no recovery
        exit(1);
    }
}

void Program(){   
        list<token> emptyList;
        BST.root = BST.insert(&(BST.root), "<program>", emptyList);    //Create the root node with <program and use an empty list
        Node* parentNode = BST.root;

        Vars(parentNode);
        Block(parentNode);     
        return;       // done, explicit return
}

void Vars(Node* parentNode) {
    if (currentToken->TID == KW_tk && currentToken->content == "var") {
        list<token> tokenList;
        for (int i = 0; i < 5; i++){    //Loop to add 5 tokens to the newNode. (should be var, identifier, :, integer, ;) 
            if(i == 0) checkIfError(currentToken, "var", IDENT_tk);
            if(i == 1) checkIfError(currentToken, IDENT_tk, ":");
            if(i == 2) checkIfError(currentToken, ":", INT_tk);
            if(i == 3) checkIfError(currentToken, INT_tk,  ";" );
            if(i == 4) checkIfError(currentToken, ";");
      
            tokenList.push_back(recreateToken(currentToken));
            ++currentToken; //Iterate to next token
        }
        parentNode = BST.insert(&parentNode, "<vars>", tokenList);
        Vars(parentNode);
    }
    //empty string
    else{
        return;
    } 
    return;
}

    //Collects th ebeing and end topkens, 
void Block(Node* parentNode) {
    if (currentToken->TID == KW_tk && currentToken->content == "begin") {
        list<token> beginTokenList;
        beginTokenList.push_back(recreateToken(currentToken));
        ++currentToken;
        parentNode = BST.insert(&parentNode, "<block>", beginTokenList); //Parent node becomes block, inserts the begin token into the tree
    }else{
        cout << "Error, file formatted incorrectly. Line # " << currentToken->lineNum << endl;
        exit(0);
    }
    Vars(parentNode);
    Stats(parentNode);
    if (currentToken->TID == KW_tk && currentToken->content == "end") { //Parentnode has top be the <block> node, need to update the block node, not insert a new one
        list<token> endTokenList;
        endTokenList.push_back(recreateToken(currentToken));
        ++currentToken;
        updateToken(&parentNode, endTokenList);
    }else{
        cout << "Error, file formatted incorrectly. Line # " << currentToken->lineNum << endl;
        exit(0);
    }
    return;
}

void Stats(Node* parentNode){
    list<token> emptyList;
    parentNode = BST.insert(&parentNode, "<stats>", emptyList);    //Create the node  <stats> and use an empty list

    Stat(parentNode);
    mStat(parentNode);
    return;
}

void Stat(Node* parentNode) {
    list<token> emptyList;
    parentNode = BST.insert(&parentNode, "<stat>", emptyList);    //Create the node <stat> and use an empty list, update parentnode to <stat>

    if(currentToken->TID == KW_tk && currentToken->content == "scan"){  //<in>
        In(parentNode); //Enter the <in> node

        checkIfError(currentToken, ";"); //Error check

        list<token> semicolonTokenList;
        semicolonTokenList.push_back(recreateToken(currentToken));
        ++currentToken;
        updateToken(&parentNode, semicolonTokenList);
        return;
    }

    if(currentToken->TID == KW_tk && currentToken->content == "print"){ //<out>
        Out(parentNode); //Enter the <out> node

        checkIfError(currentToken, ";"); //Error check

        list<token> semicolonTokenList;
        semicolonTokenList.push_back(recreateToken(currentToken));
        ++currentToken;
        updateToken(&parentNode, semicolonTokenList);
        return;
    }

    if(currentToken->TID == KW_tk && currentToken->content == "begin"){
        Block(parentNode);
        return;
    }

    if(currentToken->TID == KW_tk && currentToken->content == "fork"){
        If(parentNode);

        checkIfError(currentToken, ";"); //Error check

        list<token> semicolonTokenList;
        semicolonTokenList.push_back(recreateToken(currentToken));
        ++currentToken;
        updateToken(&parentNode, semicolonTokenList);
        return;
    }

    if(currentToken->TID == KW_tk && currentToken->content == "loop"){
        Loop(parentNode);

        checkIfError(currentToken, ";"); //Error check

        list<token> semicolonTokenList;
        semicolonTokenList.push_back(recreateToken(currentToken));
        ++currentToken;
        updateToken(&parentNode, semicolonTokenList);
        return;
    }
    if(currentToken->TID == IDENT_tk){
        Assign(parentNode);

        checkIfError(currentToken, ";"); //Error check
        
        list<token> semicolonTokenList;
        semicolonTokenList.push_back(recreateToken(currentToken));
        ++currentToken;
        updateToken(&parentNode, semicolonTokenList);
        return;
    }
    return;
}

    //Checks if there are futher tokens to be consumed 
void mStat(Node* parentNode) {
    if(currentToken->content == "end" || currentToken->content == ";"){
        if(currentToken->content == ";"){
            cout<< "Error token Added. Line # " << currentToken->lineNum << endl;
            exit(0);
        }
        return; //There are no more tokens to consume
    }else{
        list<token> emptyList;
        parentNode = BST.insert(&parentNode, "<mStat>", emptyList);    //Create the node

        Stat(parentNode);
        mStat(parentNode);
        
        return;
    }
}

void Exp(Node* parentNode){
    list<token> emptyList;
    parentNode = BST.insert(&parentNode, "<exp>", emptyList);    
    A(parentNode);

    //Code here for ++ option (He removed this from the assignment though)
    if(currentToken->TID == OP_tk && currentToken->content == "++"){
       list<token> TokenList;
        TokenList.push_back(recreateToken(currentToken));
        ++currentToken;
        updateToken(&parentNode, TokenList);
        Exp(parentNode);
    }
    return;
}

void A(Node* parentNode){
    list<token> emptyList;
    parentNode = BST.insert(&parentNode, "<A>", emptyList);
    N(parentNode);

    //--
    if(currentToken->TID == OP_tk && currentToken->content == "--"){
        list<token> TokenList;
        TokenList.push_back(recreateToken(currentToken));
        ++currentToken;
        updateToken(&parentNode, TokenList);
        A(parentNode);
    }
    return;

}

void N(Node* parentNode){
    list<token> emptyList;
    parentNode = BST.insert(&parentNode, "<N>", emptyList);

    M(parentNode);

    list<token> tokenList;
    if(currentToken->TID == OP_tk && currentToken->content == "/"){
        tokenList.push_back(recreateToken(currentToken));
        ++currentToken;
        updateToken(&parentNode, tokenList);    //Add the '/' operator to the node

        N(parentNode);
        return;
    }else if(currentToken->TID == OP_tk && currentToken->content == "*"){
        tokenList.push_back(recreateToken(currentToken));
        ++currentToken;
        updateToken(&parentNode, tokenList);    //Add the '*' operator to the node

        N(parentNode);
        return;
    }else{
        return;
    }
}

void M(Node* parentNode){
    //--
    if(currentToken->TID == OP_tk && currentToken->content == "--"){
        list<token> TokenList;
        TokenList.push_back(recreateToken(currentToken));
        ++currentToken;
        updateToken(&parentNode, TokenList);
        M(parentNode);

    } else{
        list<token> emptyList;
        parentNode = BST.insert(&parentNode, "<A>", emptyList);
        R(parentNode);
        return;
    }
}

void R(Node* parentNode){
    if(currentToken->TID == DEL_tk && currentToken->content == "["){
        list<token> tokenList;
        tokenList.push_back(recreateToken(currentToken));
        ++currentToken;
        parentNode = BST.insert(&parentNode, "<R>", tokenList); //Add the node <R> with the [ token

        Exp(parentNode);

        list<token> lastTokenList;
        lastTokenList.push_back(recreateToken(currentToken));
        ++currentToken;
        updateToken(&parentNode, lastTokenList);    //Add the ] delimiter to the node
        return;

    } else if(currentToken->TID == IDENT_tk){
        list<token> tokenList;
        tokenList.push_back(recreateToken(currentToken));
        ++currentToken;
        parentNode = BST.insert(&parentNode, "<R>", tokenList); //Add the node <R> with the identity token

        return;

    }else if(currentToken->TID == INT_tk){
        list<token> tokenList;
        tokenList.push_back(recreateToken(currentToken));
        ++currentToken;
        parentNode = BST.insert(&parentNode, "<R>", tokenList); //Add the node <R> with the INTEGER token

        return;
    }
}


void In(Node* parentNode){
    list<token> tokenList;
    for (int i = 0; i < 2; i++){    //Loop to add 2 tokens to the newNode. (should be scan, ID)   //SHOULD VERIFY that this is the corect tokens
        if(i == 0) checkIfError(currentToken, "scan", IDENT_tk); 
        if(i == 1) checkIfError(currentToken, IDENT_tk);  //Error check} //Error check

        tokenList.push_back(recreateToken(currentToken));
        ++currentToken; //Iterate to next token
    }
    parentNode = BST.insert(&parentNode, "<in>", tokenList);    //INsert <in> node to tree
    return;
}

void Out(Node* parentNode){
    list<token> tokenList;
    for (int i = 0; i < 2; i++){    //Loop to add 2 tokens to the newNode. (should be print, ( )   //SHOULD VERIFY that this is the corect tokens
        if(i == 0) checkIfError(currentToken, "print", "("); 
        if(i == 1) checkIfError(currentToken, "(");
        tokenList.push_back(recreateToken(currentToken));
        ++currentToken;
    }
    parentNode = BST.insert(&parentNode, "<out>", tokenList);    //INsert <out> node to tree

    Exp(parentNode);

    checkIfError(currentToken, ")"); // error checking
    list<token> closeBracketTokenList;
    closeBracketTokenList.push_back(recreateToken(currentToken));
    ++currentToken;
    updateToken(&parentNode, closeBracketTokenList);    //Add the final closed brack to the <out> node

    //BST.printPreorder(BST.root, 0, "outputhalfway");

    return;
}

void If(Node* parentNode){
    list<token> tokenList;
    for (int i = 0; i < 2; i++){    //Loop to add 2 tokens to the newNode. (should be fork, ( )   //SHOULD VERIFY that this is the corect tokens
        if(i == 0) checkIfError(currentToken, "fork", "(");
        if(i == 1) checkIfError(currentToken, "(");

        tokenList.push_back(recreateToken(currentToken));
        ++currentToken;
    }
    parentNode = BST.insert(&parentNode, "<if>", tokenList);    //INsert <if> node to tree

    Exp(parentNode);
    RO(parentNode);
    Exp(parentNode);

    list<token> secondTokenList;
    for (int i = 0; i < 2; i++){    //Loop to add 2 tokens to the newNode. (should be ), then )   //SHOULD VERIFY that this is the corect tokens
        if(i == 0) checkIfError(currentToken, ")", "then");
        if(i == 1) checkIfError(currentToken, ")");
        secondTokenList.push_back(recreateToken(currentToken));
        ++currentToken;
    }
    updateToken(&parentNode, secondTokenList);  //Add the two tokens to the node

    Stat(parentNode);
    return;
}


void Loop(Node* parentNode){
    list<token> tokenList;
    for (int i = 0; i < 2; i++){    //Loop to add 2 tokens to the newNode. (should be loop, ( )   //SHOULD VERIFY that this is the corect tokens
        if(i == 0) checkIfError(currentToken, "loop", "(");
        if(i == 1) checkIfError(currentToken, "(");
        tokenList.push_back(recreateToken(currentToken));
        ++currentToken;
    }
    parentNode = BST.insert(&parentNode, "<loop>", tokenList);    //INsert <loop> node to tree

    Exp(parentNode);
    RO(parentNode);
    Exp(parentNode);

    checkIfError(currentToken, ")"); //error checking

    list<token> secondTokenList;
    secondTokenList.push_back(recreateToken(currentToken));
    ++currentToken;
    updateToken(&parentNode, secondTokenList);  //Add the ) token to the node

    Stat(parentNode);  

    return;
    
}

void Assign(Node* parentNode){
    list<token> tokenList;
    for (int i = 0; i < 2; i++){    //Loop to add 2 tokens to the newNode. (should be IDENTIFIER, == )   //SHOULD VERIFY that this is the corect tokens
        if(i == 0) checkIfError(currentToken, IDENT_tk, "==");
        if(i == 1) checkIfError(currentToken, "==");
        tokenList.push_back(recreateToken(currentToken));
        ++currentToken;
    }
    parentNode = BST.insert(&parentNode, "<assign>", tokenList);    //INsert <assign> node to tree

    Exp(parentNode); 

    return;
}

void RO(Node* parentNode){
    //<=
    if(currentToken->TID == OP_tk && currentToken->content == "<="){
        list<token> tokenList;
        for (int i = 0; i < 2; i++){    //Loop to add 2 tokens to the newNode. (should be <=, and then some token )   //SHOULD VERIFY that this is the corect tokens
            tokenList.push_back(recreateToken(currentToken));
            ++currentToken;
        }
        parentNode = BST.insert(&parentNode, "<RO>", tokenList);    //INsert <RO> node to tree
        return;
    //>=
    }else if(currentToken->TID == OP_tk && currentToken->content == ">="){
        list<token> tokenList;
        for (int i = 0; i < 2; i++){    //Loop to add 2 tokens to the newNode. (should be >=, and then some token )   //SHOULD VERIFY that this is the corect tokens
            tokenList.push_back(recreateToken(currentToken));
            ++currentToken;
        }
        parentNode = BST.insert(&parentNode, "<RO>", tokenList);    //INsert <RO> node to tree
        return;
    //=
    }else if(currentToken->TID == OP_tk && currentToken->content == "="){
        list<token> tokenList;
        tokenList.push_back(recreateToken(currentToken));
        ++currentToken;
        parentNode = BST.insert(&parentNode, "<RO>", tokenList);    //INsert <RO> node to tree
        return;
    //%
    }else if(currentToken->TID == OP_tk && currentToken->content == "%"){
        list<token> tokenList;
        tokenList.push_back(recreateToken(currentToken));
        ++currentToken;
        parentNode = BST.insert(&parentNode, "<RO>", tokenList);    //INsert <RO> node to tree
        return;
    }
}


token recreateToken(list<token>::iterator i){
    token newToken;
    newToken.TID = i->TID;
    newToken.name = i-> name;
    newToken.lineNum = i-> lineNum;
    newToken.content = i->content;
    return newToken;
}


void updateToken(Node** nodeToUpdate, list<token> tokensToAdd){
    list<token>::iterator i;

    for (i = tokensToAdd.begin(); i != tokensToAdd.end(); ++i){
        (*nodeToUpdate)->data.push_back(recreateToken(i));
    }
    return;
}

bool checkIfError(list<token>::iterator currentToken, string content, tokenID TID){
    list<token>::iterator tokenChecker;
    tokenChecker = currentToken; ++tokenChecker;

    if(currentToken->content != content){
        if(tokenChecker->content == content){
            cout << "Error token Added. Line # " << currentToken->lineNum << endl;
            exit(0);
        }
        else if(currentToken->TID == TID){
            cout << "Error token Deleted. Line # " << currentToken->lineNum << endl;
            exit(0);
        }else{
            cout << "Error token Replaced. Line # " << currentToken->lineNum << endl;
            exit(0);
        }
    }
    return false;
}

bool checkIfError(list<token>::iterator currentToken, string content, string nextContent){
    list<token>::iterator tokenChecker;
    tokenChecker = currentToken; ++tokenChecker;

    if(currentToken->content != content){
        if(tokenChecker->content == content){
            cout << "Error token Added. Line # " << currentToken->lineNum << endl;
            exit(0);
        }
        else if(currentToken->content == nextContent){
            cout << "Error token Deleted. Line # " << currentToken->lineNum << endl;
            exit(0);
        }else{
            cout << "Error token Replaced. Line # " << currentToken->lineNum << endl;
            exit(0);
        }
    }
    return false;
}

bool checkIfError(list<token>::iterator currentToken, tokenID TID, tokenID nextTID){
    list<token>::iterator tokenChecker;
    tokenChecker = currentToken; ++tokenChecker;

    if(currentToken->TID != TID){
        if(tokenChecker->TID == TID){
            cout << "Error token Added. Line # " << currentToken->lineNum << endl;
            exit(0);
        }
        else if(currentToken->TID == nextTID){
            cout << "Error token Deleted. Line # " << currentToken->lineNum << endl;
            exit(0);
        }else{
            cout << "Error token Replaced. Line # " << currentToken->lineNum << endl;
            exit(0);
        }
    }
    return false;
}

bool checkIfError(list<token>::iterator currentToken, tokenID TID, string nextContent){
    list<token>::iterator tokenChecker;
    tokenChecker = currentToken; ++tokenChecker;

    if(currentToken->TID != TID){
        if(tokenChecker->TID == TID){
            cout << "Error token Added. Line # " << currentToken->lineNum << endl;
            exit(0);
        }
        else if(currentToken->content == nextContent){
            cout << "Error token Deleted. Line # " << currentToken->lineNum << endl;
            exit(0);
        }else{
            cout << "Error token Replaced. Line # " << currentToken->lineNum << endl;
            exit(0);
        }
    }
    return false;
}

bool checkIfError(list<token>::iterator currentToken, tokenID TID){
    if(currentToken->TID != TID){
        cout << "Error token Deleted. Line # " << currentToken->lineNum << endl;
        exit(0);
    }
    return false;
}

bool checkIfError(list<token>::iterator currentToken, string content){
    if(currentToken->content != content){
        cout << "Error token Deleted. Line # " << currentToken->lineNum << endl;
        exit(0);
    }
    return false;
}