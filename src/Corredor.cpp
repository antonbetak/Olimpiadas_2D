#include "Corredor.h"
#include <cmath>
#include <algorithm>
#include <utility>
Corredor::Corredor(std::string nombre, bool jugador) : Atleta(std::move(nombre), jugador) {}
float Corredor::energia() const { return energia_; }
void Corredor::accionEspecial() {}
void Corredor::actualizar(float dt) {
    if (!termino()) { faseAnimacion_ += dt * 13.0f; energia_ = std::min(100.0f, energia_ + 12.0f * dt); }
}
void Corredor::avanzarPorAcierto(float meta, float tiempoActual) { mover(energia_ >= 20.0f ? 4.2f : 1.8f, meta, tiempoActual); energia_ = std::max(0.0f, energia_ - 12.0f); }
void Corredor::avanzarIA(float dt, float velocidad, float meta, float tiempoActual) { mover(velocidad*dt, meta, tiempoActual); }
void Corredor::dibujar(Vector2 p, Color c) const {
    float z = std::sin(faseAnimacion_)*12.0f;
    float y = p.y + std::fabs(std::sin(faseAnimacion_))*2.0f;
    // Sombra y zapatillas; personaje orientado a la derecha.
    DrawEllipse((int)p.x,(int)(y+22),25,5,Color{24,29,37,70});
    DrawLineEx({p.x-4,y+3},{p.x-14-z*.6f,y+22},6,Color{34,49,67,255});
    DrawLineEx({p.x+2,y+3},{p.x+12+z*.6f,y+21},6,Color{34,49,67,255});
    DrawLineEx({p.x-14-z*.6f,y+22},{p.x-4-z*.6f,y+22},5,WHITE);
    DrawLineEx({p.x+12+z*.6f,y+21},{p.x+21+z*.6f,y+21},5,WHITE);
    DrawLineEx({p.x-3,y-22},{p.x-1,y+4},13,c);
    DrawRectangleRounded({p.x-10,y-18,17,12},.25f,6,Color{255,255,255,45});
    DrawLineEx({p.x-3,y-16},{p.x-15+z*.5f,y-2},5,Color{239,181,135,255});
    DrawLineEx({p.x-2,y-15},{p.x+13-z*.5f,y-3},5,Color{239,181,135,255});
    DrawCircleV({p.x+1,y-32},10,Color{240,183,138,255});
    DrawCircleSector({p.x+1,y-34},11,190,350,12,Color{31,49,67,255});
    DrawCircleV({p.x+7,y-32},1.5f,Color{20,30,45,255});
}
