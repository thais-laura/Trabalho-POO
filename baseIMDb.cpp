#include "./baseIMDb.hpp"
#include "./filme.hpp"
#include "./auxiliar.hpp"


std::vector<std::string> buscaFilmeNome(std::string nomeFilme, std::unordered_map<std::string, std::shared_ptr<Filme>>& listaConhecidos){
    std::ifstream arqInfoFilmes("infoFilmes");
    arqInfoFilmes.clear();                  // Limpa os flags de erro, se houver
    arqInfoFilmes.seekg(0, std::ios::beg);  // Volta para o início
    std::vector<std::string> ids;
    if (!arqInfoFilmes.is_open()) {
        msg_erro_arq();
        return ids;
    }
    std::string linha;
    std::string n = converteMinusculo(nomeFilme);
    std::string id, tipo, titulo, tituloOrig, adulto, genero, subgenero, subsubgenero, anoInicio, anoFim, duracao;
    while (std::getline(arqInfoFilmes, linha)){
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

        // Verifica se o título corresponde ao filme buscado
        std::string aux = converteMinusculo(titulo);
         if (aux == n && tipo == "movie") {
            if(listaConhecidos.find(id)==listaConhecidos.end()){    // Se não estiver na lista, pega outras informações
                verificaGeneros(genero, subgenero, subsubgenero);   // Realiza as trocas de genero e subgenero se necessário
                std::vector<std::string> elenco;
                elenco = buscaElenco(id);
                std::string media;
                std::string numPessoas;
                buscaAvaliacao(id, media, numPessoas);

                // Algumas informações podem estar vazias ou mal formatadas
                int classificacao = (adulto.empty() || std::any_of(adulto.begin(), adulto.end(), [](char c){ return !isdigit(c); })) ? 0 : std::stoi(adulto);
                int med = (media.empty() || std::any_of(media.begin(), media.end(), [](char c){ return !isdigit(c); })) ? 0 : std::stoi(media);
                int nMed = (numPessoas.empty() || std::any_of(numPessoas.begin(), numPessoas.end(), [](char c){ return !isdigit(c); })) ? 0 : std::stoi(numPessoas);
                int ano = (anoInicio.empty() || std::any_of(anoInicio.begin(), anoInicio.end(), [](char c){ return !isdigit(c); })) ? 0 : std::stoi(anoInicio);
                int d = (duracao.empty() || std::any_of(duracao.begin(), duracao.end(), [](char c){ return !isdigit(c); })) ? 0 : std::stoi(duracao);

                auto filme = criarFilmePorGenero(genero);
                filme->set(id, titulo, genero, subgenero, elenco, classificacao, med, nMed, ano, d);
                listaConhecidos[id] = filme;    // Armazena no mapa
            }
            ids.push_back(id);                  // Importante para a busca na lista de filmes
        }
    }
    arqInfoFilmes.close();
    return ids;
} 

std::shared_ptr<Filme> buscaFilmeId(const std::string &id, std::unordered_map<std::string, std::shared_ptr<Filme>> &listaConhecidos) {
    std::ifstream arqInfoFilmes("infoFilmes");
    arqInfoFilmes.clear();                  // Limpa os flags de erro, se houver
    arqInfoFilmes.seekg(0, std::ios::beg);  // Volta para o início
    if (!arqInfoFilmes.is_open()) {
        msg_erro_arq();
        return nullptr;
    }

    auto it = listaConhecidos.find(id);
    if (it != listaConhecidos.end()) {    // Filme já procurado anteriormente
        return it->second;
    }

    std::string linha;
    std::string aux, tipo, titulo, tituloOrig, adulto, genero, subgenero, subsubgenero, anoInicio, anoFim, duracao;
    while (std::getline(arqInfoFilmes, linha)) {
        std::istringstream ss(linha);
        // Lê os campos separados por tabulação
        getline(ss, aux, '\t');
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

        // Verifica se o título corresponde ao filme buscado
        if (aux == id) {
            verificaGeneros(genero, subgenero, subsubgenero);      // Realiza as trocas de genero e subgenero se necessário
            std::vector<std::string> elenco = buscaElenco(aux);
            std::string media, numPessoas;
            buscaAvaliacao(aux, media, numPessoas);

            int classificacao = adulto.empty() ? 0 : stoi(adulto);
            float med = media.empty() ? 0.0 : stod(media);
            int nMed = numPessoas.empty() ? 0 : stoi(numPessoas);
            int ano = anoInicio.empty() ? 0 : stoi(anoInicio);
            int d = duracao.empty() ? 0 : stoi(duracao);

            auto filme = criarFilmePorGenero(genero);
            filme->set(id, titulo, genero, subgenero, elenco, classificacao, med, nMed, ano, d);

            listaConhecidos[id] = filme;    // Armazena no mapa
            return filme;                   // Como o id é unico, encontra-se somente um filme
        }
    }
    arqInfoFilmes.close();
    return nullptr;
}


