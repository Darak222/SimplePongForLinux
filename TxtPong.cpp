#include <iostream>
#include <time.h>
#include <ncurses.h>
#include <chrono>
#include <math.h>

using namespace std;
using namespace std::chrono;

enum direction { STOP = 0, LEFT = 1, UPLEFT = 2, DOWNLEFT = 3, RIGHT = 4, UPRIGHT = 5, DOWNRIGHT = 6 }; //deklarowanie kierunków za pomocą numerów

class Ball{ //Klasa odpowiadająca za poruszanie piłki
    private:
    int x, y;
    int originX, originY;
    direction dir;
    public:
    Ball(int posX, int posY){ //Zapamiętanie oryginalnych pozycji piłki, potrzebne przy restarcie gry
        originX = posX;
        originY = posY;
        x = posX;
        y = posY;
        dir = STOP;
    }

    void Reset(){ //Reset pozycji piłki do oryginalnej
        x = originX;
        y = originY;
        dir = STOP;
    }

    void changeDir(direction d){ //Zmiana kierunnku
        dir = d;
    }

    int getX(){  //Pobranie X
        return x;
    }

    int getY(){ //Pobranie Y
        return y;
    }

    inline direction getDir(){ //Pobranie kierunku piłki
         return dir;
    }

    void randomDirection(){ //Losowanie kierunku piłki
        dir = (direction)((rand() % 6) + 1); // rand() % 6 aby losować kierunek piłki, +1 aby kierunek nie był STOP
    }

    void Move(){ //Ruch piłki w zależności od kierunku
        switch(dir){
            case STOP:
                break;
            case LEFT:
                x--;
                break;
            case RIGHT:
                x++;
                break;
            case UPLEFT:
                x--;
                y--;
                break;
            case DOWNLEFT:
                x--;
                y++;
                break;
            case UPRIGHT:
                x++;
                y--;
                break;
            case DOWNRIGHT:
                x++;
                y++;
                break;
            default:
                break;
        }
    }
    /*friend ostream & operator<<(ostream & o, Ball b){ //Funkcja pomocnnicza do wypisywania koordynatów
        o << "Piłka [" << b.x << "," << b.y << "][" << b.dir<< "]" << endl;
        return o;
    }*/


};

class Paddle{ //Klasa odpowiadająca za paletki graczy
    private:
    int x = 0, y = 0;
    int originX, originY;
    public:

    Paddle(int posX, int posY){ //Zapamiętanie początkowych koordynatów paletek, przydatne przy resecie
        originX = posX;
        originY = posY;
        x = originX;
        y = originY;
    }

    void Reset(){ //Reset paletek do oryginalnych pozycji
        x = originX;
        y = originY;
    }

    int getX(){ //Pobranie X paletki
        return x;
    }

    int getY(){ //Pobranie Y paletki
        return y;
    }

    void moveUp(){ //Ruch paletki w górę
        y--;
    }

    void moveDown(){ //Ruch paletki w dół
        y++;
    }
    //Pomocnik do debugowania
    /*friend ostream & operator<<(ostream & o, Paddle b){ // Pomocnicza funkcja do wypisywania koordynatów
        o << "Paletka [" << b.x << "," << b.y << "]" << endl;
        return o;
    }*/



};

class AIController{ //Klasa odpowiadająca za rozgrywkę
    private:
    int width, height;
    int score1, score2;
    int victory;
    int game_speed;
    int secondsCounter;
    int nanosecondCounter;
    bool quit;
    bool displaySurvivalTimer;
    Ball * ball;
    Paddle *player1;
    Paddle *player2;
    public:
    AIController(int wid, int hei){ //Inicjalizacja wszystkich składników niezbędnych do rozgrywki
        quit = false;
        score1 = score2 = 0; //inicjalizacja wyniku
        victory = 0; //punkty potrzebne do zwycięstwa
        game_speed = 0; //prędkość piłki
        displaySurvivalTimer = false; //Licznik sekund dla trybu super sonic
        secondsCounter = 0;
        nanosecondCounter = 0;
        width = wid; //szerokość i wysokość okna
        height = hei;
        ball = new Ball(wid / 2, hei / 2); //inicjalizacja piłki
        player1 = new Paddle(1, hei / 2 - 3); //inicjalizacja paletki gracza 1 (lewa)
        player2 = new Paddle(wid - 2, hei / 2 - 3); //inicjalizacja paletki gracza 2 (prawa)
    }

