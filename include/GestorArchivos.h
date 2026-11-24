#pragma once
#include "Competencia.h"
#include <string>
class GestorArchivos {
private:
    std::string ruta_;
public:
    explicit GestorArchivos(std::string ruta);
    void guardar(Disciplina disciplina, float segundos, int posicion, int aciertos, int errores) const;
    float mejorTiempo(Disciplina disciplina) const;
};
