#include "Juego.h"
#include "raylib.h"
#include <cmath>

namespace {
constexpr Color BG{10, 18, 34, 255};
constexpr Color PANEL{20, 34, 57, 255};
constexpr Color TEXT{236, 244, 255, 255};
constexpr Color MUTED{151, 170, 193, 255};
constexpr Color CYAN{69, 212, 233, 255};
constexpr Color GOLD2{255, 202, 91, 255};

void textoCentrado(const char* s, int y, int size, Color c) {
    DrawText(s, (1100 - MeasureText(s, size))/2, y, size, c);
}
void boton(Rectangle r, Color fill, const char* titulo, const char* subtitulo, int numero) {
    DrawRectangleRounded({r.x + 5, r.y + 7, r.width, r.height}, .22f, 12, Color{5, 12, 24, 130});
    DrawRectangleRounded(r, .22f, 12, fill);
    DrawRectangleRoundedLinesEx(r, .22f, 12, 2, Color{255, 255, 255, 52});
    DrawCircle((int)(r.x+52), (int)(r.y+r.height/2), 29, Color{255,255,255,29});
    DrawText(TextFormat("%d", numero), (int)r.x+43, (int)r.y+27, 27, WHITE);
    DrawText(titulo, (int)r.x+100, (int)r.y+20, 28, WHITE);
    DrawText(subtitulo, (int)r.x+101, (int)r.y+57, 17, Color{229,238,252,220});
    DrawText(">", (int)(r.x+r.width-48), (int)r.y+26, 30, WHITE);
}
void fondo() {
    ClearBackground(BG);
    DrawCircleGradient({160, 140}, 360, Color{35, 84, 145, 115}, BG);
    DrawCircleGradient({980, 650}, 350, Color{24, 118, 121, 86}, BG);
    for (int i=0; i<12; ++i) {
        int x = i*110-100;
        DrawLine(x, 0, x+370, 740, Color{90, 145, 183, 17});
    }
}
void medalla(int puesto, int y) {
    Color c = puesto == 1 ? GOLD2 : (puesto == 2 ? Color{201,216,230,255} : Color{204,142,99,255});
    DrawTriangle({537.0f,(float)y-25}, {547.0f,(float)y+7}, {555.0f,(float)y-25}, Color{71,132,202,255});
    DrawTriangle({555.0f,(float)y-25}, {564.0f,(float)y+7}, {573.0f,(float)y-25}, Color{109,82,197,255});
    DrawCircle(555,y,32,c);
    DrawCircleLines(555,y,27,Color{255,255,255,155});
    DrawText(TextFormat("%d",puesto), 547,y-16,31,BG);
}
}

void Juego::iniciar(Disciplina disciplina) {
    competencia_ = std::make_unique<Competencia>(disciplina);
    pantalla_ = Pantalla::Carrera;
}
void Juego::actualizar() {
    if (pantalla_ == Pantalla::Menu) {
        if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) iniciar(Disciplina::Atletismo);
        else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) iniciar(Disciplina::Natacion);
    } else if (IsKeyPressed(KEY_M)) {
        competencia_.reset();
        pantalla_ = Pantalla::Menu;
    } else if (pantalla_ == Pantalla::Carrera) {
        competencia_->actualizar(GetFrameTime());
        if (competencia_->finalizada()) {
            archivos_.guardar(competencia_->disciplina(), competencia_->jugador().tiempoLlegada(),
                              competencia_->posicionJugador(), competencia_->aciertos(), competencia_->errores());
            pantalla_ = Pantalla::Resultados;
        }
    } else if (pantalla_ == Pantalla::Resultados) {
        if (IsKeyPressed(KEY_R)) iniciar(competencia_->disciplina());
    }
}
void Juego::dibujar() const {
    BeginDrawing();
    if (pantalla_ == Pantalla::Menu) {
        fondo();
        DrawText("OLYMPIC / 2D", 45, 30, 20, CYAN);
        DrawText("PROYECTO POO", 905, 30, 16, MUTED);
        textoCentrado("OLIMPIADAS", 111, 69, TEXT);
        textoCentrado("CHALLENGE 2D", 188, 43, GOLD2);
        textoCentrado("ELIGE TU DISCIPLINA", 273, 23, MUTED);
        boton({245, 335, 610, 96}, Color{29, 105, 113, 255}, "ATLETISMO", "100 metros  |  A  S  D  F", 1);
        boton({245, 452, 610, 96}, Color{36, 74, 153, 255}, "NATACION", "50 metros  |  J  K  L", 2);
        textoCentrado("PRESIONA 1 O 2 PARA EMPEZAR", 594, 20, TEXT);
        textoCentrado("ACERTA LA TECLA EN PANTALLA PARA AVANZAR", 635, 16, MUTED);
        DrawRectangle(0, 721, 1100, 19, Color{7, 14, 25, 255});
    } else if (pantalla_ == Pantalla::Carrera) {
        competencia_->dibujar();
    } else {
        fondo();
        textoCentrado("COMPETENCIA FINALIZADA", 70, 37, TEXT);
        int puesto = competencia_->posicionJugador();
        medalla(puesto, 222);
        textoCentrado(puesto == 1 ? "MEDALLA DE ORO" : puesto == 2 ? "MEDALLA DE PLATA" : "MEDALLA DE BRONCE", 280, 28, GOLD2);
        DrawRectangleRounded({260, 349, 580, 188}, .13f, 12, PANEL);
        DrawText(TextFormat("POSICION            %d / 3", puesto), 315, 373, 24, TEXT);
        DrawText(TextFormat("TIEMPO              %.2f s", competencia_->jugador().tiempoLlegada()), 315, 413, 24, TEXT);
        DrawText(TextFormat("ACIERTOS / ERRORES   %d / %d", competencia_->aciertos(), competencia_->errores()), 315, 453, 24, TEXT);
        float mejor = archivos_.mejorTiempo(competencia_->disciplina());
        if (mejor >= 0) textoCentrado(TextFormat("MEJOR REGISTRO: %.2f s", mejor), 558, 22, CYAN);
        textoCentrado("R  REVANCHA             M  MENU", 635, 24, TEXT);
    }
    EndDrawing();
}
void Juego::ejecutar() {
    InitWindow(1100, 740, "Olimpiadas 2D - Proyecto POO");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        actualizar();
        dibujar();
    }
    CloseWindow();
}
