#include <iostream>
#include <conio.h>
#include <windows.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

class Juego {
protected:
    bool gameOver;
    int height; // Dimensiones del mapa
    int width;
    int score;
    int speed; // Velocidad del juego
    enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN }; // Mover la cabeza
    eDirection dir; // Dirección de la cabeza

    HANDLE hConsole;
    CHAR_INFO* buffer;
    COORD bufferSize;
    COORD characterPos;
    SMALL_RECT consoleWriteArea;

public:

    Juego() : gameOver(false), score(0), dir(STOP), height(20), width(20), speed(100) {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // inicializa un identificador de la consola estándar del Windows
        bufferSize = { (short)(width + 2), (short)(height + 2) };// establecemos un objeto con las dimensiones del búfer de la pantalla de la consola con un ancho de + 2 y un alto de + 2
        buffer = new CHAR_INFO[bufferSize.X * bufferSize.Y];
        characterPos = { 0, 0 }; // lo inicializamos en la posición (0,0), que es la posición inicial del cursor en la pantalla
        consoleWriteArea = { 0, 0, bufferSize.X - 1, bufferSize.Y - 1 }; // lo establecemos como un rectángulo que abarca todo el búfer de pantalla, excepto los bordes
    }

    ~Juego() //mandamos a llamar a nuestro destructor que libera la memoria asignada para el buffer utilizando el delete
    {
        delete[] buffer;
    }

    virtual void Setup() = 0;
    virtual void Draw() = 0;
    virtual void Input() = 0;
    virtual void Logic() = 0;
    virtual void run() = 0;
    //las funciones no tienen una implementación en la clase actual, lo cual debemos implementarlas en cualquier clase que sea heredada de nuestra clase padre
    void DrawGameOver() {
        std::string message = "GAME OVER";
        int startX = (width + 2) / 2 - message.size() / 2;
        int startY = (height + 2) / 2;

        for (int i = 0; i < message.size(); i++) {
            buffer[startY * (width + 2) + startX + i].Char.AsciiChar = message[i];
            buffer[startY * (width + 2) + startX + i].Attributes = 0x0C; // Red text
        }

        WriteConsoleOutputA(hConsole, buffer, bufferSize, characterPos, &consoleWriteArea);
    }

    bool isGameOver() const { return gameOver; }
    void setGameOver(bool state) { gameOver = state; }
    void setDirection(eDirection d) { dir = d; }
    eDirection getDirection() const { return dir; }
};

class Serpiente : public Juego {
protected:
    int x, y; // Coordenadas de la cabeza
    int tailX[100], tailY[100]; // Array para el cuerpo de la serpiente (almacenar sus posiciones)
    int nTail; // tamaño de la serpiente

public:
    Serpiente() : x(0), y(0), nTail(0) {} //definimos nuestro constructor

    void Setup() override {
        gameOver = false; //establecemos el game-over en falso
        dir = STOP; //dir es la variable que indica la dirección de la serpiente y el stop es el valor que nos indica que la serpiente no se está moviendo
        x = width / 2;
        y = height / 2; //la mitad del ancho y la altura respectivamente
        score = 0;
        nTail = 0;
    }

    void Draw() override {
        for (int i = 0; i < bufferSize.X * bufferSize.Y; i++) {
            buffer[i].Char.AsciiChar = ' ';
            buffer[i].Attributes = 0x07;
        }

        for (int i = 0; i < width + 2; i++) { // borde superior
            buffer[i].Char.AsciiChar = '-';
        }

        for (int i = 1; i <= height; i++) {
            buffer[i * (width + 2)].Char.AsciiChar = '|'; // borde izquierdo
            buffer[(i + 1) * (width + 2) - 1].Char.AsciiChar = '|'; // borde derecho
        }

        for (int i = 0; i < width + 2; i++) { // borde inferior
            buffer[(height + 1) * (width + 2) + i].Char.AsciiChar = '-';
        }

        buffer[(y + 1) * (width + 2) + (x + 1)].Char.AsciiChar = '0'; // dibujar cabeza de la serpiente

        for (int i = 0; i < nTail; i++) {
            buffer[(tailY[i] + 1) * (width + 2) + (tailX[i] + 1)].Char.AsciiChar = 'o';
        }

        for (int i = 0; i < bufferSize.X * bufferSize.Y; i++) {
            buffer[i].Attributes = 0x07;
        }

        WriteConsoleOutputA(hConsole, buffer, bufferSize, characterPos, &consoleWriteArea);
        // hconsole representa el identificador del búfer de pantalla de la consola en el que deseas escribir
        // el buffer especifica el contenido que deseas escribir en la pantalla
        // el bufferSize se establece en {1, 1}, lo que significa que solo se está escribiendo un carácter
        // el characterPos representa la posición dentro del bufer
        // &consoleWriteArea define la región en la pantalla de la consola donde escribimos el contenido
        // Mostrar puntuación
        SetConsoleCursorPosition(hConsole, { 0, (short)(height + 2) });
        cout << "Score: " << score << endl;
    }

