#include "ControlTeclas.h"
#include "raylib.h"
#include <algorithm>
ControlTeclas::ControlTeclas(bool natacion)
    : teclas_(natacion ? std::vector<int>{KEY_J, KEY_K, KEY_L}
                       : std::vector<int>{KEY_A, KEY_S, KEY_D, KEY_F}),
      teclaActual_(teclas_.front()), generador_(std::random_device{}()) { siguiente(); }
void ControlTeclas::siguiente() {
    std::uniform_int_distribution<std::size_t> dist(0, teclas_.size() - 1);
    int nueva;
    do { nueva = teclas_[dist(generador_)]; } while (nueva == teclaActual_);
    teclaActual_ = nueva;
}
int ControlTeclas::teclaActual() const { return teclaActual_; }
bool ControlTeclas::procesar(int teclaPresionada) {
    if (teclaPresionada != teclaActual_) return false;
    siguiente();
    return true;
}
