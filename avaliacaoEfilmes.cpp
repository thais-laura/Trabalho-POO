#include "avaliacaoEfilmes.hpp"

// Implementação da classe Avaliacao
Avaliacao::Avaliacao() : _idUsuario(""), _idFilme(""), _nomeFilme(""), _comentario(""), _nota(-1) {}

Avaliacao::Avaliacao(const std::string& idUsuario, const std::string& idFilme, 
                     const std::string& nomeFilme, float nota, const std::string& comentario)
    : _idUsuario(idUsuario), _idFilme(idFilme), _nomeFilme(nomeFilme), _nota(nota), _comentario(comentario) {}

void Avaliacao::set(const std::string& idUsuario, const std::string& idFilme, 
                    const std::string& nomeFilme, float nota, const std::string& comentario) {
    _idUsuario = idUsuario;
    _idFilme = idFilme;
    _nomeFilme = nomeFilme;
    _nota = nota;
    _comentario = comentario;
}

std::string Avaliacao::getIdUsuario() const { return _idUsuario; }
std::string Avaliacao::getIdFilme() const { return _idFilme; }
std::string Avaliacao::getNomeFilme() const { return _nomeFilme; }
float Avaliacao::getNota() const { return _nota; }
std::string Avaliacao::getComentario() const { return _comentario; }

nlohmann::json Avaliacao::toJSON() const {
    return {
        {"login", _idUsuario},
        {"id filme", _idFilme},
        {"nome", _nomeFilme},
        {"nota", _nota},
        {"comentario", _comentario}
    };
}

std::ostream& Avaliacao::operator<<(std::ostream& out, const Avaliacao& avaliacao) {
    out << "Filme: " << avaliacao._nomeFilme << " (ID: " << avaliacao._idFilme << ")\n";
    out << "Usuario: " << avaliacao._idUsuario << "\n";
    out << "Nota: " << avaliacao._nota << "/10\n";
    if (!avaliacao._comentario.empty()) {
        out << "Comentario: " << avaliacao._comentario << "\n";
    }
    return out;
}

// Função `recomendaGenero` declarada previamente
extern std::vector<Filme> Filme::recomendaGenero(std::string genero, std::unordered_map<std::string, Filme> listaConhecidos);

// Implementação da classe Filme
Filme::Filme() : _classificacao(false), _mediaBase(-1), _nMediasBase(0), _somaNotas(0), _ano(0), _duracao(0) {}

Filme::Filme(const std::string& id, const std::string& nome, const std::string& genero, const std::string& subgenero,
             const std::vector<std::string>& elenco, bool classificacao, float mediaBase, 
             int nMediasBase, int ano, int duracao)
    : _id(id), _nome(nome), _genero(genero), _subgenero(subgenero), _elenco(elenco), 
      _classificacao(classificacao), _mediaBase(mediaBase), _nMediasBase(nMediasBase), 
      _somaNotas(mediaBase * nMediasBase), _ano(ano), _duracao(duracao) {}

// Construtor com integração ao `recomendaGenero`
Filme::Filme(const std::string& genero, const std::unordered_map<std::string, Filme>& listaConhecidos) {
    auto recomendacoes = recomendaGenero(genero, listaConhecidos);

    if (!recomendacoes.empty()) {
        const Filme& filmeRecomendado = recomendacoes[0]; // Seleciona o filme com melhor avaliação
        *this = filmeRecomendado; // Copia os dados do filme recomendado
    } else {
        std::cerr << "Nenhuma recomendacao encontrada para o genero: " << genero << std::endl;
    }
}