    void Input() override {
        if (_kbhit()) //aqui solamente declaramos cuales son las teclas que se utilizaran al mover la serpiente
        {
            switch (_getch()) {
            case 'a':
                if (dir != RIGHT) dir = LEFT;
                break;
            case 'd':
                if (dir != LEFT) dir = RIGHT;
                break;
            case 'w':
                if (dir != DOWN) dir = UP;
                break;
            case 's':
                if (dir != UP) dir = DOWN;
                break;
            case 'x':
                gameOver = true;
                break;
            }
        }
    }

    void Logic() override {
        int prevX = tailX[0], prevY = tailY[0];
        int prev2X, prev2Y;
        tailX[0] = x;
        tailY[0] = y;
        // se almacenan las coordenadas actuales de la cabeza del objeto

        for (int i = 1; i < nTail; i++)
        {
            prev2X = tailX[i];
            prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = prev2X;
            prev2Y = prev2Y;
        }

        switch (dir) {
        case LEFT:
            x--;
            break;
        case RIGHT:
            x++;
            break;
        case UP:
            y--;
            break;
        case DOWN:
            y++;
            break;
        default:
            break;
        }

        // Wrap-around logica para que no haya colisiones con los bordes del mapa para nivel facil
        if (width == 10 && height == 10) { // Only for easy difficulty
            if (x >= width) x = 0; else if (x < 0) x = width - 1;
            if (y >= height) y = 0; else if (y < 0) y = height - 1;
        }
        else {
            // aqui entran las colisiones con las paredes para la dificultad media y dificil
            if (x >= width || x < 0 || y >= height || y < 0)
                gameOver = true;
        }

        for (int i = 0; i < nTail; i++)
            if (tailX[i] == x && tailY[i] == y)
                gameOver = true;
    }

    void run() override {
        Setup();
        auto lastTime = steady_clock::now();
        while (!isGameOver()) {
            auto currentTime = steady_clock::now();
            auto elapsedTime = duration_cast<milliseconds>(currentTime - lastTime);
            if (elapsedTime.count() >= speed) {
                Input();
                Logic();
                Draw();
                lastTime = currentTime;
            }
            else {
                Input();
            }
        }
        DrawGameOver(); // muestra un mensaje de game over
        Sleep(2000); // espera 2 segundo spra volver a mostrar el menú
    }
};

class Comida : public Serpiente {
protected:
    int fruitX, fruitY;

public:
    Comida() : fruitX(0), fruitY(0) {}

    void Setup() override {
        Serpiente::Setup();
        fruitX = rand() % width;
        fruitY = rand() % height;
    }

    void Draw() override {
        for (int i = 0; i < bufferSize.X * bufferSize.Y; i++) {
            buffer[i].Char.AsciiChar = ' ';
            buffer[i].Attributes = 0x07;
        }

        for (int i = 0; i < width + 2; i++) { // borde superior
            buffer[i].Char.AsciiChar = '-';
        }

        for (int i = 1; i <= height; i++) {
            buffer[i * (width + 2)].Char.AsciiChar = '|'; // borde izquierdo
            buffer[(i + 1) * (width + 2) - 1].Char.AsciiChar = '|'; // borde derecho
        }

        for (int i = 0; i < width + 2; i++) { // borde inferior
            buffer[(height + 1) * (width + 2) + i].Char.AsciiChar = '-';
        }

        buffer[(y + 1) * (width + 2) + (x + 1)].Char.AsciiChar = '0'; // dibujar cabeza de la serpiente

        for (int i = 0; i < nTail; i++) {
            buffer[(tailY[i] + 1) * (width + 2) + (tailX[i] + 1)].Char.AsciiChar = 'o';
        }

        buffer[(fruitY + 1) * (width + 2) + (fruitX + 1)].Char.AsciiChar = '*'; // dibujar fruta

        WriteConsoleOutputA(hConsole, buffer, bufferSize, characterPos, &consoleWriteArea);

        // Mostrar puntuación
        SetConsoleCursorPosition(hConsole, { 0, (short)(height + 2) });
        cout << "Score: " << score << endl;
    }

