#include "./armazenamento.hpp"
#include "./avaliacaoEfilmes.cpp"
//jsoneditoronline.org
using json = nlohmann::json;


void salvarFilmes(std::vector<Filme> listaFilmes){
    nlohmann::json jsonFilmes = nlohmann::json::array();

    for (Filme filme : listaFilmes) {
        jsonFilmes.push_back(filme.toJSON());
    }

    std::ofstream arquivo("armazFilmes.json");
    arquivo << jsonFilmes.dump(2); // Formatação com 4 espaços de indentação
}

void salvarUsuarios(std::vector<Usuario> listaUsuarios){
    json jsonUsuarios = json::array();

    for (Usuario usuario : listaUsuarios) {
        jsonUsuarios.push_back(usuario.toJSON());
    }

    std::ofstream arquivo("armazUsuarios.json");
    arquivo << jsonUsuarios.dump(2); // Formatação com 4 espaços de indentação
}

void carregarFilmes(std::map<std::string, std::shared_ptr<Filme>>& filmes){
    std::ifstream arquivo("armazFilmes.json");
    if (!arquivo.is_open()) return;

    json jsonFilmes;
    arquivo >> jsonFilmes;        // Carrega todos os objetos json do arquivo

    for (const auto& aux : jsonFilmes) {
        auto filme = std::make_shared<Filme>(
            aux["id"].get<std::string>(),
            aux["titulo"].get<std::string>(),
            aux["media"].get<std::string>(),
            aux["qtdAv"].get<std::string>(),
            aux["elenco"].get<std::vector<std::string>>(),
            aux["ano"].get<std::string>(),
            aux["generos"].get<std::vector<std::string>>()
        );

        // Carregando as avaliações
        for (const auto& av : jsonFilmes["avaliacoes"]) {
            filme->adicionarOuAtualizar(av["login"].get<std::string>(), av["nota"].get<double>(),
                av["comentario"].get<std::string>()); // adiciona a avaliacao do usuario
        }
    }
}

void carregarUsuarios(std::map<std::string, std::shared_ptr<Usuario>>& usuarios) {
    std::ifstream arquivo("armazUsuarios.json");
    if (!arquivo.is_open()) return;

    json jsonUsuarios;
    arquivo >> jsonUsuarios;        // Carrega todos os objetos json do arquivo

    for (const auto& jsonUsuario : jsonUsuarios) {
        auto usuario = std::make_shared<Usuario>(
            jsonUsuario["login"].get<std::string>(),
            jsonUsuario["senha"].get<std::string>(),
            jsonUsuario["email"].get<std::string>(),
            jsonUsuario["data_nascimento"].get<std::string>(),
            jsonUsuario["generos_favoritos"].get<std::vector<std::string>>()
        );

        // Carregando as avaliações
        for (const auto& av : jsonUsuario["avaliacoes"]) {
            Avaliacao avaliacao(
                av["login"].get<std::string>(),  // ID do usuário
                av["id filme"].get<std::string>(),
                av["titulo"].get<std::string>(),
                av["nota"].get<double>(),
                av["comentario"].get<std::string>()
            );  // inicializador da avaliacao
            usuario->adicionarAvaliacao(avaliacao); // adiciona a avaliacao no usuario
        }
// inserir atributos do usuario na lista de usuarios (em memoria)
        usuarios[jsonUsuario["login"]] = usuario; 

    }
}

/* void verG(){
    std::ifstream arqInfoFilmes("infoFilmes");
    if (!arqInfoFilmes.is_open()) {
        printf("Falha no processamento do arquivo\n");
        return;
    }
    int i =0;
    std::set<std::string> generos, subgeneros, subsubg;
    std::string linha;
    std::string id, tipo, titulo, tituloOrig, adulto, genero, subgenero, subsubgenero, anoInicio, anoFim, duracao;
    while (std::getline(arqInfoFilmes, linha)) {
        std::istringstream ss(linha);
        // Lê os campos separados por tabulação
        getline(ss, id, '\t');
        getline(ss, tipo, '\t');
        getline(ss, titulo, '\t');
        getline(ss, tituloOrig, '\t');
        getline(ss, adulto, '\t');
        getline(ss, anoInicio, '\t');
        getline(ss, anoFim, '\t');
        getline(ss, duracao, '\t');
        getline(ss, genero, ',');
        getline(ss, subgenero, ',');
        getline(ss, subsubgenero, '\t');
        if(tipo == "movie"){
            generos.insert(genero);
        }
        // if((genero == "Biography" || genero == "Documentary")&& (subgenero == "Drama" || subsubgenero == "Drama") && tipo == "movie"){
        //     std::cout << titulo << "\n";
        // }
    }
    printf("i : %d\n", i);
    // for(std::string a : generos){
    //     std::cout << a << "\n";
    // }
    arqInfoFilmes.close();
}
 */
