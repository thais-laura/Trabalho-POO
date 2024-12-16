#ifndef AVALIACAO_E_FILMES_HPP
#define AVALIACAO_E_FILMES_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <json_fwd.hpp>

// Classe Avaliacao
class Avaliacao {
private:
    std::string _idUsuario;
    std::string _idFilme;
    std::string _nomeFilme;
    std::string _comentario;
    float _nota;

public:
    Avaliacao();
    Avaliacao(const std::string& idUsuario, const std::string& idFilme, 
              const std::string& nomeFilme, float nota, const std::string& comentario = "");

    void set(const std::string& idUsuario, const std::string& idFilme, 
             const std::string& nomeFilme, float nota, const std::string& comentario = "");

    std::string getIdUsuario() const;
    std::string getIdFilme() const;
    std::string getNomeFilme() const;
    float getNota() const;
    std::string getComentario() const;

    nlohmann::json toJSON() const;

    friend std::ostream& operator<<(std::ostream& out, const Avaliacao& avaliacao);
};

// Função `recomendaGenero` declarada previamente
extern std::vector<Filme> recomendaGenero(std::string genero, std::unordered_map<std::string, Filme> listaConhecidos);

// Classe Filme
class Filme {
protected:
    std::string _id;
    std::string _nome;
    std::string _genero;
    std::string _subgenero;
    std::vector<std::string> _elenco;
    bool _classificacao;
    int _ano;
    int _duracao;
    float _mediaBase;
    int _nMediasBase;
    float _somaNotas;
    std::unordered_map<std::string, Avaliacao> _avaliacoes; 

public:
    Filme();
    Filme(const std::string& id, const std::string& nome, const std::string& genero, const std::string& subgenero,
          const std::vector<std::string>& elenco, bool classificacao, float mediaBase,
          int nMediasBase, int ano, int duracao);

   // Construtor com integração ao `recomendaGenero`
    Filme(const std::string& genero, const std::unordered_map<std::string, Filme>& listaConhecidos);

    void set(const std::string& id, const std::string& nome, const std::string& genero, const std::string& subgenero,
             const std::vector<std::string>& elenco, bool classificacao, float mediaBase,
             int nMediasBase, int ano, int duracao);

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
    // Sobrecarga de operadores
    friend std::ostream& operator<<(std::ostream& out, const Filme& filme);

    // Métodos de serialização e descrição
    nlohmann::json toJSON() const;
    
};

// Classes específicas para gêneros
class Acao : public Filme {
public:
    Acao(const std::string& id, const std::string& nome, const std::string& genero, const std::string& subgenero,
          const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao);
    std::string descricao() const override;
};

class Comedia : public Filme {
public:
    Comedia(const std::string& id, const std::string& nome, const std::string& genero, const std::string& subgenero,
          const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao);
    std::string descricao() const override;
};

class Animacao : public Filme {
public:
    Animacao(const std::string& id, const std::string& nome, const std::string& genero, const std::string& subgenero,
          const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao);
    std::string descricao() const override;
};

class Terror : public Filme {
public:
    Terror(const std::string& id, const std::string& nome, const std::string& genero, const std::string& subgenero,
          const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao);
    std::string descricao() const override;
};

class Romance : public Filme {
public:
    Romance(const std::string& id, const std::string& nome, const std::string& genero, const std::string& subgenero,
          const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao);
    std::string descricao() const override;
};

class Suspense : public Filme {
public:
    Suspense(const std::string& id, const std::string& nome, const std::string& genero, const std::string& subgenero,
          const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao);
    std::string descricao() const override;
};

class Drama : public Filme {
public:
    Drama(const std::string& id, const std::string& nome, const std::string& genero, const std::string& subgenero,
          const std::vector<std::string>& elenco, bool classificacao, float mediaBase, int nMediasBase, int ano, int duracao);
    std::string descricao() const override;
};

#endif // AVALIACAO_E_FILMES_HPP