#pragma once
#include "Atleta.h"
class Corredor final : public Atleta {
public:
    Corredor(std::string nombre, bool jugador);
    void actualizar(float dt) override;
    void avanzarPorAcierto(float meta, float tiempoActual) override;
    void avanzarIA(float dt, float velocidad, float meta, float tiempoActual) override;
    void dibujar(Vector2 centro, Color color) const override;
};
