#include <iostream>
#include <fstream>
#include <string>

#include "parser/lexer.h"
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
  Lexer lex;
  const auto& result=lex.analize(code);
  for(const auto& result_token : result){
    std::cout << (string)result_token << '\n';
  }
  return 0;
}
