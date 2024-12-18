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
#include "./auxiliar.hpp"

void salvarFilmes(std::unordered_map<std::string, std::shared_ptr<Filme>> & listaFilmes);
void salvarUsuarios(std::unordered_map<std::string, Usuario>& listaUsuarios);
std::unordered_map<std::string, Usuario> carregarUsuarios();
std::unordered_map<std::string, std::shared_ptr<Filme>> carregarFilmes();
#endif // ARMAZENAMENTO_HPP