    void Logic() override {
        Serpiente::Logic();

        if (x == fruitX && y == fruitY) {
            score += 100;
            fruitX = rand() % width;
            fruitY = rand() % height;
            nTail++;
        }
    }
};

class Bonus : public Comida {
protected:
    int bonusX, bonusY;
    int boostX, boostY;
    //declaramos las variables que nos serviran para la comida bonus

public:
    Bonus() : bonusX(0), bonusY(0), boostX(0), boostY(0) {}

    void Setup() override {
        Comida::Setup();
        bonusX = rand() % width;
        bonusY = rand() % height;
        boostX = rand() % width;
        boostY = rand() % height;
    }

    void Draw() override {
        for (int i = 0; i < bufferSize.X * bufferSize.Y; i++) {
            buffer[i].Char.AsciiChar = ' ';
            buffer[i].Attributes = 0x07;
        }

        for (int i = 0; i < width + 2; i++) { // borde superior
            buffer[i].Char.AsciiChar = '-';
        }

        for (int i = 1; i <= height; i++) {
            buffer[i * (width + 2)].Char.AsciiChar = '|'; // borde izquierdo
            buffer[(i + 1) * (width + 2) - 1].Char.AsciiChar = '|'; // borde derecho
        }

        for (int i = 0; i < width + 2; i++) { // borde inferior
            buffer[(height + 1) * (width + 2) + i].Char.AsciiChar = '-';
        }

        buffer[(y + 1) * (width + 2) + (x + 1)].Char.AsciiChar = '0'; // dibujar cabeza de la serpiente

        for (int i = 0; i < nTail; i++) {
            buffer[(tailY[i] + 1) * (width + 2) + (tailX[i] + 1)].Char.AsciiChar = 'o';
        }

        buffer[(fruitY + 1) * (width + 2) + (fruitX + 1)].Char.AsciiChar = '*'; // dibujar fruta
        buffer[(bonusY + 1) * (width + 2) + (bonusX + 1)].Char.AsciiChar = '+'; // dibujar bonus
        buffer[(boostY + 1) * (width + 2) + (boostX + 1)].Char.AsciiChar = '>'; // dibujar boost

        WriteConsoleOutputA(hConsole, buffer, bufferSize, characterPos, &consoleWriteArea);

        // Mostrar puntuación
        SetConsoleCursorPosition(hConsole, { 0, (short)(height + 2) });
        cout << "Score: " << score << endl;
    }

    void Logic() override {
        Comida::Logic();

        if (x == bonusX && y == bonusY) {
            score += 500;
            bonusX = rand() % width;
            bonusY = rand() % height;
            nTail++;
        }

        if (x == boostX && y == boostY) {
            // implementar función para ir más rápido
            boostX = rand() % width;
            boostY = rand() % height;
        }
    }
};

class DificultadFacil : public Comida {
public:
    void Setup() override {
        width = 10;
        height = 10;//estableceremos el ancho y la altura a una medida de 10 unidades
        speed = 250;//le ponemos una velocidad de 250 para que vaya lento
        bufferSize = { (short)(width + 2), (short)(height + 2) };
        delete[] buffer;
        buffer = new CHAR_INFO[bufferSize.X * bufferSize.Y];
        consoleWriteArea = { 0, 0, bufferSize.X - 1, bufferSize.Y - 1 };
        Comida::Setup();
    }

    void run() override 
    {
        Setup();
        auto lastTime = steady_clock::now();//Aquí se guarda el tiempo actual en la variable lastTime
        while (!isGameOver()) {
            auto currentTime = steady_clock::now();
            auto elapsedTime = duration_cast<milliseconds>(currentTime - lastTime);//calculamos el tiempo transcurrido desde la última actualización del lastTime
            if (elapsedTime.count() >= speed) 
            {
                Input();
                Logic();
                Draw();
                lastTime = currentTime;
            }
            else 
            {
                Input();
            }
        }
        DrawGameOver(); // muestra un mensaje de game over
        Sleep(2000); // espera 2 segundo para volver a mostrar el menú
    }
};

