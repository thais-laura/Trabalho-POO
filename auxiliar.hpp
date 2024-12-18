#ifndef AUXILIAR_HPP
#define AUXILIAR_HPP
#include <iostream>

extern std::map<std::string,std::string> combGenero;
extern std::map<std::string,std::string> tradGenero;
extern std::set<std::string> classeGenero;
void msg_erro_arq();
bool isFloat(const std::string& str);
bool isInteger(const std::string& str);

#endif //AUXILIAR_HPP
