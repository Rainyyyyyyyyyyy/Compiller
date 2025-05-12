
#include <string>
#include <Windows.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <queue>
#include <stack>
using namespace std;
class leksema {
    int l_type;         // номер лексеммы
    std::string name;   // название (если перем-я, то имя перем-й)
    int value;          // значение (если константа, то знач. константы)
    int start_str_position;
    int start_str_number;
public:
    leksema(int l_type1 = 0, std::string name1 = "", int value1 = 0, int st_str_pos=0, int st_str_num=0);
    leksema(const leksema& a);                  // конструктор копирования
    ~leksema() {};                              // деструктор
    leksema& operator = (const leksema& a);     // =
    bool operator ==(const leksema&);
    int get_l_type();                           // номер лексеммы
    std::string get_name();
    int get_value();
    int get_start_str_position();
    int get_start_str_number();

    void set_l_type(int a);
    void set_name(std::string a);
    void set_value(int a);
    void set_start_str_position(int a);
    void set_start_str_number(int a);

};
void leksema::set_start_str_position(int a){
    start_str_position = a;
}
void leksema::set_start_str_number(int a){
    start_str_number = a; 
}
int leksema::get_start_str_position(){
    return start_str_position;
}
int leksema::get_start_str_number(){
    return start_str_number;
}
leksema::leksema(int l_type1, std::string name1, int value1, int st_str_pos, int st_str_num){
    l_type = l_type1;
    name = name1;
    value = value1;
    start_str_position = st_str_pos;
    start_str_number = st_str_num;
};// конструктор копирования
leksema& leksema::operator = (const leksema& a){
    if (this == &a)return *this;
    l_type = a.l_type;
    name = a.name;
    value = a.value;
    start_str_number = a.start_str_number;
    start_str_position = a.start_str_position;
    return *this;
};
leksema::leksema(const leksema &a){
    l_type = a.l_type;
    name = a.name;
    value = a.value;
    start_str_number = a.start_str_number;
    start_str_position = a.start_str_position;
};


int leksema::get_l_type() {
    return l_type;
}
std::string leksema::get_name() {
    return name; 
}
int leksema::get_value() {
    return value;
}

void leksema::set_l_type(int a) {
    l_type = a;
}
void leksema::set_name(std::string a) {
    name = a;
}
void leksema::set_value(int a) {
    value = a;
}

bool leksema::operator==(const leksema& s) {
    if (l_type != s.l_type)return false;
    if (l_type == 1)return value == s.value;
    if (l_type == 2)return name == s.name;
    return true;
}

leksema Lexical_tokenizator(std::ifstream& program_file, int& str_number, int& str_position, bool& error_flag) { // Лексический анализатор
    unsigned char Ci;
    std::string name;
    int value, index;
    std::string a = "+-*/()[];,";
    std::string a1 = "><";
    std::vector<int> b = { 3,4,5,6,7,8,16,17,24,29 }; // номера лексем из a
    std::vector<std::string> function_words_a{ "if", "endif", "else", "endelse", "while", "endwihle", "read", "write", "int", "int1" }; // служебные слова
    std::vector<int> function_words_b = { 18,19,20,21,22,23,25,26,27,28 }; // номера лексем служебных слов из function_words_a
    std::vector<std::string>::iterator is_fw;
    leksema res;
S:
    Ci = program_file.peek();
    res.set_start_str_position(str_position);
    res.set_start_str_number(str_number);

    if (isalpha(Ci)) { // проверка на букву
        name = Ci; // 1
        program_file.get();
        ++str_position;
        goto B;
    }
    if (isdigit(Ci)) { // проверка на цифру
        value = Ci - '0'; // 2
        program_file.get();
        ++str_position;
        goto C;
    }
    if ((index = a.find(Ci)) != std::string::npos) { // проверка на наличие в +-*/()[];,
        res.set_l_type(b[index]); // 5
        program_file.get();
        ++str_position;
        goto Z;
    }
    if (Ci == '=') { // проверка на =
        program_file.get();
        ++str_position;
        goto A;
    }
    if (Ci == '!') { // проверка на !
        program_file.get();
        ++str_position;
        goto D;
    }
    if ((index = a1.find(Ci)) != std::string::npos) { // проверка на наличие в >< , 1
        program_file.get();
        ++str_position;
        goto E;
    }
    if (isblank(Ci)) { // проверка на пробел и табуляцию
        program_file.get();
        ++str_position;
        goto S;
    }
    if (Ci == '\n') {  // проверка на перевод строки
        program_file.get();
        str_position = 1; // 14
        ++str_number;
        goto S;
    }
    if (program_file.eof()) { // проверка на конец файла
        res.set_l_type(0);
        program_file.get();
        ++str_position;
        goto Z;
    }
    std::cout << "Строка: " << str_number << " Позиция: " << str_position << "\nОписание: " << "Лексема не распознана: Недопустимый символ в программе." << std::endl;
    error_flag = true;  // ошибка: недопустимый символ языка
    goto Z;
B:
    Ci = program_file.peek();

    if (isalnum(Ci)) { // проверка на букву или цифру
        name += Ci; // 4
        program_file.get();
        ++str_position;
        goto B;
    }
    is_fw = std::find(begin(function_words_a), end(function_words_a), name); // 8
    if (is_fw == end(function_words_a)) { // не найдено значит имя переменной
        res.set_l_type(2);
        res.set_name(name);
        goto Z;
    }
    else { // иначе служебное слово
        res.set_l_type(function_words_b[is_fw - begin(function_words_a)]);
        goto Z;
    }

C:
    Ci = program_file.peek();

    if (isalpha(Ci)) { // проверка на букву
        std::cout << "Строка: " << str_number << " Позиция: " << str_position << "\nОписание: " << "Лексема не распознана: неожиданный символ." << std::endl;
        error_flag = true;  // ошибка
        goto Z;
    }
    if (isdigit(Ci)) { // проверка на цифру
        value = value * 10 + (Ci - '0'); // 3
        program_file.get();
        ++str_position;
        goto C;
    }
    res.set_l_type(1); res.set_value(value); // 9
    goto Z;

A:
    Ci = program_file.peek();

    if (Ci == '=') { // проверка на =
        res.set_l_type(10); // 10
        program_file.get();
        ++str_position;
        goto Z;
    }
    res.set_l_type(9); // 11
    goto Z;

D:
    Ci = program_file.peek();

    if (Ci == '=') { // проверка на =
        res.set_l_type(11); // 12
        program_file.get();
        ++str_position;
        goto Z;
    }
    std::cout << "Строка: " << str_number << " Позиция: " << str_position << "\nОписание: " << "Лексема не распознана: ожидалось !=." << std::endl;
    error_flag = true; // ошибка не !=
    goto Z;

E:
    Ci = program_file.peek();

    if (Ci == '=') { // проверка на =
        res.set_l_type(14 + index); // 6
        program_file.get();
        ++str_position;
        goto Z;
    }
    res.set_l_type(12 + index); // 7
    goto Z;
Z:
    return res;
};
struct Gramar_stack_element {
    int leks_number = 0;        // номер лексеммы (если лексемма)
    /* leks_number<0    <->    не лексемма, а нетерминал     */
    char name = 0;           // название нетерминала

