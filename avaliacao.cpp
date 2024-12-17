#include "./avaliacao.hpp"

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

std::ostream& operator<<(std::ostream& out, const Avaliacao& avaliacao) {
    out << "Filme: " << avaliacao._nomeFilme << " (ID: " << avaliacao._idFilme << ")\n";
    out << "Usuario: " << avaliacao._idUsuario << "\n";
    out << "Nota: " << avaliacao._nota << "/10\n";
    if (!avaliacao._comentario.empty()) {
        out << "Comentario: " << avaliacao._comentario << "\n";
    }
    return out;
}