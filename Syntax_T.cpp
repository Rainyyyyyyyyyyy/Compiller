
#include <string>
#include <Windows.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <queue>
using namespace std;
class leksema {
    int l_type;         // номер лексеммы
    std::string name;   // название (если перем-я, то имя перем-й)
    int value;          // значение (если константа, то знач. константы)
public:
    leksema(int l_type1 = 0, std::string name1 = "", int value1 = 0);
    leksema(const leksema& a);                  // конструктор копирования
    ~leksema() {};                              // деструктор
    leksema& operator = (const leksema& a);     // =
    bool operator ==(const leksema&);
    int get_l_type();                           // номер лексеммы
    std::string get_name();
    int get_value();
    void set_l_type(int a);
    void set_name(std::string a);
    void set_value(int a);
};
leksema::leksema(int l_type1, std::string name1, int value1){
    l_type = l_type1;
    name = name1;
    value = value1;
};// конструктор копирования
leksema& leksema::operator = (const leksema& a){
    l_type = a.l_type;
    name = a.name;
    value = a.value;
    return *this;
};
leksema::leksema(const leksema &a){
    l_type = a.l_type;
    name = a.name;
    value = a.value;
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

leksema Lexical_tokenizator(std::ifstream& program_file, int& str_number, int& str_position, int& error_flag) { // Лексический анализатор
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
    error_flag = 1;  // ошибка: недопустимый символ языка
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
        error_flag = 2; // ошибка
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
    error_flag = 3; // ошибка не !=
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

struct G {
    bool is_terminal = false;
    std::string name = "";   // если нетерминал, то здесь будет название нетерминала
};


struct stack {                      // магазин синтаксического анализатора
    stack *next = NULL;
    G data;
};

struct opc_stack {                  // магазин генератора ОПС
    opc_stack *next = NULL;
    string data;
};

void stack_pop(stack* head) {
    if (head->next == NULL) {
        delete head; head = NULL;
        return;
    }
    stack* t = head->next;
    delete head;
    head = t;
}


/*
* program_file - файл с проверяемой программов
* str_number - номер строки
* str_position - позиция в строке
* error_flag - код ошибки
* g_stack - магазин синтаксического анализатора
* (*) G - структура для одной ячейки магазина синтаксического генератора (описана выше)
*  
*/


void imply_Grammar(std::ifstream &program_file, int &str_number, int &str_position, int &error_flag, stack* g_stack, opc_stack* gen_opc, opc_stack *opc, G* t_not_list) {
    if (g_stack->data.is_terminal == false){
        if (g_stack->data.name == "X"){
            leksema input_leks = 
                Lexical_tokenizator(program_file, str_number, str_position, error_flag);
            if (input_leks.get_l_type() == 2){  // X -> aH=SZ; PZ   |   a...=..9    | '.' - пусто квадратик
                stack* temp = new stack;
                temp->next = g_stack;
                g_stack = temp;
                temp->data = t_not_list[39];    // Z
                //
                stack* temp = new stack;
                temp->next = g_stack;
                g_stack = temp;
                temp->data = t_not_list[31];    // P
                //
                stack* temp = new stack;
                temp->next = g_stack;
                g_stack = temp;
                temp->data = t_not_list[24];    // ;
                //
                stack* temp = new stack;
                temp->next = g_stack;
                g_stack = temp;
                temp->data = t_not_list[39];    // Z
                //
                stack* temp = new stack;
                temp->next = g_stack;
                g_stack = temp;
                temp->data = t_not_list[32];    // S
                //
                stack* temp = new stack;
                temp->next = g_stack;
                g_stack = temp;
                temp->data = t_not_list[9];     // =
                //
                stack* temp = new stack;
                temp->next = g_stack;
                g_stack = temp;
                temp->data = t_not_list[38];    // H
                //
                stack* temp = new stack;
                temp->next = g_stack;
                g_stack = temp;
                temp->data = t_not_list[2];    // a
                //
                //
                // semantic action for opc's generator:    a...=..9
                opc_stack *ttemp = new opc_stack;
                ttemp->next = gen_opc;
                gen_opc = ttemp;
                ttemp->data = "9";
                //
                opc_stack* ttemp = new opc_stack;
                ttemp->next = gen_opc;
                gen_opc = ttemp;
                ttemp->data = ".";
                //
                opc_stack* ttemp = new opc_stack;
                ttemp->next = gen_opc;
                gen_opc = ttemp;
                ttemp->data = ".";
                //
                opc_stack* ttemp = new opc_stack;
                ttemp->next = gen_opc;
                gen_opc = ttemp;
                ttemp->data = "=";
                //
                opc_stack* ttemp = new opc_stack;
                ttemp->next = gen_opc;
                gen_opc = ttemp;
                ttemp->data = ".";
                //
                opc_stack* ttemp = new opc_stack;
                ttemp->next = gen_opc;
                gen_opc = ttemp;
                ttemp->data = ".";
                //
                opc_stack* ttemp = new opc_stack;
                ttemp->next = gen_opc;
                gen_opc = ttemp;
                ttemp->data = ".";
                //
                opc_stack* ttemp = new opc_stack;
                ttemp->next = gen_opc;
                gen_opc = ttemp;
                ttemp->data = "a";
                


            }// // X -> aH=SZ; PZ   |   a...=..9    | '.' - пусто квадратик
        }
    }
}

int main()
{

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
    };
    G terms_and_noterms[47];
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
    
    /*
    SetConsoleCP(1251); // устанавливаем кодировку для ввода\вывода на консоль
    SetConsoleOutputCP(1251);
    std::ifstream program_file("prog.txt"); // программа пишется в файле
    if (!program_file.is_open()) { return 0; }
    int index;
    std::vector<std::string>leksema_list = { "конец программы", "константа", "переменная", "+", "-", "*", "/", "(", ")" , "=" , "==" , "!=" , ">" , "<" , ">=" , "<=" , "[" , "]" , "if" , "endif" , "else" , "endelse" , "while" , "endwhile" , ";" , "read" , "write" , "int" , "int1", "," };
    std::vector<std::string>error_list = { "Недопустимый символ в программе.", "Лексема не распознана: Что-то", "Лексема не распознана: ожидалось !=." };
    bool end_flag = true; int error_flag = 0, str_number = 1, str_position = 1;
    leksema x;

    while (error_flag == 0 && end_flag) {
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

    program_file.close();
    */
    return 0;
}
