#include "./filme.hpp"
#include "./avaliacao.hpp"

// Implementação da classe Filme
Filme::Filme() : _classificacao(false), _mediaBase(-1), _nMediasBase(0), _somaNotas(0), _ano(0), _duracao(0) {}

Filme::Filme(const std::string& genero) : _genero(genero), _classificacao(false), _mediaBase(-1), _nMediasBase(0), _somaNotas(0), _ano(0), _duracao(0) {}

Filme::Filme(const std::string& id, const std::string& nome, const std::string& genero, const std::string& subgenero,
             const std::vector<std::string>& elenco, bool classificacao, float mediaBase, 
             int nMediasBase, int ano, int duracao)
    : _id(id), _nome(nome), _genero(genero), _subgenero(subgenero), _elenco(elenco), 
      _classificacao(classificacao), _mediaBase(mediaBase), _nMediasBase(nMediasBase), 
      _somaNotas(mediaBase * nMediasBase), _ano(ano), _duracao(duracao) {}

void Filme::set(const std::string& id, const std::string& nome, const std::string& genero, const std::string& subgenero,
    const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao) {
    _id = id;
    _nome = nome;
    _genero = genero;
    _subgenero = subgenero;
    _elenco = elenco;
    _classificacao = classificacao;
    _mediaBase = mediaBase;
    _nMediasBase = nMediasBase;
    _somaNotas = mediaBase * nMediasBase;
    _ano = ano;
    _duracao = duracao;
}

std::string Filme::getId() const { return _id; }
std::string Filme::getNome() const { return _nome; }
std::string Filme::getGenero() const { return _genero; }
std::string Filme::getSubgenero() const { return _subgenero; }
std::vector<std::string> Filme::getElenco() const { return _elenco; }
bool Filme::getClassificacao() const { return _classificacao; }
float Filme::getMediaBase() const { return _mediaBase; }
int Filme::getNMediasBase() const { return _nMediasBase; }
int Filme::getAno() const { return _ano; }
int Filme::getDuracao() const { return _duracao; }

// Adicionar ou atualizar uma avaliação
void Filme::adicionarOuAtualizar(const std::string& idUsuario, float nota, const std::string& comentario) {
    if (nota < 0 || nota > 10) {
        std::cout << "Nota invalida. Deve estar entre 0 e 10.\n";
        return;
    }
    if (_avaliacoes.find(idUsuario) != _avaliacoes.end()) {  // Verifica se já existe uma avaliação para o usuário (ID do usuário)
        // Atualizar avaliação existente
        _somaNotas -= _avaliacoes[idUsuario].getNota();                         
        _avaliacoes[idUsuario].set(idUsuario, _id, _nome, nota, comentario); 
    } else {
        // Adicionar nova avaliação
        _avaliacoes[idUsuario] = Avaliacao(idUsuario, _id, _nome, nota, comentario); 
        _nMediasBase++; 
    }
    _somaNotas += nota;                                                     
    atualizarMedia();                                                       // Atualiza a média de notas do filme
}

// Remover uma avaliação
void Filme::removerAvaliacao(const std::string& idUsuario) {
    if (_avaliacoes.find(idUsuario) != _avaliacoes.end()) { // Verifica se há uma avaliação existente para o ID do usuário fornecido
        _somaNotas -= _avaliacoes[idUsuario].getNota();     // Remove a nota correspondente da soma total de notas
        _avaliacoes.erase(idUsuario);                       // Remove a avaliação do mapa
        _nMediasBase--;                                     // Decrementa o número de avaliações registradas
        atualizarMedia();                                   // Atualiza a média de notas do filme
        std::cout << "Avaliacao removida com sucesso.\n";
    } else {
        std::cout << "Nenhuma avaliacao encontrada para este usuario.\n";
    }
}

// Atualizar a média (modifica _mediaBase)
void Filme::atualizarMedia() {
    if (_nMediasBase > 0) {
        _mediaBase = _somaNotas / _nMediasBase; // Calcula a média dividindo a soma total das notas pelo número de avaliações
    } else {
        _mediaBase = _mediaBase;                // Mantém o valor atual de _mediaBase se não houver avaliações
    }
}

