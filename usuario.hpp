#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "json_fwd.hpp"
#include "./avaliacao.hpp"
#include "./filme.hpp"

class Usuario {
private:
    std::string senha;
    std::string nomeUsuario;
    std::string email;
    std::vector<std::string> generoFav;
    std::vector<std::string> amigos;
    std::string nomeCompleto;

    // Lista de avaliações: id_filme -> Avaliação
    std::unordered_map<std::string, Avaliacao> avaliacoes;

public:
    // Construtor
    Usuario(std::string nomeU, std::string password, std::string email1, std::vector<std::string> genero, std::string nomeC);

    // Métodos get
    std::string getSenha() const;
    std::string getNomeUsuario() const;
    std::string getEmail() const;
    std::vector<std::string> getGeneroFav() const;
    std::string getNomeCompleto() const;
    std::vector<std::string> getAmigos() const;

    // Métodos para amigos
    void adicionarAmigo(const std::string& amigo);
    void excluirAmigo(const std::string& amigo);

    // Métodos para avaliações
    void adicionarAvaliacao(const std::string& idFilme, const std::string& nomeFilme, float nota, const std::string& comentario);
    void excluirAvaliacao(const std::string& idFilme);

    friend std::ostream& operator<<(std::ostream& out, const Usuario& usuario){
        out << "Login: " << usuario.getNomeUsuario() << "\n"
            << "Nome completo: " << usuario.getNomeCompleto() << "\n"
            << "Email: " << usuario.getEmail()
            << "Gêneros Favoritos: ";
        for(auto a : usuario.getGeneroFav())
            out << a << "\n";
        out << "Amigos: ";
        for(auto a : usuario.getAmigos())
            out << a << "\n";
        return out;
    }
    
    // Converter Usuario para JSON
    nlohmann::json toJSON() const;
};

#endif // USUARIO_HPP
