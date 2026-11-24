# Olimpiadas 2D — C++ + raylib

Proyecto POO para la modalidad **Videojuego 2D**. Esta primera versión contiene dos disciplinas jugables: atletismo (100 m) y natación (50 m), tres atletas por carrera, teclas aleatorias, cronómetro, clasificaciones y registro en archivo.

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

- Menú: `1` atletismo, `2` natación.
- Atletismo: pulsar la letra indicada entre `A`, `S`, `D`, `F`.
- Natación: pulsar la letra indicada entre `J`, `K`, `L`.
- Carrera: `M` vuelve al menú.
- Resultados: `R` revancha, `M` menú.
- `Esc` cierra la ventana (raylib).

Cada acierto avanza al personaje una distancia fija, distinta por deporte. Los rivales avanzan automáticamente. La carrera termina cuando todos alcanzan la meta; se registra el tiempo y posición del jugador en `records.csv` en el directorio desde el que se ejecuta.

## Diseño POO

- `Atleta`: clase base **abstracta**, con estado privado y métodos virtuales.
- `Corredor` y `Nadador`: especializaciones de movimiento y representación visual con `override`.
- `ControlTeclas`: elige una tecla aleatoria distinta de la anterior y comprueba aciertos.
- `Competencia`: mantiene `std::vector<std::unique_ptr<Atleta>>`, actualiza los participantes y calcula resultados.
- `GestorArchivos`: lee y escribe récords en CSV.
- `Juego`: organiza escenas y el ciclo principal.

Ver `docs/diagrama_uml.md` para la relación de clases.

## Mejoras previstas antes de la entrega

Esta es una **base funcional**, no la versión final. Se pueden añadir dificultades, respiración en natación, velocistas con comportamientos más avanzados, animaciones con sprites, sonidos y pantalla de medallas. Para una evaluación de nivel ALTO, conviene justificar las mejoras de lógica y POO, no solo gráficos.
