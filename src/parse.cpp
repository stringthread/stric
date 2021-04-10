#include <iostream>
#include <fstream>
#include <string>

#include "parser/parser.h"
#include "parser/syntax.h"

using std::string;

int main(int argc, char *argv[]){
  string code;
  if(argc<=1){
    std::cin>>code;
  }else{
    std::ifstream ifs(argv[1]);
    if(!ifs){
      std::cout << "invalid filename: " << argv[1] << '\n';
      return -1;
    }
    code=string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
  }
  Parser parser=Parser::create();
  AST_Node result=parser.parse(code);
  std::cout << result.str() << '\n';
  return 0;
}
