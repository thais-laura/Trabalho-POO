#ifndef BASEIMDB_H
#define BASEIMDB_H

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <map>
#include <set>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <ctype.h>
#include <regex>
#include <queue>
#include "./avaliacao.hpp"
#include "./armazenamento.hpp"

class Filme;

std::vector<std::string> buscaFilmeNome(std::string nomeFilme, std::unordered_map<std::string, std::shared_ptr<Filme> >& listaConhecidos);
std::shared_ptr<Filme> buscaFilmeId(const std::string &id, std::unordered_map<std::string, std::shared_ptr<Filme>> &listaConhecidos);
std::vector<std::string> buscaElenco(std::string &id);
void buscaAvaliacao(std::string &id, std::string &mediaAv, std::string &numAv);
std::string converteMinusculo(std::string &str);
void verificaGeneros(std::string &genero, std::string &subgenero, std::string &subsubgenero);
std::vector<std::shared_ptr<Filme>> recomendaAtor(std::string &nome, std::unordered_map<std::string, std::shared_ptr<Filme> > &listaConhecidos);
std::vector<Filme> recomendaGenero(std::string genero, std::unordered_map<std::string, std::shared_ptr<Filme> >& listaConhecidos);

#endif //BASEIMDB_H