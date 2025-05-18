#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>
#include <fstream>
#include <stack>

class leksema {
    int l_type; std::string name; int value; int start_str_position; int start_str_number;
public:
    leksema(int l_type1 = 0, std::string name1 = "", int value1 = 0, int start_str_position1 = 0, int start_str_number1 = 0);
    leksema(const leksema&); // конструктор копирования
    ~leksema() {}; // деструктор
    leksema& operator = (const leksema& a);
    int get_l_type();
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

leksema::leksema(int l_type1, std::string name1, int value1, int st_str_pos, int st_str_num) {
    l_type = l_type1;
    name = name1;
    value = value1;
    start_str_position = st_str_pos;
    start_str_number = st_str_num;
};// конструктор копирования
leksema& leksema::operator = (const leksema& a) {
    if (this == &a)return *this;
    l_type = a.l_type;
    name = a.name;
    value = a.value;
    start_str_number = a.start_str_number;
    start_str_position = a.start_str_position;
    return *this;
};
leksema::leksema(const leksema& a) {
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
int leksema::get_start_str_position() {
    return start_str_position;
}
int leksema::get_start_str_number() {
    return start_str_number;
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
void leksema::set_start_str_position(int a) {
    start_str_position = a;
}
void leksema::set_start_str_number(int a) {
    start_str_number = a;
}

leksema Lexical_tokenizator(std::ifstream& program_file, int& str_number, int& str_position, bool& error_flag) { // Лексический анализатор
    unsigned char Ci;
    std::string name;
    int value, index;
    std::string a = "+-*/()[];,";
    std::string a1 = "><";
    std::vector<int> b = { 3,4,5,6,7,8,16,17,24,29 }; // номера лексем из a
    std::vector<std::string> function_words_a{ "if", "endif", "else", "endelse", "while", "endwhile", "read", "write", "int", "int1" }; // служебные слова
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
    std::cout << "Строка: " << str_number << " Позиция: " << str_position
        << "\nОписание: " << "Лексема не распознана: Недопустимый символ в программе." << std::endl;
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
        std::cout << "Строка: " << str_number << " Позиция: " << str_position
            << "\nОписание: " << "Лексема не распознана: неожиданный символ." << std::endl;
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
    std::cout << "Строка: " << str_number << " Позиция: " << str_position
        << "\nОписание: " << "Лексема не распознана: ожидалось !=." << std::endl;
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
    int leks_number;        // номер лексеммы (если лексемма)
    /* leks_number<0    <->    не лексемма, а нетерминал     */
    char name;           // название нетерминала

    Gramar_stack_element(int n = NULL, char nam = NULL) {
        leks_number = n;
        name = nam;
    }
};
struct Mas_passport {
    int size;               // кол-во ячеек
    int* link;              // ссылка на начало массива

    Mas_passport(int n = NULL) {
        size = n;
        link = NULL;
    }
};
struct OPC_element {
    int element_type;           // номер смыслового элемента
    int value;                  // значение константы (если element_type = 1)
    int* a;                     // ссылка на значение переменной (если element_type = 2)
    Mas_passport* m;            // ссылка на паспорт массива (если element_type = 3)
    int str_number;             // позиция соответствующей лексеммы (строка)
    int str_position;           // позиция соответствующей лексеммы (символ в строке)

    OPC_element(int t = NULL, int v = NULL, int* a1 = NULL, Mas_passport* m1 = NULL, int sn = NULL, int sp = NULL) {
        element_type = t;           // номер смыслового элемента
        value = v;                  // значение (
        a = a1;
        m = m1;
        str_number = sn;
        str_position = sp;
    }
};
struct Var_table_element {          // таблица переменных
    std::string name;               // название переменной
    int a;                          // значение переменной

    Var_table_element(std::string name1 = "", int a1 = 0) {
        name = name1;
        a = a1;
    }
};
struct Passport_table_element {         // паспорт массива
    std::string name;                   // название массива
    Mas_passport m;                     // под-структура для адреса начала и количества ячеек

    Passport_table_element(std::string name1 = "", int n = 0) {
        name = name1;
        m = Mas_passport(n);
    }
};

/*
* program_file - файл с проверяемой программов
* str_number - номер строки
* str_position - позиция в строке
* error_flag - код ошибки
* g_stack - магазин синтаксического анализатора
* (*) G - структура для одной ячейки магазина синтаксического генератора (описана выше)
*
*/


// вспомогательный (для вывода) список лексемм
std::vector<std::string> leksema_list = { "конец программы", "константа", "переменная", "+", "-", "*", "/", "(", ")" , "=" , "==" , "!=" , ">" , "<" , ">=" , "<=" , "[" , "]" , "if" , "endif" , "else" , "endelse" , "while" , "endwhile" , ";" , "read" , "write" , "int" , "int1", "," };
// вспомгательный (для вывода) список операций ОПС
std::vector<std::string> OPC_operation_list = { "+", "-", "*" , "/" , "=" , "==" , ">=" , "<=" , ">" , "<" , "!=" , "~" , "read" , "write" , "Выделение памяти" , "Индексация" , "jf (переход по условию ложь)" , "j (безусловный переход)" , "Освобождение памяти" };
std::vector<Var_table_element> Var_table; // Таблица переменных
std::vector<Passport_table_element> Passport_table; // Таблица паспартов массивов

std::vector<OPC_element> imply_Grammar(std::ifstream& program_file, bool& error_flag) {
    bool end_of_program_flag = false;           // флаг конца программы
    bool ready_to_read_next_leksem = true;      // флаг готовности к чтению очередной лексеммы
    bool not_found_flag;                        // флаг успешности поиска переменной\массива в соответствующих таблицах
    leksema input_leks;                         // входящая с файла с программой лексемма

    Gramar_stack_element t;             // вспомогательный элемент магазина грамматики (для заполнения)
    Gramar_stack_element g_st_top;      // вспомогательный элемент магазина грамматики (голова стека)
    std::stack<Gramar_stack_element> g_stack;// магазин грамматики 
    t.leks_number = 0;  g_stack.push(t);        // начальная конфигурация магазина грамматики (X, перевёрнутая_Т)
    t.leks_number = -1; t.name = 'X';   g_stack.push(t); 

    std::vector<OPC_element> OPC;                  // готовая строка ОПС
    std::stack<int> gen_opc_stack;           // магазин генератора ОПС
    int gen_opc_top;                // вспомогательный элемент магазина генератора ОПС (голова)
    gen_opc_stack.push(0); gen_opc_stack.push(0); // начальная конфигурация магазина генератора ОПС (□, □)
    std::stack<int> marker_stack; // магазин меток
    int k = 0; // индекс генерируемого элемента ОПС
    std::stack<int> str_number_stack;           // позиция лексеммы (строка)
    std::stack<int> str_position_stack;         // позиция лексеммы (в строке)

    int str_number = 1;
    int str_position = 1;

    while (!error_flag && !end_of_program_flag) {   // пока не конец программы и нет ошибки
        if (ready_to_read_next_leksem) {            // если готовы читать очередную лексемму
            input_leks = Lexical_tokenizator(program_file, str_number, str_position, error_flag);
            ready_to_read_next_leksem = false;
        }

        g_st_top = g_stack.top();           // * выносим из стека магазина грамматики
        g_stack.pop();                      // * терминал\нетерминал, который был на вершине
        gen_opc_top = gen_opc_stack.top();
        gen_opc_stack.pop();                // * и удаляем его из стека

        if (g_st_top.leks_number == -1) {           // если нетерминал
            switch (g_st_top.name) {        // разбор случаев по нетерминалам
            case 'X':
                switch (input_leks.get_l_type())    // разбор случаев по текущей лексемме
                {
                case 2: // leksema = a  |   X -> aH=SZ;PZ
                        //              |        a□□□=□□9                                    
                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                    t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                    t.leks_number = 24;                 g_stack.push(t);    // ;
                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 9;                  g_stack.push(t);    // =
                    t.leks_number = -1; t.name = 'H';   g_stack.push(t);    // H
                    t.leks_number = 2;                  g_stack.push(t);    // a          

                    gen_opc_stack.push(26);                                 // 9
                    gen_opc_stack.push(0);                                  // □
                    gen_opc_stack.push(0);                                  // □
                    gen_opc_stack.push(5);                                  // =
                    gen_opc_stack.push(0);                                  // □
                    gen_opc_stack.push(0);                                  // □
                    gen_opc_stack.push(0);                                  // □
                    gen_opc_stack.push(17);                                 // a

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;

                case 18: // leksema = if    |   X -> if(C) P endif EZ; PZ
                         //                 |        □ □□1 □   □   □3□ □9
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

                    gen_opc_stack.push(26); // 9
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
                    break;

                case 22: // leksema = while     |   X -> while(C) P endwhile Z; PZ
                         //                     |          4  □□1 □     □    5□ □9
                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                    t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                    t.leks_number = 24;                 g_stack.push(t);    // ;
                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                    t.leks_number = 23;                 g_stack.push(t);    // endwhile
                    t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                    t.leks_number = 8;                  g_stack.push(t);    // )
                    t.leks_number = -1; t.name = 'C';   g_stack.push(t);    // C
                    t.leks_number = 7;                  g_stack.push(t);    // (
                    t.leks_number = 22;                 g_stack.push(t);    // while

                    gen_opc_stack.push(26); // 9
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(23); // 5
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(19); // 1
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(22); // 4
                    break;

                case 25: // leksema = read  |   X -> read(aH); PZ
                         //                 |          □ □a□r□ □9
                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                    t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                    t.leks_number = 24;                 g_stack.push(t);    // ;
                    t.leks_number = 8;                  g_stack.push(t);    // )
                    t.leks_number = -1; t.name = 'H';   g_stack.push(t);    // H
                    t.leks_number = 2;                  g_stack.push(t);    // a
                    t.leks_number = 7;                  g_stack.push(t);    // (
                    t.leks_number = 25;                 g_stack.push(t);    // read

                    gen_opc_stack.push(26); // 9
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(13); // r
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(17); // a
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;

                case 26: // leksema = write     |   X -> write(S); PZ
                         //                     |           □ □□w□ □9
                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                    t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                    t.leks_number = 24;                 g_stack.push(t);    // ;
                    t.leks_number = 8;                  g_stack.push(t);    // )
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 7;                  g_stack.push(t);    // (
                    t.leks_number = 26;                 g_stack.push(t);    // write

                    gen_opc_stack.push(26); // 9
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(14); // w
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;

                case 27: // leksema = int   |   X -> int R; PZ
                         //                 |          □ □□ □9
                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                    t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                    t.leks_number = 24;                 g_stack.push(t);    // ;
                    t.leks_number = -1; t.name = 'R';   g_stack.push(t);    // R
                    t.leks_number = 27;                 g_stack.push(t);    // int

                    gen_opc_stack.push(26); // 9
                    gen_opc_stack.push(0);  // □ 
                    gen_opc_stack.push(0);  // □ 
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □ 
                    break;

                case 28: // leksema = int1  |   X -> int1 N; PZ
                         //                 |          □  □□ □9
                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                    t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                    t.leks_number = 24;                 g_stack.push(t);    // ;
                    t.leks_number = -1; t.name = 'N';   g_stack.push(t);    // N
                    t.leks_number = 28;                 g_stack.push(t);    // int1

                    gen_opc_stack.push(26); // 9
                    gen_opc_stack.push(0);  // □ 
                    gen_opc_stack.push(0);  // □ 
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □ 
                    break;

                default: /*  error  */
                    error_flag = true;
                    std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position()
                        << "\nОписание: " << g_st_top.name << '\n';
                    break;
                }
                break;

                //------------------------------------------------------------------//

            case 'P':
                switch (input_leks.get_l_type())
                {
                case 2: // leksema = a  |   P -> aH=SZ;P
                        //              |        a□□□=□□
                    t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                    t.leks_number = 24;                 g_stack.push(t);    // ;
                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 9;                  g_stack.push(t);    // =
                    t.leks_number = -1; t.name = 'H';   g_stack.push(t);    // H
                    t.leks_number = 2;                  g_stack.push(t);    // a          

                    gen_opc_stack.push(0);                                  // □
                    gen_opc_stack.push(0);                                  // □
                    gen_opc_stack.push(5);                                  // =
                    gen_opc_stack.push(0);                                  // □
                    gen_opc_stack.push(0);                                  // □
                    gen_opc_stack.push(0);                                  // □
                    gen_opc_stack.push(17);                                 // a

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;

                case 18: // leksema = if    |   P -> if(C) P endif EZ; P
                         //                 |         □□□1 □    □  □3□ □
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
                    break;

                case 22: // leksema = while |   P -> while(C) P endwhile Z; P
                         //                 |           4 □□1 □     □    5□ □
                    t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                    t.leks_number = 24;                 g_stack.push(t);    // ;
                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                    t.leks_number = 23;                 g_stack.push(t);    // endwhile
                    t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
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
                    break;

                case 25: // leksema = read  |   P -> read(aH); P
                         //                 |         □  □a□r□ □
                    t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                    t.leks_number = 24;                 g_stack.push(t);    // ;
                    t.leks_number = 8;                  g_stack.push(t);    // )
                    t.leks_number = -1; t.name = 'H';   g_stack.push(t);    // H
                    t.leks_number = 2;                  g_stack.push(t);    // a
                    t.leks_number = 7;                  g_stack.push(t);    // (
                    t.leks_number = 25;                 g_stack.push(t);    // read

                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(13); // r
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(17); // a
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;

                case 26: // leksema = write     |   P -> write(S); P
                         //                     |           □ □□w□ □
                    t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                    t.leks_number = 24;                 g_stack.push(t);    // ;
                    t.leks_number = 8;                  g_stack.push(t);    // )
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 7;                  g_stack.push(t);    // (
                    t.leks_number = 26;                 g_stack.push(t);    // write

                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(14); // w
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;

                case 27: // leksema = int   |   P -> int R; P
                         //                           □  □□ □
                    t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                    t.leks_number = 24;                 g_stack.push(t);    // ;
                    t.leks_number = -1; t.name = 'R';   g_stack.push(t);    // R
                    t.leks_number = 27;                 g_stack.push(t);    // int

                    gen_opc_stack.push(0);  // □ 
                    gen_opc_stack.push(0);  // □ 
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □ 
                    break;

                case 28: // leksema = int1  |   P -> int1 N; P
                         //                 |          □  □□ □
                    t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                    t.leks_number = 24;                 g_stack.push(t);    // ;
                    t.leks_number = -1; t.name = 'N';   g_stack.push(t);    // N
                    t.leks_number = 28;                 g_stack.push(t);    // int1

                    gen_opc_stack.push(0);  // □ 
                    gen_opc_stack.push(0);  // □ 
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □ 
                    break;

                default: /*  λ  */
                    break;
                }
                break;

                //------------------------------------------------------------------//

            case 'S':
                switch (input_leks.get_l_type())
                {
                case 1: // leksema = const (k) | S  ->  kVU
                        //                     |        k□□
                    t.leks_number = -1; t.name = 'U';   g_stack.push(t);    // U
                    t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                    t.leks_number = 1;                  g_stack.push(t);    // k

                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(18); // k
                    break;

                case 2: // leksema = a | S  ->  aHVU
                        //             |        a□□□
                    t.leks_number = -1; t.name = 'U';   g_stack.push(t);    // U
                    t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                    t.leks_number = -1; t.name = 'H';   g_stack.push(t);    // H
                    t.leks_number = 2;                  g_stack.push(t);    // a

                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(17); // a
                    break;

                case 3: // leksema = + | S  ->  +GVU
                        //             |        □□□□
                    t.leks_number = -1; t.name = 'U';   g_stack.push(t);    // U
                    t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                    t.leks_number = -1; t.name = 'G';   g_stack.push(t);    // G
                    t.leks_number = 3;                  g_stack.push(t);    // +

                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    break;

                case 4: // leksema = - | S  ->  -GVU
                        //             |        □□~□
                    t.leks_number = -1; t.name = 'U';   g_stack.push(t);    // U
                    t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                    t.leks_number = -1; t.name = 'G';   g_stack.push(t);    // G
                    t.leks_number = 4;                  g_stack.push(t);    // -

                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(12); // ~
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;

                case 7: // leksema = ( | S  ->  (S)VU
                        //             |        □□□□□
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
                    break;

                default:  /* error */
                    error_flag = true;
                    std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position()
                        << "\nОписание: " << g_st_top.name << '\n';
                    break;
                }
                break;

                //------------------------------------------------------------------//

            case 'U':
                switch (input_leks.get_l_type())
                {
                case 3: // leksema = +  |   U -> +TU
                        //              |        □□+
                    t.leks_number = -1; t.name = 'U';   g_stack.push(t);    // U
                    t.leks_number = -1; t.name = 'T';   g_stack.push(t);    // T
                    t.leks_number = 3;                  g_stack.push(t);    // +

                    gen_opc_stack.push(1);  // +
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;

                case 4: // leksema = -  |   U -> -TU
                        //              |        □□-
                    t.leks_number = -1; t.name = 'U';   g_stack.push(t);    // U
                    t.leks_number = -1; t.name = 'T';   g_stack.push(t);    // T
                    t.leks_number = 4;                  g_stack.push(t);    // -

                    gen_opc_stack.push(2);  // -
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;

                default: /*  λ   */
                    break;
                }
                break;

                //------------------------------------------------------------------//

            case 'T':
                switch (input_leks.get_l_type())
                {
                case 1: // leksema = k (константа)  |   T -> kV
                        //                          |        k□
                    t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                    t.leks_number = 1;                  g_stack.push(t);    // k

                    gen_opc_stack.push(0);   // □
                    gen_opc_stack.push(18);  // k
                    break;

                case 2: // leksema = a | T  ->  aHV
                        //             |        a□□
                    t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                    t.leks_number = -1; t.name = 'H';   g_stack.push(t);    // H
                    t.leks_number = 2;                  g_stack.push(t);    // a

                    gen_opc_stack.push(0);   // □
                    gen_opc_stack.push(0);   // □
                    gen_opc_stack.push(17);  // a
                    break;

                case 3: // leksema = + | T  ->  +GV
                        //             |        □□□
                    t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                    t.leks_number = -1; t.name = 'G';   g_stack.push(t);    // G
                    t.leks_number = 3;                  g_stack.push(t);    // +

                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    break;

                case 4: // leksema = - | T   ->  -GV
                        //             |         □□~
                    t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                    t.leks_number = -1; t.name = 'G';   g_stack.push(t);    // G
                    t.leks_number = 4;                  g_stack.push(t);    // -
                    
                    gen_opc_stack.push(12);  // ~
                    gen_opc_stack.push(0);   // □
                    gen_opc_stack.push(0);   // □
                    

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;

                case 7: // leksema = (  |   T -> (S)V
                        //              |        □□□□
                    t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                    t.leks_number = 8;                  g_stack.push(t);    // )
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 7;                  g_stack.push(t);    // (

                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □
                    break;

                default: /* error */
                    error_flag = true;
                    std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position()
                        << "\nОписание: " << g_st_top.name << '\n';
                    break;
                }
                break;

                //------------------------------------------------------------------//

            case 'V':
                switch (input_leks.get_l_type())
                {
                case 5: // leksema = *  |   V -> *FV
                        //              |        □□*
                    t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                    t.leks_number = -1; t.name = 'F';   g_stack.push(t);    // F
                    t.leks_number = 5;                  g_stack.push(t);    // *

                    gen_opc_stack.push(3);  // *
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;

                case 6: // leksema = /  |   V -> /FV
                        //              |        □□/
                    t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                    t.leks_number = -1; t.name = 'F';   g_stack.push(t);    // F
                    t.leks_number = 6;                  g_stack.push(t);    // /

                    gen_opc_stack.push(4);  // /
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;

                default: /*  λ   */
                    break;
                }
                break;

                //------------------------------------------------------------------//

            case 'F':
                switch (input_leks.get_l_type())
                {
                case 1: // leksema = k  |   F -> k
                        //              |        k
                    t.leks_number = 1;                  g_stack.push(t);    // k

                    gen_opc_stack.push(18); // k
                    break;

                case 2: // leksema = a  |   F -> aH
                        //              |        a□
                    t.leks_number = -1; t.name = 'H';   g_stack.push(t);    // H
                    t.leks_number = 2;                  g_stack.push(t);    // a

                    gen_opc_stack.push(0);   // □
                    gen_opc_stack.push(17);  // a
                    break;

                case 3: // leksema = +  |   F -> +G
                        //              |        □□
                    t.leks_number = -1; t.name = 'G';   g_stack.push(t);    // G
                    t.leks_number = 3;                  g_stack.push(t);    // +

                    gen_opc_stack.push(0);   // □
                    gen_opc_stack.push(0);   // □
                    break;

                case 4: // leksema = -  |   F -> -GZ
                        //              |        □□~
                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                    t.leks_number = -1; t.name = 'G';   g_stack.push(t);    // G
                    t.leks_number = 4;                  g_stack.push(t);    // -

                    gen_opc_stack.push(12);    // ~
                    gen_opc_stack.push(0);     // □
                    gen_opc_stack.push(0);     // □

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;
                case 7: // leksemma = ( |   F -> (S)
                        //              |        □□□
                    t.leks_number = 8;                  g_stack.push(t);    // )
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 7;                  g_stack.push(t);    // (

                    gen_opc_stack.push(0);   // □
                    gen_opc_stack.push(0);   // □
                    gen_opc_stack.push(0);   // □
                    break;

                default: /*  error   */
                    error_flag = true;
                    std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position()
                        << "\nОписание: " << g_st_top.name << '\n';
                    break;
                }
                break;

                //------------------------------------------------------------------//

            case 'G':
                switch (input_leks.get_l_type())
                {
                case 1: // leksema = k  |   G -> k
                        //              |        k
                    t.leks_number = 1;                  g_stack.push(t);    // k

                    gen_opc_stack.push(18);  // k
                    break;

                case 2: // leksema = a  |   G -> aH
                        //              |        a□
                    t.leks_number = -1; t.name = 'H';   g_stack.push(t);    // H
                    t.leks_number = 2;                  g_stack.push(t);    // a

                    gen_opc_stack.push(0);   // □
                    gen_opc_stack.push(17);  // a
                    break;

                case 7: // leksema = (  |   G -> (S)
                        //              |        □□□
                    t.leks_number = 8;                  g_stack.push(t);    // )
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 7;                  g_stack.push(t);    // (

                    gen_opc_stack.push(0);   // □
                    gen_opc_stack.push(0);   // □
                    gen_opc_stack.push(0);   // □
                    break;

                default: /*  error   */
                    error_flag = true;
                    std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position()
                        << "\nОписание: " << g_st_top.name << '\n';
                    break;
                }
                break;

                //------------------------------------------------------------------//

            case 'H':
                switch (input_leks.get_l_type())
                {
                case 16: // leksema = [ |   H -> [S]
                         //             |        □□i
                    t.leks_number = 17;                 g_stack.push(t);    // ]
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 16;                 g_stack.push(t);    // [

                    gen_opc_stack.push(16);   // i
                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(0);    // □

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;

                default: /*  λ   */
                    break;
                }
                break;

                //------------------------------------------------------------------//

            case 'Z': /*  λ   */
                break;

                //------------------------------------------------------------------//

            case 'C':
                switch (input_leks.get_l_type())
                {
                case 1: // leksema = k  |   C -> kVUD
                        //              |        k□□□
                    t.leks_number = -1; t.name = 'D';   g_stack.push(t);    // D
                    t.leks_number = -1; t.name = 'U';   g_stack.push(t);    // U
                    t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                    t.leks_number = 1;                  g_stack.push(t);    // k

                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(18);   // k
                    break;

                case 2: // leksema = a  |   C -> aHVUD
                        //              |        a□□□□
                    t.leks_number = -1; t.name = 'D';   g_stack.push(t);    // D
                    t.leks_number = -1; t.name = 'U';   g_stack.push(t);    // U
                    t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                    t.leks_number = -1; t.name = 'H';   g_stack.push(t);    // H
                    t.leks_number = 2;                  g_stack.push(t);    // a

                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(17);   // a
                    break;

                case 3: // leksema = + | S  ->  +GVUD
                        //             |        □□□□□
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
                    break;

                case 4: // leksema = - | S  ->  -GVUD
                        //             |        □□~□□
                    t.leks_number = -1; t.name = 'D';   g_stack.push(t);    // D
                    t.leks_number = -1; t.name = 'U';   g_stack.push(t);    // U
                    t.leks_number = -1; t.name = 'V';   g_stack.push(t);    // V
                    t.leks_number = -1; t.name = 'G';   g_stack.push(t);    // G
                    t.leks_number = 4;                  g_stack.push(t);    // -

                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(12);   // ~
                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(0);    // □

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;
                case 7: // leksema = ( | S  ->  (S)VUD
                        //             |        □□□□□□
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
                    break;

                default: /* error */
                    error_flag = true;    
                    std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position()
                        << "\nОписание: " << g_st_top.name << '\n';
                    break;
                }
                break;

                //------------------------------------------------------------------//

            case 'D':
                switch (input_leks.get_l_type())
                {
                case 10: // leksema = ==    |   D -> ==SZ
                         //                 |        □□==
                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 10;                 g_stack.push(t);    // ==

                    gen_opc_stack.push(6);   // ==
                    gen_opc_stack.push(0);   // □
                    gen_opc_stack.push(0);   // □

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;

                case 11: // leksema = !=    |   D -> !=SZ
                         //                 |        □□!=
                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 11;                 g_stack.push(t);    // !=

                    gen_opc_stack.push(11);   // !=
                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(0);    // □

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;

                case 12: // leksema = > |   D -> >SZ
                         //             |        □□>
                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 12;                 g_stack.push(t);    // >

                    gen_opc_stack.push(9);   // >
                    gen_opc_stack.push(0);   // □
                    gen_opc_stack.push(0);   // □

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;

                case 13: // leksema = < |   D -> <SZ
                         //             |        □□<
                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 13;                 g_stack.push(t);    // <

                    gen_opc_stack.push(10);   // <
                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(0);    // □

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;

                case 14: // leksema = >=    |   D -> >=SZ
                         //                 |        □□>=
                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 14;                 g_stack.push(t);    // >=

                    gen_opc_stack.push(7);  // >=
                    gen_opc_stack.push(0);  // □
                    gen_opc_stack.push(0);  // □

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;

                case 15: // leksema = <=    |   D -> <=SZ
                         //                 |        □□<=
                    t.leks_number = -1; t.name = 'Z';   g_stack.push(t);    // Z
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 15;                 g_stack.push(t);    // <=

                    gen_opc_stack.push(8);   // <=
                    gen_opc_stack.push(0);   // □
                    gen_opc_stack.push(0);   // □

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;

                default: /*  error  */
                    error_flag = true;
                    std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position()
                        << "\nОписание: " << g_st_top.name << '\n';
                    break;
                }
                break;

                //------------------------------------------------------------------//

            case 'E':
                switch (input_leks.get_l_type())
                {
                case 20: // leksema = else  |   E -> else P endelse
                         //                 |           2 □  □
                    t.leks_number = 21;                 g_stack.push(t);    // endelse
                    t.leks_number = -1; t.name = 'P';   g_stack.push(t);    // P
                    t.leks_number = 20;                 g_stack.push(t);    // else

                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(20);   // 2
                    break;

                default: /*  λ  */
                    break;
                }
                break;

                //------------------------------------------------------------------//

            case 'R':
                switch (input_leks.get_l_type())
                {
                case 2: // leksema = a  |   R -> aM
                        //              |        6□
                    t.leks_number = -1; t.name = 'M';   g_stack.push(t);    // M
                    t.leks_number = 2;                  g_stack.push(t);    // a

                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(24);   // 6
                    break;

                default: /*  error  */
                    error_flag = true;
                    std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position()
                        << "\nОписание: " << g_st_top.name << '\n';
                    break;
                }
                break;

                //------------------------------------------------------------------//

            case 'M':
                switch (input_leks.get_l_type())
                {
                case 29: // leksema = , |   M -> ,aM
                         //             |        □6□
                    t.leks_number = -1; t.name = 'M';   g_stack.push(t);    // M
                    t.leks_number = 2;                  g_stack.push(t);    // a
                    t.leks_number = 29;                 g_stack.push(t);    // ,

                    gen_opc_stack.push(0);    // □
                    gen_opc_stack.push(24);   // 6
                    gen_opc_stack.push(0);    // □
                    break;

                default: /*  λ  */
                    break;
                }
                break;

                //------------------------------------------------------------------//

            case 'N':
                switch (input_leks.get_l_type())
                {
                case 2: // leksema = a  |   N -> a[S]O
                        //              |        7□□m□
                    t.leks_number = -1; t.name = 'O';   g_stack.push(t);    // O
                    t.leks_number = 17;                 g_stack.push(t);    // ]
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 16;                 g_stack.push(t);    // [
                    t.leks_number = 2;                  g_stack.push(t);    // a

                    gen_opc_stack.push(0);     // □
                    gen_opc_stack.push(15);    // m
                    gen_opc_stack.push(0);     // □
                    gen_opc_stack.push(0);     // □
                    gen_opc_stack.push(25);    // 7

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position());
                    break;

                default: /*  error  */
                    error_flag = true;
                    std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position()
                        << "\nОписание: " << g_st_top.name << '\n';
                    break;
                }
                break;

                //------------------------------------------------------------------//

            case 'O':
                switch (input_leks.get_l_type())
                {
                case 29: // leksema = , |   O -> ,a[S]O
                         //             |        □7□□m□
                    t.leks_number = -1; t.name = 'O';   g_stack.push(t);    // O
                    t.leks_number = 17;                 g_stack.push(t);    // ]
                    t.leks_number = -1; t.name = 'S';   g_stack.push(t);    // S
                    t.leks_number = 16;                 g_stack.push(t);    // [
                    t.leks_number = 2;                  g_stack.push(t);    // a
                    t.leks_number = 29;                 g_stack.push(t);    // ,

                    gen_opc_stack.push(0);     // □
                    gen_opc_stack.push(15);    // m
                    gen_opc_stack.push(0);     // □
                    gen_opc_stack.push(0);     // □
                    gen_opc_stack.push(25);    // 7
                    gen_opc_stack.push(0);     // □

                    str_number_stack.push(input_leks.get_start_str_number());
                    str_position_stack.push(input_leks.get_start_str_position() + 1);
                    break;

                default: /*  λ  */
                    break;
                }
                break;
            }
        }
        else {
            // * иначе в голове стека грамматики - терминал
            // * если типы лексемм не совпадают, то ошибка
            if (g_st_top.leks_number != input_leks.get_l_type()) {
                error_flag = true;
                std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position()
                    << "\nОписание: ожидалась лексема \"" << leksema_list[g_st_top.leks_number] << '"' << std::endl;
                //return;
            }
            else {  // если типы совпадают, то проверка на конец программы
                if (input_leks.get_l_type() == 0) { end_of_program_flag = true; }
                // не конец программы, значит продолжаем работу
                ready_to_read_next_leksem = true;
            }
        }   // не конец программы, значит продолжаем работу
        //
        //-------------------Генератор ОПС-------------------//
        //
        switch (gen_opc_top)
        {
        case 0:
            break;
        case 1:  // +
        case 2:  // -
        case 3:  // *
        case 4:  // /
        case 5:  // =       * * * * * * * * * * * * * * * * * * * * * * * * * 
        case 6:  // ==      *   Эти семантические программы генератора ОПС  *
        case 7:  // >=      *   ведут себя одинакого.                       *
        case 8:  // <=      * * * * * * * * * * * * * * * * * * * * * * * * *
        case 9:  // >
        case 10: // <
        case 11: // !=
        case 12: // ~
        case 13: // r
        case 14: // w
        case 15: // m
        case 16: // i
            //            OPC_element(type, value, int *var, Mas_passport*,   str_number    str_pos 
            OPC.push_back(OPC_element(0, gen_opc_top, NULL, NULL, str_number_stack.top(), str_position_stack.top()));
            ++k;    // счётчик смысловых элементов в ОПС
            str_number_stack.pop();
            str_position_stack.pop();
            break;

        case 17: //a - проверка наличия имени в таблицах и запись в ОПС ссылки на переменную / паспорт массива
            not_found_flag = true;
            for (int i = 0; i < Var_table.size() && not_found_flag; ++i) {
                if (input_leks.get_name() == Var_table[i].name) {
                    // нашли имя переменной в таблице -> записываем в ОПС
                    not_found_flag = false;
                    OPC.push_back(OPC_element(2, NULL, &Var_table[i].a, NULL, NULL, NULL)); // В ОПС записывается 
                    ++k;    // счётчик смысловых элементов в ОПС
                }
            }
            for (int i = 0; i < Passport_table.size() && not_found_flag; ++i) {
                if (input_leks.get_name() == Passport_table[i].name) {
                    // нашли паспорт массива в таблице -> записываем в ОПС
                    not_found_flag = false;
                    OPC.push_back(OPC_element(3, NULL, NULL, &Passport_table[i].m, NULL, NULL)); // В ОПС записывается 
                    ++k;    // счётчик смысловых элементов в ОПС
                }
            }
            // ничего не нашли, значит такой переменной\паспорта массива не было объявлено
            if (not_found_flag) {
                error_flag = true;
                std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position()
                    << "\nОписание: Использование необъявленной переменной/массива \"" << input_leks.get_name() << '"' << std::endl;
            }
            break;

        case 18: // запись в ОПС константы
            OPC.push_back(OPC_element(1, input_leks.get_value(), NULL, NULL, NULL, NULL));
            ++k;
            break;

        case 19:    /*  Семантическая программа 1:
                        * в магазин  меток записывается k
                        * в ОПС записывается пустой элемент - место для будущей местки
                        * в ОПС записывается операция jf - переход при условии false            
                    */
            marker_stack.push(k); // 1. В магазин меток записывается k.
            OPC.push_back(OPC_element(1, NULL, NULL, NULL, NULL, NULL)); // В ОПС записывается пустой элемент – место для будущей метки.
            ++k;
            OPC.push_back(OPC_element(0, 17, NULL, NULL, NULL, NULL)); // В ОПС записывается операция jf – переход при условии false.
            ++k;
            break;

        case 20:    /*  Семантическая программа 2:
                        * ссылка на верхний элемент массива меток (получение оттуда k) и запись в ОПС k+2
                        * в магазин меток записывается k
                        * в ОПС записывается пустой элемент - место для будущей метки
                        * в ОПС записывается операция j - безусловный переход
                    */
            OPC[marker_stack.top()].value = k + 2;
            marker_stack.pop();
            marker_stack.push(k); // В магазин меток записывается k.
            OPC.push_back(OPC_element(1, NULL, NULL, NULL, NULL, NULL)); // В ОПС записывается пустой элемент – место для будущей метки.
            ++k;
            OPC.push_back(OPC_element(0, 18, NULL, NULL, NULL, NULL)); // В ОПС записывается операция j - безусловный переход.
            ++k;
            break;

        case 21:    /*  Семантическая программа 3:
                        * ссылка на верхний элемент массива меток (получение оттуда k) и запись в ОПС k             
                    */
            OPC[marker_stack.top()].value = k;
            marker_stack.pop();
            break;

        case 22:    /*  Семантическая программа 4:
                        * в магазин меток записывается k
                    */
            marker_stack.push(k); // В магазин меток записывается k.
            break;

        case 23:    /*  Семантическая программа 5:
                        * ссылка на верхний элемент массива меток (получение оттуда k), запись в ОПС k+2
                        * в ОПС записывается метка, значение для которой читается из магазина меток
                        * в ОПС записывается операция j - безусловный переход
                    */
            OPC[marker_stack.top()].value = k + 2;
            marker_stack.pop();
            OPC.push_back(OPC_element(1, marker_stack.top(), NULL, NULL, NULL, NULL)); // 
            marker_stack.pop();
            ++k;
            OPC.push_back(OPC_element(0, 18, NULL, NULL, NULL, NULL)); // В ОПС записывается операция j - безусловный переход.
            ++k;
            break;

        case 24:    /*  Семантическая программа 6:
                        * Проверка имени переменной, поступающей из входной цепочки, на совпадение
                                с именами, ранее занесёнными в таблицы
                                (если есть совпадение - ошибка "повторное объявление переменной")
                        * Если ошибки нет, то добавление имени переменной в таблицу переменных                            

                    */

            // проверка в таблице переменных
            for (int i = 0; i < Var_table.size() && !error_flag; ++i) {
                if (input_leks.get_name() == Var_table[i].name) {
                    error_flag = true;
                    std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position()
                        << "\nОписание: Повторное описание переменной \"" << input_leks.get_name() << '"' << std::endl;
                }
            }
            // проверка в таблице паспортов массивов
            for (int i = 0; i < Passport_table.size() && !error_flag; ++i) {
                if (input_leks.get_name() == Passport_table[i].name) {
                    error_flag = true;
                    std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position()
                        << "\nОписание: Повторное описание переменной \"" << input_leks.get_name() << '"' << std::endl;
                }
            }
            if (!error_flag) { // совпадений не найдено - заносим в таблицу переменных
                Var_table.push_back(Var_table_element(input_leks.get_name(), NULL));
            }
            break;

        case 25:    /*  Самантическая программа 7:
                        * Проверка имени массива, поступающего из входной цепочки, на совпадение
                            с именами, ранее занесёнными в таблицы
                            (если есть совпадение - ошибка "повторное объявление массива"
                        * Если ошибки нет, то добавление имени массива в таблицу паспортов массивов
                        * запись в ОПС ссылки на паспорт массива
                    */
            // проверка в таблице переменных
            for (int i = 0; i < Var_table.size() && !error_flag; ++i) {
                if (input_leks.get_name() == Var_table[i].name) {
                    error_flag = true;
                    std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position()
                        << "\nОписание: Повторное описание массива \"" << input_leks.get_name() << '"' << std::endl;
                }
            }
            // проверка в таблице паспортов массивов
            for (int i = 0; i < Passport_table.size() && !error_flag; ++i) {
                if (input_leks.get_name() == Passport_table[i].name) {
                    error_flag = true;
                    std::cout << "Строка: " << input_leks.get_start_str_number() << " Позиция: " << input_leks.get_start_str_position()
                        << "\nОписание: Повторное описание массива \"" << input_leks.get_name() << '"' << std::endl;
                }
            }
            if (!error_flag) {  // совпадений не найдено - заносим в таблицу паспортов массивов
                Passport_table.push_back(Passport_table_element(input_leks.get_name(), NULL));
                OPC.push_back(OPC_element(3, NULL, NULL, &Passport_table[Passport_table.size() - 1].m, NULL, NULL)); // В ОПС записывается 
                ++k;
            }
            break;

        case 26:    /*  Семантическая программа 9:
                            * Генерация в ОПС операций освобождение всех выделенных в процессе выполнения программы бокоов памяти
                    */
            // Генерация в ОПС операции освобождения памяти
            OPC.push_back(OPC_element(0, 19, NULL, NULL, NULL, NULL));
            break;

        default:
            std::cout << "отладочная ошибка";
            break;
        }
    }
    return OPC;
};

int main()
{
    SetConsoleCP(1251); // устанавливаем кодировку для ввода\вывода на консоль
    SetConsoleOutputCP(1251);
    std::ifstream program_file("prog.txt"); // программа пишется в файле
    if (!program_file.is_open()) { return 0; }
    bool error_flag = false;
    Var_table.reserve(SHRT_MAX);
    Passport_table.reserve(SHRT_MAX);
    std::vector<OPC_element> OPC;
    OPC = imply_Grammar(program_file, error_flag);
    
    if (!error_flag) {
        for (int i = 0; i < OPC.size(); ++i) {
            switch (OPC[i].element_type)
            {
            case 0:
                //std::cout << "Операция:\t\t" << OPC[i].value << std::endl;
                std::cout << "Операция:\t\t" << OPC_operation_list[OPC[i].value - 1] << std::endl;
                break;
            case 1:
                std::cout << "Константа / метка:\t" << OPC[i].value << std::endl;
                break;
            case 2:
                std::cout << "Переменная:\t\t" << *(std::string*)((long long)OPC[i].a - sizeof(std::string)) << std::endl;
                ////std::cout << "Переменная:\t\t" << OPC[i].a << std::endl;
                ///----------std::cout << "Переменная:\t\t" << (int)&Var_table[0].name << "\t" << (int)OPC[i].a << "\t" << sizeof(std::string) << std::endl;
                //std::cout << "Переменная:\t\t" << &Var_table[0].name << "\t" << (std::string*)((long long)OPC[i].a - sizeof(std::string)) << std::endl;
                break;
            case 3:
                std::cout << "Массив:\t\t\t" << *(std::string*)((long long)OPC[i].m - sizeof(std::string)) << std::endl;
                //std::cout << "Массив:\t\t\t" << OPC[i].m << '\t'<<std::endl;
                break;
            default:
                std::cout << "ОШИБКА" << std::endl;
                break;
            }
        }
    }

    program_file.close();
    return 0;
}
