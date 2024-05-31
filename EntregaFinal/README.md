Juego de la Serpiente en C++

Este proyecto es una implementación del clásico juego de la serpiente en C++ con diferentes niveles de dificultad y algunas características adicionales. El juego se ejecuta en la consola de Windows y permite al usuario elegir entre tres niveles de dificultad: fácil, medio y difícil.

Funcionalidades

Dificultad Fácil: Un mapa pequeño con movimiento envolvente y una velocidad baja.
Dificultad Media: Un mapa mediano con elementos de bonificación y una velocidad moderada.
Dificultad Difícil: Un mapa grande con elementos de bonificación y una velocidad alta.


Estructura del Código

Clase Base Juego
Propósito: Proveer una estructura común para cualquier tipo de juego.
Métodos Virtuales Puros: Setup(), Draw(), Input(), Logic(), run()
Atributos: gameOver, height, width, score, speed, dir
Manejo de Consola: hConsole, buffer, bufferSize, characterPos, consoleWriteArea
