#pragma once
#include <vector>
#include <random>
class ControlTeclas {
private:
    std::vector<int> teclas_;
    int teclaActual_;
    std::mt19937 generador_;
    void siguiente();
public:
    explicit ControlTeclas(bool natacion);
    int teclaActual() const;
    bool procesar(int teclaPresionada);
};
