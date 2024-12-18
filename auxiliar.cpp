#include <iostream>
#include "./auxiliar.hpp"

// Estruturas utilizadas para mapeamento de gênero dos filmes
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

// Mensagem padrão
void msg_erro_arq(){
    printf("Falha no processamento do arquivo\n");
}

// Funções que auxiliam na busca de filmes e na parte de escolha das opções pelo usuário
std::string converteMinusculo(std::string &str){
    std::string min;
    for(size_t i=0; i< str.size(); i++){
        min += tolower(str.at(i));
    }
    return min;
}
bool isFloat(const std::string& str) {
    char* end = nullptr;
    std::strtod(str.c_str(), &end);
    
    // Verifica se toda a string foi consumida e se não está vazia
    return end != str.c_str() && *end == '\0';
}
bool isInteger(const std::string& str) {
    try {
        std::stoi(str); // Tenta converter a string para int
        return true;
    } catch (const std::invalid_argument& e) {
        return false; // Não é um número
    } catch (const std::out_of_range& e) {
        return false; // Número fora do intervalo de int
    }
}