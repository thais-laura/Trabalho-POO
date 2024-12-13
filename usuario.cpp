#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_set>
#include <unordered_map>


class Usuario{

    public:
    std::string senha;
    std::string nomeUsuario;
    std::string email;
    std::vector<std::string> generoFav; //lista dos genereos fav
    std::string nome;


    Usuario(std::string nomeU, std::string password, std::string email1, std::vector<std::string> genero) : nomeUsuario(nomeU), senha(password), email(email1), generoFav(genero) {} //construtor que inicializa os atributos diretamente
    Usuario() = default;
};

class Registro {
private:
    std::unordered_map<std::string, Usuario> usuariosCadastrados; //mapa para os usuarios cadastrados

    bool validarEmail(const std::string &email) { // aqui vamos fazer a validação dos emails, ver se seguem o padrao esperado de um endereço de email e verificar se ele já esta sendo utilizado
        std::regex emailRegex("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}");
        if (!std::regex_match(email, emailRegex)) {
            std::cout << "Formato de e-mail inválido.\n";
            return false;
        }
        for (const auto &par : usuariosCadastrados) {
            if (par.second.email == email) {
                std::cout << "Esse e-mail já foi utilizado.\n";
                return false;
            }
        }

        return true;
    }
    bool validarNomeUsuario(const std::string &nomeUsuario) { //validar nome de usuário vendo se ele já existe
        if (usuariosCadastrados.find(nomeUsuario) != usuariosCadastrados.end()) {
            std::cout << "Nome de usuário já existe.\n";
            return false;
        }
        return true;
    }
    bool validarSenha(const std::string &senha) { //validar senha para ter no minimo 6 caracteres e seguir um padrão de ser uma letra, um num e um simbolo
        if (senha.length() < 6) {
            std::cout << "A senha deve ter pelo menos 6 caracteres.\n";
            return false;
        }
        std::regex senhaRegex("^(?=.*[A-Za-z])(?=.*\\d)(?=.*[\\.\\-_@#$%!]).{6,}$");
        if (!std::regex_match(senha, senhaRegex)) {
            std::cout << "A senha deve conter pelo menos:\n"
                      << "- Uma letra\n"
                      << "- Um número\n"
                      << "- Um caractere especial (ex.: . - _ @ # $ % !)\n";
            return false;
        }
        return true;
    }
    bool loginUsuarioIguais(const std::string &nomeUsuario, const std::string &senha) { //ver se o nome que o usuário colocou no cadastro é o mesmo que o do login
    auto it = usuariosCadastrados.find(nomeUsuario);  // Busca pelo nome de usuário no mapa

    if (it == usuariosCadastrados.end()) {   // Verifica se o usuário existe
        std::cout << "Nome de usuário não encontrado.\n";
        return false;
    }

    // Verifica se a senha está correta
    if (it->second.senha != senha) {
        std::cout << "Senha incorreta.\n";
        return false;
    }

    return true; // Login bem-sucedido
}


public:
 void cadastro() {
    std::string nomeUsuario;
    std::string email;
    std::string senha;
    std::string generoFav;
    std::vector<std::string> generos;

    std::cout << "Bem-vindo à página de cadastro!\n";

    // Loop para garantir que o cadastro só termina após sucesso
    while (true) {
        std::cout << "Digite o nome de usuário que gostaria de ter: ";
        std::cin >> nomeUsuario;
        if (!validarNomeUsuario(nomeUsuario)) {
            std::cout << "Nome de usuário já existe. Tente novamente.\n";
            continue; // Retorna ao início do cadastro
        }

        std::cout << "Digite seu e-mail: ";
        std::cin >> email;
        if (!validarEmail(email)) {
            std::cout << "E-mail inválido ou já utilizado. Tente novamente.\n";
            continue; // Retorna ao início do cadastro
        }

        std::cout << "Digite seus 3 gêneros favoritos (ação, comédia, romance, terror, suspense, drama, animação):\n";
        generos.clear(); // Limpa a lista de gêneros caso o cadastro seja reiniciado

        while (generos.size() < 3) {
        std::cin >> generoFav;

     
        // Adiciona o gênero à lista
        generos.push_back(generoFav);
    }

        std::cout << "Digite sua senha: ";
        std::cin >> senha;
        if (!validarSenha(senha)) {
            std::cout << "Senha inválida. Tente novamente.\n";
            continue; // Retorna ao início do cadastro
        }

        // Cadastro bem-sucedido
        Usuario novoUsuario(nomeUsuario, senha, email, generos);
        usuariosCadastrados[nomeUsuario] = novoUsuario;

        std::cout << "Usuário cadastrado com sucesso!\n";
        break; // Sai do loop após cadastro bem-sucedido
    }

    std::cout << "Retornando ao menu principal...\n";
}


