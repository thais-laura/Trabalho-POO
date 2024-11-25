#include <iostream>
#include <string>
#include <map>
#include <vector>

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

class Filme { // COLOCAR O ANO, DURACAO
private:
    std::string _id;                 // ID único do filme
    std::string _genero;             // Gênero principal
    std::string _subgenero;          // Subgênero
    std::vector<std::string> _elenco; // Vetor de strings para elenco
    bool _classificacao;            // Classificação indicativa: 1 = adulto, 0 = não adulto
    float _mediaBase;                // Média 
    int _nMediasBase;                // Número de avaliações na base de dados
    float _somaNotas;                // Soma total das notas para eficiência do código, pois em vez de recalcular a soma 
    //de todas as notas toda vez que a média é atualizada, armazenamos a soma para poder adicionar ou subtrair valores conforme necessário
    std::map<std::string, Avaliacao> _avaliacoes; // Mapa para avaliações, chave é o ID do usuário

public:
    // Construtor vazio , TERMINAR DE INICIALIZAR
    Filme() : _classificacao(false), _mediaBase(-1), _nMediasBase(0), _somaNotas(0) {}

    // Construtor completo
    Filme(const std::string& id, const std::string& genero, const std::string& subgenero,
          const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase)
        : _id(id), _genero(genero), _subgenero(subgenero), _elenco(elenco),
          _classificacao(classificacao), _mediaBase(mediaBase), _nMediasBase(nMediasBase), _somaNotas(mediaBase * nMediasBase) {}

    void set(const std::string& id, const std::string& genero, const std::string& subgenero,
             const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase) {
        _id = id;
        _genero = genero;
        _subgenero = subgenero;
        _elenco = elenco;
        _classificacao = classificacao;
        _mediaBase = mediaBase;
        _nMediasBase = nMediasBase;
        _somaNotas = mediaBase * nMediasBase;
    }

    std::string getId() const { return _id; }
    std::string getGenero() const { return _genero; }
    std::string getSubgenero() const { return _subgenero; }
    std::vector<std::string> getElenco() const { return _elenco; }
    bool getClassificacao() const { return _classificacao; }
    float getMediaBase() const { return _mediaBase; }
    int getNMediasBase() const { return _nMediasBase; }

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

    // Sobrecarga do operador <<
    friend std::ostream& operator<<(std::ostream& out, const Filme& filme) {
        out << "ID: " << filme._id << "\n";
        out << "Genero: " << filme._genero << " (" << filme._subgenero << ")\n";
        out << "Elenco: ";
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
};