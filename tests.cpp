#include <gtest/gtest.h>
#include <iostream>
#include <locale>

#include "dynamic_array.h"
#include "linked_list.h"
#include "mutable_array_sequence.h"
#include "immutable_array_sequence.h"
#include "mutable_list_sequence.h"
#include "immutable_list_sequence.h"
#include "bit_sequence.h"
#include "option.h"
#include "icollection.h"

#ifdef _WIN32
#include <windows.h>
#endif

//работа через интерфейс с массивом
TEST(ICollectionTest, WorksWithArraySequence)
{
    int items[] = {10, 20, 30};

    lab2::MutableArraySequence<int> sequence(items, 3);

    lab2::ICollection<int>* collection = &sequence;

    EXPECT_EQ(collection->GetCount(), 3u);
    EXPECT_EQ(
        collection->Get(static_cast<std::size_t>(0)),
        10
    );
    EXPECT_EQ(
        collection->Get(static_cast<std::size_t>(1)),
        20
    );
    EXPECT_EQ(
        collection->Get(static_cast<std::size_t>(2)),
        30
    );
}

//работа через интерфейс
TEST(ICollectionTest, WorksWithListSequence)
{
    int items[] = {5, 15, 25, 35};

    lab2::ImmutableListSequence<int> sequence(items, 4);

    lab2::ICollection<int>* collection = &sequence;

    EXPECT_EQ(collection->GetCount(), 4u);
    EXPECT_EQ(
        collection->Get(static_cast<std::size_t>(0)),
        5
    );
    EXPECT_EQ(
        collection->Get(static_cast<std::size_t>(3)),
        35
    );
}

//неправильный индекс
TEST(ICollectionTest, InvalidIndexThrows)
{
    int items[] = {1, 2, 3};

    lab2::MutableArraySequence<int> sequence(items, 3);

    lab2::ICollection<int>* collection = &sequence;

    EXPECT_THROW(
        collection->Get(collection->GetCount()),
        lab2::IndexOutOfRangeException
    );
}

//для bitsequence
TEST(BitSequenceTest, BasicConstruction)
{
    bool raw[] = {true,false,true,false};
    lab2::BitSequence bs(raw, 4);

    EXPECT_EQ(bs.GetLength(), 4);
    EXPECT_TRUE(bs.Get(0).value);
    EXPECT_FALSE(bs.Get(1).value);
    EXPECT_TRUE(bs.Get(2).value);
    EXPECT_FALSE(bs.Get(3).value);
}

TEST(BitSequenceTest, BitwiseAnd)
{
    bool a_raw[] = {1, 1, 0, 0};
    bool b_raw[] = {1, 0, 1, 0};
    lab2::BitSequence a(a_raw, 4);
    lab2::BitSequence b(b_raw, 4);

    lab2::BitSequence* res = a.And(&b);

    EXPECT_EQ(res->GetLength(), 4);
    EXPECT_TRUE(res->Get(0).value);
    EXPECT_FALSE(res->Get(1).value);
    EXPECT_FALSE(res->Get(2).value);
    EXPECT_FALSE(res->Get(3).value);
    delete res;
}

TEST(BitSequenceTest, BitwiseOr)
{
    bool a_raw[] = {1, 0, 0, 0};
    bool b_raw[] = {0, 0, 1, 0};
    lab2::BitSequence a(a_raw, 4);
    lab2::BitSequence b(b_raw, 4);

    lab2::BitSequence* res = a.Or(&b);

    EXPECT_TRUE(res->Get(0).value);
    EXPECT_FALSE(res->Get(1).value);
    EXPECT_TRUE(res->Get(2).value);
    EXPECT_FALSE(res->Get(3).value);
    delete res;
}

TEST(BitSequenceTest, BitwiseXor)
{
    bool a_raw[] = {1, 1, 0, 0};
    bool b_raw[] = {1, 0, 1, 0};
    lab2::BitSequence a(a_raw, 4);
    lab2::BitSequence b(b_raw, 4);

    lab2::BitSequence* res = a.Xor(&b);

    EXPECT_FALSE(res->Get(0).value);
    EXPECT_TRUE(res->Get(1).value);
    EXPECT_TRUE(res->Get(2).value);
    EXPECT_FALSE(res->Get(3).value);
    delete res;
}

