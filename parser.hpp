#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "node.hpp"

Node* Parser(list<token>);
void Program();
void Vars(Node*);
void Block(Node*);
void Stats(Node*);
void Stat(Node*);
void mStat(Node*);
void Exp(Node*);
void A(Node*);
void N(Node*);
void M(Node*);
void R(Node*);
void In(Node*);
void Out(Node*);
void If(Node*);
void Loop(Node*);
void Assign(Node*);
void RO(Node*);

#endif