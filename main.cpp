#include <iostream>
#include <fstream>
#include <list>

#include "testScanner.hpp"
#include "scanner.hpp"
#include "token.hpp"
#include "testTree.hpp"
#include "parser.hpp"
#include "checkVariables.hpp"

using namespace std; 

//Warning: if inputting data by command line, must be more than one argument input.
int main(int argc, char *argv[]){
    list<token> tokens;
    token myTok;
    string line;
    char ch; 

    // for taking input "./file < filename.extension"
    if(argc == 1){
        string line;
        ofstream outputFile("pipeInput.cs4280");
        while (getline(cin, line)){
            outputFile << line;
        }
        outputFile.close();
        tokens = scan("pipeInput.cs4280");
    }
    
    //Grabs from the file and appends .cs4280
    else if(argc == 2){
        string fileToOpen = argv[1];
        fileToOpen +=  ".cs4280";
        tokens = scan(fileToOpen);
    }

    //input data by command line
    else if(argc > 2){
         ofstream outputFile("pipeInput.cs4280");
         for(int i = 1; i < argc; i++){
            cout << "found " << argv[i] << "i is " << i << "argc is "  << argc << endl;
            outputFile << argv[i] << " ";
        }
        outputFile.close();
        tokens = scan("pipeInput.cs4280");
    }

    BinarySearchTree myTree;
    myTree.root = Parser(tokens);

    myTree.printPreorder(myTree.root, 0, "output");

    checkVariables(myTree.root);

    cout<< "Process completed successfully! *Happy noises*" << endl;

    return 0;
}