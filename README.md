# Trabalho Final de Programação Orientada a Objetos
## Sistema de avaliação e recomendação de filmes
  O projeto é baseado em um sistema de avaliação e recomendação de filmes, desenvolvido a partir de uma base de dados. O sistema permite que os usuários avaliem filmes, sigam amigos e visualizem suas avaliações, tornando a experiência mais interativa e social. \
  As recomendações são personalizadas de acordo com os gêneros favoritos de cada usuário e os filmes com as maiores notas no banco de dados. A ideia surgiu para resolver a indecisão comum na escolha de filmes, proporcionando uma maneira de descobrir obras semelhantes às que já gostamos. Além disso, o sistema incentiva os usuários a expressarem suas opiniões por meio de notas e comentários, tornando o formato mais convidativo tanto para as avaliações rápidas quanto para as mais detalhadas. \
______________________________________________________
### Grupo: Gabriella Castelari, Isabela Muller, Thaís Laura
## Histórico de Versões
V1 (23/11/2024): Criação de funções para busca de filmes pelo nome (encontra informações como id, ano de lançamento, elenco, gêneros, média de avaliações, etc). Além disso, criação de padrão de conversão das classes (Usuario, Avaliacao e Filme) para o formato do Json, além das funções de escrita e leitura do arquivo. \
V2 (16/12/2024): Separação das classes Avaliacao e Filme, criação dos arquivos .hpp e de funções de recomendação. Também houve atualização dos métodos virtuais e modificação de estruturas para shared_ptr para se utilizar o polimorfismo \
V3 (18/12/2024): Todos os testes passaram após algumas modificações. Existe proteção contra entradas inválidas (se é dada uma string no lugar de um float ou inteiro, não ocorre falha mais)
## Base de dados: IMDb
"This program uses TMDB and the TMDB APIs but is not endorsed, certified, or otherwise approved by TMDB."
