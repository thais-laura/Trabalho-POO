#ifndef BASEIMDB_H
#define BASEIMDB_H

/* As funções se relacionam à base de dados do IMDb. 
Ocorre a busca de todas as informações dos filmes (ficha técnica), 
além das recomendações dos filmes. */

/* "This [website, program, service, application, product] uses TMDB 
and the TMDB APIs but is not endorsed, certified, or otherwise approved by TMDB."*/ 

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

class Filme;    // Declaração forward

std::vector<std::string> buscaFilmeNome(std::string nomeFilme, std::unordered_map<std::string, std::shared_ptr<Filme> >& listaConhecidos);
std::shared_ptr<Filme> buscaFilmeId(const std::string &id, std::unordered_map<std::string, std::shared_ptr<Filme>> &listaConhecidos);
std::vector<std::string> buscaElenco(std::string &id);
void buscaAvaliacao(std::string &id, std::string &mediaAv, std::string &numAv);
void verificaGeneros(std::string &genero, std::string &subgenero, std::string &subsubgenero);
std::vector<std::shared_ptr<Filme>> recomendaAtor(std::string &nome, std::unordered_map<std::string, std::shared_ptr<Filme> > &listaConhecidos);
std::vector<Filme> recomendaGenero(std::string genero);

#endif //BASEIMDB_H