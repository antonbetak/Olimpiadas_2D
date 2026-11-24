#include "CarreraObstaculos.h"
#include <algorithm>
#include <cmath>

namespace {
constexpr float SUELO = 540, META = 2800;
constexpr Color TINTA{238,248,255,255};
}
CarreraObstaculos::CarreraObstaculos()
    : vallas_{{450,490,35,50},{850,480,40,60},{1350,485,38,55},{1880,475,40,65},{2350,485,38,55}},
      plataformas_{{620,410,160,18},{1080,395,170,18},{1580,415,160,18},{2070,390,170,18}},
      premios_{{{700,370}},{{1160,355}},{{1660,375}},{{2150,350}},{{2520,480}}},
      trampolines_{{980,520,55,20},{1780,520,55,20}},
      bloques_{{1220,470,34,34},{2250,450,34,54}} {}
void CarreraObstaculos::actualizar(float dt) {
    if (terminada_) return;
    dt = std::min(dt, 0.05f);
    tiempo_ += dt;
    invulnerable_ = std::max(0.0f, invulnerable_ - dt);
    float dx = (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D) ? 1.0f : 0.0f)
             - (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A) ? 1.0f : 0.0f);
    x_ = std::clamp(x_ + dx * 250.0f * dt, 15.0f, META + 20);
    if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && enSuelo_) {
        velocidadY_ = -620; enSuelo_ = false;
    }
    float pieAnterior = y_ + 30;
    velocidadY_ += 1550 * dt;
    y_ += velocidadY_ * dt;
    enSuelo_ = false;
    Rectangle jugador{x_-16,y_-28,32,58};
    if (y_ + 30 >= SUELO) { y_ = SUELO - 30; velocidadY_ = 0; enSuelo_ = true; }
    for (const auto& p : plataformas_) {
        if (velocidadY_ >= 0 && pieAnterior <= p.y && y_+30 >= p.y && x_+17 > p.x && x_-17 < p.x+p.width) {
            y_ = p.y-30; velocidadY_ = 0; enSuelo_ = true;
        }
    }
    for (const auto& t : trampolines_) {
        if (CheckCollisionRecs(jugador, t) && velocidadY_ >= 0) { velocidadY_ = -820; enSuelo_ = false; }
    }
    for (const auto& b : bloques_) {
        Rectangle anim = b; anim.y += std::sin(tiempo_ * 2.5f + b.x) * 22;
        if (invulnerable_ <= 0 && CheckCollisionRecs(jugador, anim)) { --vidas_; invulnerable_ = 1.4f; x_ = std::max(30.0f, anim.x-70); break; }
    }
    for (const auto& v : vallas_) {
        if (invulnerable_ <= 0 && CheckCollisionRecs(jugador, v)) {
            --vidas_; invulnerable_ = 1.4f; x_ = std::max(30.0f, v.x-75); y_ = SUELO-30; velocidadY_ = 0;
            if (vidas_ == 0) terminada_ = true;
            break;
        }
    }
    jugador = {x_-16,y_-28,32,58};
    for (auto& premio : premios_) {
        if (!premio.recogida && CheckCollisionCircleRec(premio.posicion, 16, jugador)) {
            premio.recogida = true; ++medallas_;
        }
    }
    if (x_ >= META) { terminada_ = true; victoria_ = true; }
}
void CarreraObstaculos::dibujar() const {
    float camara = std::clamp(x_ - 300, 0.0f, META - 950);
    // Atardecer y capas que se mueven a diferentes velocidades con la camara.
    DrawRectangleGradientV(0,0,1100,540,Color{38,59,116,255},Color{255,165,125,255});
    DrawCircleGradient({890,155},150,Color{255,220,167,145},Color{255,190,130,0});
    DrawCircle(890,155,48,Color{255,223,167,255});
    for (int i=0;i<9;++i) {
        float x=std::fmod(i*190.0f-camara*0.12f+3800, 1710.0f)-200;
        DrawTriangle({x-130,400},{x+20,205+(float)(i%3)*25},{x+170,400},Color{89,105,145,150});
    }
    for (int i=0;i<18;++i) {
        float x=std::fmod(i*105.0f-camara*0.27f+4200, 1890.0f)-130;
        int h=75+(i*37%100);
        DrawRectangle((int)x,440-h,80,h,Color{47,72,104,210});
        for (int row=0;row<4;++row) for(int col=0;col<3;++col)
            DrawRectangle((int)x+12+col*22,450-h+row*22,7,9,Color{255,216,162,130});
    }
    // Gradas del estadio y publico.
    DrawRectangle(0,425,1100,115,Color{35,64,82,255});
    for (int row=0;row<4;++row) {
        DrawLine(0,443+row*25,1100,443+row*25,Color{128,178,181,90});
        for (int col=0;col<47;++col) {
            int x=(int)(col*26-std::fmod(camara*.45f,26));
            Color c=(col+row)%3==0 ? Color{255,194,115,210} : Color{119,205,217,190};
            DrawCircle(x,435+row*25,3,c);
        }
    }
    DrawRectangle(0,540,1100,200,Color{89,47,64,255});
    DrawRectangleGradientV(0,540,1100,110,Color{212,111,86,255},Color{142,65,66,255});
    DrawRectangle(0,540,1100,7,Color{255,222,186,255});
    DrawLineEx({0,610},{1100,610},3,Color{255,221,198,120});
    for (int i=0;i<15;++i) DrawRectangle((int)(i*120-std::fmod(camara,120)),570,55,3,Color{255,227,197,130});
    for (const auto& p : plataformas_) {
        Rectangle r{p.x-camara,p.y,p.width,p.height};
        DrawRectangleRounded({r.x+5,r.y+6,r.width,r.height},.25f,6,Color{32,39,60,100});
        DrawRectangleRounded(r,.25f,6,Color{42,94,106,255});
        DrawRectangleRoundedLinesEx(r,.25f,6,2,Color{169,232,213,200});
        DrawRectangle((int)r.x+8,(int)r.y+4,(int)r.width-16,3,Color{151,238,200,255});
    }
    for (const auto& v : vallas_) {
        float x=v.x-camara;
        DrawRectangle((int)x+3,(int)v.y+5,(int)v.width,(int)v.height,Color{42,40,51,95});
        DrawRectangle((int)x,(int)v.y,(int)v.width,(int)v.height,Color{239,105,72,255});
        DrawRectangleLines((int)x,(int)v.y,(int)v.width,(int)v.height,Color{255,222,183,255});
        DrawRectangle((int)x,(int)v.y+8,(int)v.width,9,Color{255,240,218,255});
        DrawRectangle((int)x,(int)v.y+31,(int)v.width,9,Color{255,240,218,255});
    }
    for (const auto& t : trampolines_) {
        Rectangle r{t.x-camara,t.y,t.width,t.height};
        DrawRectangleRounded(r,.3f,7,Color{73,210,188,255});
        DrawTriangle({r.x+8,r.y},{r.x+r.width/2,r.y-12},{r.x+r.width-8,r.y},Color{242,239,157,255});
    }
    for (const auto& b : bloques_) {
        float y=b.y+std::sin(tiempo_*2.5f+b.x)*22;
        DrawRectangle((int)(b.x-camara),(int)y,(int)b.width,(int)b.height,Color{113,80,165,255});
        DrawRectangleLines((int)(b.x-camara),(int)y,(int)b.width,(int)b.height,Color{224,176,255,255});
    }
    for (const auto& p : premios_) if (!p.recogida) {
        DrawCircleGradient({p.posicion.x-camara,p.posicion.y},29,Color{255,218,85,100},Color{255,218,85,0});
        DrawCircle((int)(p.posicion.x-camara),(int)p.posicion.y,16,GOLD);
        DrawCircleLines((int)(p.posicion.x-camara),(int)p.posicion.y,12,WHITE);
        DrawText("*",(int)(p.posicion.x-camara)-6,(int)p.posicion.y-11,20,WHITE);
    }
    float metaX=META-camara;
    DrawRectangle((int)metaX,315,8,225,WHITE);
    DrawTriangle({metaX+8,315},{metaX+85,338},{metaX+8,360},Color{255,100,86,255});
    if (invulnerable_ <= 0 || (int)(invulnerable_*10)%2==0) {
        float x=x_-camara;
        float paso = enSuelo_ && (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D) || IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) ? std::sin(tiempo_*14)*10 : 0;
        DrawEllipse((int)x, (int)(enSuelo_ ? y_+29 : SUELO-2), 23, 5, Color{29,35,49,100});
        DrawLineEx({x-4,y_+12},{x-11-paso,y_+27},7,Color{30,47,78,255});
        DrawLineEx({x+4,y_+12},{x+11+paso,y_+27},7,Color{30,47,78,255});
        DrawLineEx({x-13-paso,y_+28},{x-3-paso,y_+28},5,WHITE);
        DrawLineEx({x+10+paso,y_+28},{x+20+paso,y_+28},5,WHITE);
        DrawRectangleRounded({x-12,y_-10,25,26},.3f,8,Color{255,198,72,255});
        DrawRectangle((int)x-8,(int)y_-3,17,5,Color{255,236,188,180});
        DrawLineEx({x-9,y_-5},{x-17+paso*.5f,y_+8},5,Color{238,175,130,255});
        DrawLineEx({x+10,y_-5},{x+17-paso*.5f,y_+8},5,Color{238,175,130,255});
        DrawCircle((int)x,(int)y_-20,12,Color{248,190,144,255});
        DrawCircleSector({x,y_-24},12,190,350,12,Color{34,45,66,255});
        DrawCircle((int)x+5,(int)y_-20,2,Color{27,43,58,255});
    }
    DrawRectangle(0,0,1100,75,Color{9,28,48,225});
    DrawText("CARRERA DE OBSTACULOS",25,17,25,TINTA);
    DrawText(TextFormat("VIDAS: %d   MEDALLAS: %d/5   %.1f s",vidas_,medallas_,tiempo_),460,20,21,TINTA);
    DrawRectangleRounded({18,663,690,48},.25f,8,Color{9,28,48,210});
    DrawText("A / D o FLECHAS: MOVER    ESPACIO: SALTAR    M: MENU",34,678,17,TINTA);
    DrawRectangleRounded({745,675,320,14},.5f,8,Color{20,44,64,210});
    if (x_>0) DrawRectangleRounded({745,675,320*std::min(x_/META,1.0f),14},.5f,8,Color{255,207,94,255});
    DrawText("META",1015,695,15,TINTA);
}
