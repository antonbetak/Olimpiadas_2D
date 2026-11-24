#pragma once
#include "Atleta.h"
#include "ControlTeclas.h"
#include <memory>
#include <vector>

enum class Disciplina { Atletismo, Natacion };
class Competencia {
private:
    Disciplina disciplina_;
    float meta_;
    float tiempo_ = 0;
    int aciertos_ = 0;
    int errores_ = 0;
    bool finalizada_ = false;
    std::vector<std::unique_ptr<Atleta>> atletas_;
    std::vector<float> velocidadesIA_;
    ControlTeclas control_;
public:
    explicit Competencia(Disciplina disciplina);
    void actualizar(float dt);
    void dibujar() const;
    bool finalizada() const;
    Disciplina disciplina() const;
    const Atleta& jugador() const;
    int posicionJugador() const;
    int aciertos() const;
    int errores() const;
    float tiempo() const;
};
