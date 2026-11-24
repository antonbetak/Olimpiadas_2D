#include "Competencia.h"
#include "Corredor.h"
#include "Nadador.h"
#include "raylib.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <string>

namespace {
constexpr float INICIO = 132.0f;
constexpr float FIN = 1005.0f;
constexpr Color TINTA{234,243,255,255};
constexpr Color APAGADO{157,179,204,255};
const char* teclaTexto(int tecla) {
    switch (tecla) {
        case KEY_A: return "A"; case KEY_S: return "S";
        case KEY_D: return "D"; case KEY_F: return "F";
        case KEY_J: return "J"; case KEY_K: return "K";
        case KEY_L: return "L"; default: return "?";
    }
}
void fondoEstadio(bool agua, float t) {
    Color cielo = agua ? Color{14,43,75,255} : Color{38,69,105,255};
    ClearBackground(cielo);
    DrawRectangleGradientV(0, 0, 1100, 280, cielo, agua ? Color{19,94,125,255} : Color{98,151,165,255});
    DrawCircle(970, 99, 47, agua ? Color{118,214,231,75} : Color{255,228,158,175});
    DrawRectangle(0, 125, 1100, 90, Color{17,35,56,255});
    for (int row=0; row<4; ++row) {
        for (int col=0; col<56; ++col) {
            int x = col*20 + (row%2)*9;
            Color c = (col+row)%3 == 0 ? Color{100,166,198,200} : ((col+row)%3 == 1 ? Color{185,200,225,165} : Color{64,111,158,210});
            DrawCircle(x, 142+row*19, 3.4f, c);
        }
    }
    DrawRectangle(0, 208, 1100, 9, Color{227,240,246,190});
    DrawRectangle(0, 219, 1100, 325, agua ? Color{12,89,133,255} : Color{45,104,71,255});
    if (agua) {
        for (int j=0; j<18; ++j) {
            float x = (float)(j*72) + std::sin(t*1.1f+j)*8;
            DrawLineEx({x,230}, {x+25,235}, 2, Color{110,218,247,90});
        }
        for (int j=0; j<9; ++j) {
            float x = 80.0f + j*137.0f + std::sin(t*.8f+j)*13.0f;
            float y = 238.0f + std::sin(t*1.4f+j)*8.0f;
            DrawCircleLines((int)x,(int)y,4,Color{186,244,255,100});
        }
    }
}
void carril(int i, bool agua, float t) {
    float y = 280.0f+i*102;
    Color base = agua ? (i%2 ? Color{24,135,180,255} : Color{27,153,195,255})
                      : (i%2 ? Color{169,81,69,255} : Color{185,95,73,255});
    DrawRectangleRounded({29,y-36,1042,91}, .08f, 8, base);
    if (agua) {
        for (int j=0; j<17; ++j) {
            float x = j*68.0f + std::sin(t*2+j+i)*9;
            DrawLineEx({x+32,y+17}, {x+55,y+14}, 2, Color{169,240,251,90});
        }
        for (int x=35; x<1060; x+=21)
            DrawCircle(x, (int)(y+50), 4, ((x/21)%2) ? WHITE : Color{255,94,95,255});
        DrawLineEx({34,y-4},{1064,y-4},2,Color{194,247,255,120});
        DrawText(TextFormat("CARRIL %d", i+1), 950, (int)y-26, 13, Color{220,250,255,180});
    } else {
        DrawLineEx({34,y+49},{1064,y+49},3,Color{255,255,255,195});
        for (int x=220; x<1000; x+=170)
            DrawLineEx({(float)x,y-28},{(float)x,y+41},1,Color{255,255,255,52});
    }
    // Linea de meta a cuadros
    for (int j=0; j<8; ++j)
        DrawRectangle((int)FIN-3, (int)(y-34+j*11), 8, 11, j%2 ? BLACK : WHITE);
}
void etiqueta(float progreso, float y) {
    DrawRectangleRounded({42,y-31,78,27}, .35f, 8, Color{13,26,44,205});
    DrawText(TextFormat("%3.0f%%", progreso*100), 54, (int)y-27, 17, TINTA);
}
}

