#pragma once
#include "raylib.h"
#include <vector>

// Carrera lateral con saltos, vallas y medallas coleccionables.
class CarreraObstaculos {
    struct Medalla { Vector2 posicion; bool recogida = false; };
    float x_ = 80, y_ = 510, velocidadY_ = 0, tiempo_ = 0, invulnerable_ = 0;
    int vidas_ = 3, medallas_ = 0;
    bool enSuelo_ = true, terminada_ = false, victoria_ = false;
    std::vector<Rectangle> vallas_;
    std::vector<Rectangle> plataformas_;
    std::vector<Medalla> premios_;
    std::vector<Rectangle> trampolines_;
    std::vector<Rectangle> bloques_;
public:
    CarreraObstaculos();
    void actualizar(float dt);
    void dibujar() const;
    bool terminada() const { return terminada_; }
    bool victoria() const { return victoria_; }
    float tiempo() const { return tiempo_; }
    int medallas() const { return medallas_; }
};