class DificultadMedia : public Bonus {
public:
    void Setup() override {
        width = 20;  
        height = 20; // con el width y el heigth establecemos el ancho y la altura de la pantalla de juego a 20 unidades
        speed = 100; // Establecemos que ira la serpiente ira a una velocidad de 100
        bufferSize = { (short)(width + 2), (short)(height + 2) }; //establecemos el tamaño del bufer a 2 unidades de ancho y altura
        delete[] buffer;
        buffer = new CHAR_INFO[bufferSize.X * bufferSize.Y];
        consoleWriteArea = { 0, 0, bufferSize.X - 1, bufferSize.Y - 1 };// definimos el área en la que escribiremos en la consola la cual el área comienza en la posición (0,0) y termina en la posición definida como (bufferSize.X - 1, bufferSize.Y - 1)
        Bonus::Setup();
    }

    void run() override {
        Setup();
        auto lastTime = steady_clock::now(); //Aquí se guarda el tiempo actual en la variable lastTime
        while (!isGameOver()) {
            auto currentTime = steady_clock::now();
            auto elapsedTime = duration_cast<milliseconds>(currentTime - lastTime);//calculamos el tiempo transcurrido desde la última actualización del lastTime
            if (elapsedTime.count() >= speed) // Si el tiempo es mayor o igual a la velocidad establecida, mandamos a llamar las funciones input, logic, draw y actualizamos el lastime
            {
                Input();
                Logic();
                Draw();
                lastTime = currentTime;
            }
            else //sino solamente mandamos a llamar la funcion de input
            {
                Input();
            }
        }
        DrawGameOver(); // muestra el mensaje de game over
        Sleep(2000); // espera dos segundos para volver a mostrar el menú
    }
};

class DificultadDificil : public Bonus {
public:
    void Setup() override {
        width = 30;
        height = 30;// hacemos la misma declaracion de variables como en la clase dificultad media la cual establecemos el ancho y la altura de la pantalla de juego a 30 unidades
        speed = 50;// La velocidad la ponemos en 50 para que vaya mas rapido
        bufferSize = { (short)(width + 2), (short)(height + 2) };
        delete[] buffer;
        buffer = new CHAR_INFO[bufferSize.X * bufferSize.Y];
        consoleWriteArea = { 0, 0, bufferSize.X - 1, bufferSize.Y - 1 };
        Bonus::Setup();
    }

    void run() override //elaboramos las mismas declaraciones de variables ya que lo unico que cambiamos es la altura, el ancho y la velocidad para que sea mas dificil
    {
        Setup();
        auto lastTime = steady_clock::now();
        while (!isGameOver()) {
            auto currentTime = steady_clock::now();
            auto elapsedTime = duration_cast<milliseconds>(currentTime - lastTime);
            if (elapsedTime.count() >= speed) 
            {
                Input();
                Logic();
                Draw();
                lastTime = currentTime;
            }
            else 
            {
                Input();
            }
        }
        DrawGameOver(); // muestra un mensaje de game over 
        Sleep(2000); // espera 2 segundos para volver al menu
    }
};

void mostrarMenu() //solamente el menu el cual puedes elegir el diferente tipo de dificultad para jugar
{
    cout << "Seleccione la dificultad:\n";
    cout << "1. Facil\n";
    cout << "2. Medio\n";
    cout << "3. Dificil\n";
    cout << "4. Salir\n";
}

int main() //solo mandamos a llamar y aplicar un swith para aplicar los diferentes tipos de niveles
{
    int opcion;
    bool continuar = true;

    while (continuar) {
        system("cls"); // Limpiar la pantalla
        mostrarMenu();
        cin >> opcion;

        switch (opcion) {
        case 1: {
            DificultadFacil juego;
            juego.run();
            break;
        }
        case 2: {
            DificultadMedia juego;
            juego.run();
            break;
        }
        case 3: {
            DificultadDificil juego;
            juego.run();
            break;
        }
        case 4:
            continuar = false;
            system("cls"); // Limpiar la pantalla antes de salir
            break;
        default:
            cout << "Opcion invalida.\n";
            break;
        }
    }
    return 0;
}
