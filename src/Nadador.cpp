#include "Nadador.h"
#include <cmath>
#include <utility>
Nadador::Nadador(std::string nombre, bool jugador) : Atleta(std::move(nombre), jugador) {}
void Nadador::actualizar(float dt) { faseAnimacion_ += dt * (termino() ? 0.0f : 9.0f); }
void Nadador::avanzarPorAcierto(float meta, float tiempoActual) { mover(3.0f, meta, tiempoActual); }
void Nadador::avanzarIA(float dt, float velocidad, float meta, float tiempoActual) { mover(velocidad*dt, meta, tiempoActual); }
void Nadador::dibujar(Vector2 p, Color c) const {
    float brazada = std::sin(faseAnimacion_)*11.0f;
    float patada = std::cos(faseAnimacion_*1.35f)*4.0f;
    Color piel{239,181,137,255};
    Color traje{24,72,130,255};
    Color brillo{214,249,255,150};

    // Estela y sombra bajo el agua.
    DrawEllipse((int)p.x,(int)(p.y+11),42,9,Color{5,54,91,150});
    for (int i=0; i<4; ++i) {
        float radio = 5.0f + i*3.0f + std::sin(faseAnimacion_+i)*1.5f;
        DrawCircleLines((int)(p.x-28-i*12), (int)(p.y+7+i*2), radio, brillo);
    }

    // Brazos alternados en estilo crol.
    DrawLineEx({p.x-12,p.y-3}, {p.x-32+brazada*.35f,p.y-15}, 6, piel);
    DrawLineEx({p.x+5,p.y-7}, {p.x+22-brazada*.35f,p.y-24}, 6, piel);
    DrawCircleV({p.x-34+brazada*.35f,p.y-16}, 3, piel);
    DrawCircleV({p.x+24-brazada*.35f,p.y-25}, 3, piel);

    // Cuerpo, gorra y gafas.
    DrawEllipse((int)p.x,(int)(p.y-5),30,10,traje);
    DrawLineEx({p.x-20,p.y+1},{p.x-39,p.y+5+patada},5,traje);
    DrawLineEx({p.x-18,p.y+5},{p.x-38,p.y+12-patada},5,traje);
    DrawCircleV({p.x+23,p.y-14},10,piel);
    DrawCircleV({p.x+26,p.y-16},3,Color{28,174,211,255});
    DrawRectangleRounded({p.x+16,p.y-23,16,7},.45f,8,Color{28,63,111,255});
    DrawLineEx({p.x+18,p.y-23},{p.x+29,p.y-25},3,Color{88,215,238,255});
    DrawCircleV({p.x+29,p.y-14},1.5f,WHITE);

    // Brillos que dan profundidad al agua.
    DrawLineEx({p.x-8,p.y-8},{p.x+10,p.y-10},2,Color{107,220,239,180});
    DrawCircle((int)(p.x-43), (int)(p.y-17), 3, Color{210,245,255,130});
}
