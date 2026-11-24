#pragma once
#include "Competencia.h"
#include "GestorArchivos.h"
#include <memory>

class Juego {
private:
    enum class Pantalla { Menu, Carrera, Resultados };
    Pantalla pantalla_ = Pantalla::Menu;
    std::unique_ptr<Competencia> competencia_;
    GestorArchivos archivos_{"records.csv"};
    void iniciar(Disciplina disciplina);
    void actualizar();
    void dibujar() const;
public:
    void ejecutar();
};
