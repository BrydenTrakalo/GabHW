#ifndef TOKEN_HPP_
#define TOKEN_HPP_

#include <string>

enum tokenID
{
    IDENT_tk,
    KW_tk,
    OP_tk,
    DEL_tk,
    INT_tk,
    COM_tk,
    EOF_tk,
    ERROR_tk
};

struct token { 
    tokenID TID; 
    std::string name; 
    int lineNum;
    std::string content;
};

#endif