    void login() {
        std::string nomeUsuario;
        std::string senha;
        char escolha;

        // Primeira tentativa de login
        std::cout << "Digite seu nome de usuário: ";
        std::cin >> nomeUsuario;

        std::cout << "Digite sua senha: ";
        std::cin >> senha;

        if (loginUsuarioIguais(nomeUsuario, senha)) {
            std::cout << "Login realizado com sucesso! Bem-vindo, " << nomeUsuario << "!\n";
            return;
        }

        // Segunda tentativa de login
        std::cout << "Nome de usuário e/ou senha inválidos. Tente novamente.\n";
        std::cout << "Digite seu nome de usuário novamente: ";
        std::cin >> nomeUsuario;

        std::cout << "Digite sua senha novamente: ";
        std::cin >> senha;

        if (loginUsuarioIguais(nomeUsuario, senha)) {
            std::cout << "Login realizado com sucesso! Bem-vindo, " << nomeUsuario << "!\n";
            return;
        }

        // Após a segunda falha, a função de redefinição de senha pode ser utilizada
        std::cout << "Nome de usuário e/ou senha inválidos novamente.\n";
        std::cout << "Deseja redefinir sua senha? (s/n): ";
        std::cin >> escolha;

        if (escolha == 's' || escolha == 'S') {
            esqueciSenha(); // Chama a função para redefinir a senha
        } else {
            std::cout << "Retornando ao menu principal...\n";
        }
}


    void esqueciSenha() {
        std::string email;
        std::string nomeUsuario;
        std::string novaSenha;
        std::string confirmaSenha;

        std::cout << "Digite seu e-mail cadastrado: ";
        std::cin >> email;

        std::cout << "Digite seu nome de usuário cadastrado: ";
        std::cin >> nomeUsuario;

        auto it = usuariosCadastrados.find(nomeUsuario);
        if (it == usuariosCadastrados.end() || it->second.email != email) {
            std::cout << "E-mail ou nome de usuário não correspondem aos cadastrados.\n";
            return;
        }

        do {
            std::cout << "Digite a nova senha: ";
            std::cin >> novaSenha;

            std::cout << "Repita a nova senha: ";
            std::cin >> confirmaSenha;

            if (novaSenha != confirmaSenha) {
                std::cout << "As senhas não correspondem. Tente novamente.\n";
            }
        } while (novaSenha != confirmaSenha);

        if (!validarSenha(novaSenha)) {
            std::cout << "A senha não atende aos requisitos.\n";
            return;
        }

        it->second.senha = novaSenha;
        std::cout << "Senha alterada com sucesso!\n";
    }
    void sair() {
    std::cout << "O programa esta sendo encerrado!\n";
    exit(0); // Finaliza o programa
}


};

int main() {
    Registro registro;
    char opcao;

    do {
        std::cout << "\n=== Sistema de Avaliação de Filmes ===\n";
        std::cout << "1. Cadastro\n";
        std::cout << "2. Login\n";
        std::cout << "3. Sair\n";
        std::cout << "Escolha uma opção: ";
        std::cin >> opcao;

        switch (opcao) {
            case '1':
                registro.cadastro(); //realiza o cadastro do usuario
                break;
            case '2':
                registro.login(); // realiza o login do usuario
                break;
            case '3':
                registro.sair(); // Encerra o programa
                break;
            default:
                std::cout << "Opção inválida. Tente novamente.\n";
        }
    } while (opcao != '3'); 

    return 0;
}