    Gramar_stack_element(int n=0, char nam=0) {
        leks_number = n;
        name = nam;
    }


};



/*
void stack_pop(stack* head) {
    if (head->next == NULL) {
        delete head; head = NULL;
        return;
    }
    stack* t = head->next;
    delete head;
    head = t;
}

*/
/*
* program_file - файл с проверяемой программов
* str_number - номер строки
* str_position - позиция в строке
* error_flag - код ошибки
* g_stack - магазин синтаксического анализатора
* (*) G - структура для одной ячейки магазина синтаксического генератора (описана выше)
*  
*/
std::vector<std::string>leksema_list = { "конец программы", "константа", "переменная", "+", "-", "*", "/", "(", ")" , "=" , "==" , "!=" , ">" , "<" , ">=" , "<=" , "[" , "]" , "if" , "endif" , "else" , "endelse" , "while" , "endwhile" , ";" , "read" , "write" , "int" , "int1", "," };


/*
opc_stack* generate_part_of_opc_for_gen(std::vector<int> seq) {
    if (seq.size() == 0)return NULL;
    opc_stack* head = new opc_stack;
    int i = 0;  // i<seq.size()
    opc_stack* t;
    while (i < seq.size()){
        t = new opc_stack;
        t->next = head;
        head = t;
        switch (seq[i]) {
        case 0: head->data = "."; break;
        case 1: head->data = "k"; break;
        case 2: head->data = "a"; break;
        case 3: head->data = "+"; break;
        case 4: head->data = "-"; break;
        case 5: head->data = "*"; break;
        case 6: head->data = "/"; break;
        case 7: head->data = "("; break;
        case 8: head->data = ")"; break;
        case 9: head->data = "="; break;
        case 10: head->data = "=="; break;
        case 11: head->data = ">"; break;
        case 12: head->data = "<"; break;
        case 13: head->data = ">="; break;
        case 14: head->data = "<="; break;
        case 15: head->data = "r"; break;
        case 16: head->data = "w"; break;
        case 17: head->data = "1"; break;
        case 18: head->data = "2"; break;
        case 19: head->data = "3"; break;
        case 20: head->data = "4"; break;
        case 21: head->data = "5"; break;
        case 22: head->data = "6"; break;
        case 23: head->data = "7"; break;
        case 24: head->data = "8"; break;
        case 25: head->data = "9"; break;
        case 26: head->data = "10"; break;
        case 27: head->data = "11"; break;
        }
        i++;
    }
    return head;
}
*/
               
                                                                                                                                                   // массив, НЕ СТЕК              
int str_to_int(string s) {
    int i = 0;
    int n = 0;
    while (i < s.size()) {
        n = n * 10 + s[i] - 48;
        i++;
    }
    return n;
}

string int_to_str(int n) {
    string s = "";
    while (n) {
        s = (char)(n % 10) + s;
        n = n / 10;
    }
}
void imply_one_operation_from_gen_opc(stack<int>& gen_opc, leksema leks, stack<string>& opc) {
    int gen_top = gen_opc.top();
    gen_opc.pop();
    switch (gen_top) {
    case 0: break;
    case 1: opc.push("+"); break;
    case 2: opc.push("-"); break;
    case 3: opc.push("*"); break;
    case 4: opc.push("/"); break;
    case 5: opc.push("="); break;
    case 6: opc.push("=="); break;
    case 7: opc.push(">="); break;
    case 8: opc.push("<="); break;
    case 9: opc.push(">"); break;
    case 10: opc.push("<"); break;
    case 11: opc.push("!="); break;
    case 12: opc.push(leks.get_name());
    case 13: opc.push(int_to_str(leks.get_value())); break;
    case 14: opc.push("~"); break;
    case 15: opc.push("r"); break;
    case 16: opc.push("w"); break;
    case 17: opc.push("m"); break;
    }
    
}

