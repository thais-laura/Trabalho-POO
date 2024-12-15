#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "./avaliacaoEfilmes.hpp"

class Avaliacao {
private:
    std::string _idUsuario;    // ID do usuário que fez a avaliação
    std::string _idFilme;      // ID do filme avaliado
    std::string _nomeFilme;    // Nome do filme avaliado
    std::string _comentario;   // Comentário feito pelo usuário
    float _nota;               // Nota dada pelo usuário

public:
    // Construtor vazio, para quando não houver avaliação
    Avaliacao() : _idUsuario(""), _idFilme(""), _nomeFilme(""), _comentario(""), _nota(-1) {}

    // Construtor completo
    Avaliacao(const std::string& idUsuario, const std::string& idFilme, 
              const std::string& nomeFilme, float nota, const std::string& comentario = "")
        : _idUsuario(idUsuario), _idFilme(idFilme), _nomeFilme(nomeFilme), 
          _nota(nota), _comentario(comentario) {}

    void set(const std::string& idUsuario, const std::string& idFilme, 
             const std::string& nomeFilme, float nota, const std::string& comentario = "") {
        _idUsuario = idUsuario;
        _idFilme = idFilme;
        _nomeFilme = nomeFilme;
        _nota = nota;
        _comentario = comentario;
    }

    std::string getIdUsuario() const { 
        return _idUsuario; 
    }
    std::string getIdFilme() const { 
        return _idFilme; 
    }
    std::string getNomeFilme() const { 
        return _nomeFilme; 
    }
    float getNota() const { 
        return _nota; 
    }
    std::string getComentario() const { 
        return _comentario; 
    }

    nlohmann::json toJSON() const {
        return {
            {"login", _idUsuario},
            {"id filme", _idFilme},
            {"nome", _nomeFilme},
            {"nota", _nota},
            {"comentario", _comentario} // por ser um vector de outra classe, é necessário sobrecarrgear em avaliacoes
        };
    }

    // Friend para exibição
    friend std::ostream& operator<<(std::ostream& out, const Avaliacao& avaliacao) {
        out << "Filme: " << avaliacao._nomeFilme << " (ID: " << avaliacao._idFilme << ")\n";
        out << "Usuario: " << avaliacao._idUsuario << "\n";
        out << "Nota: " << avaliacao._nota << "/10\n";
        if (!avaliacao._comentario.empty()) {
            out << "Comentario: " << avaliacao._comentario << "\n";
        }
        out << "--------------------------\n";
        return out;
    }
};

// Função `recomendaGenero` declarada previamente
extern std::vector<Filme> recomendaGenero(std::string genero, std::unordered_map<std::string, Filme> listaConhecidos);

class Filme { 
private:
    std::string _id;                 // ID único do filme
    std::string _genero;             // Gênero principal
    std::string _subgenero;          // Subgênero
    std::vector<std::string> _elenco; // Vetor de strings para elenco
    bool _classificacao;            // Classificação indicativa: 1 = adulto, 0 = não adulto
    int _ano;                        // Ano de lançamento do filme
    int _duracao;                    // Duração do filme em minutos
    float _mediaBase;                // Média 
    int _nMediasBase;                // Número de avaliações na base de dados
    float _somaNotas;                // Soma total das notas para eficiência do código, pois em vez de recalcular a soma 
    //de todas as notas toda vez que a média é atualizada, armazenamos a soma para poder adicionar ou subtrair valores conforme necessário
    std::unordered_map<std::string, Avaliacao> _avaliacoes; 

public:
    // Construtor vazio 
    Filme() : _classificacao(false), _mediaBase(-1), _nMediasBase(0), _somaNotas(0), _ano(0), _duracao(0) {}

    // Construtor completo
    Filme(const std::string& id, const std::string& genero, const std::string& subgenero,
          const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao)
        : _id(id), _genero(genero), _subgenero(subgenero), _elenco(elenco),
          _classificacao(classificacao), _mediaBase(mediaBase), _nMediasBase(nMediasBase), _somaNotas(mediaBase * nMediasBase), _ano(ano), _duracao(duracao) {}

    // Construtor com integração ao `recomendaGenero`
    Filme(const std::string& genero, const std::unordered_map<std::string, Filme>& listaConhecidos) {
        auto recomendacoes = recomendaGenero(genero, listaConhecidos);

        if (!recomendacoes.empty()) {
            const Filme& filmeRecomendado = recomendacoes[0]; // Seleciona o filme com melhor avaliação
            *this = filmeRecomendado; // Copia os dados do filme recomendado
        } else {
            std::cerr << "Nenhuma recomendacao encontrada para o genero: " << genero << std::endl;
        }
    }

