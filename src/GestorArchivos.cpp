#include "GestorArchivos.h"
#include <fstream>
#include <sstream>
#include <utility>

namespace {
    const char* nombreDisciplina(Disciplina d) {
        return d == Disciplina::Atletismo ? "Atletismo" : "Natacion";
    }
}
GestorArchivos::GestorArchivos(std::string ruta) : ruta_(std::move(ruta)) {}
void GestorArchivos::guardar(Disciplina d, float segundos, int posicion, int aciertos, int errores) const {
    std::ofstream archivo(ruta_, std::ios::app);
    if (archivo) archivo << nombreDisciplina(d) << ',' << segundos << ','
                        << posicion << ',' << aciertos << ',' << errores << '\n';
}
float GestorArchivos::mejorTiempo(Disciplina d) const {
    std::ifstream archivo(ruta_);
    std::string linea;
    float mejor = -1.0f;
    while (std::getline(archivo, linea)) {
        std::istringstream fila(linea);
        std::string deporte, tiempoStr;
        if (!std::getline(fila, deporte, ',') || !std::getline(fila, tiempoStr, ',')) continue;
        if (deporte != nombreDisciplina(d)) continue;
        try {
            float t = std::stof(tiempoStr);
            if (t > 0 && (mejor < 0 || t < mejor)) mejor = t;
        } catch (...) { /* Ignorar lineas invalidas. */ }
    }
    return mejor;
}
