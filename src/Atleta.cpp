#include "Atleta.h"
#include <algorithm>
#include <utility>

Atleta::Atleta(std::string nombre, bool esJugador)
    : nombre_(std::move(nombre)), esJugador_(esJugador) {}
const std::string& Atleta::nombre() const { return nombre_; }
float Atleta::distancia() const { return distancia_; }
float Atleta::tiempoLlegada() const { return tiempoLlegada_; }
bool Atleta::esJugador() const { return esJugador_; }
bool Atleta::termino() const { return tiempoLlegada_ >= 0.0f; }
void Atleta::mover(float metros, float meta, float tiempoActual) {
    if (termino()) return;
    distancia_ = std::clamp(distancia_ + std::max(0.0f, metros), 0.0f, meta);
    if (distancia_ >= meta) tiempoLlegada_ = tiempoActual;
}
