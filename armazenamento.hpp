#ifndef ARMAZENAMENTO_HPP
#define ARMAZENAMENTO_HPP

/* As funções se relacionam ao armazenamento de informações dos filmes buscados e de usuários cadastrados.
No início do programa, são carregadas as variáveis salvas; no fim, são salvas novamente, atualizando os registros.
Ambos os arquivos são da extensão JSON. */

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
#include "./auxiliar.hpp"

std::unordered_map<std::string, Usuario> carregarUsuarios();
std::unordered_map<std::string, std::shared_ptr<Filme>> carregarFilmes();
void salvarFilmes(std::unordered_map<std::string, std::shared_ptr<Filme>> & listaFilmes);
void salvarUsuarios(std::unordered_map<std::string, Usuario>& listaUsuarios);

#endif // ARMAZENAMENTO_HPP