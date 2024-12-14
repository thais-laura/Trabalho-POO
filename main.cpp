#include "./armazenamento.hpp"
#include "./avaliacaoEfilmes.cpp"
#include "usuario.cpp"

int main() {
    Registro registro;
    char opcao;
    std::unordered_map<std::string, Filme> listaConhecidos;

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