    ~AIController(){ //Destruktor
        delete ball, player1, player2;
    }

    void ScorePlus(Paddle * player){ //Zwiększenie wyniku gracza
        if(player == player1){
            score1++;
        }
        else if(player == player2){
            score2++;
        }
        ball->Reset(); //Piłka omineła paletkę jednego z graczy więc obiekty resetują się do oryginalnych pozycji
        player1->Reset();
        player2->Reset();
    }

    void Draw(int timer){ //Funkcja rysująca
        clear();
        printw("Wynik gracza 1: %d\t", score1);
        printw("Wynik gracza 2: %d\n", score2);
        printw("Gra do %d punktów\n", victory);

        if(game_speed == 20){ //Survival timer tylko dla trybu super sonic
            printw("Udalo Ci sie przetrwac juz %d s\n", secondsCounter);
            nanosecondCounter += timer;
            if(abs(nanosecondCounter) >= 350000){
                nanosecondCounter = 0;
                secondsCounter++;
            }
        }

        for(int i = 0; i  < width + 2; i++){
            printw("#");
        }

        printw("\n");

        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                int ballx = ball->getX();
                int bally = ball->getY();
                int player1x = player1->getX();
                int player2x = player2->getX();
                int player1y = player1->getY();
                int player2y = player2->getY();

                if(j == 0){
                    printw("#");
                }

                if(ballx == j && bally == i){
                    printw("O"); //Piłka
                }
                else if(player1x == j && player1y == i){
                    printw("@"); //Paletka gracza 1
                }
                else if(player2x == j && player2y == i){
                    printw("@"); //Paletka gracza 2
                }
                else if(player1x == j && player1y + 1 == i){
                    printw("@"); //Paletka gracza 1
                }
                else if(player1x == j && player1y + 2 == i){
                    printw("@"); //Paletka gracza 1
                }
                else if(player1x == j && player1y + 3 == i){
                    printw("@"); //Paletka gracza 1
                }
                else if(player2x == j && player2y + 1 == i){
                    printw("@"); //Paletka gracza 2
                }
                else if(player2x == j && player2y + 2 == i){
                    printw("@"); //Paletka gracza 2
                }
                else if(player2x == j && player2y + 3 == i){
                    printw("@"); //Paletka gracza 2
                }

                else{
                    printw(" "); //Pusta przestrzeń;
                }

                if(j == width - 1){
                    printw("#");
                }
            }
            printw("\n");
        }

        for(int i = 0; i < width + 2; i++){
            printw("#");
        }

        printw("\n");

    }

    void Input(){ //Ruch gracza
        ball->Move(); //Ruch piłki

        int ballx = ball->getX();
        int bally = ball->getY();
        int player1y = player1->getY();
        int player2y = player2->getY();

        int current = getch();

        switch(current){ //Sterowanie
            case 'q': //Wyjście z gry
                quit = true;
                break;
            case 'w': //Ruch gracza 1 w górę
                if(player1y > 0){
                    player1->moveUp();
                }
                break;
            case KEY_UP: //Ruch gracza 2 w górę
                if(player2y > 0){
                    player2->moveUp();
                }
                break;
            case 's': //Ruch gracza 1 w dół
                if(player1y + 4 < height){
                    player1->moveDown();
                }
                break;
            case KEY_DOWN: //Ruch gracza 2 w dół
                if(player2y + 4 < height){
                    player2->moveDown();
                }
                break;
            default:
                break;

        }

        if(ball->getDir() == STOP){ //Jeśli gra rozpoczyna się to złap losowy kierunek piłki
            ball->randomDirection();
        }

        napms(game_speed); //Prędkość gry
    }

    void CollisionDetection(){ //Kolizja piłki ze ścianami i paletkami

        int ballx = ball->getX();
        int bally = ball->getY();
        int player1x = player1->getX();
        int player2x = player2->getX();
        int player1y = player1->getY();
        int player2y = player2->getY();

        //kolizja z lewą paletką
        for(int i = 0; i < 4; i++){
            if(ballx == player1x + 1){
                if(bally == player1y + i){
                    ball->changeDir((direction)((rand() % 3) + 4)); //Zmiana ruchu piłki po odbiciu, +4 aby uniknąć kierunku w lewo lub STOP
                }
            }
        }
        //kolizja z prawą paletką
        for(int i = 0; i < 4; i++){
            if(ballx == player2x - 1){
                if(bally == player2y + i){
                    ball->changeDir((direction)((rand() % 3) + 1)); //Zmiana ruchu piłki po odbiciu, +1 aby uniknąć STOP
                }
            }
        }
        //odbicie piłki od dolnej ściany
        if(bally == height - 1){
            ball->changeDir(ball->getDir() == DOWNRIGHT ? UPRIGHT : UPLEFT); //Jeśli piłka leci w kierunku prawym dolnym przy uderzeniu to zmień na prawy górny, inaczej zmień na lewy górny
        }
        //odbici piłki od górnej ściany
        if(bally == 0){
            ball->changeDir(ball->getDir() == UPRIGHT ? DOWNRIGHT : DOWNLEFT); //Jeśli piłka leci w kierunku prawy górny przy uderzeniu to zmień na prawy dolny, inaczej zmień na lewy dolny
        }
        //kolizja piłki z prawą ścianą
        if(ballx == width - 1){
            ScorePlus(player1);
        }
        //kolizja piłki z lewą ścianą
        if(ballx == 0){
            ScorePlus(player2);
        }

    }

    void victoryMessage(){ //Wybór gracza do ilu punktów chce grać
        printw("Wybierz do ilu punktów chcesz grac: \n");
        printw("1. Do 5 punktów,\n");
        printw("2. Do 10 punktów, \n");
        printw("3 lub wiecej. Do 15 punktów.\n");
    }

    void setVictoryPoints(int condition){ //Ustawienie punktów potrzebnych do wygranej
        if(condition == '1'){
            victory = 5;
        }
        if(condition == '2'){
            victory = 10;
        }
        if(condition >= '3'){
            victory = 15;
        }
    }

    void checkVictorious(){ //Sprawdź czy któryś gracz zdobył punkty potrzebne do wygranej
        if(victory == score1 || victory == score2){
            quit = true; //Jeśli któryś z graczy wygrał to zakończ grę
        }
    }

    void winMsg(){ //Wiadomość o wygranej
        if(score1 == victory){
            printw("Wygrywa gracz 1!\n");
        }
        if(score2 == victory){
            printw("Wygrywa gracz 2!\n");
        }
        if(score1 == score2){ //Jeśli gracz wyszedł z gry wyniki były równe następuje remis
            printw("REMIS!\n");
        }
        if(game_speed == 20){
            printw("Udalo Ci sie przetrwac %d s w trybie super sonic!\n", secondsCounter);
        }
        printw("Wcisnij dowonly klawisz aby wyjsc...");
    }

    void setGameSpeed(int speed){ //Ustawienie prędkości gry w zależności od wyboru gracza
        if(speed == '1'){
            game_speed = 250;
        }
        if(speed == '2'){
            game_speed = 100;
        }
        if(speed == '3'){
            game_speed = 50;
        }
        if(speed >= '4'){
            game_speed = 20;
        }
    }

    void speedMessage(){ //Wybór prędkości gry
        printw("Wybierz predkosc rozgrywki: \n");
        printw("1. Slow\n");
        printw("2. Medium (Zalecane)\n");
        printw("3. Fast\n");
        printw("4. Super Sonic (Wybierasz na wlasne ryzyko)\n");
    }

    void Run(){
        initscr(); //Inicjalizacja ncurses
        raw();
        keypad(stdscr, true);
        noecho();
        cbreak();
        scrollok(stdscr, true);
        nodelay(stdscr, true);

        victoryMessage(); //Gracz wybiera do ilu punktów chce grać

        while(victory == 0){
            setVictoryPoints(getch()); //Pobranie danych
        }

        clear();

        speedMessage(); //Wybór prędkości piłki

        while(game_speed == 0){
            setGameSpeed(getch()); //Pobranie danych
        }
        auto start = high_resolution_clock::now();
        auto end = high_resolution_clock::now();
        //Rozgrywka
        while(!quit){
            start = high_resolution_clock::now();
            Draw(duration_cast<nanoseconds>(end - start).count());
            Input();
            CollisionDetection();
            checkVictorious();
            end = high_resolution_clock::now();
        }

        clear();

        winMsg(); //Wiadomość o wygranej jednego z graczy lub remisie

        int any_key = 0;
        quit = false;
        //Wyjście na dowolnym klawiszu
        while(!quit){
            any_key = getch();
            if(any_key > 0){
                quit = true;
            }
        }

        clear();
        endwin();
    }

};

int main(){
    //Inicjalizacja Gry
    AIController Game(40,20);
    //Uruchomienie Gry
    Game.Run();
    return 0;
};