TEST(BitSequenceTest, BitwiseNot)
{
    bool raw[] = {1, 0, 1, 0};
    lab2::BitSequence bs(raw, 4);

    lab2::BitSequence* res = bs.Not();

    EXPECT_FALSE(res->Get(0).value); // !1 = 0
    EXPECT_TRUE(res->Get(1).value);  // !0 = 1
    EXPECT_FALSE(res->Get(2).value); // !1 = 0
    EXPECT_TRUE(res->Get(3).value);  // !0 = 1
    delete res;
}

TEST(BitSequenceTest, SizeMismatchException)
{
    bool a_raw[] = {1, 0};
    bool b_raw[] = {1, 0, 1};

    lab2::BitSequence a(a_raw, 2);
    lab2::BitSequence b(b_raw, 3);

    EXPECT_THROW(a.And(&b), lab2::SizeMismatchException);
    EXPECT_THROW(a.Or(&b), lab2::SizeMismatchException);
    EXPECT_THROW(a.Xor(&b), lab2::SizeMismatchException);
}

TEST(BitSequenceTest, NullptrException)
{
    bool raw[] = {1, 0};
    lab2::BitSequence a(raw, 2);

    EXPECT_THROW(a.And(nullptr), lab2::InvalidOperationException);
}

TEST(BitSequenceTest, NullptrWithPositiveCountThrows)
{
    EXPECT_THROW(
        {
            lab2::BitSequence sequence(
                static_cast<bool*>(nullptr),
                3
            );
        },
        lab2::InvalidOperationException
    );
}

// тесты для DynamicArray
TEST(DynamicArrayTest, ConstructorFromCArray)
{
    int src[] = {10, 20, 30, 40, 50};
    lab2::DynamicArray<int> arr(src, 5);

    EXPECT_EQ(arr.GetSize(), 5);
    EXPECT_EQ(arr.Get(0), 10);
    EXPECT_EQ(arr.Get(2), 30);
    EXPECT_EQ(arr.Get(4), 50);
}

TEST(DynamicArrayTest, ConstructorWithSize)
{
    lab2::DynamicArray<int> arr(4);

    EXPECT_EQ(arr.GetSize(), 4);
    // Элементы инициализированы значениями по умолчанию (0 для int)
    EXPECT_EQ(arr.Get(0), 0);
    EXPECT_EQ(arr.Get(3), 0);
}

TEST(DynamicArrayTest, CopyConstructor)
{
    int src[] = {1, 2, 3};
    lab2::DynamicArray<int> original(src, 3);

    lab2::DynamicArray<int> copy(original);

    // Меняем оригинал
    original.Set(0, 999);

    // Копия не должна измениться
    EXPECT_EQ(copy.Get(0), 1);  // Было 1, а не 999
    EXPECT_EQ(original.Get(0), 999);
}

TEST(DynamicArrayTest, AssignmentOperator)
{
    int src1[] = {1, 2};
    int src2[] = {10, 20, 30};

    lab2::DynamicArray<int> arr1(src1, 2);
    lab2::DynamicArray<int> arr2(src2, 3);

    arr1 = arr2;  // Присваивание

    EXPECT_EQ(arr1.GetSize(), 3);
    EXPECT_EQ(arr1.Get(0), 10);
    EXPECT_EQ(arr1.Get(2), 30);

    // arr2 не должен измениться
    EXPECT_EQ(arr2.GetSize(), 3);
}

TEST(DynamicArrayTest, IndexOutOfRangeException)
{
    lab2::DynamicArray<int> arr(3);

    // Отрицательный индекс
    EXPECT_THROW(arr.Get(-1), lab2::IndexOutOfRangeException);
    EXPECT_THROW(arr.Set(-1, 5), lab2::IndexOutOfRangeException);

    // Индекс >= size
    EXPECT_THROW(arr.Get(3), lab2::IndexOutOfRangeException);
    EXPECT_THROW(arr.Set(5, 10), lab2::IndexOutOfRangeException);
}

