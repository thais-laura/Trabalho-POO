#include "registro.hpp"
#include <iostream>
#include <regex>

// Validação de e-mail
bool Registro::validarEmail(const std::string &email) {
    std::regex emailRegex("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}");
    return std::regex_match(email, emailRegex);
}

// Validação de nome de usuário
bool Registro::validarNomeUsuario(const std::string &nomeUsuario) {
    return usuariosCadastrados.find(nomeUsuario) == usuariosCadastrados.end();
}

// Validação de senha
bool Registro::validarSenha(const std::string &senha) {
    std::regex senhaRegex("^(?=.*[A-Za-z])(?=.*\\d)(?=.*[\\.\\-_@#$%!]).{6,}$");
    return senha.length() >= 6 && std::regex_match(senha, senhaRegex);
}

// Cadastro de usuários
void Registro::cadastro(std::unordered_map<std::string, Usuario>& listaUsuarios) {
    std::string nomeCompleto, nomeUsuario, email, senha, generoFav;
    std::vector<std::string> generos;
    std::set<std::string> generosValidos = {"acao", "comedia", "animacao", "terror", "romance", "suspense", "drama"};

    std::cout << "Digite seu nome completo: ";
    std::cin.ignore();
    std::getline(std::cin, nomeCompleto);

    std::cout << "Digite seu nome de usuário: ";
    std::cin >> nomeUsuario;

    std::cout << "Digite seu e-mail: ";
    std::cin >> email;

    std::cout << "Digite 3 gêneros favoritos (ação, comédia, animação, terror, romance, suspense, drama): ";
    while (generos.size() < 3) {
        std::cin >> generoFav;
        generoFav = converteMinusculo(generoFav);
        if (generosValidos.find(generoFav) != generosValidos.end()) {
            generos.push_back(generoFav);
        } else {
            std::cout << "Gênero inválido, tente novamente: ";
        }
    }
    std::cout << "Digite sua senha: ";
    std::cin >> senha;

    Usuario novoUsuario(nomeUsuario, senha, email, generos, nomeCompleto);
    listaUsuarios[nomeUsuario] = novoUsuario;

    std::cout << "Cadastro realizado com sucesso!\n";
}

// Login
std::string Registro::login(std::unordered_map<std::string, Usuario>& listaUsuarios) {
    std::string nomeUsuario, senha;
    std::cout << "Digite seu nome de usuário: ";
    std::cin >> nomeUsuario;
    std::cout << "Digite sua senha: ";
    std::cin >> senha;

    if (listaUsuarios.find(nomeUsuario) != listaUsuarios.end() && listaUsuarios[nomeUsuario].getSenha() == senha) {
        std::cout << "Login bem-sucedido! Bem-vindo, " << nomeUsuario << "!\n";
        return nomeUsuario;
    } else {
        std::cout << "Usuário ou senha incorretos.\n";
        return "";
    }
}

void Registro::sair() {
    std::cout << "O programa está sendo encerrado.\n";
}