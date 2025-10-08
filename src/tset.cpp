#include "tset.h"
#include <sstream>
#include <string>

TSet::TSet(int mp) : MaxPower(mp), BitField(mp)
{
    if (mp <= 0)
        throw std::invalid_argument("MaxPower must be positive"); // Все прочие исключения вызовутся у битполя
}

// конструктор копирования
TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField)
{
    // Копируются данные из объекта s, исключения есть в классе битполя
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf)
{
}

TSet::operator TBitField() // Оператор преобразования TSet в TBitField
{
    return BitField;
}

int TSet::GetMaxPower(void) const // Получение мощности универса
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const
{
    return BitField.GetBit(Elem); // Если элемент есть, то его бит 1, если нет - 0. Булевый диапазон значений
}

void TSet::InsElem(const int Elem) // Добавление элемента
{
    BitField.SetBit(Elem); // Установка бита элемента
}

void TSet::DelElem(const int Elem) // Удаление элемента
{
    BitField.ClrBit(Elem); // Очистка бита элемента
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // Присваивание
{
    if (this != &s)
    {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    return (MaxPower == s.MaxPower) && (BitField == s.BitField);
}

int TSet::operator!=(const TSet& s) const // Отрицание равенства
{
    return !(*this == s);
}

TSet TSet::operator+ (const TSet& s)
{
    int resultMaxPower = (MaxPower > s.MaxPower) ? MaxPower : s.MaxPower;
    TSet result(resultMaxPower);
    result.BitField = BitField | s.BitField;
    return result;
}

TSet TSet::operator+ (const int Elem) // Добавление элемента
{
    TSet result(*this); // Копируем текущее множество
    result.InsElem(Elem); // Добавляем элемент
    return result;
}

TSet TSet::operator- (const int Elem) // Удаление элемента
{
    TSet result(*this);
    result.DelElem(Elem);
    return result;
}

TSet TSet::operator* (const TSet& s)
{
    int resultMaxPower = (MaxPower > s.MaxPower) ? MaxPower : s.MaxPower;
    TSet result(resultMaxPower);
    result.BitField = BitField & s.BitField;
    return result;
}

TSet TSet::operator~ (void)
{
    TSet result(MaxPower);
    result.BitField = ~BitField;
    return result;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s)
{
    int maxPower; // Нам нужно узнать максимальную мощность
    istr >> maxPower; // Вводим чиселку
    // Если у нас будет ошибка формата ввода (или мы введем отрицательную мощность) у нас будет исключение
    if (istr.fail() || maxPower <= 0)
        throw std::invalid_argument("Invalid max power");

    TSet temp(maxPower); // Создаем временный объект

    string line;
    getline(istr, line); // Записываем весь входной поток в строку

    // Если строка пустая (был перевод строки сразу после maxPower),
    // читаем следующую строку
    if (line.empty()) {
        getline(istr, line);
    }

    istringstream iss(line); // Превращаем строку в поток
    int elem;
    while (iss >> elem)
    {
        if (elem > maxPower || elem < 0)
            throw std::invalid_argument("Element out of range");
        temp.InsElem(elem);
    }

    // Обрабатываем возникшее исключение формата
    if (iss.fail() && !iss.eof())
    {
        throw std::invalid_argument("Invalid element format");
    }

    s = temp;
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s)
{
    ostr << "{ "; // Сначала выведем открывающую множество скобку

    // Выведем все элементы множества
    for (int i = 0; i < s.GetMaxPower(); i++)
    {
        if (s.IsMember(i))
            ostr << i << " ";

        // Каждые 32 бита будем делать перенос строки для читаемости
        if ((i + 1) % 32 == 0)
            ostr << endl << " ";
    }

    // Закрываем множество скобкой
    ostr << "}";

    return ostr; // Возвращаем поток для цепочки ввода
}
