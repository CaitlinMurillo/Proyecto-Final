#include <iostream>
#include "Windows.h"
#include <conio.h>
using namespace std;

/*
Bugs:

Implementacion de clases :
 - Dificultades de nivel (velocidad y modo de juego)  [Facil, Medio y Dificil]
 - Clase serpiente
 - Clase Juego (las dificultades serian funciones de esta clase)

Futuras mejoras:
 - Agregar un menu (Start[Elegir dificultad], Instrucciones, Creditos y Exit)
 - Centrar el programa en la terminal
 - Que el mensaje de Game OVer aparezca en medio del mapa
 - Que al acabar el juego regreses al menu
 - Agregar boost

*/


// Variables globales
int x, y; // Coordenadas de la cabeza 
int tailX[100], tailY[100]; // Array para el cuerpo de la serpiente (almacenar sus posiciones)
int nTail; // tamano de la serpiente
int score;

class Juego {

    bool gameOver;
    const int height = 20; // Dimensiones del mapa
    const int width = 20;
    int score;
    enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN }; // Mover la cabeza 
    eDirection dir; // Direccion de la cabeza

public:
    // Set up del espacio de juego
    void Setup() {
        gameOver = false;
        dir = STOP;
        x = width / 2;
        y = height / 2; // hacer que la cabeza aparezca en el medio del juego
        fruitX = rand() % width;
        fruitY = rand() % height; // crear una fruta random en x y

        /*
        bonusX = rand() % width;
        bonusY = rand() % height; // crear bonus
        */

        score = 0;
    }

    // Dibujar mapa 
    void Draw() {
        //system("cls"); // clear terminal window
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 }); // stopss the flickering

        // dibujar espacio de juego
        for (int i = 0; i < width + 2; i++) // borde superior
            cout << "-";
        cout << endl;

        for (int i = 0; i < height; i++) // 
        {
            for (int j = 0; j < width; j++)
            {
                if (j == 0) // si esta en el borde izquierdo poner #
                    cout << "|";
                if (i == y && j == x) // si i (altura) y j (ancho) son iguales a las coordenadas de la "cabeza" imprimir O
                    cout << "0";
                else if (i == fruitY && j == fruitX) // imprimir la comida en las coordenadas XY
                    cout << "*";
                else { // dibujar cuerpo de la serpiente 
                    bool print = false;
                    for (int k = 0; k < nTail; k++) {
                        if (tailX[k] == j && tailY[k] == i)
                        {
                            cout << "o";
                            print = true;
                        }
                    }
                    if (!print)
                        cout << " ";
                }
                if (j == width - 1) // si esta en el borde derecho poner #
                    cout << "|";
            }
            cout << endl;
        }

        for (int i = 0; i < width + 2; i++) // borde inferior
            cout << "-";
        cout << endl;
        // fin del espacio de juego

        cout << "Score: " << score << endl;
    }

    void Input() {
        // Checar inputs del teclado
        if (_kbhit())
        {
            switch (_getch()) // Switch, recibe entradas del teclado para los controles del juego y demas
            {
            case 'a':
                dir = LEFT;
                break;
            case 'd':
                dir = RIGHT;
                break;
            case 'w':
                dir = UP;
                break;
            case 's':
                dir = DOWN;
                break;
            case 'x':
                gameOver = true;
                break;
            }
        }
    }

    void Logic() {
        int prevX = tailX[0], prevY = tailY[0]; // Variables temporales para guardar la posicion del cuerpo de la serpiente
        int prev2X, prev2Y;
        tailX[0] = x;
        tailY[0] = y;


        // crecer serpiente
        for (int i = 1; i < nTail; i++) {
            prev2X = tailX[i];
            prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }

        // Una vez recibidas las entradas del teclado, mover la serpiente
        switch (dir)
        {
        case LEFT:
            x--; // Para ir a la izquierda, disminuir el valor de x (ancho)
            break;
        case RIGHT:
            x++; // Para ir a la derecha
            break;
        case UP:
            y--; // Para ir hacia arriba, disminuir el valor de y (alto) [coordenadas empiezan en esquina superior]
            break;
        case DOWN:
            y++; // Para ir hacia abajo
            break;
        default:
            break;
        }

        // (Dificultad media, dificil) Checar si la cabeza de la serpiente choco con una pared (salirse del rango del juego)
        if (x > width || x < 0 || y > height - 1 || y < 0) //
            gameOver = true;


        // (Dificultad facil) Que la serpiente no choque contra las paredes
        // if (x >= width) x = 0; else if (x < 0) x = width - 1;
        //if (y >= height) y = 0; else if (y < 0) y = height - 1;


        // Checar si la cabeza de la serpiente choco contra su cola
        for (int i = 0; i < nTail; i++)
            if (tailX[i] == x && tailY[i] == y) // Si la posicion de la cabeza coincide con cualquier elemento i de la cola
                gameOver = true;

        // Operaciones cuando se come la comida
        if (x == fruitX && y == fruitY) {
            // aumentar tamano de la serpiente 
            score = score + 100; // aumentar score (comida normal)
            fruitX = rand() % width;
            fruitY = rand() % height; // una vez que se coma la fruta colocarla en otro lugar del mapasd
            nTail++;
        }
    }

    void dificultadFacil() { // Mapa sin bordes, velocidad baja, , mapa pequeno, NO boost

    }

    void dificultadMedia() { // Mapa grande, velocidad media, Boost y Bonus

    }

    void dificultadDificil() { // Mapa grande, velocidad alta, Boost y Bonus

    }

};

class Serpiente {
    int x, y; // Coordenadas de la cabeza 
    int tailX[100], tailY[100]; // Array para el cuerpo de la serpiente (almacenar sus posiciones)
    int nTail; // tamano de la serpiente

};

class Comida {
protected:
    int fruitX, fruitY;
public:

    void incrementarSerpiente() {
        // Operacion cuando se come la comida
        if (x == fruitX && y == fruitY) {
            score += 100; // aumentar score
            fruitX = rand() % width;
            fruitY = rand() % height; // randomizar la siguiente posicion de la comida 
            nTail++; // aumentar tamano de la cola 
        }
    }
};

class Bonus : public Comida {
protected:
    int bonusX, bonusY;
    int boostX, boostY;

    Bonus() {}
    ~Bonus() {}

    // Opereacion cuando se come Bonus (+500)
    void incrementarSerpiente() {
        if (x == bonusX && y == bonusY) {
            score += 500;
            bonusX = rand() & width;
            bonusY = rand() % height;
            nTail++;
        }
    }

    // Operacion cuando se come Boost (ir mas rapido) 
    void boost() {
        if (x == boostX && y == boostY) {
            // implementar funcion para ir mas rapido
            boostX = rand() & width;
            boostY = rand() & height;
        }
    }

};

int main() {
    return 0;
}
