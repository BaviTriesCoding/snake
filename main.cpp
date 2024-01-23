#include "library.h"

int main(){
    srand(time(NULL));
    Initialize();
    snake* std_snake = new  snake(15);
    apple* std_apple = new apple(std_snake);
    bool exit = false, dead = false;
    clear();
    getch();
    do{
        std_apple->position_apple();
        exit = evalMove(std_snake, normalizedInput(167));
        std_snake->move_snake();
        std_snake->eat(std_apple);
        dead = std_snake->is_dead();
        std_snake->show_snake();
        refresh();
    }while(!exit && !dead);
    EndProgram(std_snake->get_length());
    return 0;
}