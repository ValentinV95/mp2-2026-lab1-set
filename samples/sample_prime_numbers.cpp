#if defined(_MSC_VER)
#  pragma execution_character_set("utf-8")
#endif

#include <iostream>
#include <iomanip>
#include <clocale>

#ifndef USE_SET

#include "tbitfield.h"

using std::cout;
using std::cin;
using std::endl;
using std::setw;

int main()
{
  int n, m, k, count;

  setlocale(LC_ALL, "Russian");
  cout << "Тестирование программ поддержки битового поля" << endl;
  cout << "             Решето Эратосфена" << endl;
  cout << "Введите верхнюю границу целых значений - ";
  if (!(cin >> n) || n < 2) {
    cout << "N должно быть >= 2" << endl;
    return 0;
  }

  TBitField s(n + 1);
  for (m = 2; m <= n; m++)
    s.SetBit(m);
  for (m = 2; m * m <= n; m++)
    if (s.GetBit(m))
      for (k = 2 * m; k <= n; k += m)
        if (s.GetBit(k))
          s.ClrBit(k);
  cout << endl << "Печать множества некратных чисел" << endl << s << endl;
  cout << endl << "Печать простых чисел" << endl;
  count = 0;
  k = 1;
  for (m = 2; m <= n; m++)
    if (s.GetBit(m))
    {
      count++;
      cout << setw(3) << m << " ";
      if (k++ % 10 == 0)
        cout << endl;
    }
  cout << endl;
  cout << "В первых " << n << " числах " << count << " простых" << endl;

  return 0;
}

#else

#include "tset.h"

using std::cout;
using std::cin;
using std::endl;
using std::setw;

int main()
{
  int n, m, k, count;

  setlocale(LC_ALL, "Russian");
  cout << "Тестирование программ поддержки множества" << endl;
  cout << "              Решето Эратосфена" << endl;
  cout << "Введите верхнюю границу целых значений - ";
  if (!(cin >> n) || n < 2) {
    cout << "N должно быть >= 2" << endl;
    return 0;
  }

  TSet s(n + 1);
  for (m = 2; m <= n; m++)
    s.InsElem(m);
  for (m = 2; m * m <= n; m++)
    if (s.IsMember(m))
      for (k = 2 * m; k <= n; k += m)
        if (s.IsMember(k))
          s.DelElem(k);
  cout << endl << "Печать множества некратных чисел" << endl << s << endl;
  cout << endl << "Печать простых чисел" << endl;
  count = 0;
  k = 1;
  for (m = 2; m <= n; m++)
    if (s.IsMember(m))
    {
      count++;
      cout << setw(3) << m << " ";
      if (k++ % 10 == 0)
        cout << endl;
    }
  cout << endl;
  cout << "В первых " << n << " числах " << count << " простых" << endl;

  return 0;
}

#endif