std::vector<std::string> buscaElenco(std::string &id){
    // Primeira parte: encontrar os ids dos atores do filme
    std::ifstream arqElenco("filmeAtores");
    arqElenco.clear();                  // Limpa os flags de erro, se houver
    arqElenco.seekg(0, std::ios::beg);  // Volta para o início
    std::vector<std::string> elenco;
    if (!arqElenco.is_open()) {
        msg_erro_arq();
        return elenco;
    }

    std::string linha;
    bool filme_encontrado = false;
    std::string auxId_filme, id_pessoa, ordem, posicao;
    std::vector<std::string> ids;
    while (std::getline(arqElenco, linha)) {
        std::istringstream ss(linha);
        std::getline(ss, auxId_filme, '\t');
        std::getline(ss, ordem, '\t');
        std::getline(ss, id_pessoa, '\t');
        std::getline(ss, posicao, '\t');
        
        if (id == auxId_filme){                                 // Encontrou o filme em questão
            if(posicao == "actor" || posicao == "actress"){     // Verifica se fez parte do elenco (não necessita grupo técnico)             
                // Verifica se algum ator já foi inserido (pode ter diferentes papéis no filme)
                if(std::find(ids.begin(), ids.end(), id_pessoa) == ids.end())
                    ids.push_back(id_pessoa);
            }
            filme_encontrado = true;
        }if(filme_encontrado== true && id != auxId_filme)       // Já encontrou todos do elenco do filme
            break;
    }
    if(ids.empty()) return elenco;
    arqElenco.close();

    // Segunda parte: relacionar id com nome do ator
    std::ifstream arqNomesPessoas("nomesPessoas");
    if (!arqNomesPessoas.is_open()) {
        msg_erro_arq();
        return elenco;
    }
    
    std::string aux_nome;
    for (std::string& elem : ids){
        arqNomesPessoas.clear();                                // Limpa flags de erro, como EOF
        arqNomesPessoas.seekg(0);                               // Reposiciona o ponteiro para o início do arquivo
        while (std::getline(arqNomesPessoas, linha)) {
            std::istringstream ss(linha);
            // Lê os campos separados por tabulação
            getline(ss, id_pessoa, '\t');
            getline(ss, aux_nome, '\t');

            if (elem == id_pessoa) {
                elenco.push_back(aux_nome); 
                break;
            }
        }
    }
    arqNomesPessoas.close(); 
    return elenco;
}

void buscaAvaliacao(std::string &id, std::string &mediaAv, std::string &numAv){
    std::ifstream arqAv("mediasAvaliacoes");
    arqAv.clear();                          // Limpa os flags de erro, se houver
    arqAv.seekg(0, std::ios::beg);          // Volta para o início
    if (!arqAv.is_open()) {
        msg_erro_arq();
        return;
    }
    std::string linha;
    bool filme_encontrado = false;
    std::string aux_id, media, numero;
    while (getline(arqAv, linha)) {
        std::istringstream ss(linha);
        // Lê os campos separados por tabulação
        getline(ss, aux_id, '\t');
        getline(ss, media, '\t');
        getline(ss, numero, '\n');
        if (id == aux_id) {
            mediaAv = media;
            numAv = numero;
            filme_encontrado = true;
            break;                          // Apenas uma média por filme
        }
    }
    arqAv.close();
    if(!filme_encontrado){
        mediaAv = "-1";
        numAv = "-1";
    }
}

void verificaGeneros(std::string &genero, std::string &subgenero, std::string &subsubgenero){
    if(classeGenero.find(genero) == classeGenero.end()){        // Se o gênero não tem uma classe própria
        if(classeGenero.find(subgenero) != classeGenero.end()){ // Segundo gênero tem uma classe própria
            std::string aux;
            aux = genero;
            genero = tradGenero[subgenero];
            subgenero = tradGenero[aux];
        }else if(classeGenero.find(subsubgenero) == classeGenero.end()){
            genero = tradGenero[subsubgenero];  // Não vamos trocar pois só utilizamos dois gêneros da lista
        }else{                                  // Não há nenhum gênero da lista que tenha classe própria
            genero = combGenero[genero];
        }
    }else{
        genero = tradGenero[genero];
        subgenero = tradGenero[subgenero];
    }
}

