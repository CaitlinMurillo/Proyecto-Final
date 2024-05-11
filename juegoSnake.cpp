#include <iostream>
#include <conio.h>
using namespace std;

// Variables global
bool gameOver;
const int height = 20; // Dimensiones del mapa
const int width = 20;
int x, y; // Coordenadas de la cabeza 
int fruitX, fruitY; // Coordenadas de la comida
int score;
enum eDirection {STOP = 0, LEFT, RIGHT, UP, DOWN}; // Mover la cabeza 
eDirection dir; // Direccion de la cabeza

// prototipos
void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2; 
    y = height / 2; // hacer que la cabeza aparezca en el medio del juego
    fruitX = rand() % width;
    fruitY = rand() % height; // crear una fruta random en x y
    score = 0;
}

void Draw() {
    system("cls"); // clear terminal window
    // dibujar espacio de juego
    for (int i = 0; i < width+1; i++) // borde superior
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) // 
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0) // si esta en el borde izquierdo poner #
                cout << "#";
            if (i == y && j == x) // si i (altura) y j (ancho) son iguales a las coordenadas de la "cabeza" imprimir O
                cout << "0";
            else if (i == fruitY && j == fruitX) // imprimir la comida en las coordenadas XY
                cout << "F";
            else
                cout << " ";
            if (j == width - 1) // si esta en el borde derecho poner #
                cout << "#";
        }
        cout << endl;
    }

    for (int i = 0; i < width+1; i++) // borde inferior
        cout << "#";
    cout << endl;
    // fin del espacio de juego
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
    // Una vez recibidas las entradas del teclado mover la serpiente
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
}

int main() {
    Setup();
    // 
    while (!gameOver) // mientras gameOver sea falso
    {
        Draw();
        Input();
        Logic();
    }
    return 0;
}
