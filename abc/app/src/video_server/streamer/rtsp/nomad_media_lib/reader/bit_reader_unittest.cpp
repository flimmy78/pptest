

#include "gmock/gmock.h"

#include "reader/bit_reader.h"
#include "base/basic_types.h"

namespace nml_reader
{

TEST(Bit_Reader_Test, normal_Operation_Test)
{
    uint8 value_8;
    uint64 value_64;

    // 0101 0101 1001 1001 repeats 4 times
    uint8 buffer[] = {0x55, 0x99, 0x55, 0x99, 0x55, 0x99, 0x55, 0x99};
    BitReader reader(buffer, 6);  // Initialize with 6 bytes only

    EXPECT_TRUE( reader.readBits(1, &value_8) );
    EXPECT_EQ( value_8, 0);
    EXPECT_TRUE(reader.readBits(8, &value_8));
    EXPECT_EQ(value_8, 0xab);  // 1010 1011
    EXPECT_TRUE(reader.readBits(7, &value_64));
    EXPECT_TRUE(reader.readBits(32, &value_64));
    EXPECT_EQ(value_64, 0x55995599u);
    EXPECT_FALSE(reader.readBits(1, &value_8));
    value_8 = 0xff;
    EXPECT_TRUE(reader.readBits(0, &value_8));
    EXPECT_EQ(value_8, 0);

    BitReader reader2(buffer, 8);
    EXPECT_TRUE(reader2.readBits(64, &value_64));
    EXPECT_EQ(value_64, 0x5599559955995599ull);
    EXPECT_FALSE(reader2.readBits(1, &value_8));
    EXPECT_TRUE(reader2.readBits(0, &value_8));
}
 
TEST(BitReader_Test, read_Beyond_Exception_Test) {
    uint8 value8;
    uint8 buffer[] = {0x12};
    BitReader reader(buffer, sizeof(buffer));

    EXPECT_TRUE(reader.readBits(4, &value8));
    EXPECT_FALSE(reader.readBits(5, &value8));
    EXPECT_FALSE(reader.readBits(1, &value8));
    EXPECT_TRUE(reader.readBits(0, &value8));
}

TEST(BitReader_Test, skip_Bits_Test) {
    uint8 value8;
    uint8 buffer[] = { 0x0a, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    BitReader reader(buffer, sizeof(buffer));

    EXPECT_TRUE(reader.skipBits(2));
    EXPECT_TRUE(reader.readBits(3, &value8));
    EXPECT_EQ(value8, 1);

    EXPECT_TRUE(reader.skipBits(11));
    EXPECT_TRUE(reader.readBits(8, &value8));
    EXPECT_EQ(value8, 3);

    EXPECT_TRUE(reader.skipBits(76));
    EXPECT_TRUE(reader.readBits(4, &value8));
    EXPECT_EQ(value8, 13);

    EXPECT_FALSE(reader.skipBits(100));
    EXPECT_TRUE(reader.skipBits(0));
    EXPECT_FALSE(reader.skipBits(1));
}

} //namespace nml_reader


