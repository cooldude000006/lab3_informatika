#include <iostream>
#include <limits>
#include <cstddef>
#include <iomanip>

#include "ivector.h"
#include "array_vector.h"
#include "list_vector.h"
#include "exceptions.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

// Очистка потока после неправильного или завершённого ввода.
void clear_input()
{
    cin.clear();
    cin.ignore(
        numeric_limits<streamsize>::max(),
        '\n'
    );
}

// ожидание enter для возращения
void wait_for_enter()
{
    cout << "\nНажмите Enter, чтобы вернуться в главное меню...";
    cin.get();
}

// Чтение целого числа из заданного диапазона.
int read_int_in_range(
    const string& message,
    int minimum,
    int maximum
)
{
    int value;

    while (true)
    {
        cout << message;

        if (
            cin >> value &&
            value >= minimum &&
            value <= maximum
        )
        {
            clear_input();
            return value;
        }

        clear_input();

        cout << "Ошибка: введите целое число от "
             << minimum << " до " << maximum
             << ".\n";
    }
}

// Чтение неотрицательного целого числа.
int read_nonnegative_int(const string& message)
{
    int value;

    while (true)
    {
        cout << message;

        if (cin >> value && value >= 0)
        {
            clear_input();
            return value;
        }

        clear_input();

        cout << "Ошибка: введите неотрицательное "
             << "целое число.\n";
    }
}

// Чтение вещественного числа.
double read_double(const string& message)
{
    double value;

    while (true)
    {
        cout << message;

        if (cin >> value)
        {
            clear_input();
            return value;
        }

        clear_input();

        cout << "Ошибка: необходимо ввести число.\n";
    }
}

// Вывод математического вектора.
void print_vector(const lab2::IVector<double>& vector)
{
    cout << "(";

    for (
        size_t i = 0;
        i < vector.GetDimension();
        ++i
    )
    {
        cout << vector.Get(i);

        if (i + 1 < vector.GetDimension())
        {
            cout << ", ";
        }
    }

    cout << ")";
}

// Создание вектора с ручным вводом координат.
lab2::IVector<double>* create_vector_interactive()
{
    cout << "\n=== Создание математического вектора ===\n";
    cout << "1. ArrayVector\n";
    cout << "2. ListVector\n";

    int type = read_int_in_range(
        "Выберите реализацию: ",
        1,
        2
    );

    int dimension = read_nonnegative_int(
        "Введите размерность вектора: "
    );

    double* coordinates = nullptr;

    if (dimension > 0)
    {
        coordinates = new double[dimension];
    }

    for (int i = 0; i < dimension; ++i)
    {
        coordinates[i] = read_double(
            "Введите координату [" +
            to_string(i) +
            "]: "
        );
    }

    lab2::IVector<double>* result = nullptr;

    try
    {
        if (type == 1)
        {
            result =
                new lab2::ArrayVector<double>(
                    coordinates,
                    dimension
                );
        }
        else
        {
            result =
                new lab2::ListVector<double>(
                    coordinates,
                    dimension
                );
        }
    }
    catch (...)
    {
        delete[] coordinates;
        throw;
    }

    delete[] coordinates;

    return result;
}

// Проверка наличия текущего вектора.
bool check_current_vector(
    const lab2::IVector<double>* vector
)
{
    if (vector == nullptr)
    {
        cout << "Сначала создайте математический "
             << "вектор.\n";

        return false;
    }

    return true;
}

// Сложение текущего вектора с новым.
void perform_addition(
    const lab2::IVector<double>& current
)
{
    cout << "\nСоздайте второй вектор.\n";

    lab2::IVector<double>* other =
        create_vector_interactive();

    lab2::IVector<double>* result = nullptr;

    try
    {
        result = current.Add(*other);

        cout << "Первый вектор: ";
        print_vector(current);

        cout << "\nВторой вектор: ";
        print_vector(*other);

        cout << "\nРезультат сложения: ";
        print_vector(*result);

        cout << "\n";
    }
    catch (...)
    {
        delete result;
        delete other;
        throw;
    }

    delete result;
    delete other;
}

// Умножение на скаляр.
void perform_scalar_multiplication(
    const lab2::IVector<double>& current
)
{
    double scalar = read_double(
        "Введите значение скаляра: "
    );

    lab2::IVector<double>* result =
        current.MultiplyByScalar(scalar);

    try
    {
        cout << "Исходный вектор: ";
        print_vector(current);

        cout << "\nРезультат: ";
        print_vector(*result);

        cout << "\n";
    }
    catch (...)
    {
        delete result;
        throw;
    }

    delete result;
}

// Скалярное произведение.
void perform_dot_product(
    const lab2::IVector<double>& current
)
{
    cout << "\nСоздайте второй вектор.\n";

    lab2::IVector<double>* other =
        create_vector_interactive();

    try
    {
        double result =
            current.DotProduct(*other);

        cout << "Первый вектор: ";
        print_vector(current);

        cout << "\nВторой вектор: ";
        print_vector(*other);

        cout << "\nСкалярное произведение: "
             << result << "\n";
    }
    catch (...)
    {
        delete other;
        throw;
    }

    delete other;
}

