#include "./baseIMDb.hpp"
#include "./avaliacaoEfilmes.cpp"
// "This [website, program, service, application, product] uses TMDB and the TMDB APIs but is not endorsed, certified, or otherwise approved by TMDB."


std::string converteMinusculo(std::string &str){
    std::string min;
    for(int i=0; i< str.size(); i++){
        min += tolower(str.at(i));
    }
    return min;
}

std::vector<Filme> buscaFilmeNome(std::string nomeFilme){
    std::ifstream arqInfoFilmes("infoFilmes");
    std::vector<Filme> vetorFilme;
    if (!arqInfoFilmes.is_open()) {
        msg_erro_arquivo();
        return vetorFilme;
    }
    Filme filme;
    std::string linha;
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
        if (aux == nomeFilme && tipo == "movie") {
            verificaGeneros(genero, subgenero, subsubgenero);      // Realiza as trocas de genero e subgenero se necessário
            std::vector<std::string> elenco;
            elenco = buscaElenco(id);
            std::string media;
            std::string numPessoas;
            buscaAvaliacao(id, media, numPessoas);
            int classificacao = stoi(adulto);
            int med = stod(media);
            int nMed = stod(numPessoas);
            filme.set(id,titulo, genero, subgenero, elenco, classificacao, med, nMed);
            vetorFilme.push_back(filme);
        }
    }
    arqInfoFilmes.close();
    return vetorFilme;
}

Filme buscaFilmeId(std::string &id, std::unordered_map<std::string, Filme> listaConhecidos){
    std::ifstream arqInfoFilmes("infoFilmes");
    if (!arqInfoFilmes.is_open()) {
        msg_erro_arquivo();
        return Filme();
    }
    auto it = listaConhecidos.find(id);
    if(it != listaConhecidos.end()){    // Filme já procurado anteriormente
        return listaConhecidos.at(id);
    }
    
    std::string linha;
    std::string aux, tipo, titulo, tituloOrig, adulto, genero, subgenero, subsubgenero, anoInicio, anoFim, duracao;
    while (std::getline(arqInfoFilmes, linha)){
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
            std::vector<std::string> elenco;
            elenco = buscaElenco(id);
            std::string media;
            std::string numPessoas;
            buscaAvaliacao(id, media, numPessoas);
            int classificacao = stoi(adulto);
            int med = stod(media);
            int nMed = stod(numPessoas);
            return Filme(id, titulo, genero, subgenero, elenco, classificacao, med, nMed);
        }
    }
    arqInfoFilmes.close();
    return Filme();
}

std::vector<std::string> buscaElenco(std::string &id){
    // Primeira parte: encontrar os ids dos atores do filme
    std::ifstream arqElenco("filmeAtores");
    std::vector<std::string> elenco;
    if (!arqElenco.is_open()) {
        msg_erro_arquivo();
        // elenco.push_back("-1");
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
        
        if (id == auxId_filme){                            // Encontrou o filme em questão
            if(posicao == "actor" || posicao == "actress"){  // Verifica se fez parte do elenco (não necessita grupo técnico)             
                // Verifica se algum ator já foi inserido (pode ter diferentes papéis no filme)
                if(std::find(ids.begin(), ids.end(), id_pessoa) == ids.end())
                    ids.push_back(id_pessoa);
            }
            filme_encontrado = true;
        }if(filme_encontrado== true && id != auxId_filme) // Já encontrou todos do elenco do filme
            break;
    }
    if(ids.empty()) return elenco;
    arqElenco.close();

    // Segunda parte: relacionar id com nome do ator
    std::ifstream arqNomesPessoas("nomesPessoas");
    if (!arqNomesPessoas.is_open()) {
        msg_erro_arquivo();
        return elenco;
    }
    
    std::string aux_nome;
    for (std::string& elem : ids){
        arqNomesPessoas.clear(); // Limpa flags de erro, como EOF
        arqNomesPessoas.seekg(0); // Reposiciona o ponteiro para o início do arquivo
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
    if (!arqAv.is_open()) {
        msg_erro_arquivo();
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
            break;
        }
    }
    arqAv.close();
    if(!filme_encontrado){
        mediaAv = "-1";
        numAv = "-1";
    }
}