TEST(DynamicArrayTest, Resize)
{
    int src[] = {10, 20, 30};
    lab2::DynamicArray<int> arr(src, 3);

    // Увеличение размера
    arr.Resize(5);
    EXPECT_EQ(arr.GetSize(), 5);
    EXPECT_EQ(arr.Get(0), 10);  // Старые данные сохранены
    EXPECT_EQ(arr.Get(2), 30);
    // Новые элементы инициализированы 0
    EXPECT_EQ(arr.Get(3), 0);

    // Уменьшение размера
    arr.Resize(2);
    EXPECT_EQ(arr.GetSize(), 2);
    EXPECT_EQ(arr.Get(0), 10);
    EXPECT_EQ(arr.Get(1), 20);
    // arr.Get(2) теперь выбросит исключение
    EXPECT_THROW(arr.Get(2), lab2::IndexOutOfRangeException);
}

TEST(DynamicArrayTest, OperatorBracketRead)
{
    int src[] = {5, 15, 25};
    lab2::DynamicArray<int> arr(src, 3);

    EXPECT_EQ(arr[0], 5);
    EXPECT_EQ(arr[1], 15);
    EXPECT_EQ(arr[2], 25);
}

TEST(DynamicArrayTest, IsEmptyAndClear)
{
    lab2::DynamicArray<int> arr(2);
    EXPECT_FALSE(arr.IsEmpty());

    arr.Clear();
    EXPECT_TRUE(arr.IsEmpty());
    EXPECT_EQ(arr.GetSize(), 0);
}

TEST(DynamicArrayTest, NullptrWithPositiveCountThrows)
{
    EXPECT_THROW(
        {
            lab2::DynamicArray<int> array(nullptr, 3);
        },
        lab2::InvalidOperationException
    );
}

// тесты для LinkedList
TEST(LinkedListTest, BasicOperations)
{
    lab2::LinkedList<int> list;

    EXPECT_EQ(list.GetLength(), 0);

    // Append
    list.Append(10);
    list.Append(20);
    list.Append(30);

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.GetFirst(), 10);
    EXPECT_EQ(list.GetLast(), 30);

    // Prepend
    list.Prepend(5);
    EXPECT_EQ(list.GetFirst(), 5);
    EXPECT_EQ(list.GetLength(), 4);
}

TEST(LinkedListTest, InsertAt)
{
    int src[] = {1, 2, 4, 5};
    lab2::LinkedList<int> list(src, 4);

    // Вставка в середину
    list.InsertAt(3, 2);  // Вставить 3 на индекс 2

    EXPECT_EQ(list.GetLength(), 5);
    EXPECT_EQ(list.Get(2), 3);  // Новый элемент
    EXPECT_EQ(list.Get(3), 4);  // Сдвинутый элемент
}

TEST(LinkedListTest, GetSubList)
{
    int src[] = {10, 20, 30, 40, 50};
    lab2::LinkedList<int> list(src, 5);

    lab2::LinkedList<int>* sub = list.GetSublist(1, 3);  // {20, 30, 40}

    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->GetFirst(), 20);
    EXPECT_EQ(sub->GetLast(), 40);

    delete sub;  // Важно: освободить память!
}

TEST(LinkedListTest, Concat)
{
    int src1[] = {1, 2};
    int src2[] = {3, 4, 5};

    lab2::LinkedList<int> list1(src1, 2);
    lab2::LinkedList<int> list2(src2, 3);

    lab2::LinkedList<int>* result = list1.Concat(&list2);

    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(4), 5);

    delete result;
}

TEST(LinkedListTest, Exceptions)
{
    lab2::LinkedList<int> list;

    // Пустой список
    EXPECT_THROW(list.GetFirst(), lab2::InvalidOperationException);
    EXPECT_THROW(list.GetLast(), lab2::InvalidOperationException);
    EXPECT_THROW(list.Get(0), lab2::IndexOutOfRangeException);

    // Неправильный индекс
    list.Append(10);
    EXPECT_THROW(list.Get(-1), lab2::IndexOutOfRangeException);
    EXPECT_THROW(list.Get(1), lab2::IndexOutOfRangeException);
    EXPECT_THROW(list.InsertAt(5, 5), lab2::IndexOutOfRangeException);  // index > size
}

