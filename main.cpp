#include <iostream>
#include <string>
#include <limits>
#include <locale>
#include <cstring> // для std::strlen

#include "mutable_array_sequence.h"
#include "immutable_array_sequence.h"
#include "mutable_list_sequence.h"
#include "immutable_list_sequence.h"
#include "bit_sequence.h"
#include "option.h"

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#endif
#include <cstdio>  // для кириллицы

using namespace std;

// Вспомогательные функции для демонстрации
void demo_mutable_vs_immutable() {
    cout << "\n=== Демонстрация Mutable vs Immutable ===\n";
    int data[] = {10, 20, 30};
    lab2::MutableArraySequence<int> mut_seq(data, 3);
    lab2::ImmutableArraySequence<int> imm_seq(data, 3);

    cout << "Исходные:  Mut: " << mut_seq << " | Imm: " << imm_seq << "\n";

    // Mutable меняет себя
    mut_seq.Append(40);
    cout << "После mut_seq.Append(40):  Mut: " << mut_seq << " | Imm: " << imm_seq << "\n";

    // Immutable создаёт новый объект, старый не трогает
    lab2::Sequence<int>* imm_new = imm_seq.Append(40);
    cout << "После imm_seq.Append(40):  Mut: " << mut_seq << " | Imm: " << imm_seq << " | New: " << *imm_new << "\n";

    delete imm_new; // Освобождаем память
}

void demo_map_reduce() {
    cout << "\n=== Демонстрация Map / Where / Reduce ===\n";
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto seq = lab2::MutableListSequence<int>(data, 10);
    cout << "Исходная: " << seq << "\n";

    // Where: оставляем только чётные
    auto evens = seq.Where([](int x){ return x % 2 == 0; });
    cout << "Чётные (Where): " << *evens << "\n";

    // Map: возводим в квадрат
    auto squared = evens->Map([](int x){ return x * x; });
    cout << "Квадраты чётных (Map): " << *squared << "\n";

    // Reduce: сумма всех квадратов
    int sum = squared->Reduce([](int acc, int x){ return acc + x; }, 0);
    cout << "Сумма квадратов (Reduce): " << sum << "\n";

    delete evens;
    delete squared;
}

void demo_bit_sequence() {
    cout << "\n=== Демонстрация BitSequence ===\n";
    bool a[] = {1, 0, 1, 1, 0};
    bool b[] = {1, 1, 0, 0, 1};
    lab2::BitSequence bs_a(a, 5);
    lab2::BitSequence bs_b(b, 5);

    cout << "A: " << bs_a << "\n";
    cout << "B: " << bs_b << "\n";

    auto and_res = bs_a.And(&bs_b);
    auto or_res  = bs_a.Or(&bs_b);
    auto xor_res = bs_a.Xor(&bs_b);
    auto not_res = bs_a.Not();

    cout << "A AND B: " << *and_res << "\n";
    cout << "A OR  B: " << *or_res  << "\n";
    cout << "A XOR B: " << *xor_res << "\n";
    cout << "NOT A:   " << *not_res << "\n";

    delete and_res; delete or_res; delete xor_res; delete not_res;
}

void demo_exceptions_and_try() {
    cout << "\n=== Демонстрация Исключений и Try-семантики ===\n";
    lab2::MutableArraySequence<int> seq;
    seq.Append(100);

    // 1. Корректный доступ
    try {
        cout << "seq[0] = " << seq[0] << "\n";
        seq.InsertAt(-1, 5); // Должно выбросить исключение
    } catch (const lab2::Exception& e) {
        cout << " Перехвачено ожидаемое исключение: " << e.what() << "\n";
    }

    // 2. Try-семантика (Option)
    auto opt = seq.TryGet(5);
    if (opt) {
        cout << "TryGet(5) нашёл: " << opt.GetValue() << "\n";
    } else {
        cout << " TryGet(5) вернул None (элемента нет)\n";
    }
}

