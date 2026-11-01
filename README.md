# Tarea 2 INF134
Juego con temática de Pokemón

## Contenido

| Archivo | Descripción |
|---|---|
| `tarea2.cpp` | Programa principal. Define las estructuras (`Habitacion`, `Enemigo`, `Evento`, `Mejora`, `Arco`), la clase `Jugador` y la función `leerMapa`, que carga `data.map`. |
| `Recorrer map.cpp` | Versión alternativa/anterior del lector del mapa. Solo lee habitaciones, enemigos y eventos (no lee arcos ni mejoras). |
| `test.cpp` | Programa de prueba que imprime "Hola mundo". |
| `data.map` | Datos del mundo: 18 habitaciones, 17 arcos, 4 enemigos, 3 eventos y 5 mejoras de combate. |
| `settings.json` | Ruta del compilador (`g++` de MSYS2/UCRT64) para la extensión C/C++. |
| `tasks.json` | Tarea de compilación que ejecuta `g++ -g` sobre el archivo activo. |
| `launch.json` | Configuraciones de depuración (GDB con g++, depuración con `cppvsdbg` y ejecución de `tarea2.exe`). |
| `c_cpp_properties.json` | Configuración de IntelliSense: C17 / C++17 y compilador de MSYS2. |
| `tarea2.exe`, `test.exe` | Ejecutables compilados (binarios de Windows). |
| `.vscode/` (anidada) | Copia de `tasks.json` y `launch.json`. |

## El mapa (`data.map`)

El archivo tiene varias secciones:

- **HABITACIONES**: cada habitación tiene un ID, un nombre, un tipo entre paréntesis (`INICIO`, `FIN`, `EVENTO` o `COMBATE`) y una descripción.
- **ARCOS**: conexiones dirigidas `origen -> destino` entre habitaciones.
- **ENEMIGOS**: nombre, vida, ataque, velocidad, precisión y probabilidad de aparición.
- **EVENTOS**: eventos con probabilidad y dos opciones (A y B), cada una con su efecto sobre el jugador.
- **MEJORAS DE COMBATE**: bonificaciones de vida, precisión, ataque, recuperación y velocidad.

La historia comienza en el *Bosque lumirinto* (ID 0), donde el jugador debe reencontrarse con su entrenador. Hay varios finales posibles, como reencontrarse en la Ciudad Artejo, el Centro Pokémon o la Gran Estación, o terminar en el Lago de lava, donde Moltres puede ayudar o dominar al jugador.

Cada habitación puede tener hasta tres conexiones, almacenadas como `izquierda`, `centro` y `derecha`, por lo que la estructura de datos es un árbol ternario.

## Cómo compilar y ejecutar

### En VS Code (Windows)

1. Instalar MSYS2 en `C:\msys64` y, dentro de MSYS2 UCRT64, instalar `g++` y `gdb`.
2. Instalar la extensión **C/C++** de Microsoft en VS Code.
3. Abrir la carpeta `TAREA2` y el archivo `.cpp` que se quiere compilar.
4. Compilar con **Ctrl+Shift+B** (usa la tarea de `tasks.json`).
5. Depurar con **F5** (usa `launch.json`).

### Desde la terminal

```bash
g++ -g tarea2.cpp -o tarea2
./tarea2
```

Ejecutar desde la carpeta donde está `data.map`, porque el programa lo abre con una ruta relativa.

## Estado actual

- `tarea2.cpp` carga el mapa, valida que exista la habitación inicial y muestra el estado del jugador. El bucle principal del juego todavía no está implementado.
- Las funciones `conectarHabitaciones`, `buscarHabitacion` y `mostrarHabitaciones` existen, pero `main` no las llama directamente.
- La sección `MEJORAS DE COMBATE` del archivo `data.map` no se lee en ninguna de las dos versiones del código. La función `aplicarMejora` usa opciones numeradas del 1 al 5.
- `Recorrer map.cpp` no procesa la sección `ARCOS`, así que no construye las conexiones entre habitaciones.
- Hay rutas configuradas solo para Windows (`C:\msys64\...`), así que en otros sistemas operativos será necesario cambiar `settings.json`, `tasks.json` y `c_cpp_properties.json`.
- `test.cpp` sirve únicamente para verificar que el compilador funciona.

## Autores
R. Palma J. Bravo