TEST(LinkedListTest, CopyConstructor)
{
    int src[] = {100, 200};
    lab2::LinkedList<int> original(src, 2);

    lab2::LinkedList<int> copy(original);

    original.Append(300);  // Меняем оригинал

    EXPECT_EQ(original.GetLength(), 3);
    EXPECT_EQ(copy.GetLength(), 2);  // Копия не изменилась
}

TEST(LinkedListTest, NullptrWithPositiveCountThrows)
{
    EXPECT_THROW(
        {
            lab2::LinkedList<int> list(nullptr, 3);
        },
        lab2::InvalidOperationException
    );
}

TEST(LinkedListTest, InsertAtIndexEqualToLengthThrows)
{
    int items[] = {1, 2, 3};
    lab2::LinkedList<int> list(items, 3);

    EXPECT_THROW(
        list.InsertAt(4, list.GetLength()),
        lab2::IndexOutOfRangeException
    );

    // После исключения список не должен измениться.
    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.GetFirst(), 1);
    EXPECT_EQ(list.GetLast(), 3);
}

// тесты полиморфизма Sequence
TEST(SequencePolymorphismTest, ArraySequenceViaBasePointer)
{
    int src[] = {10, 20, 30};
    lab2::Sequence<int>* seq = new lab2::MutableArraySequence<int>(src, 3);

    // Вызываем виртуальные методы через базовый указатель
    EXPECT_EQ(seq->GetLength(), 3);
    EXPECT_EQ(seq->Get(0), 10);
    EXPECT_EQ(seq->GetFirst(), 10);
    EXPECT_EQ(seq->GetLast(), 30);

    delete seq;
}

TEST(SequencePolymorphismTest, ListSequenceViaBasePointer)
{
    int src[] = {5, 15, 25};
    lab2::Sequence<int>* seq = new lab2::ImmutableListSequence<int>(src, 3);

    EXPECT_EQ(seq->GetLength(), 3);
    EXPECT_EQ(seq->Get(1), 15);

    delete seq;
}

TEST(SequencePolymorphismTest, MapViaBasePointer)
{
    int src[] = {1, 2, 3, 4};
    lab2::Sequence<int>* seq = new lab2::MutableListSequence<int>(src, 4);

    // Map: умножить каждый элемент на 10
    lab2::Sequence<int>* result = seq->Map([](int x) { return x * 10; });

    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 10);
    EXPECT_EQ(result->Get(3), 40);

    delete seq;
    delete result;
}

TEST(SequencePolymorphismTest, MapWorksForImmutableArraySequence)
{
    int src[] = {1, 2, 3, 4};

    lab2::Sequence<int>* seq =
        new lab2::ImmutableArraySequence<int>(src, 4);

    lab2::Sequence<int>* result =
        seq->Map([](int x) { return x * 10; });

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 10);
    EXPECT_EQ(result->Get(1), 20);
    EXPECT_EQ(result->Get(2), 30);
    EXPECT_EQ(result->Get(3), 40);

    // Исходная immutable-последовательность не должна измениться.
    EXPECT_EQ(seq->GetLength(), 4);
    EXPECT_EQ(seq->Get(0), 1);
    EXPECT_EQ(seq->Get(3), 4);

    delete result;
    delete seq;
}

TEST(SequencePolymorphismTest, WhereViaBasePointer)
{
    int src[] = {1, 2, 3, 4, 5, 6};
    lab2::Sequence<int>* seq = new lab2::ImmutableArraySequence<int>(src, 6);

    // Where: оставить только чётные
    lab2::Sequence<int>* evens = seq->Where([](int x) { return x % 2 == 0; });

    EXPECT_EQ(evens->GetLength(), 3);
    EXPECT_EQ(evens->Get(0), 2);
    EXPECT_EQ(evens->Get(2), 6);

    delete seq;
    delete evens;
}

