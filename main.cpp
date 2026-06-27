#include <iostream>
#include <limits>
#include <cstddef>
#include <iomanip>
#include <chrono>
#include <string>

#include "ivector.h"
#include "array_vector.h"
#include "list_vector.h"
#include "exceptions.h"

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

int read_int_in_range(
    const string& message,
    int minimum,
    int maximum
);

//оценка производительности
struct VectorPerformanceResult
{
    double accessMicroseconds;
    double scalarMultiplicationMicroseconds;
    double dotProductMicroseconds;
};

template<class Operation>
double measure_average_microseconds(
    Operation operation,
    int repetitions
)
{
    volatile double controlValue = 0.0;

    auto start =
        chrono::steady_clock::now();

    for (int i = 0; i < repetitions; ++i)
    {
        controlValue =
            controlValue + operation();
    }

    auto end =
        chrono::steady_clock::now();

    auto totalNanoseconds =
        chrono::duration_cast<chrono::nanoseconds>(
            end - start
        ).count();

    // Не позволяет компилятору полностью удалить
    // измеряемые вычисления при оптимизации.
    (void)controlValue;

    return static_cast<double>(totalNanoseconds)
        / 1000.0
        / repetitions;
}

VectorPerformanceResult measure_vector_performance(
    const lab3::IVector<double>& vector,
    int repetitions
)
{
    VectorPerformanceResult result{};

    result.accessMicroseconds =
        measure_average_microseconds(
            [&vector]()
            {
                double sum = 0.0;

                for (
                    size_t i = 0;
                    i < vector.GetDimension();
                    ++i
                )
                {
                    sum += vector.Get(i);
                }

                return sum;
            },
            repetitions
        );

    result.scalarMultiplicationMicroseconds =
        measure_average_microseconds(
            [&vector]()
            {
                lab3::IVector<double>* multiplied =
                    vector.MultiplyByScalar(1.5);

                double control = 0.0;

                if (multiplied->GetDimension() > 0)
                {
                    control += multiplied->Get(0);

                    control += multiplied->Get(
                        multiplied->GetDimension() - 1
                    );
                }

                delete multiplied;

                return control;
            },
            repetitions
        );

    result.dotProductMicroseconds =
        measure_average_microseconds(
            [&vector]()
            {
                return vector.DotProduct(vector);
            },
            repetitions
        );

    return result;
}

void print_performance_table(
    const VectorPerformanceResult& arrayResult,
    const VectorPerformanceResult& listResult
)
{
    cout << "\n=== Результаты измерения ===\n";
    cout << "Время указано в микросекундах.\n\n";

    cout << left
         << setw(34) << "Операция"
         << setw(20) << "ArrayVector"
         << setw(20) << "ListVector"
         << "\n";

    cout << string(74, '-') << "\n";

    cout << left
         << setw(34) << "Получение всех координат"
         << setw(20) << arrayResult.accessMicroseconds
         << setw(20) << listResult.accessMicroseconds
         << "\n";

    cout << left
         << setw(34) << "Умножение на скаляр"
         << setw(20)
         << arrayResult.scalarMultiplicationMicroseconds
         << setw(20)
         << listResult.scalarMultiplicationMicroseconds
         << "\n";

    cout << left
         << setw(34) << "Скалярное произведение"
         << setw(20)
         << arrayResult.dotProductMicroseconds
         << setw(20)
         << listResult.dotProductMicroseconds
         << "\n";
}

void perform_performance_comparison()
{
    cout << "\n=== Сравнение производительности ===\n";

    int dimension = read_int_in_range(
        "Введите размерность векторов (1..3000): ",
        1,
        3000
    );

    int repetitions;

    if (dimension <= 200)
    {
        repetitions = 20;
    }
    else if (dimension <= 1000)
    {
        repetitions = 5;
    }
    else
    {
        repetitions = 1;
    }

    double* coordinates =
        new double[dimension];

    for (int i = 0; i < dimension; ++i)
    {
        coordinates[i] =
            static_cast<double>((i % 100) + 1);
    }

    try
    {
        lab3::ArrayVector<double> arrayVector(
            coordinates,
            dimension
        );

        lab3::ListVector<double> listVector(
            coordinates,
            dimension
        );

        // Векторы уже скопировали координаты.
        delete[] coordinates;
        coordinates = nullptr;

        cout << "Размерность: "
             << dimension << "\n";

        cout << "Количество повторений: "
             << repetitions << "\n";

        cout << "Выполняется измерение...\n";

        VectorPerformanceResult arrayResult =
            measure_vector_performance(
                arrayVector,
                repetitions
            );

        VectorPerformanceResult listResult =
            measure_vector_performance(
                listVector,
                repetitions
            );

        print_performance_table(
            arrayResult,
            listResult
        );

        cout << "\nПояснение:\n";
        cout << "ArrayVector обеспечивает прямой доступ "
             << "к координатам.\n";

        cout << "ListVector при Get(index) проходит "
             << "по связанному списку до нужного узла.\n";

        cout << "Поэтому операции с последовательным "
             << "обращением по индексам обычно медленнее "
             << "для ListVector.\n";
    }
    catch (...)
    {
        delete[] coordinates;
        throw;
    }
}

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
template<class T>
void print_vector(const lab3::IVector<T>& vector)
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
lab3::IVector<double>* create_vector_interactive()
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

    lab3::IVector<double>* result = nullptr;

    try
    {
        if (type == 1)
        {
            result =
                new lab3::ArrayVector<double>(
                    coordinates,
                    dimension
                );
        }
        else
        {
            result =
                new lab3::ListVector<double>(
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
    const lab3::IVector<double>* vector
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
    const lab3::IVector<double>& current
)
{
    cout << "\nСоздайте второй вектор.\n";

    lab3::IVector<double>* other =
        create_vector_interactive();

    lab3::IVector<double>* result = nullptr;

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
    const lab3::IVector<double>& current
)
{
    double scalar = read_double(
        "Введите значение скаляра: "
    );

    lab3::IVector<double>* result =
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
    const lab3::IVector<double>& current
)
{
    cout << "\nСоздайте второй вектор.\n";

    lab3::IVector<double>* other =
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
    const lab3::IVector<double>& current
)
{
    lab3::IVector<double>* result =
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
    const lab3::IVector<double>& current
)
{
    lab3::IVector<double>* result =
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
    const lab3::IVector<double>& current
)
{
    double sum = current.Reduce(
    [](double value, double accumulator)
    {
        return value + accumulator;
    },
        0.0
    );

    cout << "Сумма координат: "
         << sum << "\n";
}

// Демонстрация итератора.
void perform_iteration(
    const lab3::IVector<double>& current
)
{
    lab3::IEnumerator<double>* enumerator =
        current.GetEnumerator();

    try
    {
        cout << "Обход через IEnumerator: ";

        bool first = true;

        while (enumerator->MoveNext())
        {
            lab3::Option<double> value =
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
    const lab3::IVector<double>* current
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
         << "11. Сравнить производительность\n"
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

    lab3::IVector<double>* current = nullptr;

    bool running = true;

    while (running)
    {
        show_menu(current);

        int choice = read_int_in_range(
            "Выберите пункт меню: ",
            0,
            11
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
                    lab3::IVector<double>* newVector =
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

                case 11:
                {
                    perform_performance_comparison();
                    break;
                }

                default:
                    break;
            }
        }
        catch (const lab3::Exception& error)
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