// Map: умножение каждой координаты на 2.
void perform_map(
    const lab2::IVector<double>& current
)
{
    lab2::IVector<double>* result =
        current.Map(
            [](double value)
            {
                return value * 2.0;
            }
        );

    try
    {
        cout << "Исходный вектор: ";
        print_vector(current);

        cout << "\nMap(x * 2): ";
        print_vector(*result);

        cout << "\n";
    }
    catch (...)
    {
        delete result;
        throw;
    }

    delete result;
}

// Where: оставить только положительные координаты.
void perform_where(
    const lab2::IVector<double>& current
)
{
    lab2::IVector<double>* result =
        current.Where(
            [](double value)
            {
                return value > 0.0;
            }
        );

    try
    {
        cout << "Исходный вектор: ";
        print_vector(current);

        cout << "\nWhere(x > 0): ";
        print_vector(*result);

        cout << "\n";
    }
    catch (...)
    {
        delete result;
        throw;
    }

    delete result;
}

// Reduce: сумма координат.
void perform_reduce(
    const lab2::IVector<double>& current
)
{
    double sum = current.Reduce(
        [](double accumulator, double value)
        {
            return accumulator + value;
        },
        0.0
    );

    cout << "Сумма координат: "
         << sum << "\n";
}

// Демонстрация итератора.
void perform_iteration(
    const lab2::IVector<double>& current
)
{
    lab2::IEnumerator<double>* enumerator =
        current.GetEnumerator();

    try
    {
        cout << "Обход через IEnumerator: ";

        bool first = true;

        while (enumerator->MoveNext())
        {
            lab2::Option<double> value =
                enumerator->GetCurrent();

            if (value.HasValue())
            {
                if (!first)
                {
                    cout << ", ";
                }

                cout << value.GetValue();
                first = false;
            }
        }

        cout << "\n";
    }
    catch (...)
    {
        delete enumerator;
        throw;
    }

    delete enumerator;
}

void show_menu(
    const lab2::IVector<double>* current
)
{
    cout << "\n"
         << "========================================\n"
         << " Лабораторная работа №3: Вектор\n"
         << "       Мусин Иван Б25-507      \n"
         << "========================================\n";

    if (current == nullptr)
    {
        cout << "Текущий вектор: не создан\n";
    }
    else
    {
        cout << "Текущий вектор: ";
        print_vector(*current);

        cout << "\nРазмерность: "
             << current->GetDimension()
             << "\n";
    }

    cout << "----------------------------------------\n"
         << "1. Создать или заменить вектор\n"
         << "2. Показать текущий вектор\n"
         << "3. Сложить с другим вектором\n"
         << "4. Умножить на скаляр\n"
         << "5. Вычислить норму\n"
         << "6. Вычислить скалярное произведение\n"
         << "7. Выполнить Map (x * 2)\n"
         << "8. Выполнить Where (x > 0)\n"
         << "9. Выполнить Reduce (сумма)\n"
         << "10. Выполнить обход итератором\n"
         << "0. Выход\n"
         << "----------------------------------------\n";
}

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    cout << fixed << setprecision(4);

    lab2::IVector<double>* current = nullptr;

    bool running = true;

    while (running)
    {
        show_menu(current);

        int choice = read_int_in_range(
            "Выберите пункт меню: ",
            0,
            10
        );

        try
        {
            switch (choice)
            {
                case 0:
                {
                    running = false;
                    break;
                }

                case 1:
                {
                    lab2::IVector<double>* newVector =
                        create_vector_interactive();

                    delete current;
                    current = newVector;

                    cout << "Вектор успешно создан: ";
                    print_vector(*current);
                    cout << "\n";

                    break;
                }

                case 2:
                {
                    if (!check_current_vector(current))
                    {
                        break;
                    }

                    cout << "Текущий вектор: ";
                    print_vector(*current);
                    cout << "\n";

                    break;
                }

                case 3:
                {
                    if (!check_current_vector(current))
                    {
                        break;
                    }

                    perform_addition(*current);
                    break;
                }

                case 4:
                {
                    if (!check_current_vector(current))
                    {
                        break;
                    }

                    perform_scalar_multiplication(*current);
                    break;
                }

                case 5:
                {
                    if (!check_current_vector(current))
                    {
                        break;
                    }

                    cout << "Норма вектора: "
                         << current->Norm()
                         << "\n";

                    break;
                }

                case 6:
                {
                    if (!check_current_vector(current))
                    {
                        break;
                    }

                    perform_dot_product(*current);
                    break;
                }

                case 7:
                {
                    if (!check_current_vector(current))
                    {
                        break;
                    }

                    perform_map(*current);
                    break;
                }

                case 8:
                {
                    if (!check_current_vector(current))
                    {
                        break;
                    }

                    perform_where(*current);
                    break;
                }

                case 9:
                {
                    if (!check_current_vector(current))
                    {
                        break;
                    }

                    perform_reduce(*current);
                    break;
                }

                case 10:
                {
                    if (!check_current_vector(current))
                    {
                        break;
                    }

                    perform_iteration(*current);
                    break;
                }

                default:
                    break;
            }
        }
        catch (const lab2::Exception& error)
        {
            cout << "Ошибка: "
                 << error.what()
                 << "\n";
        }
        catch (const exception& error)
        {
            cout << "Стандартная ошибка: "
                 << error.what()
                 << "\n";
        }
        catch (...)
        {
            cout << "Произошла неизвестная ошибка.\n";
        }
        if (running)
        {
            wait_for_enter();
        }
    }

    delete current;

    cout << "Программа завершена.\n";

    return 0;
}