TEST(SequencePolymorphismTest, WhereWorksForImmutableListSequence)
{
    int src[] = {1, 2, 3, 4, 5, 6};

    lab2::Sequence<int>* seq =
        new lab2::ImmutableListSequence<int>(src, 6);

    lab2::Sequence<int>* result =
        seq->Where([](int x) { return x > 3; });

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(result->GetLength(), 3);
    EXPECT_EQ(result->Get(0), 4);
    EXPECT_EQ(result->Get(1), 5);
    EXPECT_EQ(result->Get(2), 6);

    // Исходная immutable-последовательность не должна измениться.
    EXPECT_EQ(seq->GetLength(), 6);
    EXPECT_EQ(seq->GetFirst(), 1);
    EXPECT_EQ(seq->GetLast(), 6);

    delete result;
    delete seq;
}

TEST(SequencePolymorphismTest, ReduceViaBasePointer)
{
    int src[] = {1, 2, 3, 4, 5};
    lab2::Sequence<int>* seq = new lab2::MutableArraySequence<int>(src, 5);

    // Reduce: сумма всех элементов
    int sum = seq->Reduce([](int acc, int x) { return acc + x; }, 0);

    EXPECT_EQ(sum, 15);  // 1+2+3+4+5 = 15

    delete seq;
}

//MutableArraySequence
//изм эл копии не меняет оригинал и доб эл не меняет длину оригинала
TEST(MutableArraySequenceTest, CopyConstructorCreatesIndependentCopy)
{
    int source[] = {1, 2, 3};

    lab2::MutableArraySequence<int> original(source, 3);
    lab2::MutableArraySequence<int> copy(original);

    copy[0] = 999;
    copy.Append(4);

    EXPECT_EQ(original.GetLength(), 3);
    EXPECT_EQ(original.Get(0), 1);
    EXPECT_EQ(original.Get(1), 2);
    EXPECT_EQ(original.Get(2), 3);

    EXPECT_EQ(copy.GetLength(), 4);
    EXPECT_EQ(copy.Get(0), 999);
    EXPECT_EQ(copy.Get(1), 2);
    EXPECT_EQ(copy.Get(2), 3);
    EXPECT_EQ(copy.Get(3), 4);
}

//проверка concat
TEST(MutableArraySequenceTest, ConcatDoesNotModifyOriginal)
{
    int firstItems[] = {1, 2, 3};
    int secondItems[] = {4, 5};

    lab2::MutableArraySequence<int> first(firstItems, 3);
    lab2::MutableArraySequence<int> second(secondItems, 2);

    lab2::Sequence<int>* result = first.Concat(&second);

    ASSERT_NE(result, nullptr);

    EXPECT_EQ(first.GetLength(), 3);
    EXPECT_EQ(first.Get(0), 1);
    EXPECT_EQ(first.Get(1), 2);
    EXPECT_EQ(first.Get(2), 3);

    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 2);
    EXPECT_EQ(result->Get(2), 3);
    EXPECT_EQ(result->Get(3), 4);
    EXPECT_EQ(result->Get(4), 5);

    delete result;

    // После удаления результата исходный объект должен оставаться рабочим.
    EXPECT_EQ(first.GetLength(), 3);
    EXPECT_EQ(first.GetFirst(), 1);
    EXPECT_EQ(first.GetLast(), 3);
}

TEST(MutableArraySequenceTest, AssignmentCreatesIndependentCopy)
{
    int sourceItems[] = {1, 2, 3};
    int targetItems[] = {10, 20};

    lab2::MutableArraySequence<int> source(sourceItems, 3);
    lab2::MutableArraySequence<int> target(targetItems, 2);

    target = source;

    source[0] = 999;
    source.Append(4);

    EXPECT_EQ(source.GetLength(), 4);
    EXPECT_EQ(source.Get(0), 999);

    EXPECT_EQ(target.GetLength(), 3);
    EXPECT_EQ(target.Get(0), 1);
    EXPECT_EQ(target.Get(1), 2);
    EXPECT_EQ(target.Get(2), 3);

    target = target;

    EXPECT_EQ(target.GetLength(), 3);
    EXPECT_EQ(target.GetFirst(), 1);
    EXPECT_EQ(target.GetLast(), 3);
}