void verificaGeneros(std::string &genero, std::string &subgenero, std::string &subsubgenero){
    if(classeGenero.find(genero) == classeGenero.end()){    // Se o gênero não tem uma classe própria
        if(classeGenero.find(subgenero) != classeGenero.end()){ // Segundo gênero tem uma classe própria
            std::string aux;
            aux = genero;
            genero = tradGenero[subgenero];
            subgenero = tradGenero[aux];
        }else if(classeGenero.find(subsubgenero) == classeGenero.end()){
            genero = tradGenero[subsubgenero]; // Não vamos trocar pois só utilizamos dois gêneros da lista
        }else{  // Não há nenhum gênero da lista que tenha classe própria
            genero = combGenero[genero];
        }
    }else{
        genero = tradGenero[genero];
        subgenero = tradGenero[subgenero];
    }
}

std::vector<Filme> recomendacaoAtor(std::string &nome, std::unordered_map<std::string, Filme> listaConhecidos){
    std::string linha;
    std::ifstream arqNomesPessoas("nomesPessoas");
    std::vector<Filme> filmes;
    if (!arqNomesPessoas.is_open()) {
        msg_erro_arquivo();
        return filmes;
    }
    std::string id_pessoa, aux_nome, nasc, morte, profissao, titulos;
    Filme f;
    std::string id_filme;
    while (std::getline(arqNomesPessoas, linha)) {
        std::istringstream ss(linha);
        // Lê os campos separados por tabulação
        std::getline(ss, id_pessoa, '\t');
        std::getline(ss, aux_nome, '\t');
        
        if (nome == aux_nome) {
            std::getline(ss, nasc, '\t');
            std::getline(ss, morte, '\t');
            std::getline(ss, profissao, '\t');
            std::getline(ss, titulos, '\t');

            std::regex separador("([^,]+)");

            // iterador que prercorre do início ao fim da string
            std::sregex_iterator it(titulos.begin(), titulos.end(), separador);
            std::sregex_iterator fim;

            while (it != fim) {
                id_filme = (*it).str();
                f = buscaFilmeId(id_filme, listaConhecidos);
                if(!(f.getId()).empty())
                    filmes.push_back(f);    // Filme encontrado
            }
            break;  // Já encontrou o ator e os seus filmes famosos
        }
    }
    arqNomesPessoas.close(); 
    return filmes;
}

struct Comparador {
    bool operator()(const Filme& f1, const Filme& f2) {
        return f1.getMediaBase() > f2.getMediaBase(); // Min-Heap: menor nota no topo
    }
};

std::vector<Filme> recomendaGenero(std::string genero, std::unordered_map<std::string, Filme> listaConhecidos){
    std::ifstream arqInfoFilmes("infoFilmes");
    std::vector<Filme> filmes;
    if (!arqInfoFilmes.is_open()) {
        msg_erro_arquivo();
        return filmes;
    }
    Filme f;
    // Utilizada para guardar os cinco filmes com maiores médias de avaliações
    std::priority_queue<Filme, std::vector<Filme>, Comparador> minHeap;
    std::string linha;
    std::string id, tipo, titulo, tituloOrig, adulto, aux_genero, subgenero, subsubgenero, anoInicio, anoFim, duracao;
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
        getline(ss, aux_genero, ',');
        getline(ss, subgenero, ',');
        getline(ss, subsubgenero, '\t');

        verificaGeneros(aux_genero, subgenero, subsubgenero);      // Realiza as trocas de genero e subgenero se necessário
        if (genero == aux_genero) {
            auto it = listaConhecidos.find(id);
            if (it != listaConhecidos.end()) {
                f = it->second; // Filme já conhecido
            } else {
                std::vector<std::string> elenco;
                elenco = buscaElenco(id);
                std::string media;
                std::string numPessoas;
                buscaAvaliacao(id, media, numPessoas);
                int classificacao = stoi(adulto);
                int med = stod(media);
                int nMed = stod(numPessoas);
                f.set(id,titulo, genero, subgenero, elenco, classificacao, med, nMed);
            }
            // Tenta inserir o filme no Min-Heap
            if (minHeap.size() < 5) {
                minHeap.push(f); // Insere diretamente se ainda não há 5 filmes
            } else if (f.getMediaBase() > minHeap.top().getMediaBase()) {
                minHeap.pop();    // Remove o menor filme
                minHeap.push(f);  // Insere o novo filme
            }
        }
    }
    arqInfoFilmes.close();

    while (!minHeap.empty()) {
        filmes.push_back(minHeap.top());
        minHeap.pop();
    }

    return filmes;
}