#ifndef FILME_HPP
#define FILME_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "./json.hpp"
#include "./json_fwd.hpp"
#include "./avaliacao.hpp"
// #include "./baseIMDb.hpp"

// Classe Filme
class Filme {
private:
    std::string _id;                 // ID único do filme
    std::string _nome;              // Nome
    std::string _genero;             // Gênero principal
    std::string _subgenero;          // Subgênero
    std::vector<std::string> _elenco; // Vetor de strings para elenco
    bool _classificacao;            // Classificação indicativa: 1 = adulto, 0 = não adulto
    float _mediaBase;                // Média 
    int _nMediasBase;                // Número de avaliações na base de dados
    float _somaNotas;                // Soma total das notas para eficiência do código, pois em vez de recalcular a soma 
    int _ano;                        // Ano de lançamento do filme
    int _duracao;                    // Duração do filme em minutos
    //de todas as notas toda vez que a média é atualizada, armazenamos a soma para poder adicionar ou subtrair valores conforme necessário
    std::unordered_map<std::string, Avaliacao> _avaliacoes; 

public:
    Filme();
    explicit Filme(const std::string& genero);
    Filme(const std::string& id, const std::string& nome, const std::string& genero, const std::string& subgenero,
          const std::vector<std::string>& elenco, bool classificacao, float mediaBase,
          int nMediasBase, int ano, int duracao);


    void set(const std::string& id, const std::string& nome, const std::string& genero, const std::string& subgenero,
             const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao);

    std::string getId() const;
    std::string getNome() const;
    std::string getGenero() const;
    std::string getSubgenero() const;
    std::vector<std::string> getElenco() const;
    bool getClassificacao() const;
    float getMediaBase() const;
    int getNMediasBase() const;
    int getAno() const;
    int getDuracao() const;

    void adicionarOuAtualizar(const std::string& idUsuario, float nota, const std::string& comentario = "");
    void removerAvaliacao(const std::string& idUsuario);
    void atualizarMedia();
    void mostrarMelhorEPior() const;

    // Destrutor virtual
    virtual ~Filme() = default;
    virtual std::string descricao() const;
    virtual void imprimeInfo(std::ostream& out) const;
    friend std::ostream& operator<<(std::ostream& out, const Filme& filme);

    // Métodos de serialização e descrição
    nlohmann::json toJSON() const;
    
};

// Classes específicas para gêneros
class Acao : public Filme {
public:
    Acao();
    Acao(const std::string& id, const std::string& nome, const std::string& subgenero,
          const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao);
    std::string descricao() const override;
};

class Comedia : public Filme {
public:
    Comedia();
    Comedia(const std::string& id, const std::string& nome, const std::string& subgenero,
          const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao);
    std::string descricao() const override;
};

class Animacao : public Filme {
public:
    Animacao();
    Animacao(const std::string& id, const std::string& nome, const std::string& subgenero,
          const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao);
    std::string descricao() const override;
};

class Terror : public Filme {
public:
    Terror();
    Terror(const std::string& id, const std::string& nome, const std::string& subgenero,
          const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao);
    std::string descricao() const override;
};

class Romance : public Filme {
public:
    Romance();
    Romance(const std::string& id, const std::string& nome, const std::string& subgenero,
          const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao);
    std::string descricao() const override;
};

class Suspense : public Filme {
public:
    Suspense();
    Suspense(const std::string& id, const std::string& nome, const std::string& subgenero,
          const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao);
    std::string descricao() const override;
};

class Drama : public Filme {
public:
    Drama();
    Drama(const std::string& id, const std::string& nome, const std::string& subgenero,
          const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao);
    std::string descricao() const override;
};

std::shared_ptr<Filme> criarFilmePorGenero(const std::string& genero);


#endif // AVALIACAO_E_FILMES_HPP