TEST(MutableArraySequenceTest, InsertAtIndexEqualToLengthThrows)
{
    int items[] = {1, 2, 3};
    lab2::MutableArraySequence<int> sequence(items, 3);

    EXPECT_THROW(
        sequence.InsertAt(4, sequence.GetLength()),
        lab2::IndexOutOfRangeException
    );

    // После исключения последовательность не должна измениться.
    EXPECT_EQ(sequence.GetLength(), 3);
    EXPECT_EQ(sequence.Get(0), 1);
    EXPECT_EQ(sequence.Get(1), 2);
    EXPECT_EQ(sequence.Get(2), 3);
}

TEST(MutableArraySequenceTest, InsertAtLastValidIndexWorks)
{
    int items[] = {1, 2, 3};
    lab2::MutableArraySequence<int> sequence(items, 3);

    // Индекс 2 допустим, поскольку длина до вставки равна 3.
    sequence.InsertAt(9, 2);

    EXPECT_EQ(sequence.GetLength(), 4);
    EXPECT_EQ(sequence.Get(0), 1);
    EXPECT_EQ(sequence.Get(1), 2);
    EXPECT_EQ(sequence.Get(2), 9);
    EXPECT_EQ(sequence.Get(3), 3);
}

//MutableListSequence
TEST(MutableListSequenceTest, AssignmentCreatesIndependentCopy)
{
    int sourceItems[] = {1, 2, 3};
    int targetItems[] = {10, 20};

    lab2::MutableListSequence<int> source(sourceItems, 3);
    lab2::MutableListSequence<int> target(targetItems, 2);

    target = source;

    source.Prepend(999);
    source.Append(4);

    EXPECT_EQ(source.GetLength(), 5);
    EXPECT_EQ(source.GetFirst(), 999);
    EXPECT_EQ(source.GetLast(), 4);

    EXPECT_EQ(target.GetLength(), 3);
    EXPECT_EQ(target.GetFirst(), 1);
    EXPECT_EQ(target.Get(1), 2);
    EXPECT_EQ(target.GetLast(), 3);

    target = target;

    EXPECT_EQ(target.GetLength(), 3);
    EXPECT_EQ(target.GetFirst(), 1);
    EXPECT_EQ(target.GetLast(), 3);
}

//ImmutableArraySequence
TEST(ImmutableArraySequenceTest, AssignmentCreatesIndependentCopy)
{
    int initialItems[] = {10, 20};

    lab2::ImmutableArraySequence<int> target(initialItems, 2);

    {
        int sourceItems[] = {1, 2, 3};
        lab2::ImmutableArraySequence<int> source(sourceItems, 3);

        target = source;

        EXPECT_EQ(target.GetLength(), 3);
        EXPECT_EQ(target.GetFirst(), 1);
        EXPECT_EQ(target.GetLast(), 3);
    }

    EXPECT_EQ(target.GetLength(), 3);
    EXPECT_EQ(target.Get(0), 1);
    EXPECT_EQ(target.Get(1), 2);
    EXPECT_EQ(target.Get(2), 3);

    target = target;

    EXPECT_EQ(target.GetLength(), 3);
    EXPECT_EQ(target.GetFirst(), 1);
    EXPECT_EQ(target.GetLast(), 3);
}

//ImmutableListSequence
TEST(ImmutableListSequenceTest, AssignmentCreatesIndependentCopy)
{
    int initialItems[] = {10, 20};

    lab2::ImmutableListSequence<int> target(initialItems, 2);

    {
        int sourceItems[] = {1, 2, 3};
        lab2::ImmutableListSequence<int> source(sourceItems, 3);

        target = source;

        EXPECT_EQ(target.GetLength(), 3);
        EXPECT_EQ(target.GetFirst(), 1);
        EXPECT_EQ(target.GetLast(), 3);
    }

    EXPECT_EQ(target.GetLength(), 3);
    EXPECT_EQ(target.Get(0), 1);
    EXPECT_EQ(target.Get(1), 2);
    EXPECT_EQ(target.Get(2), 3);

    target = target;

    EXPECT_EQ(target.GetLength(), 3);
    EXPECT_EQ(target.GetFirst(), 1);
    EXPECT_EQ(target.GetLast(), 3);
}

