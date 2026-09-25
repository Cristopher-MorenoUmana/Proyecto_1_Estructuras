# Tetris - Proyecto de Estructuras de Datos en C++

Este proyecto es una implementación completa del clásico juego Tetris desarrollada en C++, orientada a la aplicación práctica de Estructuras de Datos dinámicas creadas desde cero mediante nodos y punteros (sin uso de contenedores de la STL para las mecánicas del juego).

## Biblioteca Gráfica Utilizada

Para el renderizado de gráficos, manejo de ventanas, eventos de teclado y ratón, este proyecto utiliza la biblioteca **SFML 3** *(Simple and Fast Multimedia Library)*.

## Requisitos Previos

Antes de compilar, asegúrate de tener instalado:
* **Visual Studio** (2019, 2022 o superior) con soporte para C++17 o C++20.
* **SFML 3**: Descargado y configurado en tu entorno de desarrollo.

Además, el proyecto requiere una carpeta llamada `assets/` en el mismo directorio donde se ejecute el programa (junto al `.exe`). Esta carpeta debe contener:
* `assets/bloques_De_Colores.png` (Spritesheet de las piezas).
* `assets/fonts/GradvisRegular-lxoyd.ttf` y `pix32.ttf` (Fuentes tipográficas).
* `assets/buttons/` (Imágenes para los estados normales y *hover* de los botones de la interfaz).
* El archivo `highscores.txt` se generará automáticamente en esta misma ruta al guardar el primer puntaje.

## Cómo Compilar y Ejecutar

### Usando Visual Studio 

1. Abre el archivo de solución (`.sln`) del proyecto en Visual Studio.
2. Asegúrate de configurar la plataforma de destino en **x64** (ya sea en modo *Debug* o *Release*).
3. Verifica que las rutas de **SFML 3** estén correctamente configuradas en las propiedades del proyecto:
   * **C/C++ -> General -> Directorios de inclusión adicionales:** (Ruta a la carpeta `include` de SFML).
   * **Vinculador -> General -> Directorios de bibliotecas adicionales:** (Ruta a la carpeta `lib` de SFML).
   * **Vinculador -> Entrada -> Dependencias adicionales:** (Asegúrate de enlazar `sfml-graphics.lib`, `sfml-window.lib` y `sfml-system.lib`, añadiendo `-d` si estás en Debug).
4. Copia los archivos `.dll` de la carpeta `bin` de SFML al directorio donde se compilará tu ejecutable.
5. Presiona **Ctrl + F5** (Iniciar sin depurar) o compila desde el menú `Compilar -> Compilar solución` e inicia el ejecutable manualmente.
