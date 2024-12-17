#include "./armazenamento.hpp"
//jsoneditoronline.org
using json = nlohmann::json;


void salvarFilmes(std::unordered_map<std::string, Filme> &listaFilmes){
    nlohmann::json jsonFilmes = nlohmann::json::array();

    for (auto [id, filme] : listaFilmes) {
        jsonFilmes.push_back(filme.toJSON());
    }

    std::ofstream arquivo("armazFilmes.json");
    if(arquivo.is_open())
        arquivo << jsonFilmes.dump(2); // Formatação com 4 espaços de indentação
    else msg_erro_arq();
}

void salvarUsuarios(std::unordered_map<std::string, Usuario> &listaUsuarios){
    json jsonUsuarios = json::array();

    for (auto [id, usuario] : listaUsuarios) {
        jsonUsuarios.push_back(usuario.toJSON());
    }

    std::ofstream arquivo("armazUsuarios.json");
    if(arquivo.is_open())
        arquivo << jsonUsuarios.dump(2); // Formatação com 4 espaços de indentação
    else msg_erro_arq();
}

std::unordered_map<std::string, Filme> carregarFilmes(){
    std::unordered_map<std::string, Filme> listaFilmes;
    std::ifstream arquivo("armazFilmes.json");
    if (!arquivo.is_open()) {
        msg_erro_arq();
        return listaFilmes;
    }

    nlohmann::json jsonFilmes;
    arquivo >> jsonFilmes; // Carrega todos os objetos JSON do arquivo
    if(!jsonFilmes.empty()){

        for (const auto& aux : jsonFilmes) {
            // Cria o objeto Filme a partir dos dados do JSON
            Filme filme(
                aux["id"].get<std::string>(),
                aux["titulo"].get<std::string>(),
                aux["genero"].get<std::string>(),
                aux["subgenero"].get<std::string>(),
                aux["elenco"].get<std::vector<std::string>>(),
                aux["classificacao"].get<bool>(),
                aux["mediaBase"].get<float>(),
                aux["nMediasBase"].get<int>(),
                aux["ano"].get<int>(),
                aux["duracao"].get<int>()
            );

            // Carrega as avaliações associadas ao filme
            if (aux.contains("avaliacoes")) {
                for (const auto& av : aux["avaliacoes"]) {
                    filme.adicionarOuAtualizar(
                        av["login"].get<std::string>(),
                        av["nota"].get<float>(),
                        av["comentario"].get<std::string>()
                    );
                }
            }

            // Adiciona o filme ao unordered_map usando o ID como chave
            listaFilmes[filme.getId()] = std::move(filme);
        }
    }
    return listaFilmes;
}


std::unordered_map<std::string, Usuario> carregarUsuarios(){
    std::unordered_map<std::string, Usuario> listaUsuarios;
    std::ifstream arquivo("armazUsuarios.json");
    if (!arquivo.is_open()) {
        msg_erro_arq();
        return listaUsuarios;
    }

    nlohmann::json jsonUsuarios;
    arquivo >> jsonUsuarios; // Carrega todos os objetos JSON do arquivo
    if(!jsonUsuarios.empty()){
        for (const auto& jsonUsuario : jsonUsuarios) {
            Usuario usuario(
                jsonUsuario["login"].get<std::string>(),
                jsonUsuario["senha"].get<std::string>(),
                jsonUsuario["email"].get<std::string>(),
                jsonUsuario["generos"].get<std::vector<std::string>>(),
                jsonUsuario["nome"].get<std::string>()
            );

            // Carregando as avaliações
            if (jsonUsuario.contains("avaliacoes")) {
                for (const auto& av : jsonUsuario["avaliacoes"]) {
                    Avaliacao avaliacao(
                        av["login"].get<std::string>(),  // ID do usuário
                        av["id filme"].get<std::string>(),
                        av["titulo"].get<std::string>(),
                        av["nota"].get<double>(),
                        av["comentario"].get<std::string>()
                    );
                    usuario.adicionarAvaliacao(avaliacao.getIdFilme(), avaliacao.getNomeFilme(),avaliacao.getNota(), avaliacao.getComentario()); // Adiciona a avaliação ao usuário
                }
            }
            // Inserir o objeto Usuario no mapa usando o login como chave
            listaUsuarios[usuario.getNomeUsuario()] = std::move(usuario);  // std::move evita criação de outro objeto, sendo que esse já seria destruído no final da função
        }
    }
    return listaUsuarios;
}