TEST(ImmutableListSequenceTest, InsertAtInvalidIndexThrows)
{
    int items[] = {1, 2, 3};
    lab2::ImmutableListSequence<int> sequence(items, 3);

    EXPECT_THROW(
        sequence.InsertAt(4, sequence.GetLength()),
        lab2::IndexOutOfRangeException
    );

    // Исходная последовательность не должна измениться.
    EXPECT_EQ(sequence.GetLength(), 3);
    EXPECT_EQ(sequence.Get(0), 1);
    EXPECT_EQ(sequence.Get(1), 2);
    EXPECT_EQ(sequence.Get(2), 3);
}

// ТЕСТЫ MUTABLE VS IMMUTABLE
TEST(MutableVsImmutableTest, MutableModifiesInPlace)
{
    int src[] = {10, 20};
    lab2::MutableArraySequence<int> seq(src, 2);

    auto* result = seq.Append(30);

    // Для mutable: result == &seq (тот же объект)
    EXPECT_EQ(result, &seq);
    EXPECT_EQ(seq.GetLength(), 3);  // Исходный объект изменился
    EXPECT_EQ(seq.GetLast(), 30);
}

TEST(MutableVsImmutableTest, ImmutableCreatesNewObject)
{
    int src[] = {10, 20};
    lab2::ImmutableArraySequence<int> seq(src, 2);

    auto* result = seq.Append(30);

    // Для immutable: result != &seq (новый объект)
    EXPECT_NE(result, &seq);
    EXPECT_EQ(seq.GetLength(), 2);        // Исходный не изменился
    EXPECT_EQ(result->GetLength(), 3);    // Новый — изменился
    EXPECT_EQ(result->GetLast(), 30);

    delete result;  // Освободить память!
}

TEST(MutableVsImmutableTest, ImmutableArrayPrependDoesNotModifyOriginal)
{
    int src[] = {1, 2, 3};

    lab2::ImmutableArraySequence<int> seq(src, 3);

    lab2::Sequence<int>* result = seq.Prepend(9);

    ASSERT_NE(result, nullptr);
    EXPECT_NE(result, &seq);

    // Проверяем новую последовательность: {9, 1, 2, 3}.
    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 9);
    EXPECT_EQ(result->Get(1), 1);
    EXPECT_EQ(result->Get(2), 2);
    EXPECT_EQ(result->Get(3), 3);

    // Исходная immutable-последовательность не изменилась.
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.Get(2), 3);

    delete result;
}

TEST(MutableVsImmutableTest, ImmutableInsertAtDoesNotModifyOriginal)
{
    int src[] = {1, 3, 4};
    lab2::ImmutableListSequence<int> seq(src, 3);

    auto* result = seq.InsertAt(2, 1);  // Вставить 2 на индекс 1

    EXPECT_EQ(seq.GetLength(), 3);              // Исходный: {1, 3, 4}
    EXPECT_EQ(result->GetLength(), 4);          // Новый: {1, 2, 3, 4}
    EXPECT_EQ(result->Get(1), 2);               // Вставленный элемент

    delete result;
}


// тесты для Option
TEST(OptionTest, SomeAndNone)
{
    auto some = lab2::Option<int>::Some(42);
    EXPECT_TRUE(some.HasValue());
    EXPECT_EQ(some.GetValue(), 42);

    auto none = lab2::Option<int>::None();
    EXPECT_FALSE(none.HasValue());
    EXPECT_THROW(none.GetValue(), lab2::InvalidOperationException);
}

TEST(OptionTest, BoolConversion)
{
    auto some = lab2::Option<int>::Some(100);
    if (some) {
        SUCCEED() << "Some преобразуется в true";
    } else {
        FAIL();
    }

    auto none = lab2::Option<int>::None();
    if (!none) {
        SUCCEED() << "None преобразуется в false";
    } else {
        FAIL();
    }
}