void Filme::set(const std::string& id, const std::string& genero, const std::string& subgenero,
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
void Filme::adicionarOuAtualizar(const std::string& idUsuario, float nota, const std::string& comentario = "") {
    if (nota < 0 || nota > 10) {
        std::cerr << "Nota invalida. Deve estar entre 0 e 10.\n";
        return;
    }
    if (_avaliacoes.find(idUsuario) != _avaliacoes.end()) {  // Verifica se já existe uma avaliação para o usuário (ID do usuário)
        // Atualizar avaliação existente
        _somaNotas -= _avaliacoes[idUsuario].getNota(); // Remove a nota antiga da soma total
        _avaliacoes[idUsuario].set(idUsuario, _id, _genero, nota, comentario); // Atualiza os detalhes da avaliação existente
    } else {
        // Adicionar nova avaliação
        _avaliacoes[idUsuario] = Avaliacao(idUsuario, _id, _genero, nota, comentario); // Cria uma nova avaliação
        _nMediasBase++; // Incrementa o contador de avaliações
    }
    _somaNotas += nota; // Adiciona a nova nota à soma total das notas
    atualizarMedia(); // Atualiza a média de notas do filme
}

// Remover uma avaliação
void Filme::removerAvaliacao(const std::string& idUsuario) {
    if (_avaliacoes.find(idUsuario) != _avaliacoes.end()) { // Verifica se há uma avaliação existente para o ID do usuário fornecido
        _somaNotas -= _avaliacoes[idUsuario].getNota(); // Remove a nota correspondente da soma total de notas
        _avaliacoes.erase(idUsuario); // Remove a avaliação do mapa
        _nMediasBase--; // Decrementa o número de avaliações registradas
        atualizarMedia(); // Atualiza a média de notas do filme
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
        _mediaBase = _mediaBase; // Mantém o valor atual de _mediaBase se não houver avaliações
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
        float nota = avaliacao.getNota(); // Obtém a nota da avaliação
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

    std::cout << "Melhor avaliacao:\n";
    std::cout << "Usuario: " << melhorUsuario << "\n";
    std::cout << "Nota: " << melhorNota << "/10\n";
    std::cout << "Comentario: " << melhorComentario << "\n";
    std::cout << "--------------------------\n";
    std::cout << "Pior avaliacao:\n";
    std::cout << "Usuario: " << piorUsuario << "\n";
    std::cout << "Nota: " << piorNota << "/10\n";
    std::cout << "Comentario: " << piorComentario << "\n";
    std::cout << "--------------------------\n";
}

virtual Filme::~Filme() = default; // Virtual destructor para herança

virtual std::string Filme::descricao() const {
    return "Descricao do genero.";
}

// Sobrecarga do operador <<
friend std::ostream& Filme::operator<<(std::ostream& out, const Filme& filme) {
    out << "ID: " << filme._id << "\n";
    out << "Nome: " << filme._nome << "\n";
    out << "Genero: " << filme._genero << " (" << filme._subgenero << ")\n";
    out << "Ano: " << filme._ano << "\n";
    out << "Duracao: " << filme._duracao << " minutos\n";
    out << "Elenco: ";
    out << filme.descricao() << "\n";
    for (const auto& ator : filme._elenco) {
        out << ator << ", ";
    }
    out << "\nClassificacao: " << (filme._classificacao ? "Adulto" : "Nao Adulto") << "\n";
    out << "Media de avaliacoes: " << filme._mediaBase << "/10\n";
    out << "Avaliacoes:\n";
    for (const auto& [idUsuario, avaliacao] : filme._avaliacoes) {
        out << avaliacao;
    }
    return out;
}

nlohmann::json Filme::toJSON() const {
    std::vector<nlohmann::json> avaliacoesJson;
    for (auto [id, av] : _avaliacoes) {   // criar os modelos para salvar no arquivo a partir da sobrecarga
        avaliacoesJson.push_back(av.toJSON());
    }
    return {
        {"id", _id},
        {"titulo", _nome},
        {"elenco", _elenco},    // apesar de ser um vector, o json consegue manipular as std::string
        {"genero", _genero},
        {"subgenero", _subgenero},
        {"ano", _ano},
        {"duracao", _duracao},
        {"classificacao", _classificacao},
        {"media", _mediaBase},
        {"quantidade de avaliações", _nMediasBase},
        {"avaliacoes", avaliacoesJson} // por ser um vector de outra classe, é necessário sobrecarrgear em avaliacoes
    };
}

Acao(const std::string& id, const std::string& nome, const std::string& genero, const std::string& subgenero,
    const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao) {}

std::string descricao() const override {
    return "Os filmes de acao sao conhecidos por sua intensidade e energia. Desse modo, eles trazem sequencias emocionantes, 
    como por exemplo: lutas, perseguicoes, explosoes e muita adrenalina.";
}

Comedia(const std::string& id, const std::string& nome, const std::string& genero, const std::string& subgenero,
    const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao) {}

std::string descricao() const override {
    return "A comedia apresenta personagens engracados, situacoes absurdas e dialogos espirituosos. Com isso,
    esse genero busca entreter e proporcionar leveza ao publico.";
}

Animacao(const std::string& id, const std::string& nome, const std::string& genero, const std::string& subgenero,
    const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao) {}

std::string descricao() const override {
    return "Os filmes de animacao encantam a todos com historias cativantes e visuais impressionantes, que transportam para mundos
    magicos e ludicos. Podem ser criados com tecnicas como desenho a mao, modelagem 3D ou stop-motion.";
}

Terror(const std::string& id, const std::string& nome, const std::string& genero, const std::string& subgenero,
    const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao) {}

std::string descricao() const override {
    return "O terror explora uma atmosfera sombria e cheia de suspense. Utilizando de artificios para causar sustos, 
    trilhas sonoras arrepiantes e temas macabros, provocando fortes emocoes.";
}

Romance(const std::string& id, const std::string& nome, const std::string& genero, const std::string& subgenero,
    const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao) {}

std::string descricao() const override {
    return "Os filmes de romance celebram o amor e as conexoes emocionais. Assim, contam historias de relacionamentos, 
    dramas pessoais e momentos inesqueciveis.";
}

Suspense(const std::string& id, const std::string& nome, const std::string& genero, const std::string& subgenero,
    const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao) {}

std::string descricao() const override {
    return "O suspense prende a atencao com tramas inteligentes, misterios intrigantes e reviravoltas inesperadas. 
    Nesse sentido, desafia o publico a desvendar enigmas e a ficar atento em cada detalhe.";
}

Drama(const std::string& id, const std::string& nome, const std::string& genero, const std::string& subgenero,
    const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao) {}

std::string descricao() const override {
    return "O drama explora narrativas intensas e emocionais, baseadas em conflitos humanos, dilemas morais ou
    desafios da vida. Com historias profundas e reflexivas, esse genero conecta o publico as complexidades das relacoes.";
}