// Mostrar melhor e pior avaliação
void Filme::mostrarMelhorEPior() const {
    if (_avaliacoes.empty()) {     // Verifica se há avaliações registradas
        std::cout << "Sem avaliacoes disponiveis.\n";
        return;
    }
    // Inicializa as variáveis para verificar as melhores e piores avaliações
    float melhorNota = -1, piorNota = 11;
    std::string melhorUsuario, piorUsuario, melhorComentario, piorComentario; // Para armazenar os usuários das melhores e piores avaliações/comentários

    for (const auto& [idUsuario, avaliacao] : _avaliacoes) {  // Percorre todas as avaliações registradas no mapa
        float nota = avaliacao.getNota(); 
        if (nota > melhorNota) { // Atualiza a melhor avaliação encontrada
            melhorNota = nota;
            melhorUsuario = avaliacao.getIdUsuario();
            melhorComentario = avaliacao.getComentario();
        }
        if (nota < piorNota) { // Atualiza a pior avaliação encontrada
            piorNota = nota;
            piorUsuario = avaliacao.getIdUsuario();
            piorComentario = avaliacao.getComentario();
        }
    }
    std::cout << "\n>>> Melhor avaliacao \n";
    std::cout << "Usuario: " << melhorUsuario << "\n";
    std::cout << "Nota: " << melhorNota << "/10\n";
    std::cout << "Comentario: " << melhorComentario << "\n";
    std::cout << "------------------------------------\n";
    std::cout << "\n>>> Pior avaliacao \n";
    std::cout << "Usuario: " << piorUsuario << "\n";
    std::cout << "Nota: " << piorNota << "/10\n";
    std::cout << "Comentario: " << piorComentario << "\n\n";
}

std::string Filme::descricao() const {
    return "Descricao do genero.";
}

void Filme::imprimeInfo(std::ostream& out) const {
    out << "ID: " << _id << "\n";
    out << "Nome: " << _nome << "\n";
    out << "Genero: " << _genero << " (" << _subgenero << ")\n";
    out << "Ano: " << _ano << "\n";
    out << "Duracao: " << _duracao << " minutos\n";
    out << descricao() << "\n";                     // Chama a descrição dinâmica
    out << "Elenco: ";
    for (const auto& ator : _elenco) {
        out << ator << ", ";
    }
    out << "\nClassificacao: " << (_classificacao ? "Adulto" : "Nao Adulto") << "\n";
    out << "Media de avaliacoes: " << _mediaBase << "/10\n";
    out << "Avaliacoes:\n";
    for (const auto& [idUsuario, avaliacao] : _avaliacoes) {
        out << avaliacao;
    }
}

// Sobrecarga do operador <<
std::ostream& operator<<(std::ostream& out, const Filme& filme) {
    filme.imprimeInfo(out);  // Chama o método polimórfico
    return out;
}

nlohmann::json Filme::toJSON() const {
    std::vector<nlohmann::json> avaliacoesJson;
    for (auto [id, av] : _avaliacoes) {         // Criar os modelos para salvar no arquivo a partir da sobrecarga
        avaliacoesJson.push_back(av.toJSON());
    }
    return {
        {"id", _id},
        {"titulo", _nome},
        {"elenco", _elenco},                    // Apesar de ser um vector, o json consegue manipular as std::string
        {"genero", _genero},
        {"subgenero", _subgenero},
        {"ano", _ano},
        {"duracao", _duracao},
        {"classificacao", _classificacao},
        {"media", _mediaBase},
        {"quantidade de avaliações", _nMediasBase},
        {"avaliacoes", avaliacoesJson}          // Por ser um vector de outra classe, é necessário sobrecarrgear em avaliacoes
    };
}

Acao::Acao() : Filme("Ação"){}
Acao::Acao(const std::string& id, const std::string& nome, const std::string& subgenero,
    const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao) {
    Filme(id, nome, "Ação", subgenero, elenco, classificacao, mediaBase, nMediasBase, ano, duracao); }