TEST(OptionTest, GetValueOr)
{
    auto some = lab2::Option<int>::Some(10);
    auto none = lab2::Option<int>::None();

    EXPECT_EQ(some.GetValueOr(999), 10);   // Есть значение → возвращаем его
    EXPECT_EQ(none.GetValueOr(999), 999);  // Нет значения → возвращаем дефолт
}

TEST(TrySemanticsTest, TryGet)
{
    int src[] = {5, 10, 15};
    lab2::MutableArraySequence<int> seq(src, 3);

    // Корректный индекс
    auto opt = seq.TryGet(1);
    EXPECT_TRUE(opt.HasValue());
    EXPECT_EQ(opt.GetValue(), 10);

    // Некорректный индекс → возвращает None, а не выбрасывает исключение
    EXPECT_FALSE(seq.TryGet(-1).HasValue());
    EXPECT_FALSE(seq.TryGet(5).HasValue());
}

TEST(TrySemanticsTest, TryFirstAndTryLast)
{
    lab2::MutableListSequence<int> empty;
    EXPECT_FALSE(empty.TryFirst().HasValue());
    EXPECT_FALSE(empty.TryLast().HasValue());

    int src[] = {100, 200, 300};
    lab2::ImmutableArraySequence<int> seq(src, 3);

    EXPECT_EQ(seq.TryFirst().GetValue(), 100);
    EXPECT_EQ(seq.TryLast().GetValue(), 300);
}

// тесты для итераторы
TEST(IteratorTest, ArrayEnumerator)
{
    int src[] = {10, 20, 30};
    lab2::MutableArraySequence<int> seq(src, 3);

    lab2::IEnumerator<int>* it = seq.GetEnumerator();

    // Начальная позиция: до первого элемента
    EXPECT_FALSE(it->GetCurrent().HasValue());

    // Переход к первому элементу
    EXPECT_TRUE(it->MoveNext());
    EXPECT_TRUE(it->GetCurrent().HasValue());
    EXPECT_EQ(it->GetCurrent().GetValue(), 10);

    // Второй элемент
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->GetCurrent().GetValue(), 20);

    // Третий элемент
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->GetCurrent().GetValue(), 30);

    // Конец последовательности
    EXPECT_FALSE(it->MoveNext());
    EXPECT_FALSE(it->GetCurrent().HasValue());

    // Reset
    it->Reset();
    EXPECT_TRUE(it->MoveNext());
    EXPECT_EQ(it->GetCurrent().GetValue(), 10);

    delete it;
}

TEST(IteratorTest, ListEnumeratorAfterEnd)
{
    int items[] = {1, 2, 3};

    lab2::MutableListSequence<int> sequence(items, 3);
    auto* iterator = sequence.GetEnumerator();

    // До начала обхода текущего элемента нет.
    EXPECT_FALSE(iterator->GetCurrent().HasValue());

    EXPECT_TRUE(iterator->MoveNext());
    EXPECT_TRUE(iterator->GetCurrent().HasValue());

    EXPECT_TRUE(iterator->MoveNext());
    EXPECT_TRUE(iterator->GetCurrent().HasValue());

    EXPECT_TRUE(iterator->MoveNext());
    EXPECT_TRUE(iterator->GetCurrent().HasValue());

    // Все элементы уже пройдены.
    EXPECT_FALSE(iterator->MoveNext());

    // После завершения текущего элемента быть не должно.
    EXPECT_FALSE(iterator->GetCurrent().HasValue());

    delete iterator;
}

TEST(IteratorTest, EmptySequenceEnumerator)
{
    lab2::ImmutableListSequence<int> empty;

    lab2::IEnumerator<int>* it = empty.GetEnumerator();

    EXPECT_FALSE(it->MoveNext());  // Сразу конец
    EXPECT_FALSE(it->GetCurrent().HasValue());

    delete it;
}

int main(int argc, char **argv) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    std::cout
        << "=== Запуск модульных тестов лабораторной работы №2 ===\n";
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}






















