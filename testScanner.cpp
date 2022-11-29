#include <iostream>
#include <list>

#include "token.hpp"
#include "testScanner.hpp"

using namespace std;

//Print the appropriate token ID
void testScanner(list<token> tokens){
  list<token>::iterator i;

  for (i = tokens.begin(); i != tokens.end(); ++i){
    cout << tokenToString(i) << endl;
  }

  return;
}

string tokenToString(list<token>::iterator i){
  string tokenString = "";
  switch(i->TID){
      case IDENT_tk:
        tokenString = tokenString + "TID: " + "IDENT_tk";
      break;
      case KW_tk:
      tokenString = tokenString + "TID: " + "KW_tk";
      break;
      case OP_tk:
      tokenString = tokenString  + "TID: " + "OP_tk";
      break;
      case DEL_tk:
      tokenString = tokenString  + "TID: " + "DEL_tk";
      break;
      case INT_tk:
      tokenString = tokenString  + "TID: " + "INT_tk";
      break;
      case COM_tk:
      tokenString = tokenString  + "TID: " + "COM_tk";
      break;
      case EOF_tk:
      tokenString = tokenString  + "TID: " + "EOF_tk";
      break;
      case ERROR_tk:
      tokenString = tokenString  + "TID: " + "ERROR_tk";
      break;
    }

    //Print the token ID name and line number
    tokenString = tokenString + " Name: " + i->name + " Content: " + i->content;

    return tokenString;
}
//string tokenToStringPointer()