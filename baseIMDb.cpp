#include "./baseIMDb.hpp"
// "This [website, program, service, application, product] uses TMDB and the TMDB APIs but is not endorsed, certified, or otherwise approved by TMDB."

int main(){
    std::vector<Filme> filme = buscaFilme("Cars 2");
}
std::string converteMinusculo(std::string &str){
    std::string min;
    for(int i=0; i< str.size(); i++){
        min += tolower(str.at(i));
    }
    return min;
}

std::vector<Filme> buscaFilme(std::string nomeFilme){
    nomeFilme = converteMinusculo(nomeFilme);
    std::ifstream arqInfoFilmes("infoFilmes");
    std::vector<Filme> vetorFilme;
    if (!arqInfoFilmes.is_open()) {
        msg_erro_arquivo();
        return vetorFilme;
    }
    Filme filme;
    std::string linha;
    bool filme_encontrado = false;
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
            // SET FILME
           // vetorFilme.push_back(filme);

            std::cout << "ID: " << id << "\n"
                << "Tipo: " << tipo << "\n"
                << "Título: " << titulo << "\n"
                << "Título Original: " << tituloOrig << "\n"
                << "Adulto: " << adulto << "\n"
                << "Ano Início: " << anoInicio << "\n"
                << "Ano Fim: " << anoFim << "\n"
                << "Duração: " << duracao << "\n"
                << "Gênero: " << genero << "\n"
                << "Subgênero: " << subgenero << "\n";
            std::cout << "Elenco: \n";
            for(std::string ator : elenco){
                std::cout << ator << std::endl;
            } 
            std::cout << "Media: " << media << "\nNúmero de avaliações: " << numPessoas << "\n\n";
            //break;
        }
    }
    arqInfoFilmes.close();
    return vetorFilme;
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