std::vector<std::shared_ptr<Filme>> recomendaAtor(std::string &nome, std::unordered_map<std::string, std::shared_ptr<Filme>> &listaConhecidos){
    std::string linha;
    std::ifstream arqNomesPessoas("nomesPessoas");
    arqNomesPessoas.clear();                    // Limpa os flags de erro, se houver
    arqNomesPessoas.seekg(0, std::ios::beg);    // Volta para o início
    std::vector<std::shared_ptr<Filme>> filmes;
    if (!arqNomesPessoas.is_open()) {
        msg_erro_arq();
        return filmes;
    }
    std::string n = converteMinusculo(nome);
    std::cout << nome;
    std::string id_pessoa, aux_nome, nasc, morte, profissao, titulos;
    std::string id_filme;
    while (std::getline(arqNomesPessoas, linha)) {
        std::istringstream ss(linha);
        // Lê os campos separados por tabulação
        std::getline(ss, id_pessoa, '\t');
        std::getline(ss, aux_nome, '\t');
        
        aux_nome = converteMinusculo(aux_nome);
        if (n == aux_nome) {
            std::cout << aux_nome;
            std::getline(ss, nasc, '\t');
            std::getline(ss, morte, '\t');
            std::getline(ss, profissao, '\t');
            std::getline(ss, titulos, '\t');

            std::regex separador("([^,]+)");

            // iterador que prercorre do início ao fim da string
            std::sregex_iterator it(titulos.begin(), titulos.end(), separador);
            std::sregex_iterator fim;

            while (it != fim) {
                std::shared_ptr<Filme> f;
                id_filme = (*it).str();

                f = buscaFilmeId(id_filme, listaConhecidos);
                // std::cout << f;
                if(!(f->getId()).empty())
                    filmes.push_back(f);    // Filme encontrado
                it++;
            }
            break;                          // Já encontrou o ator e os seus filmes famosos
        }
    }
    arqNomesPessoas.close(); 
    return filmes;
}

std::vector<Filme> recomendaGenero(std::string genero){
    std::vector<Filme> listaFilmes;
    std::ifstream arquivo("armazRecomendacoes.json");
    if (!arquivo.is_open()) {
        msg_erro_arq();
        return listaFilmes;
    }

    nlohmann::json jsonFilmes;
    arquivo >> jsonFilmes;     // Carrega todos os objetos JSON do arquivo

    if (jsonFilmes.is_array() && !jsonFilmes.empty()) {
        std::string g;
        genero = converteMinusculo(genero);
        for (const auto& aux : jsonFilmes) {
            Filme f;
            if(aux.contains("genero")){
                g = aux["genero"].get<std::string>();
                g = converteMinusculo(g);
                if(g == genero){

                    // Cria o objeto Filme a partir dos dados do JSON
                    if (aux.contains("id") && aux.contains("titulo") && aux.contains("subgenero") &&
                        aux.contains("elenco") && aux.contains("classificacao") && aux.contains("media") &&
                        aux.contains("quantidade de avaliações") && aux.contains("ano") && aux.contains("duracao")) {
                            f.set(aux["id"].get<std::string>(),
                                    aux["titulo"].get<std::string>(),
                                    aux["genero"].get<std::string>(),
                                    aux["subgenero"].get<std::string>(),
                                    aux["elenco"].get<std::vector<std::string>>(),
                                    aux["classificacao"].get<bool>(),
                                    aux["media"].get<float>(),
                                    aux["quantidade de avaliações"].get<int>(),
                                    aux["ano"].get<int>(),
                                    aux["duracao"].get<int>());
                    }
                    // Carrega as avaliações associadas ao filme
                    if (aux.contains("avaliacoes")) {
                        for (const auto& av : aux["avaliacoes"]) {
                            if (av.contains("login") && av.contains("nota") && av.contains("comentario")) {
                                f.adicionarOuAtualizar(
                                    av["login"].get<std::string>(),
                                    av["nota"].get<float>(),
                                    av["comentario"].get<std::string>()
                                );
                            }
                        }
                    }
                    // Adiciona o filme ao vector
                    listaFilmes.push_back(f);
                }
            }
        }
    }
    return listaFilmes;
} 