void show_menu() {
    cout << "\n==========================================================\n";
    cout << "        Лабораторная работа №2: Полиморфные АТД (C++)       \n";
    cout << "                   Мусин Иван Б25-507       \n";
    cout << "----------------------------------------------------------\n";
    cout << " 1. Mutable vs Immutable последовательности                 \n";
    cout << " 2. Map / Where / Reduce операции                           \n";
    cout << " 3. BitSequence (побитовые операции)                        \n";
    cout << " 4. Исключения и Try-семантика (Option<T>)                  \n";
    cout << " 5. Создать последовательность и выполнить операции         \n";
    cout << " 6. Информация о запуске модульных тестов                   \n";
    cout << " 0. Выход                                                   \n";
    cout << "----------------------------------------------------------\n";
    cout << "Выберите пункт меню: \n";
    cout << "==========================================================\n";
}

void clear_input() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void input_sequence_values(lab2::Sequence<int>*& sequence)
{
    int count;
    cout << "Введите количество элементов: ";
    while (!(cin >> count) || count <= 0)
    {
        clear_input();
        cout << "Введите положительное число: ";
    }
    clear_input();
    cout << "Введите " << count
         << " целых чисел через пробел:\n";
    for (int i = 0; i < count; ++i)
    {
        int value;
        while (!(cin >> value))
        {
            clear_input();
            cout << "Введите целое число для позиции "
                 << i << ": ";
        }
        lab2::Sequence<int>* updatedSequence =
            sequence->Append(value);
        // Immutable-последовательность возвращает новый объект.
        if (updatedSequence != sequence)
        {
            delete sequence;
            sequence = updatedSequence;
        }
    }
    clear_input();
}

//создание последовательности с выбором типа
lab2::Sequence<int>* create_sequence_interactive()
{
    cout << "\n=== Создание последовательности ===\n";
    cout << "Выберите тип:\n";
    cout << "1. MutableArraySequence\n";
    cout << "2. ImmutableArraySequence\n";
    cout << "3. MutableListSequence\n";
    cout << "4. ImmutableListSequence\n";
    cout << "> ";

    int type;
    while (!(cin >> type) || type < 1 || type > 4)
    {
        clear_input();
        cout << "Введите число от 1 до 4: ";
    }
    clear_input();

    lab2::Sequence<int>* seq = nullptr;
    switch (type)
    {
        case 1:
            seq = new lab2::MutableArraySequence<int>();
            cout << "Создана MutableArraySequence\n";
            break;

        case 2:
            seq = new lab2::ImmutableArraySequence<int>();
            cout << "Создана ImmutableArraySequence\n";
            break;

        case 3:
            seq = new lab2::MutableListSequence<int>();
            cout << "Создана MutableListSequence\n";
            break;

        case 4:
            seq = new lab2::ImmutableListSequence<int>();
            cout << "Создана ImmutableListSequence\n";
            break;
    }
    input_sequence_values(seq);
    return seq;
}

