#pragma once
#include "Atleta.h"
class Nadador final : public Atleta {
public:
    Nadador(std::string nombre, bool jugador);
    void actualizar(float dt) override;
    void avanzarPorAcierto(float meta, float tiempoActual) override;
    void avanzarIA(float dt, float velocidad, float meta, float tiempoActual) override;
    void dibujar(Vector2 centro, Color color) const override;
};
