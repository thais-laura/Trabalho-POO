#ifndef REGISTRO_HPP
#define REGISTRO_HPP

#include "./usuario.hpp"
#include "./auxiliar.hpp"
#include <unordered_map>
#include <string>
#include <set>

// Classe Registro
class Registro {
private:
    std::unordered_map<std::string, Usuario> usuariosCadastrados;

    // Métodos auxiliares
    bool validarEmail(const std::string &email);
    bool validarNomeUsuario(const std::string &nomeUsuario);
    bool validarSenha(const std::string &senha);

public:
    // Cadastro e login
    void cadastro(std::unordered_map<std::string, Usuario>& listaUsuarios);
    std::string login(std::unordered_map<std::string, Usuario>& listaUsuarios);

    // Encerrar o programa
    void sair();
};

#endif // REGISTRO_HPP
