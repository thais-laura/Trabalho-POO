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

class Filme{};
std::map<std::string,std::string> combGenero = {{"Adventure","Ação"}, {"War","Ação"},  
{"Western","Ação"}, {"Thriller", "Terror"}, {"Crime","Suspense"},
{"Mystery","Suspense"}, {"Film-Noir","Suspense"}, {"Biography","Drama"}, {"Documentary","Drama"}};

std::map<std::string, std::string> tradGenero = {{"Comedy", "Comédia"}, {"Action", "Ação"}, 
{"Animation", "Animação"}, {"Adventure", "Aventura"}, {"War", "Guerra"}, {"Western", "Faroeste"},
{"Horror","Terror"}, {"Mystery","Mistério"}, {"Documentary","Documentário"}, {"Biography","Biografia"},
{"Drama","Drama"}, {"Romance","Romance"}, {"Adult","Adulto"}, {"Family","Família"}, {"Reality-TV","Reality de TV"},
{"Game-Show","Game Show"}, {"History","Histórico"}, {"Music","Musical"}, {"News","Notícias"},
{"Sport","Esporte"}, {"Talk-Show","Talk Show"}};

std::set<std::string> classeGenero = {"Action", "Comedy", "Animation", "Romance", "Drama", "Horror"};

void msg_erro_arquivo(){
    printf("Falha no processamento do arquivo\n");
}
std::vector<Filme> buscaFilmeNome(std::string nomeFilme);
Filme buscaFilmeId(std::string &id, std::unordered_map<std::string, Filme> listaConhecidos);
std::vector<std::string> buscaElenco(std::string &id);
void buscaAvaliacao(std::string &id, std::string &mediaAv, std::string &numAv);
std::string converteMinusculo(std::string &str);
void verificaGeneros(std::string &genero, std::string &subgenero, std::string &subsubgenero);
std::vector<Filme> recomendacaoAtor(std::string &nome, std::unordered_map<std::string, Filme> listaConhecidos);
std::vector<Filme> recomendaGenero(std::string genero, std::unordered_map<std::string, Filme> listaConhecidos);