void imply_Grammar(std::ifstream& program_file, bool &error_flag) {
    bool end_of_program_flag = false;
    bool ready_to_read_next_leksem = true;
    leksema input_leks;
    Gramar_stack_element t;             // вспомогательный элемент магазина грамматики (для заполнения)
    Gramar_stack_element g_st_top;      // вспомогательный элемент магазина грамматики (голова)
    stack<Gramar_stack_element> g_stack;// магазин грамматики 
    stack<int> gen_opc_stack;           // магазин генератора ОПС
    stack<string> OPC;                  // готовая строка ОПС
    int tt = 0;                         // вспомогательный элемент магазина генератора ОПС (для заполнения)
    int gen_opc_top = 0;                // вспомогательный элемент магазина генератора ОПС (голова)
    gen_opc_stack.push(tt); gen_opc_stack.push(tt);
    t.leks_number = 0;  g_stack.push(t);        // начальная конфигурация магазина грамматики (X, перевёрнутая_Т)
    t.leks_number = -1; t.name = 'X';   g_stack.push(t);
    //stack <string> gen_opc;
    int str_number = 1;
    int str_position = 1;
    //int error_flag = 0;
    while (!error_flag && !end_of_program_flag) {
        if (ready_to_read_next_leksem) {
            input_leks = Lexical_tokenizator(program_file, str_number, str_position, error_flag);
            ready_to_read_next_leksem = false;
        }
        //Gramar_stack_element t(-1, 'Z');
        g_st_top = g_stack.top();
        g_stack.pop();
        if (g_st_top.leks_number == -1) {
            switch (g_st_top.name) {
            case 'X':  
                if (input_leks.get_l_type() == 2) {         // leksema = a
                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                    t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                    t.leks_number = 24;                 g_stack.push(t);    // ;
                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 9;                  g_stack.push(t);    // =
                    t.leks_number = -1; t.name = 'H';   g_stack.push(t);    // H
                    t.leks_number = 2;                  g_stack.push(t);    // a          
                
                    gen_opc_stack.push(27);                                 // 9
                    gen_opc_stack.push(0);                                  // □
                    gen_opc_stack.push(0);                                  // □
                    gen_opc_stack.push(5);                                  // =
                    gen_opc_stack.push(0);                                  // □
                    gen_opc_stack.push(0);                                  // □
                    gen_opc_stack.push(0);                                  // □
                    gen_opc_stack.push(12);                                 // a
                }
                else
                    if (input_leks.get_l_type() == 18) {        // leksema = if
                        t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                        t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                        t.leks_number = 24;                 g_stack.push(t);    // ;
                        t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                        t.leks_number = -1; t.name = 'E';   g_stack.push(t);    // E
                        t.leks_number = 19;                 g_stack.push(t);    // endif
                        t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                        t.leks_number = 8;                  g_stack.push(t);    // )
                        t.leks_number = -1; t.name = 'C';   g_stack.push(t);    // C
                        t.leks_number = 7;                  g_stack.push(t);    // (
                        t.leks_number = 18;                 g_stack.push(t);    // if
                        
                        gen_opc_stack.push(27); // 9
                        gen_opc_stack.push(0);  // □
                        gen_opc_stack.push(0);  // □
                        gen_opc_stack.push(21); // 3
                        gen_opc_stack.push(0);  // □
                        gen_opc_stack.push(0);  // □
                        gen_opc_stack.push(0);  // □
                        gen_opc_stack.push(19); // 1
                        gen_opc_stack.push(0);  // □
                        gen_opc_stack.push(0);  // □
                        gen_opc_stack.push(0);  // □
                    }
                    else
                        if (input_leks.get_l_type() == 22) {        // leksema = while
                            t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                            t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                            t.leks_number = 24;                 g_stack.push(t);    // ;
                            t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                            t.leks_number = 23;                 g_stack.push(t);    // endwhile
                            t.leks_number = -1; t.name = 'p';   g_stack.push(t);    // P
                            t.leks_number = 8;                  g_stack.push(t);    // )
                            t.leks_number = -1; t.name = 'C';   g_stack.push(t);    // C
                            t.leks_number = 7;                  g_stack.push(t);    // (
                            t.leks_number = 22;                 g_stack.push(t);    // while
                            
                            gen_opc_stack.push(27); // 9
                            gen_opc_stack.push(0);  // □
                            gen_opc_stack.push(0);  // □
                            gen_opc_stack.push(23); // 5
                            gen_opc_stack.push(0);  // □
                            gen_opc_stack.push(0);  // □
                            gen_opc_stack.push(19); // 1
                            gen_opc_stack.push(0);  // □
                            gen_opc_stack.push(0);  // □
                            gen_opc_stack.push(22); // 4
                        }
                        else
                            if (input_leks.get_l_type() == 25) {        // leksema = read
                                t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                                t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                                t.leks_number = 24;                 g_stack.push(t);    // ;
                                t.leks_number = 8;                  g_stack.push(t);    // )
                                t.leks_number = -1; t.name = 'H';   g_stack.push(t);    // H
                                t.leks_number = 2;                  g_stack.push(t);    // a
                                t.leks_number = 7;                  g_stack.push(t);    // (
                                t.leks_number = 25;                 g_stack.push(t);    // read
                                
                                gen_opc_stack.push(27); // 9
                                gen_opc_stack.push(0);  // □
                                gen_opc_stack.push(0);  // □
                                gen_opc_stack.push(15); // r
                                gen_opc_stack.push(0);  // □
                                gen_opc_stack.push(12); // a
                                gen_opc_stack.push(0);  // □
                                gen_opc_stack.push(0);  // □
                            }
                            else
                                if (input_leks.get_l_type() == 26) {        // leksema = write
                                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                                    t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                                    t.leks_number = 24;                 g_stack.push(t);    // ;
                                    t.leks_number = 8;                  g_stack.push(t);    // )
                                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                                    t.leks_number = 7;                  g_stack.push(t);    // (
                                    t.leks_number = 26;                 g_stack.push(t);    // write
                                    
                                    gen_opc_stack.push(27); // 9
                                    gen_opc_stack.push(0);  // □
                                    gen_opc_stack.push(0);  // □
                                    gen_opc_stack.push(16); // w
                                    gen_opc_stack.push(0);  // □
                                    gen_opc_stack.push(0);  // □
                                    gen_opc_stack.push(0);  // □
                                }
                                else
                                    if (input_leks.get_l_type() == 27) {            // leksema = int
                                        t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                                        t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                                        t.leks_number = 24;                 g_stack.push(t);    // ;
                                        t.leks_number = -1; t.name = 'R';   g_stack.push(t);    // R
                                        t.leks_number = 27;                 g_stack.push(t);    // int
                                        
                                        gen_opc_stack.push(27); // 9
                                        gen_opc_stack.push(0);  // □ 
                                        gen_opc_stack.push(26); // 8
                                        gen_opc_stack.push(0);  // □
                                        gen_opc_stack.push(24); // 6
                                    }
                                    else
                                        if (input_leks.get_l_type() == 28) {            // leksema = int1
                                            t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                                            t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                                            t.leks_number = 24;                 g_stack.push(t);    // ;
                                            t.leks_number = -1; t.name = 'N';   g_stack.push(t);    // N
                                            t.leks_number = 28;                 g_stack.push(t);    // int1
                                            
                                            gen_opc_stack.push(27); // 9
                                            gen_opc_stack.push(0);  // □ 
                                            gen_opc_stack.push(29); // 11
                                            gen_opc_stack.push(0);  // □
                                            gen_opc_stack.push(25); // 7
                                        }
                                        else {
                                            /*  error   */
                                            error_flag = true;
                                            std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position() << "\nОписание: " << g_st_top.name << '\n';
                                        }
                break;
                //
                //
                //
            case 'P':   
                if (input_leks.get_l_type() == 2) {         // leksema = a
                    t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                    t.leks_number = 24;                 g_stack.push(t);    // ;
                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 9;                  g_stack.push(t);    // =
                    t.leks_number = -1; t.name = 'H';   g_stack.push(t);    // H
                    t.leks_number = 2;                  g_stack.push(t);    // a          
                    
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(21); // 3
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(19); // 1
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                }
                else
                    if (input_leks.get_l_type() == 18) {        // leksema = if
                        t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                        t.leks_number = 24;                 g_stack.push(t);    // ;
                        t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                        t.leks_number = -1; t.name = 'E';   g_stack.push(t);    // E
                        t.leks_number = 19;                 g_stack.push(t);    // endif
                        t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                        t.leks_number = 8;                  g_stack.push(t);    // )
                        t.leks_number = -1; t.name = 'C';   g_stack.push(t);    // C
                        t.leks_number = 7;                  g_stack.push(t);    // (
                        t.leks_number = 18;                 g_stack.push(t);    // if
                        
                        gen_opc_stack.push(0);  // □
                        gen_opc_stack.push(0);  // □
                        gen_opc_stack.push(21); // 3
                        gen_opc_stack.push(0);  // □
                        gen_opc_stack.push(0);  // □
                        gen_opc_stack.push(0);  // □
                        gen_opc_stack.push(19); // 1
                        gen_opc_stack.push(0);  // □
                        gen_opc_stack.push(0);  // □
                        gen_opc_stack.push(0);  // □
                    }
                    else
                        if (input_leks.get_l_type() == 22) {        // leksema = while
                            t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                            t.leks_number = 24;                 g_stack.push(t);    // ;
                            t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                            t.leks_number = 23;                 g_stack.push(t);    // endwhile
                            t.leks_number = -1; t.name = 'p';   g_stack.push(t);    // P
                            t.leks_number = 8;                  g_stack.push(t);    // )
                            t.leks_number = -1; t.name = 'C';   g_stack.push(t);    // C
                            t.leks_number = 7;                  g_stack.push(t);    // (
                            t.leks_number = 22;                 g_stack.push(t);    // while
                            
                            gen_opc_stack.push(0);  // □
                            gen_opc_stack.push(0);  // □
                            gen_opc_stack.push(23); // 5
                            gen_opc_stack.push(0);  // □
                            gen_opc_stack.push(0);  // □
                            gen_opc_stack.push(19); // 1
                            gen_opc_stack.push(0);  // □
                            gen_opc_stack.push(0);  // □
                            gen_opc_stack.push(22); // 4
                        }
                        else
                            if (input_leks.get_l_type() == 25) {        // leksema = read
                                t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                                t.leks_number = 24;                 g_stack.push(t);    // ;
                                t.leks_number = 8;                  g_stack.push(t);    // )
                                t.leks_number = -1; t.name = 'H';   g_stack.push(t);    // H
                                t.leks_number = 2;                  g_stack.push(t);    // a
                                t.leks_number = 7;                  g_stack.push(t);    // (
                                t.leks_number = 25;                 g_stack.push(t);    // read

                                gen_opc_stack.push(0);  // □
                                gen_opc_stack.push(0);  // □
                                gen_opc_stack.push(15); // r
                                gen_opc_stack.push(0);  // □
                                gen_opc_stack.push(12); // a
                                gen_opc_stack.push(0);  // □
                                gen_opc_stack.push(0);  // □
                            }
                            else
                                if (input_leks.get_l_type() == 26) {        // leksema = write
                                    t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                                    t.leks_number = 24;                 g_stack.push(t);    // ;
                                    t.leks_number = 8;                  g_stack.push(t);    // )
                                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                                    t.leks_number = 7;                  g_stack.push(t);    // (
                                    t.leks_number = 26;                 g_stack.push(t);    // write

                                    gen_opc_stack.push(0);  // □
                                    gen_opc_stack.push(0);  // □
                                    gen_opc_stack.push(16); // w
                                    gen_opc_stack.push(0);  // □
                                    gen_opc_stack.push(0);  // □
                                    gen_opc_stack.push(0);  // □    
                                }
                                else
                                    if (input_leks.get_l_type() == 27) {            // leksema = int
                                        t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                                        t.leks_number = 24;                 g_stack.push(t);    // ;
                                        t.leks_number = -1; t.name = 'R';   g_stack.push(t);    // R
                                        t.leks_number = 27;                 g_stack.push(t);    // int
                                    
                                        gen_opc_stack.push(0);  // □ 
                                        gen_opc_stack.push(26); // 8
                                        gen_opc_stack.push(0);  // □
                                        gen_opc_stack.push(24); // 6
                                    }
                                    else
                                        if (input_leks.get_l_type() == 28) {            // leksema = int1
                                            t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                                            t.leks_number = 24;                 g_stack.push(t);    // ;
                                            t.leks_number = -1; t.name = 'N';   g_stack.push(t);    // N
                                            t.leks_number = 28;                 g_stack.push(t);    // int1
                                        
                                            gen_opc_stack.push(0);  // □ 
                                            gen_opc_stack.push(29); // 11
                                            gen_opc_stack.push(0);  // □
                                            gen_opc_stack.push(25); // 7
                                        }
                                        else {
                                            /*  λ   */
                                        }
                break;
                //
                //
                //
            case 'S':   
                //  S   ->  kVU
                if (input_leks.get_l_type() == 1) {             // leksema = const (k)
                    t.leks_number = -1; t.name = 'U';   g_stack.push(t);    // U
                    t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                    t.leks_number = 1;                  g_stack.push(t);    // k
                
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(13); // k
                }
                else   //  S   ->  aHVU
                    if (input_leks.get_l_type() == 2) {              // leksema = a
                        t.leks_number = -1; t.name = 'U';   g_stack.push(t);    // U
                        t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                        t.leks_number = -1; t.name = 'H';   g_stack.push(t);    // H
                        t.leks_number = 2;                  g_stack.push(t);    // a
                    
                        gen_opc_stack.push(0);  // □
                        gen_opc_stack.push(0);  // □
                        gen_opc_stack.push(0);  // □
                        gen_opc_stack.push(12); // a
                    }
                    else   //  S   ->  +GVU
                        if (input_leks.get_l_type() == 3) {             // leksema = +
                            t.leks_number = -1; t.name = 'U';   g_stack.push(t);    // U
                            t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                            t.leks_number = -1; t.name = 'G';   g_stack.push(t);    // G
                            t.leks_number = 3;                  g_stack.push(t);    // +
                            
                            gen_opc_stack.push(0);  // □
                            gen_opc_stack.push(0);  // □
                            gen_opc_stack.push(0);  // □
                            gen_opc_stack.push(0);  // □
                        }
                        else   // S    ->  -GVU
                            if (input_leks.get_l_type() == 4) {             // leksema = -
                                t.leks_number = -1; t.name = 'U';   g_stack.push(t);    // U
                                t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                                t.leks_number = -1; t.name = 'G';   g_stack.push(t);    // G
                                t.leks_number = 4;                  g_stack.push(t);    // -
                            
                                gen_opc_stack.push(0);  // □
                                gen_opc_stack.push(14); // ~
                                gen_opc_stack.push(0);  // □
                                gen_opc_stack.push(0);  // □
                            }
                            else   // S    ->  (S)VU
                                if (input_leks.get_l_type() == 7) {             // leksema = (
                                    t.leks_number = -1; t.name = 'U';   g_stack.push(t);    // U
                                    t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                                    t.leks_number = 8;                  g_stack.push(t);    // )
                                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                                    t.leks_number = 7;                  g_stack.push(t);    // (
                                
                                    gen_opc_stack.push(0);  // □
                                    gen_opc_stack.push(0);  // □
                                    gen_opc_stack.push(0);  // □
                                    gen_opc_stack.push(0);  // □
                                    gen_opc_stack.push(0);  // □
                                }
                                else {
                                    /* error */
                                    error_flag = true;    // пусть будет 1 пока что
                                    std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position() << "\nОписание: " << g_st_top.name << '\n';

                                }
                //
                //
                //
            case 'U':
                if (input_leks.get_l_type() == 3) {             // leksema = +
                    t.leks_number = -1; t.name = 'U';   g_stack.push(t);    // U
                    t.leks_number = -1; t.name = 'T';   g_stack.push(t);    // T
                    t.leks_number = 3;                  g_stack.push(t);    // +

                    gen_opc_stack.push(1);  // +
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                }
                else
                    if (input_leks.get_l_type() == 4) {             // leksema = -
                        t.leks_number = -1; t.name = 'U';   g_stack.push(t);    // U
                        t.leks_number = -1; t.name = 'T';   g_stack.push(t);    // T
                        t.leks_number = 4;                  g_stack.push(t);    // -
                    
                        gen_opc_stack.push(2);  // -
                        gen_opc_stack.push(0);  // □
                        gen_opc_stack.push(0);  // □

                    }
                    else {
                        /*  λ   */
                    }
                //
                //
                //
            case 'T':  
                if (input_leks.get_l_type() == 1) {             // leksema = const (k)
                    t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                    t.leks_number = 1;                  g_stack.push(t);    // k
                
                    gen_opc_stack.push(0);   // □
                    gen_opc_stack.push(13);  // k
                }
                else       //  T   ->  aHV
                    if (input_leks.get_l_type() == 2) {              // leksema = a
                        t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                        t.leks_number = -1; t.name = 'H';   g_stack.push(t);    // H
                        t.leks_number = 2;                  g_stack.push(t);    // a
                    
                        gen_opc_stack.push(0);   // □
                        gen_opc_stack.push(0);   // □
                        gen_opc_stack.push(12);  // a
                    }
                    else       //  T   ->  +GV
                        if (input_leks.get_l_type() == 3) {             // leksema = +
                            t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                            t.leks_number = -1; t.name = 'G';   g_stack.push(t);    // G
                            t.leks_number = 3;                  g_stack.push(t);    // +
                            
                            gen_opc_stack.push(0);  // □
                            gen_opc_stack.push(0);  // □
                            gen_opc_stack.push(0);  // □
                        }
                        else       //  T   ->  -GV
                            if (input_leks.get_l_type() == 4) {             // leksema = -
                                t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                                t.leks_number = -1; t.name = 'G';   g_stack.push(t);    // G
                                t.leks_number = 4;                  g_stack.push(t);    // -
                                
                                gen_opc_stack.push(0);   // □
                                gen_opc_stack.push(0);   // □
                                gen_opc_stack.push(14);  // ~
                            }
                            else
                                if (input_leks.get_l_type() == 7) {             // leksema = (
                                    t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                                    t.leks_number = 8;                  g_stack.push(t);    // )
                                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                                    t.leks_number = 7;                  g_stack.push(t);    // (
                                    
                                    gen_opc_stack.push(0);  // □
                                    gen_opc_stack.push(0);  // □
                                    gen_opc_stack.push(0);  // □
                                    gen_opc_stack.push(0);  // □
                                }
                                else {
                                    /* error */
                                    error_flag = true;
                                    std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position() << "\nОписание: " << g_st_top.name << '\n';

                                }
                //
                //
                //
            case 'V':  
                if (input_leks.get_l_type() == 5) {             // leksema = *
                    t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                    t.leks_number = -1; t.name = 'F';   g_stack.push(t);    // F
                    t.leks_number = 5;                  g_stack.push(t);    // *
                    
                    gen_opc_stack.push(3);  // *
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                }
                else
                    if (input_leks.get_l_type() == 6) {             // leksema = /
                        t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                        t.leks_number = -1; t.name = 'F';   g_stack.push(t);    // F
                        t.leks_number = 6;                  g_stack.push(t);    // /
                        
                        gen_opc_stack.push(4);  // /
                        gen_opc_stack.push(0);  // □
                        gen_opc_stack.push(0);  // □
                    }
                    else {
                        /*  λ   */
                    } break;
                //
                //
                //
            case 'F':  
                if (input_leks.get_l_type() == 1) {             // leksema = k
                    t.leks_number = 1;                  g_stack.push(t);    // k
                    
                    gen_opc_stack.push(13); // k
                }
                else
                    if (input_leks.get_l_type() == 2) {             // leksema = a
                        t.leks_number = -1; t.name = 'H';   g_stack.push(t);    // H
                        t.leks_number = 2;                  g_stack.push(t);    // a
                    
                        gen_opc_stack.push(0);   // □
                        gen_opc_stack.push(12);  // a

                    }
                    else
                        if (input_leks.get_l_type() == 3) {             // leksema = +
                            t.leks_number = -1; t.name = 'G';   g_stack.push(t);    // G
                            t.leks_number = 3;                  g_stack.push(t);    // +
                        
                            gen_opc_stack.push(0);   // □
                            gen_opc_stack.push(0);   // □
                        }
                        else
                            if (input_leks.get_l_type() == 4) {             // leksema = -
                                t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                                t.leks_number = -1; t.name = 'G';   g_stack.push(t);    // G
                                t.leks_number = 4;                  g_stack.push(t);    // -
                            
                                gen_opc_stack.push(14);    // ~
                                gen_opc_stack.push(0);     // □
                                gen_opc_stack.push(0);     // □
                            }
                            else
                                if (input_leks.get_l_type() == 7) {               // leksemma = (
                                    t.leks_number = 8;                  g_stack.push(t);    // )
                                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                                    t.leks_number = 7;                  g_stack.push(t);    // (

                                    gen_opc_stack.push(0);   // □
                                    gen_opc_stack.push(0);   // □
                                    gen_opc_stack.push(0);   // □
                                }
                                else {
                                    /*  error   */
                                    error_flag = true;
                                    std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position() << "\nОписание: " << g_st_top.name << '\n';

                                } break;
                //
                //
                //
            case 'G':   
                if (input_leks.get_l_type() == 1) {             // leksema = k
                    t.leks_number = 1;                  g_stack.push(t);    // k
                    
                    gen_opc_stack.push(13);  // k
                }
                else
                    if (input_leks.get_l_type() == 2) {               // leksema = a
                        t.leks_number = -1; t.name = 'H';   g_stack.push(t);    // H
                        t.leks_number = 2;                  g_stack.push(t);    // a
                    
                        gen_opc_stack.push(0);   // □
                        gen_opc_stack.push(12);  // a
                    }
                    else
                        if (input_leks.get_l_type() == 7) {               // leksema = (
                            t.leks_number = 8;                  g_stack.push(t);    // )
                            t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                            t.leks_number = 7;                  g_stack.push(t);    // (

                            gen_opc_stack.push(0);   // □
                            gen_opc_stack.push(0);   // □
                            gen_opc_stack.push(0);   // □
                        }
                        else {
                            /*  error   */
                            error_flag = true;
                            std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position() << "\nОписание: " << g_st_top.name << '\n';

                        } break;
                //
                //
                //
            case 'H':  
                if (input_leks.get_l_type() == 16) {            // leksema = [
                    t.leks_number = 17;                 g_stack.push(t);    // )
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 16;                 g_stack.push(t);    // (
                    

                    gen_opc_stack.push(18);   // i
                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(0);    // □
                }
                else {
                    /*  λ   */
                } break;
                //
                //
                //
            case 'Z':  
                /*  λ   */
                break;
                //
                //
                //
            case 'C':
                if (input_leks.get_l_type() == 1) {             // leksema = k
                    t.leks_number = -1; t.name = 'D';   g_stack.push(t);    // D
                    t.leks_number = -1; t.name = 'U';   g_stack.push(t);    // U
                    t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                    t.leks_number = 1;                  g_stack.push(t);    // k
                
                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(13);   // k
                }
                else
                    if (input_leks.get_l_type() == 2) {              // leksema = a
                        t.leks_number = -1; t.name = 'D';   g_stack.push(t);    // D
                        t.leks_number = -1; t.name = 'U';   g_stack.push(t);    // U
                        t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                        t.leks_number = -1; t.name = 'H';   g_stack.push(t);    // H
                        t.leks_number = 2;                  g_stack.push(t);    // a
                        
                        gen_opc_stack.push(0);    // □
                        gen_opc_stack.push(0);    // □
                        gen_opc_stack.push(0);    // □
                        gen_opc_stack.push(0);    // □
                        gen_opc_stack.push(12);   // a
                    }
                    else   //  S   ->  +GVU
                        if (input_leks.get_l_type() == 3) {             // leksema = +
                            t.leks_number = -1; t.name = 'D';   g_stack.push(t);    // D
                            t.leks_number = -1; t.name = 'U';   g_stack.push(t);    // U
                            t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                            t.leks_number = -1; t.name = 'G';   g_stack.push(t);    // G
                            t.leks_number = 3;                  g_stack.push(t);    // +
                        
                            gen_opc_stack.push(0);   // □
                            gen_opc_stack.push(0);   // □
                            gen_opc_stack.push(0);   // □
                            gen_opc_stack.push(0);   // □
                            gen_opc_stack.push(0);   // □
                        }
                        else   // S    ->  -GVU
                            if (input_leks.get_l_type() == 4) {             // leksema = -
                                t.leks_number = -1; t.name = 'D';   g_stack.push(t);    // D
                                t.leks_number = -1; t.name = 'U';   g_stack.push(t);    // U
                                t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                                t.leks_number = -1; t.name = 'G';   g_stack.push(t);    // G
                                t.leks_number = 4;                  g_stack.push(t);    // -
                            
                                gen_opc_stack.push(0);    // □
                                gen_opc_stack.push(0);    // □
                                gen_opc_stack.push(14);   // ~
                                gen_opc_stack.push(0);    // □
                                gen_opc_stack.push(0);    // □
                            }
                            else   // S    ->  (S)VU
                                if (input_leks.get_l_type() == 7) {             // leksema = (
                                    t.leks_number = -1; t.name = 'D';   g_stack.push(t);    // D
                                    t.leks_number = -1; t.name = 'U';   g_stack.push(t);    // U
                                    t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                                    t.leks_number = 8;                  g_stack.push(t);    // )
                                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                                    t.leks_number = 7;                  g_stack.push(t);    // (
                                    
                                    gen_opc_stack.push(0);   // □
                                    gen_opc_stack.push(0);   // □
                                    gen_opc_stack.push(0);   // □
                                    gen_opc_stack.push(0);   // □
                                    gen_opc_stack.push(0);   // □
                                    gen_opc_stack.push(0);   // □
                                }
                                else {
                                    /* error */
                                    error_flag = true;    // пусть будет 1 пока что
                                    std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position() << "\nОписание: " << g_st_top.name << '\n';

                                } break;
                //
                //
                //
            case 'D':
                if (input_leks.get_l_type() == 10) {            // leksema = ==
                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 10;                 g_stack.push(t);    // ==
                
                    gen_opc_stack.push(6);   // ==
                    gen_opc_stack.push(0);   // □
                    gen_opc_stack.push(0);   // □
                    
                }
                else
                    if (input_leks.get_l_type() == 11) {            // leksema = !=
                        t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                        t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                        t.leks_number = 11;                 g_stack.push(t);    // !=
                        
                        gen_opc_stack.push(11);   // !=
                        gen_opc_stack.push(0);    // □
                        gen_opc_stack.push(0);    // □
                        
                    }
                    else
                        if (input_leks.get_l_type() == 12) {            // leksema = >
                            t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                            t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                            t.leks_number = 12;                 g_stack.push(t);    // >
                            
                            gen_opc_stack.push(9);   // >
                            gen_opc_stack.push(0);   // □
                            gen_opc_stack.push(0);   // □
                            
                        }
                        else
                            if (input_leks.get_l_type() == 13) {            // leksema = <
                                t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                                t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                                t.leks_number = 13;                 g_stack.push(t);    // <
                                
                                gen_opc_stack.push(10);   // <
                                gen_opc_stack.push(0);    // □
                                gen_opc_stack.push(0);    // □
                                
                            }
                            else
                                if (input_leks.get_l_type() == 14) {            // leksema = >=
                                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                                    t.leks_number = 14;                 g_stack.push(t);    // >=
                                
                                    gen_opc_stack.push(7);  // >=
                                    gen_opc_stack.push(0);  // □
                                    gen_opc_stack.push(0);  // □
                                    
                                }
                                else
                                    if (input_leks.get_l_type() == 15) {            // leksema = <=
                                        t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                                        t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                                        t.leks_number = 15;                 g_stack.push(t);    // <=

                                        gen_opc_stack.push(8);   // <=
                                        gen_opc_stack.push(0);   // □
                                        gen_opc_stack.push(0);   // □
                                        
                                    }
                                    else {
                                        /*  error   */
                                        error_flag = true;
                                        std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position() << "\nОписание: " << g_st_top.name << '\n';

                                    }break;
                //
                //
                //
            case 'E':   
                if (input_leks.get_l_type() == 20) {            // leksema = else
                    t.leks_number = 21;                 g_stack.push(t);    // endelse
                    t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                    t.leks_number = 20;                 g_stack.push(t);    // else
                
                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(20);   // 2
                }
                else {
                    /*  λ   */
                } break;
                //
                //
                //
            case 'R':   
                if (input_leks.get_l_type() == 2) {             // leksema = a
                    t.leks_number = -1; t.name = 'M';   g_stack.push(t);    // M
                    t.leks_number = 2;                  g_stack.push(t);    // a
                    
                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(28);   // 10
                }
                else {
                    /*      error   */
                    error_flag = true;
                    std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position() << "\nОписание: " << g_st_top.name << '\n';

                } break;
            case 'M':   
                if (input_leks.get_l_type() == 29) {            // leksema = ,
                    t.leks_number = -1; t.name = 'M';   g_stack.push(t);    // M
                    t.leks_number = 2;                  g_stack.push(t);    // a
                    t.leks_number = 29;                 g_stack.push(t);    // ,
                    
                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(28);   // 10
                    gen_opc_stack.push(0);    // □
                }
                else {
                    /*   λ   */
                } break;
            case 'N':   
                if (input_leks.get_l_type() == 2) {             // leksema = a
                    t.leks_number = -1; t.name = 'O';   g_stack.push(t);    // O
                    t.leks_number = 17;                 g_stack.push(t);    // ]
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 16;                 g_stack.push(t);    // [
                    t.leks_number = 2;                  g_stack.push(t);    // a
                    
                    gen_opc_stack.push(0);     // □
                    gen_opc_stack.push(17);    // m
                    gen_opc_stack.push(0);     // □
                    gen_opc_stack.push(0);     // □
                    gen_opc_stack.push(28);    // 10
                }
                else {
                    /*  error   */
                    error_flag = true;
                } break;
            case 'O':   
                if (input_leks.get_l_type() == 29) {            // leksema = ,
                    t.leks_number = -1; t.name = 'O';   g_stack.push(t);    // O
                    t.leks_number = 17;                 g_stack.push(t);    // ]
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 16;                 g_stack.push(t);    // [
                    t.leks_number = 2;                  g_stack.push(t);    // a
                    t.leks_number = 29;                 g_stack.push(t);    // ,
                    
                    gen_opc_stack.push(0);     // □
                    gen_opc_stack.push(17);    // m
                    gen_opc_stack.push(0);     // □
                    gen_opc_stack.push(0);     // □
                    gen_opc_stack.push(28);    // 10
                    gen_opc_stack.push(0);     // □
                }
                else {
                    /*   λ   */
                } break;
            }
        }
        else {
            if (g_st_top.leks_number != input_leks.get_l_type()) {
                error_flag = true;
                std::cout << "Строка: " << input_leks.get_start_str_number() <<
                    " Позиция: " << input_leks.get_start_str_position() << "\nОписание: ожидалось ." << leksema_list[g_st_top.leks_number] << '.';
                //return;
            }
            else {
                ready_to_read_next_leksem = true;
            }

        }
    }
    return;
}

