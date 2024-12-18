#include "./armazenamento.hpp"

//jsoneditoronline.org
using json = nlohmann::json;

// Carregar para a memória principal as informações das sessões antigas
std::unordered_map<std::string, std::shared_ptr<Filme>> carregarFilmes(){
    std::unordered_map<std::string, std::shared_ptr<Filme>> listaFilmes;
    std::ifstream arquivo("armazFilmes.json");
    if (!arquivo.is_open()) {
        return listaFilmes;
    }

    nlohmann::json jsonFilmes;
    arquivo >> jsonFilmes; // Carrega todos os objetos JSON do arquivo
    if (jsonFilmes.is_array() && !jsonFilmes.empty()) {

        for (const auto& aux : jsonFilmes) {
            std::shared_ptr<Filme> filme;
            // Cria o objeto Filme a partir dos dados do JSON
            if (aux.contains("id") && aux.contains("titulo") && aux.contains("genero") && aux.contains("subgenero") &&
                aux.contains("elenco") && aux.contains("classificacao") && aux.contains("media") &&
                aux.contains("quantidade de avaliações") && aux.contains("ano") && aux.contains("duracao")) {

                filme = std::make_shared<Filme>(
                    aux["id"].get<std::string>(),
                    aux["titulo"].get<std::string>(),
                    aux["genero"].get<std::string>(),
                    aux["subgenero"].get<std::string>(),
                    aux["elenco"].get<std::vector<std::string>>(),
                    aux["classificacao"].get<bool>(),
                    aux["media"].get<float>(),
                    aux["quantidade de avaliações"].get<int>(),
                    aux["ano"].get<int>(),
                    aux["duracao"].get<int>()
                );

            }
            // Carrega as avaliações associadas ao filme
            if (aux.contains("avaliacoes")) {
                for (const auto& av : aux["avaliacoes"]) {
                    if (av.contains("login") && av.contains("nota") && av.contains("comentario")) {
                        filme->adicionarOuAtualizar(
                            av["login"].get<std::string>(),
                            av["nota"].get<float>(),
                            av["comentario"].get<std::string>()
                        );
                    } else {
                        std::cout << "Erro: avaliação incompleta no filme " << aux["titulo"].get<std::string>() << std::endl;
                    }
                }
            }

            // Adiciona o filme ao unordered_map usando o ID como chave (usando o shared_ptr)
            listaFilmes[filme->getId()] = filme;
        }
    }
    return listaFilmes;
}


std::unordered_map<std::string, Usuario> carregarUsuarios(){
    std::unordered_map<std::string, Usuario> listaUsuarios;
    std::ifstream arquivo("armazUsuarios.json");
    if (!arquivo.is_open()) {
        return listaUsuarios;
    }

    nlohmann::json jsonUsuarios;
    arquivo >> jsonUsuarios; // Carrega todos os objetos JSON do arquivo
    if (jsonUsuarios.is_array() && !jsonUsuarios.empty()) {
        for (const auto& aux : jsonUsuarios) {
            Usuario usuario(
                aux["login"].get<std::string>(),
                aux["senha"].get<std::string>(),
                aux["email"].get<std::string>(),
                aux["generos"].get<std::vector<std::string>>(),
                aux["nome"].get<std::string>()
            );
            // Carregando as avaliações
            if (aux.contains("avaliacoes")) {
                for (const auto& av : aux["avaliacoes"]) {
                    if (av.contains("id filme") && av.contains("titulo") && 
                        av.contains("nota") && av.contains("comentario")) {
                            // Se todas as chaves existem, processar
                            usuario.adicionarAvaliacao(
                                av["id filme"].get<std::string>(),
                                av["titulo"].get<std::string>(),
                                av["nota"].get<double>(),
                                av["comentario"].get<std::string>()
                            );
                    }
                }
            }

            // Inserir o objeto Usuario no mapa usando o login como chave
            listaUsuarios[usuario.getNomeUsuario()] = std::move(usuario);  // std::move evita criação de outro objeto, sendo que esse já seria destruído no final da função
        }
    }
    return listaUsuarios;
}

// Atualizar o arquivo com as novas informações após a finalização do programa
void salvarFilmes(std::unordered_map<std::string, std::shared_ptr<Filme>> &listaFilmes){
    nlohmann::json jsonFilmes = nlohmann::json::array();

    for (auto [id, filme] : listaFilmes) {
        jsonFilmes.push_back((*filme).toJSON());
    }

    std::ofstream arquivo("armazFilmes.json");
    if(arquivo.is_open())
        arquivo << jsonFilmes.dump(2); // Formatação com 4 espaços de indentação
    else msg_erro_arq();

    std::cout << "Filmes salvos\n";
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

    std::cout << "Usuários salvos\n";
}