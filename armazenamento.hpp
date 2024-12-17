#ifndef ARMAZENAMENTO_HPP
#define ARMAZENAMENTO_HPP

#include <iostream>
#include <fstream>
#include <set>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include "./json.hpp"
#include "./json_fwd.hpp"
#include "./avaliacao.hpp"
#include "./filme.hpp"
#include "./usuario.hpp"

void msg_erro_arq(){
    printf("Falha no processamento do arquivo\n");
}
void salvarFilmes(std::unordered_map<std::string, Filme> & listaFilmes);
void salvarUsuarios(std::unordered_map<std::string, Usuario>& listaUsuarios);
std::unordered_map<std::string, Usuario> carregarUsuarios();
std::unordered_map<std::string, Filme> carregarFilmes();
#endif // ARMAZENAMENTO_HPP