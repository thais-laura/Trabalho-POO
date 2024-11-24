#include <iostream>
#include <fstream>
#include <set>
#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
#include "./json.hpp"
#include "./json_fwd.hpp"

class Avaliacao{
    nlohmann::json toJSON() const {
        return {
            {"login", _login},
            {"filme", _id_filme},
            {"nota", _nota},
            {"comentario", _comentario} // por ser um vector de outra classe, é necessário sobrecarrgear em avaliacoes
        };
    }
};
class Filme{
// id, genero, subgenero, elenco, classificacao, media, qtdAv, avaliacoes
public:
    nlohmann::json toJSON() const {
        std::vector<nlohmann::json> avaliacoesJson;
        for (Avaliacao avaliacao : _avaliacoes) {   // criar os modelos para salvar no arquivo a partir da sobrecarga
            avaliacoesJson.push_back(avaliacao.toJSON());
        }
        return {
            {"id", _id}
            {"titulo", _titulo},
            {"elenco", _elenco},    // apesar de ser um vector, o json consegue manipular as std::string
            {"genero", _genero},
            {"subgenero", subgenero},
            {"ano", _ano},
            {"classificacao", _classificacao},
            {"media", _media},
            {"quantidade de avaliações", _qtdAv},
            {"avaliacoes", avaliacoesJson} // por ser um vector de outra classe, é necessário sobrecarrgear em avaliacoes
        };
    }
};
class Usuario{

    nlohmann::json toJSON() const {
        std::vector<nlohmann::json> avaliacoesJson;
        for (Avaliacao avaliacao : _avaliacoes) {   // criar os modelos para salvar no arquivo a partir da sobrecarga
            avaliacoesJson.push_back(avaliacao.toJSON());
        }
        return {
            {"id", _id}
            {"login", _login},
            {"senha", _senha},    // apesar de ser um vector, o json converte para um vetor normal
            {"generos", _generosFav},
            {"email", _email},
            {"ano", _dataNasc},
            {"avaliacoes", avaliacoesJson} // por ser um vector de outra classe, é necessário sobrecarrgear em avaliacoes
        };
    }
};
void msg_erro_arq(){
    printf("Falha no processamento do arquivo\n");
}
void salvarFilmes(std::vector<Filme> listaFilmes);
void salvarUsuarios(std::vector<Usuario> listaUsuarios);
void carregarFilmes(std::map<std::string, std::shared_ptr<Filme>>& filmes);
void carregarUsuarios(std::map<std::string, std::shared_ptr<Usuario>>& usuarios);
void verG();