int main()
{
  
   
    /*stack<int> s;
    s.push(1);
    s.push(4);
    s.push(12);
    while (!s.empty()) {
        cout << s.top()<<' ';
        s.pop();
    }
    return 0;

    string leksema_list_ST[30] = {"T", "k", "a",
        "+", "-", "*", "/",
        "(", ")" ,
        "=" , "==" , "!=" , ">" , "<" , ">=" , "<=" ,
        "[" , "]" ,
        "if" , "endif" ,
        "else" , "endelse" ,
        "while" , "endwhile" , 
        ";" , "read" , "write" ,
        "int" , "int1", "," };
    string neterms[17] = {
        "X","P","S","U","T","V","F","G","H","Z","C","D","E","R","M","N","O"
    };*/
    /*G terms_and_noterms[47];
    for (int i = 0; i < 30; i++) {
        terms_and_noterms[i].is_terminal = true;
        terms_and_noterms[i].name = leksema_list_ST[i];
        //terms_and_noterms[i].leks.set_l_type(i + 1);
    }
    for (int i = 30; i < 47; i++) {
        terms_and_noterms[i].is_terminal = false;
        terms_and_noterms[i].name = neterms[i-30];
    }
    //terms_and_noterms[0]

    for (int i = 0; i < 47; i++) {
        cout << i<<' '<<terms_and_noterms[i].is_terminal << " - " << terms_and_noterms[i].name << '\n';
    }
    return 0;
    */
    
    SetConsoleCP(1251); // устанавливаем кодировку для ввода\вывода на консоль
    SetConsoleOutputCP(1251);
    std::ifstream program_file("prog.txt"); // программа пишется в файле
    if (!program_file.is_open()) { return 0; }
    int index;
    std::vector<std::string>leksema_list = { "конец программы", "константа", "переменная", "+", "-", "*", "/", "(", ")" , "=" , "==" , "!=" , ">" , "<" , ">=" , "<=" , "[" , "]" , "if" , "endif" , "else" , "endelse" , "while" , "endwhile" , ";" , "read" , "write" , "int" , "int1", "," };
    std::vector<std::string>error_list = { "Недопустимый символ в программе.", "Лексема не распознана: Что-то", "Лексема не распознана: ожидалось !=." };
    bool end_flag = true; bool error_flag = 0, str_number = 1, str_position = 1;
    leksema x;

    /*while (error_flag == 0 && end_flag) {
        x = Lexical_tokenizator(program_file, str_number, str_position, error_flag);
        if (error_flag == 0) {
            index = x.get_l_type();
            switch (index) {
            case 0:
                std::cout << index << " : " << leksema_list[index] << std::endl;
                end_flag = false;
                break;
            case 1:
                std::cout << index << " : " << leksema_list[index] << " : " << x.get_value() << std::endl;
                break;
            case 2:
                std::cout << index << " : " << leksema_list[index] << " : " << x.get_name() << std::endl;
                break;
            default:
                std::cout << index << " : " << leksema_list[index] << std::endl;
                break;
            }
        }
    }
    if (error_flag > 0) {
        std::cout << "Строка: " << str_number << " Позиция: " << str_position << " Код ошибки: " << error_flag << "\nОписание: " << error_list[error_flag - 1] << std::endl;
    }
    */
    imply_Grammar(program_file, error_flag);
    program_file.close();
    
    return 0;
}