    void set(const std::string& id, const std::string& genero, const std::string& subgenero,
             const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao) {
        _id = id;
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

    std::string getId() const { return _id; }
    std::string getGenero() const { return _genero; }
    std::string getSubgenero() const { return _subgenero; }
    std::vector<std::string> getElenco() const { return _elenco; }
    bool getClassificacao() const { return _classificacao; }
    float getMediaBase() const { return _mediaBase; }
    int getNMediasBase() const { return _nMediasBase; }
    int getAno() const { return _ano; }
    int getDuracao() const { return _duracao; }

    // Adicionar ou atualizar uma avaliação
    void adicionarOuAtualizar(const std::string& idUsuario, float nota, const std::string& comentario = "") {
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
    void removerAvaliacao(const std::string& idUsuario) {
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
    void atualizarMedia() {
        if (_nMediasBase > 0) {
            _mediaBase = _somaNotas / _nMediasBase; // Calcula a média dividindo a soma total das notas pelo número de avaliações
        } else {
            _mediaBase = _mediaBase; // Mantém o valor atual de _mediaBase se não houver avaliações
        }
    }


    // Mostrar melhor e pior avaliação
    void mostrarMelhorEPior() const {
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

    virtual ~Filme() = default; // Virtual destructor para herança

    virtual std::string descricao() const {
        return "Descricao generica de um filme.";
    }

    // Sobrecarga do operador <<
    friend std::ostream& operator<<(std::ostream& out, const Filme& filme) {
        out << "ID: " << filme._id << "\n";
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

    nlohmann::json toJSON() const {
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

};

// Classes derivadas para gêneros específicos com texto explicativo
class Acao : public Filme {
public:
    Acao(const std::string& id, const std::vector<std::string>& elenco, int ano, int duracao)
        : Filme(id, "acao", "", elenco, true, 0, 0, ano, duracao) {}

    std::string descricao() const override {
        return "Os filmes de acao sao conhecidos por sua intensidade e energia. Desse modo, eles trazem sequencias emocionantes, 
        como por exemplo: lutas, perseguicoes, explosoes e muita adrenalina.";
    }
};

class Comedia : public Filme {
public:
    Comedia(const std::string& id, const std::vector<std::string>& elenco, int ano, int duracao)
        : Filme(id, "comedia", "", elenco, false, 0, 0, ano, duracao) {}

    std::string descricao() const override {
        return "A comedia apresenta personagens engracados, situacoes absurdas e dialogos espirituosos. Com isso,
        esse genero busca entreter e proporcionar leveza ao publico.";
    }
};

class Animacao : public Filme {
public:
    Animacao(const std::string& id, const std::vector<std::string>& elenco, int ano, int duracao)
        : Filme(id, "animacao", "", elenco, false, 0, 0, ano, duracao) {}

    std::string descricao() const override {
        return "Os filmes de animacao encantam a todos com historias cativantes e visuais impressionantes, que transportam para mundos
        magicos e ludicos. Podem ser criados com tecnicas como desenho a mao, modelagem 3D ou stop-motion.";
    }
};

class Terror : public Filme {
public:
    Terror(const std::string& id, const std::vector<std::string>& elenco, int ano, int duracao)
        : Filme(id, "terror", "", elenco, true, 0, 0, ano, duracao) {}

    std::string descricao() const override {
        return "O terror explora uma atmosfera sombria e cheia de suspense. Utilizando de artificios para causar sustos, 
        trilhas sonoras arrepiantes e temas macabros, provocando fortes emocoes.";
    }
};

class Romance : public Filme {
public:
    Romance(const std::string& id, const std::vector<std::string>& elenco, int ano, int duracao)
        : Filme(id, "romance", "", elenco, false, 0, 0, ano, duracao) {}

    std::string descricao() const override {
        return "Os filmes de romance celebram o amor e as conexoes emocionais. Assim, contam historias de relacionamentos, 
        dramas pessoais e momentos inesqueciveis.";
    }
};

class Suspense : public Filme {
public:
    Suspense(const std::string& id, const std::vector<std::string>& elenco, int ano, int duracao)
        : Filme(id, "suspense", "", elenco, true, 0, 0, ano, duracao) {}

    std::string descricao() const override {
        return "O suspense prende a atencao com tramas inteligentes, misterios intrigantes e reviravoltas inesperadas. 
        Nesse sentido, desafia o publico a desvendar enigmas e a ficar atento em cada detalhe.";
    }
};

class Drama : public Filme {
public:
    Drama(const std::string& id, const std::vector<std::string>& elenco, int ano, int duracao)
        : Filme(id, "drama", "", elenco, false, 0, 0, ano, duracao) {}

    std::string descricao() const override {
        return "O drama explora narrativas intensas e emocionais, baseadas em conflitos humanos, dilemas morais ou
        desafios da vida. Com historias profundas e reflexivas, esse genero conecta o publico as complexidades das relacoes.";
    }
};



int main() {
    // Criação de um filme
    Filme filme(
        "F001",                       // ID do filme
        "Acao",                       // Gênero
        "Aventura",                   // Subgênero
        {"Ator 1", "Ator 2"},         // Elenco
        true,                         // Classificação (1 = adulto)
        8.0,                          // Média inicial da base de dados
        2                             // Número de avaliações na base
    );

    // Adicionando algumas avaliações
    filme.adicionarOuAtualizar("U001", 9.5, "Excelente filme, cheio de acao");
    filme.adicionarOuAtualizar("U002", 7.0, "Gostei, mas poderia ter mais enredo");
    filme.adicionarOuAtualizar("U003", 8.5, "Otimos efeitos especiais");

    // Atualizando uma avaliação existente
    filme.adicionarOuAtualizar("U002", 8.0, "Reassistindo, percebi mais detalhes");

    // Removendo uma avaliação
    filme.removerAvaliacao("U001");

    // Exibindo as informações do filme
    std::cout << "Informacoes do filme:\n";
    std::cout << filme;

    // Mostrando a melhor e a pior avaliação
    std::cout << "\nMelhor e Pior Avaliacao:\n";
    filme.mostrarMelhorEPior();

    return 0;
}


