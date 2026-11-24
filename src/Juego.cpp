#include "Juego.h"
#include "raylib.h"
#include "Corredor.h"
#include "Nadador.h"
#include <cmath>
#include <algorithm>

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
    bool hover = CheckCollisionPointRec(GetMousePosition(), r);
    if (hover) { fill.r = (unsigned char)std::min(255, (int)fill.r + 25); fill.g = (unsigned char)std::min(255, (int)fill.g + 25); fill.b = (unsigned char)std::min(255, (int)fill.b + 25); }
    DrawRectangleRounded({r.x + 5, r.y + 7, r.width, r.height}, .22f, 12, Color{5, 12, 24, 160});
    DrawRectangleRounded(r, .22f, 12, fill);
    DrawRectangleRoundedLinesEx(r, .22f, 12, hover ? 3 : 2, Color{255, 255, 255, hover ? (unsigned char)160 : (unsigned char)65});
    int cx = (int)(r.x+52), cy = (int)(r.y+r.height/2);
    DrawCircle(cx, cy, 29, Color{255,255,255,35});
    const char* n = TextFormat("%d", numero);
    DrawText(n, cx - MeasureText(n, 28)/2, cy - 14, 28, WHITE);
    DrawText(titulo, (int)r.x+100, (int)r.y+20, 28, WHITE);
    DrawText(subtitulo, (int)r.x+101, (int)r.y+57, 17, Color{229,238,252,220});
    DrawText(">", (int)(r.x+r.width-48), (int)r.y+26, 30, WHITE);
}
void fondoMenu(float t) {
    ClearBackground(Color{10, 24, 48, 255});
    DrawRectangleGradientV(0, 0, 1100, 740, Color{19,48,83,255}, Color{7,17,34,255});
    DrawCircleGradient({905, 108}, 180, Color{255,199,115,90}, Color{255,199,115,0});
    DrawCircle(905,108,34,Color{255,217,150,190});
    // Gradas, luces y publico animado forman un estadio al atardecer.
    DrawRectangle(0, 262, 1100, 230, Color{12,28,48,255});
    for (int row=0; row<7; ++row) {
        DrawLine(0, 285+row*28, 1100, 285+row*28, Color{87,125,154,45});
        for (int col=0; col<55; ++col) {
            int x=col*21+(row%2)*10;
            float brillo = 0.5f + 0.5f*std::sin(t*1.8f+col*0.7f+row);
            DrawCircle(x, 275+row*28, 2.5f, Color{120,195,218,(unsigned char)(60+brillo*100)});
        }
    }
    for (int i=0; i<4; ++i) {
        int x=90+i*310;
        DrawLineEx({(float)x,115},{(float)x,270},4,Color{86,120,142,150});
        DrawRectangle(x-34,108,68,9,Color{158,203,216,180});
        for (int j=0;j<5;++j) DrawCircle(x-26+j*13,112,3,Color{255,242,198,230});
    }
    DrawRectangle(0, 482, 1100, 258, Color{100,48,52,255});
    for (int i=0;i<5;++i) DrawLineEx({0.0f,500.0f+i*54},{1100.0f,500.0f+i*54},2,Color{255,236,217,80});
    // Oscurecer el centro mantiene el texto legible sobre el escenario.
    DrawRectangle(0,0,1100,740,Color{4,13,28,105});
    for (int i=0;i<5;++i) {
        const Color colores[5]={{89,176,243,210},{246,213,86,210},{235,239,245,210},{103,210,161,210},{238,112,105,210}};
        DrawCircleLines(452+i*48, 105+(i%2)*18, 20, colores[i]);
        DrawCircleLines(453+i*48, 105+(i%2)*18, 20, colores[i]);
    }
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
    obstaculos_ = false;
    carreraObstaculos_.reset();
    competencia_ = std::make_unique<Competencia>(disciplina);
    tiempoCinematica_ = 0.0f;
    pantalla_ = Pantalla::Cinematica;
}
void Juego::iniciarObstaculos() {
    obstaculos_ = true;
    competencia_.reset();
    carreraObstaculos_ = std::make_unique<CarreraObstaculos>();
    tiempoCinematica_ = 0;
    pantalla_ = Pantalla::Cinematica;
}
void Juego::actualizar() {
    if (pantalla_ == Pantalla::Menu) {
        if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1) || (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), {245, 300, 610, 82}))) iniciar(Disciplina::Atletismo);
        else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2) || (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), {245, 395, 610, 82}))) iniciar(Disciplina::Natacion);
        else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3) || (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), {245,490,610,82}))) iniciarObstaculos();
    } else if (IsKeyPressed(KEY_M)) {
        competencia_.reset();
        carreraObstaculos_.reset();
        pantalla_ = Pantalla::Menu;
    } else if (pantalla_ == Pantalla::Cinematica) {
        tiempoCinematica_ += GetFrameTime();
        if (tiempoCinematica_ >= 7.0f || IsKeyPressed(KEY_SPACE)) pantalla_ = Pantalla::Carrera;
    } else if (pantalla_ == Pantalla::Carrera) {
        if (obstaculos_) {
            carreraObstaculos_->actualizar(GetFrameTime());
            if (carreraObstaculos_->terminada()) pantalla_ = Pantalla::Resultados;
        } else {
        competencia_->actualizar(GetFrameTime());
        if (competencia_->finalizada()) {
            archivos_.guardar(competencia_->disciplina(), competencia_->jugador().tiempoLlegada(),
                              competencia_->posicionJugador(), competencia_->aciertos(), competencia_->errores());
            pantalla_ = Pantalla::Resultados;
        }
        }
    } else if (pantalla_ == Pantalla::Resultados) {
        if (IsKeyPressed(KEY_R)) { if (obstaculos_) iniciarObstaculos(); else iniciar(competencia_->disciplina()); }
    }
}
void Juego::dibujar() const {
    BeginDrawing();
    if (pantalla_ == Pantalla::Menu) {
        fondoMenu((float)GetTime());
        DrawText("OLYMPIC / 2D", 45, 30, 20, CYAN);
        DrawText("PROYECTO POO", 905, 30, 16, MUTED);
        textoCentrado("OLIMPIADAS", 105, 60, TEXT);
        textoCentrado("CHALLENGE 2D", 173, 36, GOLD2);
        textoCentrado("ELIGE TU DISCIPLINA", 242, 23, MUTED);
        boton({245, 300, 610, 82}, Color{29, 105, 113, 255}, "ATLETISMO", "100 metros  |  A  S  D  F", 1);
        boton({245, 395, 610, 82}, Color{36, 74, 153, 255}, "NATACION", "50 metros  |  RITMO + RESPIRACION", 2);
        boton({245,490,610,82}, Color{145,75,54,255}, "OBSTACULOS", "PLATAFORMAS  |  SALTA Y RECOGE MEDALLAS", 3);
        textoCentrado("PRESIONA 1, 2 O 3, O HAZ CLIC PARA EMPEZAR", 602, 20, TEXT);
        textoCentrado("TRES PRUEBAS. UN MISMO OBJETIVO: LLEGAR A LA META", 638, 16, MUTED);
        DrawRectangle(0, 721, 1100, 19, Color{7, 14, 25, 255});
    } else if (pantalla_ == Pantalla::Cinematica) {
        bool agua = !obstaculos_ && competencia_->disciplina() == Disciplina::Natacion;
        float t = tiempoCinematica_;
        fondoMenu(t);
        if (agua) {
            DrawRectangleGradientV(0,250,1100,490, Color{13,86,125,255}, Color{4,34,67,255});
            for (int i=0; i<9; ++i) {
                float y=300.0f+i*52;
                DrawLineEx({0,y},{1100,y},2,Color{119,218,240,80});
                for (int j=0; j<12; ++j) {
                    float x=std::fmod(j*120.0f+t*55.0f+i*30.0f, 1200.0f)-50;
                    DrawLineEx({x,y+20},{x+35,y+18},2,Color{182,239,255,90});
                }
            }
        }
        DrawRectangle(0,0,1100,85,Color{2,8,17,245});
        DrawRectangle(0,655,1100,85,Color{2,8,17,245});
        if (t < 2.3f) {
            textoCentrado("EL ESTADIO ESTA LISTO", 210, 36, TEXT);
            textoCentrado(obstaculos_ ? "SALTA. ESQUIVA. LLEGA A LA META." : (agua ? "UNA PISCINA. UNA OPORTUNIDAD." : "UNA PISTA. UNA OPORTUNIDAD."), 275, 22, GOLD2);
        } else if (t < 4.7f) {
            textoCentrado(obstaculos_ ? "OBSTACULOS" : (agua ? "NATACION" : "ATLETISMO"), 160, 58, TEXT);
            textoCentrado(obstaculos_ ? "LA GRAN CARRERA" : (agua ? "50 METROS" : "100 METROS"), 230, 30, GOLD2);
            if (obstaculos_) {
                DrawRectangle(260, 470, 580, 18, Color{137,193,100,255});
                DrawRectangle(650, 410, 35, 60, Color{231,105,74,255});
                DrawCircle(530, 360, 15, GOLD);
            }
            for (int i=0;i<(obstaculos_ ? 1 : 3);++i) {
                float x=(obstaculos_ ? 420.0f : 330.0f+i*220.0f) + std::sin(t*2+i)*8;
                Color c=i==0 ? GOLD2 : (i==1 ? Color{118,239,183,255} : Color{255,127,125,255});
                if (agua) { Nadador nadador("", false); nadador.actualizar(t); nadador.dibujar({x,425}, c); }
                else { Corredor corredor("", false); corredor.actualizar(t); corredor.dibujar({x,425}, c); }
                DrawText(i==0 ? "TU" : (i==1 ? "RAPIDO" : "RITMICO"), (int)x-30, 480, 18, TEXT);
            }
        } else {
            int cuenta = std::max(1, 3-(int)(t-4.7f));
            textoCentrado("PREPARADOS", 225, 38, TEXT);
            textoCentrado(TextFormat("%d", cuenta), 310, 100, GOLD2);
            textoCentrado(obstaculos_ ? "A / D: MOVER  |  ESPACIO: SALTAR" : (agua ? "ESPACIO EN VERDE / B RESPIRA" : "ACERTA LAS LETRAS / CUIDA TU RESISTENCIA"), 475, 20, TEXT);
        }
        DrawText("ESPACIO: SALTAR", 840, 687, 18, MUTED);
    } else if (pantalla_ == Pantalla::Carrera) {
        if (obstaculos_) carreraObstaculos_->dibujar(); else competencia_->dibujar();
    } else if (obstaculos_) {
        fondo();
        textoCentrado(carreraObstaculos_->victoria() ? "META ALCANZADA" : "SIN VIDAS", 145, 48, TEXT);
        textoCentrado(carreraObstaculos_->victoria() ? "VICTORIA" : "INTENTALO DE NUEVO", 235, 32, GOLD2);
        textoCentrado(TextFormat("MEDALLAS: %d / 5", carreraObstaculos_->medallas()), 340, 30, CYAN);
        textoCentrado(TextFormat("TIEMPO: %.2f s", carreraObstaculos_->tiempo()), 395, 26, TEXT);
        textoCentrado("R  REINTENTAR             M  MENU", 600, 24, TEXT);
    } else {
        fondo();
        int puesto = competencia_->posicionJugador();
        textoCentrado(puesto == 1 ? "VICTORIA" : "DERROTA", 70, 37, TEXT);
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
