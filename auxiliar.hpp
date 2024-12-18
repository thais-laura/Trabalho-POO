#ifndef AUXILIAR_HPP
#define AUXILIAR_HPP
#include <iostream>
#include <set>
#include <map>

/* As funções são auxiliares para diversas partes do programa. */
extern std::map<std::string,std::string> combGenero;
extern std::map<std::string,std::string> tradGenero;
extern std::set<std::string> classeGenero;
void msg_erro_arq();
std::string converteMinusculo(std::string &str);
bool isFloat(const std::string& str);
bool isInteger(const std::string& str);

#endif //AUXILIAR_HPP
