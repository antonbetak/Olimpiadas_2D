#pragma once
#include "Competencia.h"
#include "CarreraObstaculos.h"
#include "GestorArchivos.h"
#include <memory>

class Juego {
private:
    enum class Pantalla { Menu, Cinematica, Carrera, Resultados };
    Pantalla pantalla_ = Pantalla::Menu;
    std::unique_ptr<Competencia> competencia_;
    bool obstaculos_ = false;
    std::unique_ptr<CarreraObstaculos> carreraObstaculos_;
    float tiempoCinematica_ = 0.0f;
    GestorArchivos archivos_{"records.csv"};
    void iniciar(Disciplina disciplina);
    void iniciarObstaculos();
    void actualizar();
    void dibujar() const;
public:
    void ejecutar();
};
