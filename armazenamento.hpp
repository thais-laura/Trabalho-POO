#include <iostream>
#include <fstream>
#include <set>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include "./json.hpp"
#include "./json_fwd.hpp"
#include "./avaliacaoEfilmes.cpp"
#include "./usuario.cpp"

void msg_erro_arq(){
    printf("Falha no processamento do arquivo\n");
}
void salvarFilmes(std::vector<Filme> listaFilmes);
void salvarUsuarios(std::vector<Usuario> listaUsuarios);
void carregarFilmes(std::map<std::string, std::shared_ptr<Filme>>& filmes);
void carregarUsuarios(std::map<std::string, std::shared_ptr<Usuario>>& usuarios);
