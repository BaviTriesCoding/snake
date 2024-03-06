#include "library.h"

void Initialize(){
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, true);
    keypad(stdscr, true);
    getch();
    if(has_colors()){start_color();}
}
void EndProgram(int _points){
    int exit;
    clear();
    printw("You scored %d\npress backspace to exit", _points-15);
    nodelay(stdscr, false);
    do{ exit=getch(); }while(exit!=KEY_BACKSPACE && exit!=KEY_SEND);
    endwin();
}
//CLOCKS_PER_SEC = 1,000,000
//CLOCK_PER_MILLISEC = 1,000
int normalizedInput(int _milliseconds){
    clock_t start, end;
    int input, i, output=0;
    bool found;
    int permitted_input[]={(int)'w',(int)'W',(int)'a',(int)'A',(int)'s',(int)'S',(int)'d',(int)'D', KEY_UP, KEY_LEFT, KEY_DOWN, KEY_RIGHT, (int)'q', (int)'Q'};
    start = clock();
    do{
        input = getch();
        found = false;
        i=0;
        while(i<14 && !found ){
            found = permitted_input[i]==input;
            i++;
        }
        if(found){output = input;}
        end = clock();
    }while(_milliseconds * (CLOCKS_PER_SEC / 1000) >= int(end - start));
    return output;
}
bool evalMove(snake* _scope, int _pressedKey){
    bool exit=false;
    //0->right(x++) | 1->left(x--) | 2->top(y--) | 3->bottom(y++)
    switch (_pressedKey) {
        case (int)'w':
        case (int)'W':
        case KEY_UP:
            if(_scope->get_direction()!=3){
                _scope->set_direction(2);
            }
            break;

        case (int)'a':
        case (int)'A':
        case KEY_LEFT:
            if(_scope->get_direction()!=0){
                _scope->set_direction(1);
            }
            break;

        case (int)'s':
        case (int)'S':
        case KEY_DOWN:
            if(_scope->get_direction()!=2){
                _scope->set_direction(3);
            }
            break;

        case (int)'d':
        case (int)'D':
        case KEY_RIGHT:
            if(_scope->get_direction()!=1){
                _scope->set_direction(0);
            }
            break;

        case (int)'q':
        case (int)'Q':
            exit=true;
            break;
    }
    return exit;
}
bool is_in_snake(point _point, snake* _scope){
    bool is_in = false;
    ptr_snake pointer = _scope -> get_head();
    pointer = pointer -> next;
    while(pointer!=nullptr && !is_in){
        is_in = _point.x == pointer->position.x && _point.y == pointer->position.y;
        pointer = pointer -> next;
    }
    return is_in;
}

apple::apple(snake* _scope) {
    bool can_place;
    do{
        can_place = true;
        if(COLS%2==0){ this->position.x = 2*(rand() % (COLS/2)); }
        else{this->position.x = 2*(rand() % ((COLS+1)/2)); }
        this->position.y = rand() % LINES;
        ptr_snake pointer = _scope->get_head();
        while(pointer!=nullptr){
            can_place = !is_in_snake(this->position, _scope);
            pointer = pointer->next;
        }
    }while(!can_place);
    init_pair(160, 160, 160);
    this->color = COLOR_PAIR(160);
}
void apple::position_apple(){
    attron(this->color);
    mvprintw(this->position.y, this->position.x, "[]");
    attroff(this->color);
    move(LINES-1, COLS-1);
}
void apple::change_position(snake* _scope){
    bool can_place;
    do{
        can_place = true;
        if(COLS%2==0){ this->position.x = 2*(rand() % (COLS/2)); }
        else{this->position.x = 2*(rand() % ((COLS+1)/2)); }
        this->position.y = rand() % LINES;
        ptr_snake pointer = _scope->get_head();
        while(pointer!=nullptr){
            can_place = !is_in_snake(this->position, _scope);
            pointer = pointer->next;
        }
    }while(!can_place);
}
point apple::get_position() {
    return this->position;
}

snake::snake(int _start_length){
    ptr_snake pointer;
    point start_position;
    start_position.y=0;
    start_position.x=_start_length*2;
    this->direction = 0;
    this->length = _start_length;
    this->head = new snake_block;
    this->head->before = nullptr;
    this->head->position = start_position;
    pointer = this -> head;
    for(int i=1; i<_start_length; i++){
        pointer -> next = new snake_block;
        pointer -> next -> before = pointer;
        pointer = pointer -> next;
        start_position.x-=2;
        pointer -> position = start_position;
        pointer -> direction = pointer -> before -> position;
    }
    this->tail = pointer;
    pointer -> next = nullptr;
    init_pair(45, 45, 45);
    this->color = COLOR_PAIR(45);
};
void snake::show_snake() {
    clear();
    ptr_snake pointer = this->head;
    while(pointer!=nullptr){
        move(pointer->position.y, pointer->position.x);
        attron(this->color);
        printw("[]");
        attroff(this->color);
        pointer= pointer->next;
    }
    move(LINES-1, COLS-1);
}
int snake::get_length() {
    return this->length;
}
void snake::move_snake(){
    //0->right(x++) | 1->left(x--) | 2->top(y--) | 3->bottom(y++)
    if(this->direction == 0){this->head->position.x = this->head->position.x + 2;}
    if(this->direction == 1){this->head->position.x = this->head->position.x - 2;}
    if(this->direction == 2){this->head->position.y = this->head->position.y - 1;}
    if(this->direction == 3){this->head->position.y = this->head->position.y + 1;}
    ptr_snake pointer = this->head->next;
    while(pointer!=nullptr){
        pointer -> position = pointer -> direction;
        pointer -> direction = pointer -> before -> position;
        pointer = pointer -> next;
    }

}
bool snake::is_dead() {
    bool dead = false;
    if(
            this->head->position.x < 0 ||
            this->head->position.x >= COLS ||
            this->head->position.y < 0 ||
            this->head->position.y >= LINES ||
            is_in_snake(this->head->position, this)
            ){
        dead = true;
    }
    return dead;
}
void snake::set_direction(int _direction) {
    this->direction = _direction;
}
int snake::get_direction() {
    return this->direction;
}
ptr_snake snake::get_head() {
    return this->head;
}
void snake::eat(apple* _food){
    point apple_position = _food->get_position();
    if(this->head->position.x == apple_position.x && this->head->position.y == apple_position.y){
        _food->change_position(this);
        this->length++;
        this->tail->next = new snake_block;
        this->tail->next->before = this->tail;
        this->tail->next->next = nullptr;
        this->tail->next->position = this->tail->next->direction = this->tail->position;
        this->tail = this->tail->next;
    }
}