std::string Acao::descricao() const {
    return "Os filmes de acao sao conhecidos por sua intensidade e energia. Desse modo, eles trazem sequencias emocionantes, como por exemplo: lutas, perseguicoes, explosoes e muita adrenalina.";
}

Comedia::Comedia() : Filme("Comédia"){}
Comedia::Comedia(const std::string& id, const std::string& nome, const std::string& subgenero,
    const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao) {        
        Filme(id, nome, "Comédia", subgenero, elenco, classificacao, mediaBase, nMediasBase, ano, duracao); }
std::string Comedia::descricao() const {
    return "A comedia apresenta personagens engracados, situacoes absurdas e dialogos espirituosos. Com isso, esse genero busca entreter e proporcionar leveza ao publico.";
}

Animacao::Animacao() : Filme("Animação"){}
Animacao::Animacao(const std::string& id, const std::string& nome, const std::string& subgenero,
    const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao) {       
        Filme(id, nome, "Animação", subgenero, elenco, classificacao, mediaBase, nMediasBase, ano, duracao); }
std::string Animacao::descricao() const {
    return "Os filmes de animacao encantam a todos com historias cativantes e visuais impressionantes, que transportam para mundos magicos e ludicos. Podem ser criados com tecnicas como desenho a mao, modelagem 3D ou stop-motion.";
}

Terror::Terror() : Filme("Terror"){}
Terror::Terror(const std::string& id, const std::string& nome,  const std::string& subgenero,
    const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao) {
        Filme(id, nome, "Terror", subgenero, elenco, classificacao, mediaBase, nMediasBase, ano, duracao); }
std::string Terror::descricao() const {
    return "O terror explora uma atmosfera sombria e cheia de suspense. Utilizando de artificios para causar sustos, trilhas sonoras arrepiantes e temas macabros, provocando fortes emocoes.";
}

Romance::Romance() : Filme("Romance"){}
Romance::Romance(const std::string& id, const std::string& nome,  const std::string& subgenero,
    const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao) { 
        Filme(id, nome, "Romance", subgenero, elenco, classificacao, mediaBase, nMediasBase, ano, duracao); }
std::string Romance::descricao() const {
    return "Os filmes de romance celebram o amor e as conexoes emocionais. Assim, contam historias de relacionamentos, dramas pessoais e momentos inesqueciveis.";
}

Suspense::Suspense() : Filme("Suspense"){}
Suspense::Suspense(const std::string& id, const std::string& nome, const std::string& subgenero,
const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao) {
            Filme(id, nome, "Suspense", subgenero, elenco, classificacao, mediaBase, nMediasBase, ano, duracao);
}
std::string Suspense::descricao() const {
    return "O suspense prende a atencao com tramas inteligentes, misterios intrigantes e reviravoltas inesperadas. Nesse sentido, desafia o publico a desvendar enigmas e a ficar atento em cada detalhe.";
}

Drama::Drama() : Filme("Drama"){}
Drama::Drama(const std::string& id, const std::string& nome, const std::string& subgenero,
    const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao)  {
        Filme(id, nome, "Drama", subgenero, elenco, classificacao, mediaBase, nMediasBase, ano, duracao); }
std::string Drama::descricao() const {
    return "O drama explora narrativas intensas e emocionais, baseadas em conflitos humanos, dilemas morais ou desafios da vida. Com historias profundas e reflexivas, esse genero conecta o publico as complexidades das relacoes.";
}

// Fábrica de ponteiro para classe derivada
std::shared_ptr<Filme> criarFilmePorGenero(const std::string& genero) {
    if (genero == "Ação") {
        return std::make_shared<Acao>();
    } else if (genero == "Comédia") {
        return std::make_shared<Comedia>();
    } else if (genero == "Animação") {
        return std::make_shared<Animacao>();
    } else if (genero == "Terror") {
        return std::make_shared<Terror>();
    } else if (genero == "Romance") {
        return std::make_shared<Romance>();
    } else if (genero == "Suspense") {
        return std::make_shared<Suspense>();
    } else if (genero == "Drama" || genero == "drama") {
        return std::make_shared<Drama>();
    } else {
        return std::make_shared<Filme>(); // Gênero genérico
    }
}