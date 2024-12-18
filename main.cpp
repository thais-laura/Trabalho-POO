#include "usuario.hpp"
#include "registro.hpp"
#include "./armazenamento.hpp"
#include "./baseIMDb.hpp"
#include "./avaliacao.hpp" 
#include "./filme.hpp"
#include <iostream>


int main() {
    Registro registro;
    char opcao;
    std::unordered_map<std::string, std::shared_ptr<Filme>> listaFilmes;

    std::unordered_map<std::string, Usuario> listaUsuarios;

    listaFilmes = carregarFilmes();
    listaUsuarios = carregarUsuarios();

    do {
        std::cout << "\n=== Sistema de Avaliação de Filmes ===\n";
        std::cout << "1. Cadastro\n2. Login\n3. Sair\nEscolha uma opção: ";
        std::cin >> opcao;

        switch (opcao) {
            case '1':
                registro.cadastro(listaUsuarios);
                break;

            case '2': {
                std::string usuarioLogado = registro.login(listaUsuarios);
                if (!usuarioLogado.empty()) {
                    char subOpcao;
                    do {
                        std::cout << "\n=== Menu do Usuário ===\n";
                        std::cout << "1. Adicionar Amigo\n";
                        std::cout << "2. Excluir Amigo\n";
                        std::cout << "3. Avaliar Filme\n";
                        std::cout << "4. Pior e Melhor Avaliação\n";
                        std::cout << "5. Recomendar Filme\n";
                        std::cout << "6. Ver Meu Perfil\n";
                        std::cout << "7. Logout\n";
                        std::cout << "Escolha uma opção: ";
                        std::cin >> subOpcao;

                        if (subOpcao == '1') {  // Adicionar amigo
                            std::string amigo;
                            std::cout << "Digite o nome do amigo a ser adicionado: ";
                            std::cin >> amigo;
                            auto it = listaUsuarios.find(amigo);
                            if(it != listaUsuarios.end()){
                                std::cout << listaUsuarios[amigo];
                                std::cout << "Deseja mesmo adicioná-lo? [S, N]: ";
                                std::string sn;
                                std::cin >> sn;
                                if(sn == "S" || sn == "s"){
                                    listaUsuarios[usuarioLogado].adicionarAmigo(amigo);
                                    std::cout << "Amigo adicionado com sucesso!\n";
                                }else std::cout << "Amigo não adicionado\n";
                            }else std::cout << "Amigo não encontrado.\n";
                        } 
                        else if (subOpcao == '2') { // Excluir amigo
                            std::string amigo;
                            std::cout << "Digite o nome do amigo a ser removido: ";
                            std::cin >> amigo;
                            auto it = listaUsuarios.find(amigo);
                            if(it != listaUsuarios.end()){
                                std::cout << listaUsuarios[amigo];
                                std::cout << "Deseja mesmo exclui-lo? [S, N]: ";
                                std::string sn;
                                std::cin >> sn;
                                if(sn == "S" || sn == "s"){
                                    listaUsuarios[usuarioLogado].excluirAmigo(amigo);
                                    std::cout << "Amigo removido com sucesso!\n";
                                }else std::cout << "Amigo não removido\n";
                            }else std::cout << "Amigo não encontrado.\n";
                        } 
                        else if (subOpcao == '3') { // Avaliar Filme
                            std::string nomeFilme;
                            std::string aux;
                            float nota;
                            std::string comentario;

                            std::cout << "Digite o nome do filme: ";
                            std::cin.ignore();
                            std::getline(std::cin, nomeFilme);

                            // Buscar o filme na lista de filmes
                            std::vector<std::string> ids = buscaFilmeNome(nomeFilme, listaFilmes);
                            if (!ids.empty()) { // Verifica se o filme foi encontrado
                                int i = 0;
                                std::cout << "--------------------------\n";
                                for(auto& f : ids){
                                    std::cout << "Filme " << i+1 << ":\n" << *listaFilmes[f];
                                    std::cout << "--------------------------\n";

                                    i++;
                                }
                                size_t escolhido = -1;
                                do {
                                    std::cout << "Digite qual filme vai ser avaliado (1-" << i << "): ";
                                    std::cin >> escolhido;

                                    if (std::cin.fail()) {  // Verifica se houve erro na entrada
                                        std::cin.clear();   // Limpa o estado de erro
                                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Descarta a entrada inválida
                                        std::cout << "Entrada inválida. Por favor, insira um número inteiro.\n";
                                    }
                                } while ((escolhido < 1) || (escolhido > i));

                                auto filmeEscolhido = listaFilmes[ids[escolhido-1]];
                                std::cout << "Digite a nota do filme (0-10): ";
                                std::cin >> aux;
                                while(!isFloat(aux)){
                                    std::cout << "Digite a nota do filme (0-10): ";
                                    std::cin >> aux;
                                }
                                nota = std::stod(aux);
                                std::cin.ignore();
                                std::cout << "Digite um comentário: ";
                                std::getline(std::cin, comentario);

                                // Adicionar a avaliação ao filme e ao usuário
                                filmeEscolhido->adicionarOuAtualizar(listaUsuarios[usuarioLogado].getNomeUsuario(), nota, comentario);
                                listaFilmes[ids[escolhido-1]] = filmeEscolhido;
                                listaUsuarios[usuarioLogado].adicionarAvaliacao(filmeEscolhido->getId(), filmeEscolhido->getNome(), nota, comentario);
                                std::cout << "Avaliação adicionada com sucesso!\n";
                            } else {
                                std::cout << "Filme '"<< nomeFilme<<"' não encontrado.\n";
                            }
                        }else if (subOpcao == '4') { // Ver a melhor e a pior avaliação de certo filme
                            std::string nomeFilme;
                            std::cout << "Digite o nome do filme: ";
                            std::cin.ignore();
                            std::getline(std::cin, nomeFilme);

                            std::vector<std::string> ids = buscaFilmeNome(nomeFilme, listaFilmes);
                            // Buscar o filme na lista de filmes
                            if (!ids.empty()) { // Verifica se o filme foi encontrado
                                int i = 0;
                                std::cout << "--------------------------\n";
                                for(auto& f : ids){
                                    listaFilmes[f]->mostrarMelhorEPior();
                                    std::cout << "--------------------------\n";
                                    i++;
                                }
                            }else
                                std::cout << "Filme '"<< nomeFilme<<"' não encontrado.\n";
                            
                        } 
                        else if (subOpcao == '5') { // Recomendar Filmes
                            char tipoRecomendacao;
                            std::cout << "Deseja recomendação por (G)ênero ou (A)tor? ";
                            std::cin >> tipoRecomendacao;

                            if (tipoRecomendacao == 'G' || tipoRecomendacao == 'g') {
                                std::string genero;
                                std::cout << "Digite o gênero desejado: ";
                                std::cin >> genero;

                                auto recomendacoes = recomendaGenero(genero, listaFilmes);
                                std::cout << "Filmes recomendados:\n";
                                for (const auto& filme : recomendacoes) {
                                    std::cout << filme << "\n";
                                }
                            } 
                            else if (tipoRecomendacao == 'A' || tipoRecomendacao == 'a') {
                                std::string nomeAtor;
                                std::cout << "Digite o nome do ator: ";
                                std::cin.ignore();
                                std::getline(std::cin, nomeAtor);

                                auto recomendacoes = recomendaAtor(nomeAtor, listaFilmes);
                                std::cout << "\nFilmes recomendados:\n";
                                std::cout << "--------------------------\n";
                                for (const auto& filme : recomendacoes) {
                                    std::cout << *filme << "\n";
                                    std::cout << "--------------------------\n";
                                }
                            } else {
                                std::cout << "Opção inválida.\n";
                            }
                        }
                        else if (subOpcao == '6') { 
                            std::cout << listaUsuarios[usuarioLogado];
                        }
                    } while (subOpcao != '7');
                    std::cout << "Logout realizado. Até logo!\n";
                }
                break;
            }
            case '3':
                registro.sair();
                break;
            default:
                std::cout << "Opção inválida. Tente novamente.\n";
        }
    } while (opcao != '3');

    // Salvar os dados no JSON
    salvarFilmes(listaFilmes);
    salvarUsuarios(listaUsuarios);

    return 0;
}