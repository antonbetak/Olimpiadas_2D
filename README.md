# Olimpiadas 2D — C++ + raylib

Proyecto POO para la modalidad **Videojuego 2D**. El juego contiene tres disciplinas jugables: atletismo (100 m), natación (50 m) y carrera de obstáculos, tres atletas por carrera, teclas aleatorias, cronómetro, clasificaciones y registro en archivo.

## Requisitos

- Windows, Linux o macOS.
- CMake 3.20+ y compilador con C++17.
- Conexión a Internet la **primera vez** que CMake descargue raylib 5.5 (a menos que esté instalada y CMake la encuentre).

## Compilar y ejecutar en Windows (PowerShell)

Desde la carpeta del proyecto:

```powershell
cmake -S . -B build
cmake --build build --config Release
.\build\Release\Olimpiadas2D.exe
```

Con generadores de un solo tipo de configuración (por ejemplo, MinGW), el ejecutable puede quedar en `build\Olimpiadas2D.exe`. Si `cmake` no encuentra compilador, instalar Visual Studio Build Tools con C++ o MinGW compatible y configurar el generador adecuado.

**Nota:** se necesita CMake y compilador C++; la extensión de VS Code sola no basta.

## Controles

- Menú: `1` atletismo, `2` natación, `3` obstáculos, o haz clic en la disciplina.
- Atletismo: pulsar la letra indicada entre `A`, `S`, `D`, `F`.
- Natación: pulsa `Espacio` cuando el indicador esté en la zona verde para dar una brazada; pulsa `B` para recuperar oxígeno (una vez cada 1.5 segundos). Fuera de la zona cuenta como error y no avanzas.
- Obstáculos: `A`/`D` o flechas para moverse; `Espacio`, `W` o flecha arriba para saltar. Esquiva las vallas, recoge medallas y llega a la bandera antes de perder tus tres vidas.
- Cinemática: `Espacio` la salta; comienza automáticamente al terminar.
- Carrera: `M` vuelve al menú.
- Resultados: `R` revancha, `M` menú.
- `Esc` cierra la ventana (raylib).

Cada acierto impulsa al personaje y consume resistencia en atletismo u oxígeno en natación. Con menos del 20% el avance disminuye. La resistencia se recupera al descansar; el oxígeno se recupera pulsando B. En natación debes acertar el ritmo de la barra para avanzar. Los dos rivales avanzan automáticamente: uno sale rápido y pierde fuerza, y el otro varía su ritmo. La carrera termina cuando todos alcanzan la meta; se registra el tiempo y posición del jugador en `records.csv` en el directorio desde el que se ejecuta.

## Diseño POO

- `Atleta`: clase base **abstracta**, con estado privado y métodos virtuales.
- `Corredor` y `Nadador`: especializaciones de movimiento y representación visual con `override`.
- `ControlTeclas`: elige una tecla aleatoria distinta de la anterior y comprueba aciertos.
- `Competencia`: mantiene `std::vector<std::unique_ptr<Atleta>>`, actualiza los participantes y calcula resultados.
- `GestorArchivos`: lee y escribe récords en CSV.
- `Juego`: organiza escenas y el ciclo principal.

Ver `docs/diagrama_uml.md` para la relación de clases.

## Ejecutar en macOS o Linux

```bash
cmake -S . -B build
cmake --build build
./build/Olimpiadas2D
```

## Ideas para la exposición

El objetivo es ganar una carrera en atletismo o natación. `Juego` administra las pantallas y `Competencia` coordina a los atletas y los controles. `Atleta` define una interfaz virtual; `Corredor` y `Nadador` implementan animaciones y recursos diferentes. La colección de atletas permite llamar a esos métodos de forma polimórfica. `GestorArchivos` guarda los resultados y consulta el mejor tiempo. El UML está en `docs/diagrama_uml.md`. Conviene demostrar ambos deportes y explicar cómo cambian el avance, la resistencia y el oxígeno.
