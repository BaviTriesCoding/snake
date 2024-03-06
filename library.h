#include <iostream>
#include <ncurses.h>
#include <ctime>
using namespace std;

struct point{
    int x;
    int y;
};
struct snake_block{
    point position;
    point direction;
    snake_block* next;
    snake_block* before;
};
typedef snake_block* ptr_snake;

class snake;
class apple;

class apple{
protected:
    point position;
    int color;
public:
    apple(snake* _scope);
    point get_position();
    void position_apple();
    void change_position(snake* _scope);
};

class snake{
protected:
    ptr_snake head;
    ptr_snake tail;
    int length;
    int direction; //0->right(x++) | 1->left(x--) | 2->top(y--) | 3->bottom(y++)
    int color;
public:
    snake(int start_length);
    void show_snake();
    void move_snake();
    bool is_dead();
    void set_direction(int _direction);
    int get_direction();
    ptr_snake get_head();
    void eat(apple* _food);
    int get_length();
};

void Initialize();
void EndProgram(int _points);
int normalizedInput(int milliseconds);
bool evalMove(snake* scope,int pressedKey);
bool is_in_snake(point _point, snake* _scope);