void sequence_operations_menu(lab2::Sequence<int>*& seq) {
    if (!seq) {
        cout << "Последовательность не инициализирована!\n";
        return;
    }

    int choice;
    do {
        cout << "\n=== Операции с последовательностью ===\n";
        cout << "Текущая: " << *seq << " (длина: " << seq->GetLength() << ")\n";
        cout << "1. Получить элемент по индексу (Get)\n";
        cout << "2. Добавить элемент в конец (Append)\n";
        cout << "3. Применить Map (x*2)\n";
        cout << "4. Применить Where (x > 5)\n";
        cout << "5. Применить Reduce (сумма)\n";
        cout << "6. TryGet (без исключения)\n";
        cout << "0. Назад в главное меню\n";
        cout << "> ";

        if (!(cin >> choice)) {
            clear_input();
            cout << "Введите число от 0 до 6.\n";
            continue;
        }
        clear_input();

        try {
            switch (choice) {
                case 1:
                {
                    int idx;
                    cout << "Введите индекс (0.." << seq->GetLength() - 1 << "): ";
                    if (!(cin >> idx))
                    {
                        clear_input();
                        cout << "Ошибка: необходимо ввести целое число.\n";
                        break;
                    }
                    clear_input();
                    cout << "seq[" << idx << "] = " << seq->Get(idx) << "\n";
                    break;
                }
                case 2:
                {
                    int val;
                    cout << "Введите значение для добавления: ";
                    if (!(cin >> val))
                    {
                        clear_input();
                        cout << "Ошибка: необходимо ввести целое число.\n";
                        break;
                    }
                    clear_input();
                    lab2::Sequence<int>* newSeq = seq->Append(val);
                    // Для immutable-последовательности создаётся новый объект.
                    if (newSeq != seq)
                    {
                        delete seq;
                        seq = newSeq;

                        cout << "Создана новая последовательность (immutable)\n";
                        cout << "Результат: " << *seq << "\n";
                    }
                    else
                    {
                        cout << "Элемент добавлен (mutable)\n";
                        cout << "Результат: " << *seq << "\n";
                    }

                    break;
                }
                case 3:
                {
                    auto* result = seq->Map([](int x) { return x * 2; });
                    cout << "Map(x*2): " << *result << "\n";
                    delete result;
                    break;
                }
                case 4:
                {
                    auto* result = seq->Where([](int x) { return x > 5; });
                    cout << "Where(x>5): " << *result << "\n";
                    delete result;
                    break;
                }
                case 5:
                {
                    int sum = seq->Reduce([](int acc, int x) { return acc + x; }, 0);
                    cout << "Reduce(сумма) = " << sum << "\n";
                    break;
                }
                case 6:
                {
                    int idx;
                    cout << "Введите индекс для TryGet: ";
                    if (!(cin >> idx))
                    {
                        clear_input();
                        cout << "Ошибка: необходимо ввести целое число.\n";
                        break;
                    }
                    clear_input();
                    auto opt = seq->TryGet(idx);
                    if (opt) {
                        cout << "TryGet(" << idx << ") = " << opt.GetValue() << "\n";
                    } else {
                        cout << "TryGet(" << idx << ") вернул None\n";
                    }
                    break;
                }
                case 0:
                    cout << "Возврат в главное меню.\n";
                    break;
                default:
                    cout << "Неверный выбор.\n";
            }
        } catch (const lab2::Exception& e) {
            cout << "Ошибка: " << e.what() << "\n";
        }

        if (choice != 0) {
            cout << "\nНажмите Enter для продолжения...";
            cin.get();
        }
    } while (choice != 0);
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    _setmode(_fileno(stdout), _O_TEXT);
    _setmode(_fileno(stdin), _O_TEXT);
#endif
    std::locale::global(std::locale(""));
    std::cout.imbue(std::locale(""));
    std::cin.imbue(std::locale(""));

    lab2::Sequence<int>* current_seq = nullptr;  // Храним текущую последовательность

    int choice = -1;
    do
    {
        show_menu();
        if (!(cin >> choice))
        {
            clear_input();
            cout << "Ошибка ввода. Введите число от 0 до 6.\n";
            continue;
        }
        clear_input();

        cout << "\n";
        switch (choice)
        {
            case 1:
                demo_mutable_vs_immutable();
                break;
            case 2:
                demo_map_reduce();
                break;
            case 3:
                demo_bit_sequence();
                break;
            case 4:
                demo_exceptions_and_try();
                break;
            case 5:
                // Создание и работа с последовательностью
                if (current_seq)
                {
                }
                current_seq = create_sequence_interactive();
                if (current_seq)
                {
                    sequence_operations_menu(current_seq);
                    // После выхода из меню операций удаляем последовательность
                    delete current_seq;
                    current_seq = nullptr;
                }
                break;
            case 6:
                cout << "Тесты запускаются отдельно. В терминале сборки выполните:\n";
                cout << "   ctest --output-on-failure\n";
                break;
            case 0:
                if (current_seq) delete current_seq;
                cout << "Работа завершена\n";
                break;
            default:
                cout << "Неверный пункт меню.\n";
        }

        if (choice != 0)
        {
            cout << "\nНажмите Enter для возврата в меню...";
            cin.get();
        }
    } while (choice != 0);

    return 0;
}