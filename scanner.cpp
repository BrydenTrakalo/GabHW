#include <string>
#include <list>
#include <fstream>
#include <iostream>

#include "token.hpp"
#include "scanner.hpp"

using namespace std;

char delimiters[]={'.',	'(',	')',	',',	'{',	'}',	';',	'[',	']'};
char operators[]={'<',		'>',	'=',	':',	'+',	'-',	'*',	'/',	'%'};
string keywords[14]={"begin", "end", "loop", "void", "var", "exit", "scan", "print", "main", "fork", "then", "let", "data", "func"};

//FSA Table
int FSA[7][8] = {
	//	a-z		0-9		operators	delim	A-Z		EOF		WS		#
	{	1,		3,		4, 			5,		-1, 		1006, 	0,		6},
	{	1, 		1, 		1000, 		1000, 	1, 		1000, 	1000,	1000},		// IDENT_tk
	{	2, 		-1, 	1001, 		1001, 	-1, 	1001, 	1001,	1001},		// KW_tk
	{	1002, 	3, 		1002, 		1002, 	1002,	1002, 	1002,	1002},		// INT_tk
	{	1003, 	1003, 	4, 			1003, 	1003,	1003, 	1003,	1003},		// OP_tk
	{	1004, 	1004, 	1004, 		1004, 	1004,	1004, 	1004,	1004},		// DEL_tk
	{	6, 		6, 		6, 			6, 		6,		-3, 	-3,		1005},		// COM_tk
};

//Grabs from the file, creates tokens and adds them to a list of tokens to be returned
list<token> scan(string fileName){
	list<token> tokens;
	char ch;
	ifstream iFile;
	int lineNum = 1;
	iFile.open(fileName.c_str());

	//character filtering
	if (iFile.is_open()){
		while (iFile.get(ch))
		{
			while(ch == ' ' || ch == '\t' || ch == '\n'){
				while(ch == ' '){
					iFile.get(ch);
				}
				while(ch == 32){
					iFile.get(ch);
				}
				if(ch == '\t'){
					iFile.get(ch);
				}
				if(ch == '\n'){
					lineNum++;
					iFile.get(ch);
				}
			}
			if(ch == '#'){
				do{
					iFile.get(ch);
				}while(ch!= '#');
				iFile.get(ch);
			}
			if(iFile.eof()){
				break;
			}
			token finished = driver(ch, lineNum, iFile);
			tokens.push_back(finished);
			if(finished.content.find(" ") != -1){
				cout << "WHAT DA FUCK |"  << finished.content  << "|" << endl;
			}
		}
		//adding the EOF_tk when end of file is reached
		if(iFile.eof()){
			tokens.push_back(driver(EOF, lineNum, iFile));
		}
		iFile.close();

		//file error if file is not found
		}else {
			cout << "Error opening file/file not found in directory. Ending program" << endl;
			return tokens;
		}
		return tokens;
}

//Determines what the token is and the following characters (if applicable) though the FSA table
token driver(char ch, int lineNum, ifstream& iFile){
	token tk;
	int state = 0;
	int nextState;
	int nextcharNum;
	tokenID TID;
	string name;
	string str = "";

	while(state < 1000){
		nextcharNum = getNextCharNum(ch);
		if(nextcharNum == -1){
			nextState = -1;
		}else{
			nextState = FSA[state][nextcharNum];
		}
		if(nextState < 0){
			tk = makeToken(ERROR_tk, "LEXICAL ERROR: Not a valid token", lineNum, "Error");
			return tk;
		}
        if (nextState >= 1000){
            switch(nextState){
                case 1000:
					//catch if string length is greater than 8 characters
					if(str.length() > 8){	
						tk = makeToken(ERROR_tk, "LEXICAL ERROR: Identifier length exceeds 8 characters", lineNum, "Error");
						cout << "LEXICAL ERROR: Identifier length exceeds 8 characters" <<endl;
						return tk;
					}
                    TID = IDENT_tk;
		    		name = "Identifier";
                    break;
                case 1001:
                    TID = KW_tk;
		    	    name = "Keyword"; 
                    break;
                case 1002:
                    TID = INT_tk;
		    		name = "Integer";
                    break;
                case 1003:
                    TID = OP_tk;
		    		name = "Operator";
                    break;
                case 1004:
                    TID = DEL_tk;
		    		name = "Delimiter";
                    break;
                case 1005:
                    TID = COM_tk;
		    		name = "Comment";
                    break;
				case 1006:
                    TID = EOF_tk;
		    		name = "End Of File";
                    break;
        	}
			tk = makeToken(TID, name, lineNum, str);

			//Find comment token and ignores 
			if((str.find('#')!= -1) && (str.find('#', str.find('#'))!= -1) || ch==EOF){	
			}else{
				iFile.unget();
			}
			return tk;
        }
		str += ch;

		//Checks if current string is a keyword
		for(int i=0;i<14;i++){	 
			if(str.find(keywords[i]) == 0){
				TID = KW_tk;
				name = "Keyword";
				tk = makeToken(TID, name, lineNum, str);
				return tk;
			}
		}

		//gets the next character
		iFile.get(ch);
		if(iFile.eof()){
			ch = EOF;
		}
		state = nextState;
	}
	return tk;
}

//Returns an integer with the character recieved to check against FSA table
int getNextCharNum(char ch){
	if (ch >= 'a' && ch <= 'z'){
		return 0;
	}
	if (ch >= 'A' && ch <= 'Z'){
		return 4;
	}
	if (ch >= '0' && ch <= '9'){
		return 1;
	}

	for (int i=0; i <= 9; i++){
		if(ch == operators[i]){
			return 2;
		}
	}

	for (int i=0; i <= 9; i++){
		if(ch == delimiters[i]){
			return 3;
		}
	}

	if (ch == EOF){
		return 5;
	}

	if ((ch == ' ') || (ch == '\n') || (ch == '\t')){
		return 6;
	}

	if (ch == '#'){
		return 7;
	}
	return -1;
}

//Token values for printing
token makeToken(tokenID TID, string name, int line, string content){
	token tk;
	tk.TID = TID;
	tk.name = name;
	tk.lineNum = line;
	tk.content = content;
	return tk;
}