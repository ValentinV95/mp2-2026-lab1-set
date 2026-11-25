#include "tbitfield.h"

#include "tset.h"

#include <gtest.h>

TEST(TBitField, invert_of_invert_returns_original)
{
    const int size = 10;
    TBitField bf(size), expBf(size);
    bf.SetBit(2); bf.SetBit(5); bf.SetBit(9);
    expBf = bf;

    bf = ~~bf;

    EXPECT_EQ(expBf, bf);
}

TEST(TSet, union_with_empty_set_dont_change_set)
{
    const int size = 4;
    TSet set(size), empty(0);
    set.InsElem(1);
    set.InsElem(3);

    TSet result = set + empty;

    EXPECT_EQ(set, result);
    EXPECT_EQ(size, result.GetMaxPower());
}

TEST(TSet, union_is_idempotent)
{
    const int size = 5;
    TSet set(size);
    set.InsElem(1); set.InsElem(3);

    TSet result = set + set;

    EXPECT_EQ(set, result);
}