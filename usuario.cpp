#include "usuario.hpp"
#include <iostream>

// Construtor
Usuario::Usuario(std::string nomeU, std::string password, std::string email1, std::vector<std::string> genero, std::string nomeC)
    : nomeUsuario(nomeU), senha(password), email(email1), generoFav(genero), nomeCompleto(nomeC) {}

// Métodos get
std::string Usuario::getSenha() const {return senha;}
std::string Usuario::getNomeUsuario() const { return nomeUsuario; }
std::string Usuario::getEmail() const { return email; }
std::vector<std::string> Usuario::getGeneroFav() const { return generoFav; }
std::string Usuario::getNomeCompleto() const { return nomeCompleto; }
std::vector<std::string> Usuario::getAmigos() const { return amigos; }

// Adicionar amigo
void Usuario::adicionarAmigo(const std::string& amigo) {
    amigos.push_back(amigo);
}
void Usuario::excluirAmigo(const std::string& amigo) {
    amigos.erase(std::remove(amigos.begin(), amigos.end(), amigo), amigos.end());
}
// Adicionar avaliação
void Usuario::adicionarAvaliacao(const std::string& idFilme, const std::string& nomeFilme, float nota, const std::string& comentario) {
    avaliacoes[idFilme] = Avaliacao((*this).getNomeUsuario(), idFilme,nomeFilme, nota , comentario); // Adiciona ou sobrescreve a avaliação
}
// Retirar uma avaliação
void Usuario::excluirAvaliacao(const std::string& idFilme){
    auto it = avaliacoes.find(idFilme);
    if (it != avaliacoes.end())
        avaliacoes.erase(it); // Remove a avaliação associada ao idFilme
}

// Converter Usuario para JSON
nlohmann::json Usuario::toJSON() const {
    std::vector<nlohmann::json> avaliacoesJson;

    for (const auto& [id, av] : avaliacoes) {
        nlohmann::json avJson = av.toJSON();
        avJson["id_filme"] = id;
        avaliacoesJson.push_back(avJson);
    }

    return {
        {"login", nomeUsuario},
        {"senha", senha},
        {"email", email},
        {"generos", generoFav},
        {"nome", nomeCompleto},
        {"avaliacoes", avaliacoesJson}
    };
}
