#include <gtest.h>
#include "tbitfield.h"
#include "tset.h"

//проверка конструктора
TEST(TBitField, constructor_TBitField)
{
    const int len = 10;
    TBitField bf(len);

    EXPECT_EQ(len, bf.GetLength());
    for (int i = 0; i < len; i++) {
        EXPECT_EQ(0, bf.GetBit(i));
    }
}

//проверка нулевой длины
TEST(TBitField, zero_length) {
    TBitField bf(0);
    EXPECT_EQ(0, bf.GetLength());
    ASSERT_ANY_THROW(bf.SetBit(0));
    ASSERT_ANY_THROW(bf.GetBit(0));
    ASSERT_ANY_THROW(bf.ClrBit(0));
}

//двойное отрицание
TEST(TBitField, double_negation) {
    TBitField bf(10);
    bf.SetBit(3);
    bf.SetBit(7);
    TBitField back = ~~bf;
    EXPECT_EQ(bf, back);
}

//проверка конструктора TSet
TEST(TSet, constructor_TSet_from_bitfield) {
    TBitField bf(5);
    bf.SetBit(1);
    TSet s(bf);
    EXPECT_EQ(5, s.GetMaxPower());
    EXPECT_EQ(1, s.IsMember(1));
}

//выход за границы
TEST(TSet, insert_out_of_range_throws) {
    TSet s(5);
    ASSERT_ANY_THROW(s.InsElem(-1));
    ASSERT_ANY_THROW(s.InsElem(5));
}

//сравнение полного и пустого множества
TEST(TSet, complement_empty_and_full) {
    TSet empty(4);
    TSet full(4);
    for (int i = 0; i < 4; ++i) full.InsElem(i);

    TSet invEmpty = ~empty;
    TSet invFull = ~full;
    EXPECT_EQ(full, invEmpty);
    EXPECT_EQ(empty, invFull);
}