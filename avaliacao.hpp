#ifndef AVALIACAO_HPP
#define AVALIACAO_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "./json.hpp"
#include "./json_fwd.hpp"
#include "baseIMDb.hpp"

// Classe Avaliacao
class Avaliacao {
private:
    std::string _idUsuario;    // ID do usuário que fez a avaliação
    std::string _idFilme;      // ID do filme avaliado
    std::string _nomeFilme;    // Nome do filme avaliado
    std::string _comentario;   // Comentário feito pelo usuário
    float _nota;               // Nota dada pelo usuário

public:
    Avaliacao();
    Avaliacao(const std::string& idUsuario, const std::string& idFilme, 
              const std::string& nomeFilme, float nota, const std::string& comentario = "");

    void set(const std::string& idUsuario, const std::string& idFilme, 
             const std::string& nomeFilme, float nota, const std::string& comentario = "");

    std::string getIdUsuario() const;
    std::string getIdFilme() const;
    std::string getNomeFilme() const;
    float getNota() const;
    std::string getComentario() const;

    nlohmann::json toJSON() const;

    friend std::ostream& operator<<(std::ostream& out, const Avaliacao& avaliacao);
};

#endif //AVALIACAO_HPP