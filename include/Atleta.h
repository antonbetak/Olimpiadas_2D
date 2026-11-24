#pragma once
#include "raylib.h"
#include <string>

class Atleta {
private:
    std::string nombre_;
    float distancia_ = 0.0f;
    float tiempoLlegada_ = -1.0f;
    bool esJugador_ = false;
protected:
    float faseAnimacion_ = 0.0f;
    void mover(float metros, float meta, float tiempoActual);
public:
    Atleta(std::string nombre, bool esJugador);
    virtual ~Atleta() = default;
    const std::string& nombre() const;
    float distancia() const;
    float tiempoLlegada() const;
    bool esJugador() const;
    bool termino() const;
    virtual void actualizar(float dt) = 0;
    virtual void avanzarPorAcierto(float meta, float tiempoActual) = 0;
    virtual void avanzarIA(float dt, float velocidad, float meta, float tiempoActual) = 0;
    virtual void dibujar(Vector2 centro, Color color) const = 0;
};
