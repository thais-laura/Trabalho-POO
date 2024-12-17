#include "usuario.hpp"
#include "registro.hpp"
#include "./armazenamento.cpp"
#include "./baseIMDb.hpp"
#include "./avaliacaoEfilmes.cpp" 
#include <iostream>


int main() {
    Registro registro;
    char opcao;
    std::unordered_map<std::string, std::shared_ptr<Filme> > listaFilmes;

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
                        std::cout << "4. Recomendar Filme\n";
                        std::cout << "5. Logout\n";
                        std::cout << "Escolha uma opção: ";
                        std::cin >> subOpcao;

                        if (subOpcao == '1') {
                            std::string amigo;
                            std::cout << "Digite o nome do amigo a ser adicionado: ";
                            std::cin >> amigo;
                            listaUsuarios[usuarioLogado].adicionarAmigo(amigo);
                            std::cout << "Amigo adicionado com sucesso!\n";
                        } 
                        else if (subOpcao == '2') {
                            std::string amigo;
                            std::cout << "Digite o nome do amigo a ser removido: ";
                            std::cin >> amigo;
                            listaUsuarios[usuarioLogado].excluirAmigo(amigo);
                            std::cout << "Amigo removido com sucesso!\n";
                        } 
                        else if (subOpcao == '3') { // Avaliar Filme
                            std::string nomeFilme;
                            float nota;
                            std::string comentario;

                            std::cout << "Digite o nome do filme: ";
                            std::cin.ignore();
                            std::getline(std::cin, nomeFilme);

                            // Buscar o filme na lista de filmes
                            std::vector<Filme> filmesEncontrados = buscaFilmeNome(nomeFilme, listaFilmes);

                            if (filmesEncontrados.size() > 0) { // Verifica se o filme foi encontrado
                                int i = 0;
                                for(auto f : filmesEncontrados){
                                    std::cout << "Filme " << i+1 << ":\n" << f;
                                    i++;
                                }
                                int escolhido;
                                do{
                                    std::cout << "Digite qual filme vai ser avaliado (1-" << i+1 << "): ";
                                    std::cin >> escolhido;
                                }while((escolhido < 1)||(escolhido > filmesEncontrados.size()+1));
                                
                                Filme filmeEscolhido = filmesEncontrados.at(escolhido - 1);

                                std::cout << "Digite a nota do filme (0-10): ";
                                std::cin >> nota;
                                std::cin.ignore();
                                std::cout << "Digite um comentário: ";
                                std::getline(std::cin, comentario);

                                // Adicionar a avaliação ao filme e ao usuário
                                filmeEscolhido.adicionarOuAtualizar(listaUsuarios[usuarioLogado].getNomeUsuario(), nota, comentario);
                                listaUsuarios[usuarioLogado].adicionarAvaliacao(filmeEscolhido.getId(), filmeEscolhido.getNome(), nota, comentario);
                                std::cout << "Avaliação adicionada com sucesso!\n";
                            } else {
                                std::cout << "Filme não encontrado.\n";
                            }
                        } 
                        else if (subOpcao == '4') { // Recomendar Filmes
                            char tipoRecomendacao;
                            std::cout << "Deseja recomendação por (G)ênero ou (A)tor? ";
                            std::cin >> tipoRecomendacao;

                            if (tipoRecomendacao == 'G' || tipoRecomendacao == 'g') {
                                std::string genero;
                                std::cout << "Digite o gênero desejado: ";
                                std::cin >> genero;

                                /* auto recomendacoes = recomendaGenero(genero, listaFilmes);
                                std::cout << "Filmes recomendados:\n";
                                for (const auto& filme : recomendacoes) {
                                    std::cout << filme << "\n";
                                } */
                            } 
                            else if (tipoRecomendacao == 'A' || tipoRecomendacao == 'a') {
                                std::string nomeAtor;
                                std::cout << "Digite o nome do ator: ";
                                std::cin.ignore();
                                std::getline(std::cin, nomeAtor);

                                auto recomendacoes = recomendaAtor(nomeAtor, listaFilmes);
                                std::cout << "Filmes recomendados:\n";
                                for (const auto& filme : recomendacoes) {
                                    std::cout << filme << "\n";
                                }
                            } else {
                                std::cout << "Opção inválida.\n";
                            }
                        }

                    } while (subOpcao != '5');
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