Competencia::Competencia(Disciplina disciplina)
  : disciplina_(disciplina), meta_(disciplina == Disciplina::Atletismo ? 100.0f : 50.0f),
    control_(disciplina == Disciplina::Natacion) {
    for (int i = 0; i < 3; ++i) {
        const std::string nombre = i == 0 ? "TU" : "CPU " + std::to_string(i);
        bool jugador = i == 0;
        if (disciplina_ == Disciplina::Atletismo)
            atletas_.push_back(std::make_unique<Corredor>(nombre, jugador));
        else
            atletas_.push_back(std::make_unique<Nadador>(nombre, jugador));
    }
    velocidadesIA_ = disciplina_ == Disciplina::Atletismo
        ? std::vector<float>{0, 7.4f, 8.1f} : std::vector<float>{0, 1.75f, 2.05f};
}
void Competencia::actualizar(float dt) {
    if (finalizada_) return;
    dt = std::min(dt, 0.05f);
    tiempo_ += dt;
    const std::array<int, 7> teclas{KEY_A, KEY_S, KEY_D, KEY_F, KEY_J, KEY_K, KEY_L};
    if (!jugador().termino()) {
        for (int tecla : teclas) {
            if (!IsKeyPressed(tecla)) continue;
            if (control_.procesar(tecla)) {
                ++aciertos_;
                atletas_.front()->avanzarPorAcierto(meta_, tiempo_);
            } else {
                ++errores_;
            }
            break;
        }
    }
    for (std::size_t i = 0; i < atletas_.size(); ++i) {
        atletas_[i]->actualizar(dt);
        if (i != 0) atletas_[i]->avanzarIA(dt, velocidadesIA_[i], meta_, tiempo_);
    }
    finalizada_ = std::all_of(atletas_.begin(), atletas_.end(),
                             [](const auto& a){ return a->termino(); });
}
void Competencia::dibujar() const {
    bool agua = disciplina_ == Disciplina::Natacion;
    fondoEstadio(agua, tiempo_);
    DrawRectangleRounded({25,22,1050,101}, .15f, 12, Color{9,20,38,224});
    DrawText(agua ? "NATACION / 50 M" : "ATLETISMO / 100 M", 50, 38, 32, TINTA);
    DrawText("COMPETENCIA EN VIVO", 51, 85, 16, APAGADO);
    DrawText(TextFormat("%06.2f s",tiempo_), 814, 41, 34, Color{255,208,109,255});
    DrawText("M  MENU", 941, 93, 16, APAGADO);
    for (int i=0; i<3; ++i) {
        float y = 280.0f+i*102;
        carril(i,agua,tiempo_);
        float pct = std::clamp(atletas_[i]->distancia()/meta_,0.0f,1.0f);
        etiqueta(pct,y);
        float x = INICIO + (FIN-INICIO)*pct;
        atletas_[i]->dibujar({x,y+11}, i==0 ? Color{255,216,100,255} : (i==1 ? Color{118,239,183,255} : Color{255,127,125,255}));
        DrawText(atletas_[i]->nombre().c_str(), 48, (int)y+10, 16, TINTA);
    }
    DrawRectangle(0,551,1100,189,Color{9,20,38,249});
    if (agua) {
        DrawRectangleGradientH(390, 551, 320, 189, Color{12,39,69,255}, Color{10,27,50,255});
        DrawText("OBJETIVO", 790, 690, 13, APAGADO);
        DrawText("LLEGA PRIMERO", 790, 710, 16, Color{114,233,238,255});
    }
    DrawText("TU PROGRESO", 48, 571, 17, APAGADO);
    DrawRectangleRounded({48,602,300,15}, .5f, 12, Color{49,69,95,255});
    float progreso = std::clamp(jugador().distancia()/meta_, 0.0f, 1.0f);
    if (progreso > 0) DrawRectangleRounded({48,602,300*progreso,15},.5f,12,Color{74,225,196,255});
    DrawText(TextFormat("%.1f / %.0f m", jugador().distancia(), meta_),48,628,20,TINTA);
    DrawText(TextFormat("ACIERTOS  %d",aciertos_),48,689,18,Color{114,233,177,255});
    DrawText(TextFormat("ERRORES  %d",errores_),215,689,18,Color{255,140,139,255});
    if (!jugador().termino()) {
        DrawRectangleRounded({431,563,239,157}, .18f, 12, Color{28,53,82,255});
        DrawRectangleRoundedLinesEx({431,563,239,157},.18f,12,2,Color{115,175,218,110});
        DrawText("PRESIONA",495,576,19,TINTA);
        const char* tecla = teclaTexto(control_.teclaActual());
        DrawRectangleRounded({505,608,90,84},.20f,12,Color{250,199,89,255});
        DrawText(tecla,550-MeasureText(tecla,53)/2,622,53,Color{17,32,51,255});
    } else {
        DrawText("META ALCANZADA",432,610,23,Color{255,208,109,255});
        DrawText("ESPERANDO RIVALES",431,642,16,APAGADO);
    }
    DrawText(agua ? "MANTEN TU RITMO" : "TECLEA RAPIDO",780,584,20,TINTA);
    DrawText(agua ? "J / K / L" : "A / S / D / F",780,622,25,Color{79,211,236,255});
    DrawText(agua ? "CADA BRAZADA TE IMPULSA" : "CADA ACIERTO TE IMPULSA",780,661,14,APAGADO);
}
bool Competencia::finalizada() const { return finalizada_; }
Disciplina Competencia::disciplina() const { return disciplina_; }
const Atleta& Competencia::jugador() const { return *atletas_.front(); }
int Competencia::posicionJugador() const {
    int posicion = 1;
    for (std::size_t i = 1; i < atletas_.size(); ++i)
        if (atletas_[i]->tiempoLlegada() < jugador().tiempoLlegada()) ++posicion;
    return posicion;
}
int Competencia::aciertos() const { return aciertos_; }
int Competencia::errores() const { return errores_; }
float Competencia::tiempo() const { return tiempo_; }
