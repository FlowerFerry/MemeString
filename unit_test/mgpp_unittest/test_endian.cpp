
#include <catch2/catch.hpp>
#include <megopp/endian/bit_field_member.h>

TEST_CASE("bit_field_member basic", "[bit_field_member]")
{
    mgpp::endian::bit_field_member<0, 8, mgpp::endian_t::little_byte> bfm_0_1;
    bfm_0_1 = 1;
    REQUIRE(bfm_0_1   == 1);
    REQUIRE(bfm_0_1++ == 1);
    REQUIRE(bfm_0_1   == 2);
    REQUIRE(++bfm_0_1 == 3);

    REQUIRE(bfm_0_1-- == 3);
    REQUIRE(bfm_0_1   == 2);
    REQUIRE(--bfm_0_1 == 1);
    
    REQUIRE(bfm_0_1 * 2 == 2);
    REQUIRE(bfm_0_1 / 2 == 0);


    union {
        mgpp::endian::bit_field_member<2, 4, mgpp::endian_t::little_byte> bfm;
        uint8_t v;
    } u_0_1;
    u_0_1.v   = 0;
    u_0_1.bfm = 3;
    
    REQUIRE(u_0_1.bfm * 2 == 6);
    REQUIRE(u_0_1.bfm / 2 == 1);
    REQUIRE(u_0_1.bfm + 2 == 5);
    REQUIRE(u_0_1.bfm - 2 == 1);
    REQUIRE(u_0_1.bfm & 2 == 2);
    REQUIRE(u_0_1.bfm | 2 == 3);
    REQUIRE(u_0_1.bfm ^ 2 == 1);
    REQUIRE(~u_0_1.bfm == 0xFC);
    REQUIRE(u_0_1.bfm << 2 == 12);
    REQUIRE(u_0_1.bfm >> 2 == 0);
    REQUIRE(!u_0_1.bfm == false);
        
    REQUIRE(u_0_1.bfm == 3);
    REQUIRE(u_0_1.v == 0x0C);
    REQUIRE(u_0_1.bfm++ == 3);
    REQUIRE(u_0_1.bfm == 4);
    REQUIRE(u_0_1.v == 0x10);
    REQUIRE(u_0_1.bfm-- == 4);
    REQUIRE(u_0_1.bfm == 3);
    REQUIRE(u_0_1.v == 0x0C);

    REQUIRE((u_0_1.bfm *= 2) == 6);
    REQUIRE(u_0_1.v == 0x18);
    REQUIRE((u_0_1.bfm /= 2) == 3);
    REQUIRE(u_0_1.v == 0x0C);
    REQUIRE((u_0_1.bfm += 2) == 5);
    REQUIRE(u_0_1.v == 0x14);
    REQUIRE((u_0_1.bfm -= 2) == 3);
    REQUIRE(u_0_1.v == 0x0C);
    REQUIRE((u_0_1.bfm &= 2) == 2);
    REQUIRE(u_0_1.v == 0x08);
    REQUIRE((u_0_1.bfm |= 1) == 3);
    REQUIRE(u_0_1.v == 0x0C);

    REQUIRE((u_0_1.bfm *= 6) == 2);
    REQUIRE(u_0_1.v == 0x08);
    REQUIRE((u_0_1.bfm += 15) == 1);
    REQUIRE(u_0_1.v == 0x04);
    REQUIRE((u_0_1.bfm /= 10) == 0);
    REQUIRE(u_0_1.v == 0x00);
    REQUIRE((u_0_1.bfm -= 1) == 0xF);
    REQUIRE(u_0_1.v == 0x3C);

    union {
        mgpp::endian::bit_field_member<2, 4, mgpp::endian_t::big_byte> bfm;
        uint8_t v;
    } u_4_1;
    u_4_1.v = 0;
    u_4_1.bfm = 3;
    REQUIRE(u_4_1.bfm == 3);
    REQUIRE(u_4_1.v == 0x0C);

    REQUIRE(u_4_1.bfm * 2 == 6);
    REQUIRE(u_4_1.bfm / 2 == 1);
    REQUIRE(u_4_1.bfm + 2 == 5);
    REQUIRE(u_4_1.bfm - 2 == 1);
    REQUIRE(u_4_1.bfm & 2 == 2);
    REQUIRE(u_4_1.bfm | 2 == 3);
    REQUIRE(u_4_1.bfm ^ 2 == 1);
    REQUIRE(~u_4_1.bfm == 0xFC);
    REQUIRE(u_4_1.bfm << 2 == 12);
    REQUIRE(u_4_1.bfm >> 2 == 0);
    REQUIRE(!u_4_1.bfm == false);

    REQUIRE(u_4_1.bfm == 3);
    REQUIRE(u_4_1.v == 0x0C);
    REQUIRE(u_4_1.bfm++ == 3);
    REQUIRE(u_4_1.bfm == 4);
    REQUIRE(u_4_1.v == 0x10);
    REQUIRE(u_4_1.bfm-- == 4);
    REQUIRE(u_4_1.bfm == 3);
    REQUIRE(u_4_1.v == 0x0C);
    
    REQUIRE((u_4_1.bfm *= 2) == 6);
    REQUIRE(u_4_1.v == 0x18);
    REQUIRE((u_4_1.bfm /= 2) == 3);
    REQUIRE(u_4_1.v == 0x0C);
    REQUIRE((u_4_1.bfm += 2) == 5);
    REQUIRE(u_4_1.v == 0x14);
    REQUIRE((u_4_1.bfm -= 2) == 3);
    REQUIRE(u_4_1.v == 0x0C);
    REQUIRE((u_4_1.bfm &= 2) == 2);
    REQUIRE(u_4_1.v == 0x08);
    REQUIRE((u_4_1.bfm |= 1) == 3);
    REQUIRE(u_4_1.v == 0x0C);
    
    REQUIRE((u_4_1.bfm *= 6) == 2);
    REQUIRE(u_4_1.v == 0x08);
    REQUIRE((u_4_1.bfm += 15) == 1);
    REQUIRE(u_4_1.v == 0x04);
    REQUIRE((u_4_1.bfm /= 10) == 0);
    REQUIRE(u_4_1.v == 0x00);
    REQUIRE((u_4_1.bfm -= 1) == 0xF);
    REQUIRE(u_4_1.v == 0x3C);
    

#if MEGO_ENDIAN__LITTLE_BYTE
    union {
        mgpp::endian::bit_field_member<6, 4, mgpp::endian_t::little_byte> bfm;
        uint16_t v;
    } u_1_1;
    u_1_1.v = 0x5006;
    REQUIRE(u_1_1.bfm == 0);
    u_1_1.bfm += 3;
    REQUIRE(u_1_1.bfm == 3);
    REQUIRE(u_1_1.v == 0x50C6);   

    REQUIRE(u_1_1.bfm * 2 == 6);
    REQUIRE(u_1_1.bfm / 2 == 1);
    REQUIRE(u_1_1.bfm + 2 == 5);
    REQUIRE(u_1_1.bfm - 2 == 1);
    REQUIRE(u_1_1.bfm & 2 == 2);
    REQUIRE(u_1_1.bfm | 2 == 3);
    REQUIRE(u_1_1.bfm ^ 2 == 1);
    REQUIRE(~u_1_1.bfm == 0xFFFC);
    REQUIRE(u_1_1.bfm << 2 == 12);
    REQUIRE(u_1_1.bfm >> 2 == 0);
    REQUIRE(!u_1_1.bfm == false);

    REQUIRE(u_1_1.bfm++ == 3);
    REQUIRE(u_1_1.bfm == 4);
    REQUIRE(u_1_1.v == 0x5106);
    REQUIRE(u_1_1.bfm-- == 4);
    REQUIRE(u_1_1.bfm == 3);
    REQUIRE(u_1_1.v == 0x50C6);
    
    REQUIRE((u_1_1.bfm *= 2) == 6);
    REQUIRE(u_1_1.v == 0x5186);
    REQUIRE((u_1_1.bfm /= 2) == 3);
    REQUIRE(u_1_1.v == 0x50C6);
    REQUIRE((u_1_1.bfm += 2) == 5);
    REQUIRE(u_1_1.v == 0x5146);
    REQUIRE((u_1_1.bfm -= 2) == 3);
    REQUIRE(u_1_1.v == 0x50C6);
    REQUIRE((u_1_1.bfm &= 2) == 2);
    REQUIRE(u_1_1.v == 0x5086);
    REQUIRE((u_1_1.bfm |= 1) == 3);
    REQUIRE(u_1_1.v == 0x50C6);

    REQUIRE((u_1_1.bfm *= 6) == 2);
    REQUIRE(u_1_1.v == 0x5086);
    REQUIRE((u_1_1.bfm += 15) == 1);
    REQUIRE(u_1_1.v == 0x5046);
    REQUIRE((u_1_1.bfm /= 10) == 0);
    REQUIRE(u_1_1.v == 0x5006);
    REQUIRE((u_1_1.bfm -= 1) == 0xF);
    REQUIRE(u_1_1.v == 0x53C6);

    union {
        mgpp::endian::bit_field_member<6, 4, mgpp::endian_t::big_byte> bfm;
        uint16_t v;
    } u_3_1;
    u_3_1.v = 0x0650;
    REQUIRE(u_3_1.bfm == 0);
    u_3_1.bfm += 3;
    REQUIRE(u_3_1.bfm == 3);
    REQUIRE(u_3_1.v == 0xC650);
    
    REQUIRE(u_3_1.bfm * 2 == 6);
    REQUIRE(u_3_1.bfm / 2 == 1);
    REQUIRE(u_3_1.bfm + 2 == 5);
    REQUIRE(u_3_1.bfm - 2 == 1);
    REQUIRE(u_3_1.bfm & 2 == 2);
    REQUIRE(u_3_1.bfm | 2 == 3);
    REQUIRE(u_3_1.bfm ^ 2 == 1);
    REQUIRE(~u_3_1.bfm == 0xFFFC);
    REQUIRE(u_3_1.bfm << 2 == 12);
    REQUIRE(u_3_1.bfm >> 2 == 0);
    REQUIRE(!u_3_1.bfm == false);
    
    REQUIRE(u_3_1.bfm++ == 3);
    REQUIRE(u_3_1.bfm == 4);
    REQUIRE(u_3_1.v == 0x0651);
    REQUIRE(u_3_1.bfm-- == 4);
    REQUIRE(u_3_1.bfm == 3);
    REQUIRE(u_3_1.v == 0xC650);

    REQUIRE((u_3_1.bfm *= 2) == 6);
    REQUIRE(u_3_1.v == 0x8651);
    REQUIRE((u_3_1.bfm /= 2) == 3);
    REQUIRE(u_3_1.v == 0xC650);
    REQUIRE((u_3_1.bfm += 2) == 5);
    REQUIRE(u_3_1.v == 0x4651);
    REQUIRE((u_3_1.bfm -= 2) == 3);
    REQUIRE(u_3_1.v == 0xC650);
    REQUIRE((u_3_1.bfm &= 2) == 2);
    REQUIRE(u_3_1.v == 0x8650);
    REQUIRE((u_3_1.bfm |= 1) == 3);
    REQUIRE(u_3_1.v == 0xC650);
    
    REQUIRE((u_3_1.bfm *= 6) == 2);
    REQUIRE(u_3_1.v == 0x8650);
    REQUIRE((u_3_1.bfm += 15) == 1);
    REQUIRE(u_3_1.v == 0x4650);
    REQUIRE((u_3_1.bfm /= 10) == 0);
    REQUIRE(u_3_1.v == 0x0650);
    REQUIRE((u_3_1.bfm -= 1) == 0xF);
    REQUIRE(u_3_1.v == 0xC653);

    union {
        mgpp::endian::bit_field_member<6, 12, mgpp::endian_t::big_byte, uint32_t> bfm;
        uint32_t v;
    } u_2_1;
    u_2_1.v = 0x050000FF;
    REQUIRE(u_2_1.bfm == 0);
    u_2_1.bfm += 0xFF;
    REQUIRE(u_2_1.bfm == 0xFF);
    REQUIRE(u_2_1.v == 0xC53F00FF);

    REQUIRE(u_2_1.bfm * 2 == 0x1FE);
    REQUIRE(u_2_1.bfm / 2 == 0x7F);
    REQUIRE(u_2_1.bfm + 2 == 0x101);
    REQUIRE(u_2_1.bfm - 2 == 0xFD);
    REQUIRE(u_2_1.bfm & 0x0F == 0x0F);
    REQUIRE(u_2_1.bfm | 0x0F == 0xFF);
    REQUIRE(u_2_1.bfm ^ 0x0F == 0xF0);
    REQUIRE(~u_2_1.bfm == 0xFFFFFF00);
    REQUIRE(u_2_1.bfm << 2 == 0x3FC);
    REQUIRE(u_2_1.bfm >> 2 == 0x3F);
    REQUIRE(!u_2_1.bfm == false);

    REQUIRE(u_2_1.bfm++ == 0xFF);
    REQUIRE(u_2_1.bfm == 0x100);
    REQUIRE(u_2_1.v == 0x054000FF);
    REQUIRE(u_2_1.bfm-- == 0x100);
    REQUIRE(u_2_1.bfm == 0xFF);
    REQUIRE(u_2_1.v == 0xC53F00FF);

    REQUIRE((u_2_1.bfm *= 2) == 0x1FE);
    REQUIRE(u_2_1.v == 0x857F00FF);
    REQUIRE((u_2_1.bfm /= 2) == 0xFF);
    REQUIRE(u_2_1.v == 0xC53F00FF);
    REQUIRE((u_2_1.bfm += 2) == 0x101);
    REQUIRE(u_2_1.v == 0x454000FF);
    REQUIRE((u_2_1.bfm -= 2) == 0xFF);
    REQUIRE(u_2_1.v == 0xC53F00FF);
    REQUIRE((u_2_1.bfm &= 0x0F) == 0x0F);
    REQUIRE(u_2_1.v == 0xC50300FF);
    REQUIRE((u_2_1.bfm |= 0xF0) == 0xFF);
    REQUIRE(u_2_1.v == 0xC53F00FF);
    
    REQUIRE((u_2_1.bfm *= 0xFF) == 0xE01);
    REQUIRE(u_2_1.v == 0x458003FF);
    REQUIRE((u_2_1.bfm += 0x1FF) == 0);
    REQUIRE(u_2_1.v == 0x050000FF);
    REQUIRE((u_2_1.bfm -= 0x02) == 0xFFE);
    REQUIRE(u_2_1.v == 0x85FF03FF);
    REQUIRE((u_2_1.bfm /= 2) == 2047);
    REQUIRE(u_2_1.v == 0xC5FF01FF);

#endif // MEGO_ENDIAN__LITTLE_BYTE
}


// =========================================================================
// Union 内存共享与相邻位域互不干扰 (Little Endian)
// =========================================================================
TEST_CASE("bit_field_member inside union (Memory Sharing & Masking)", "[endian][bitfield][union]") {
    // 定义一个 32 位的寄存器，包含 3 个位域
    union Reg32 {
        uint32_t raw;
        // 参数: BeginBit, BitSize, Endian, Type
        mgpp::endian::bit_field_member<0, 8, mgpp::endian_t::little_byte, uint32_t> field1; // bits 0-7
        mgpp::endian::bit_field_member<8, 12, mgpp::endian_t::little_byte, uint32_t> field2; // bits 8-19
        mgpp::endian::bit_field_member<20, 12, mgpp::endian_t::little_byte, uint32_t> field3; // bits 20-31
    };

    Reg32 reg;
    reg.raw = 0; // 初始化内存

    SECTION("Independent bit-field assignments") {
        reg.field1 = 0xAA;
#if MEGO_ENDIAN__LITTLE_BYTE
        REQUIRE(reg.raw == 0x000000AA);
#endif
        reg.field2 = 0xBBB;
#if MEGO_ENDIAN__LITTLE_BYTE
        REQUIRE(reg.raw == 0x000BBBAA); // 验证 field2 的写入没有破坏 field1
#endif
        reg.field3 = 0xCCC;
#if MEGO_ENDIAN__LITTLE_BYTE
        REQUIRE(reg.raw == 0xCCCBBBAA); // 验证 field3 的写入没有破坏前两个
#endif

        // 再次读取，确保 get_value 解析正确
        REQUIRE(reg.field1.get_value() == 0xAA);
        REQUIRE(reg.field2.get_value() == 0xBBB);
        REQUIRE(reg.field3.get_value() == 0xCCC);
    }
}

// =========================================================================
// 跨字节边界的位域掩码测试
// =========================================================================
TEST_CASE("bit_field_member cross-byte boundary masking", "[endian][bitfield][boundary]") {
    union Reg16 {
        uint16_t raw;
        // 一个跨越了字节边界的位域 (跨越了 bit 7 和 bit 8)
        mgpp::endian::bit_field_member<4, 8, mgpp::endian_t::little_byte, uint16_t> middle_byte;
    };

    Reg16 reg;
    reg.raw = 0xFFFF; // 全 1

    SECTION("Clear middle bits only") {
        // 将中间的 8 位清零，两端的 4 位应保持为 1
        reg.middle_byte = 0x00;

        // 预期二进制: 1111 0000 0000 1111 -> 0xF00F
#if MEGO_ENDIAN__LITTLE_BYTE
        REQUIRE(reg.raw == 0xF00F);
#endif
        REQUIRE(reg.middle_byte.get_value() == 0x00);
    }

    SECTION("Set middle bits to specific pattern") {
        reg.raw = 0x0000;
        reg.middle_byte = 0x5A; // 0101 1010

        // 预期二进制: 0000 0101 1010 0000 -> 0x05A0
#if MEGO_ENDIAN__LITTLE_BYTE
        REQUIRE(reg.raw == 0x05A0);
#endif
        REQUIRE(reg.middle_byte.get_value() == 0x5A);
    }
}

// =========================================================================
// 大端序 (Big Endian) 内存布局验证
// =========================================================================
TEST_CASE("bit_field_member Big Endian memory layout", "[endian][bitfield][big_endian]") {
    union Reg32BE {
        uint8_t raw_bytes[4];
        mgpp::endian::bit_field_member<0, 8, mgpp::endian_t::big_byte, uint32_t> byte0;
        mgpp::endian::bit_field_member<8, 8, mgpp::endian_t::big_byte, uint32_t> byte1;
    };

    Reg32BE reg;
    std::memset(reg.raw_bytes, 0, 4);

    SECTION("Verify byte placement in Big Endian mode") {
        reg.byte0 = 0xAA;
        reg.byte1 = 0xBB;

        REQUIRE(reg.byte0.get_value() == 0xAA);
        REQUIRE(reg.byte1.get_value() == 0xBB);

        // 在大端序中，_BeginBit=0 通常对应最高有效字节 (MSB)，即内存的最低地址
        // 验证底层 uint8_t 数组的实际分布
#if MEGO_ENDIAN__BIG_BYTE
        REQUIRE(reg.raw_bytes[0] == 0xAA);
        REQUIRE(reg.raw_bytes[1] == 0xBB);
        REQUIRE(reg.raw_bytes[2] == 0x00);
        REQUIRE(reg.raw_bytes[3] == 0x00);
#endif
    }
}

// =========================================================================
// 算术运算与位域截断 (Overflow Handling)
// =========================================================================
TEST_CASE("bit_field_member arithmetic and truncation", "[endian][bitfield][arithmetic]") {
    // 仅 4 位的位域，最大值为 15 (0xF)
    mgpp::endian::bit_field_member<0, 4, mgpp::endian_t::little_byte, uint8_t> nibble;
    nibble.reset(0);

    SECTION("Truncation on assignment") {
        nibble = 0xFF; // 尝试赋超大值
        REQUIRE(nibble.get_value() == 0x0F); // 必须被截断为 4 位
    }

    SECTION("Increment and overflow") {
        nibble = 14;
        REQUIRE((nibble++).get_value() == 14);
        REQUIRE(nibble.get_value() == 15);

        // 15 + 1 = 16，截断后应为 0
        ++nibble;
        REQUIRE(nibble.get_value() == 0);
    }

    SECTION("Compound assignments with truncation") {
        nibble = 10;
        nibble += 7; // 17 -> 截断为 1 (17 & 0xF)
        REQUIRE(nibble.get_value() == 1);

        nibble = 5;
        nibble *= 4; // 20 -> 截断为 4 (20 & 0xF)
        REQUIRE(nibble.get_value() == 4);
    }
}

// =========================================================================
// 位运算 (Bitwise Operations)
// =========================================================================
TEST_CASE("bit_field_member bitwise operations", "[endian][bitfield][bitwise]") {
    mgpp::endian::bit_field_member<0, 6, mgpp::endian_t::little_byte, uint8_t> bf;
    bf.reset(0b101010); // 42

    SECTION("Compound bitwise operators") {
        bf |= 0b010101;
        REQUIRE(bf.get_value() == 0b111111); // 63

        bf &= 0b110000;
        REQUIRE(bf.get_value() == 0b110000); // 48

        bf ^= 0b111111;
        REQUIRE(bf.get_value() == 0b001111); // 15

        bf <<= 2;
        // 15 << 2 = 60 (0b111100)，仍在 6 位范围内
        REQUIRE(bf.get_value() == 60);

        bf >>= 3;
        REQUIRE(bf.get_value() == 7);
    }

    SECTION("Binary bitwise operators") {
        REQUIRE((bf | 0b010101) == 0b111111);
        REQUIRE((bf & 0b100000) == 0b100000);
        REQUIRE((bf ^ 0b111111) == 0b010101);
        REQUIRE((bf >> 1) == 0b010101);

        // 测试按位取反 ~，注意会被提升为 int，我们需要手动截断或强转来验证
        uint8_t inverted = ~bf;
        REQUIRE((inverted & 0x3F) == 0b010101);
    }
}

// =========================================================================
// 比较操作与类型转换
// =========================================================================
TEST_CASE("bit_field_member comparisons and casting", "[endian][bitfield][comparison]") {
    mgpp::endian::bit_field_member<4, 4, mgpp::endian_t::little_byte, uint8_t> bf;
    bf.reset(8);

    SECTION("Comparisons") {
        REQUIRE(bf == 8);
        REQUIRE(bf != 7);
        REQUIRE(bf > 5);
        REQUIRE(bf < 10);
        REQUIRE(bf >= 8);
        REQUIRE(bf <= 8);
    }

    SECTION("Casting") {
        // explicit operator _Type()
        uint8_t val = static_cast<uint8_t>(bf);
        REQUIRE(val == 8);

        // explicit operator bool()
        REQUIRE(static_cast<bool>(bf) == true);
        bf.reset(0);
        REQUIRE(static_cast<bool>(bf) == false);

        // as<_Ty>()
        bf.reset(8);
        REQUIRE(bf.as<int>() == 8);
    }
}

// =========================================================================
// 全局操作符重载 (LHS is primitive)
// =========================================================================
TEST_CASE("bit_field_member global operators", "[endian][bitfield][global]") {
    mgpp::endian::bit_field_member<0, 8, mgpp::endian_t::little_byte, uint16_t> bf;
    bf.reset(10);

    SECTION("Arithmetic with primitive on Left-Hand Side") {
        REQUIRE((uint16_t(5) + bf) == 15);
        REQUIRE((uint16_t(20) - bf) == 10);
        REQUIRE((uint16_t(3) * bf) == 30);
        REQUIRE((uint16_t(100) / bf) == 10);
        REQUIRE((uint16_t(23) % bf) == 3);
    }

    SECTION("Bitwise with primitive on Left-Hand Side") {
        REQUIRE((uint16_t(0x0F) & bf) == 0x0A);
        REQUIRE((uint16_t(0xF0) | bf) == 0xFA);
        REQUIRE((uint16_t(0xFF) ^ bf) == 0xF5);
        REQUIRE((uint16_t(1) << bf) == 1024);
    }

    SECTION("Comparisons with primitive on Left-Hand Side") {
        REQUIRE((uint16_t(10) == bf));
        REQUIRE((uint16_t(5) != bf));
        REQUIRE((uint16_t(5) < bf));
        REQUIRE((uint16_t(20) > bf));
        REQUIRE((uint16_t(10) <= bf));
        REQUIRE((uint16_t(10) >= bf));
    }
}

#include <megopp/endian/arithmetic_member.h>

TEST_CASE("arithmetic_member inside union (Memory Layout & Endianness)", "[endian][union]") {
    // 定义一个包含 arithmetic_member 的 union
    union EndianTestUnion {
        mgpp::endian::arithmetic_member<uint32_t, mgpp::endian_t::little_byte> le_val;
        mgpp::endian::arithmetic_member<uint32_t, mgpp::endian_t::big_byte>    be_val;
        uint8_t raw[4];
    };

    EndianTestUnion u;

    SECTION("Little Endian Memory Layout") {
        // 注意：因为没有 operator=，必须使用 set_value
        u.le_val.set_value(0x12345678);

        REQUIRE(u.le_val.get_value() == 0x12345678);
        
        // 验证底层 uint8_t 数组的内存分布 (小端序：低字节在低地址)
        // 假设当前平台宏配置正确，little_byte 应该按照小端存储
#if MEGO_ENDIAN__LITTLE_BYTE
        REQUIRE(u.raw[0] == 0x78);
        REQUIRE(u.raw[1] == 0x56);
        REQUIRE(u.raw[2] == 0x34);
        REQUIRE(u.raw[3] == 0x12);
#endif
    }

    SECTION("Big Endian Memory Layout") {
        u.be_val.set_value(0x12345678);

        REQUIRE(u.be_val.get_value() == 0x12345678);
        
        // 验证底层 uint8_t 数组的内存分布 (大端序：高字节在低地址)
#if MEGO_ENDIAN__LITTLE_BYTE
        REQUIRE(u.raw[0] == 0x12);
        REQUIRE(u.raw[1] == 0x34);
        REQUIRE(u.raw[2] == 0x56);
        REQUIRE(u.raw[3] == 0x78);
#endif
    }
}

// =========================================================================
// 基本算术运算 (Integral)
// =========================================================================
TEST_CASE("arithmetic_member integral arithmetic operations", "[endian][arithmetic]") {
    mgpp::endian::arithmetic_member<int32_t, mgpp::endian_t::little_byte> a;
    a.set_value(10);

    SECTION("Compound assignment operators (+=, -=, *=, /=, %=)") {
        a += 5;  REQUIRE(a.get_value() == 15);
        a -= 3;  REQUIRE(a.get_value() == 12);
        a *= 2;  REQUIRE(a.get_value() == 24);
        a /= 4;  REQUIRE(a.get_value() == 6);
        a %= 4;  REQUIRE(a.get_value() == 2);
    }

    SECTION("Increment and Decrement (++, --)") {
        REQUIRE((a++).get_value() == 10); // 后置++
        REQUIRE(a.get_value() == 11);
        
        REQUIRE((++a).get_value() == 12); // 前置++

        REQUIRE((a--).get_value() == 12); // 后置--
        REQUIRE(a.get_value() == 11);

        REQUIRE((--a).get_value() == 10); // 前置--
    }

    SECTION("Binary operators (+, -, *, /, %)") {
        REQUIRE((a + 5) == 15);
        REQUIRE((a - 2) == 8);
        REQUIRE((a * 3) == 30);
        REQUIRE((a / 2) == 5);
        REQUIRE((a % 3) == 1);
    }
}

// =========================================================================
// 位运算 (Bitwise)
// =========================================================================
TEST_CASE("arithmetic_member bitwise operations", "[endian][bitwise]") {
    mgpp::endian::arithmetic_member<uint16_t, mgpp::endian_t::little_byte> a;
    a.set_value(0b1010); // 10

    SECTION("Compound bitwise operators (&=, |=, ^=, <<=, >>=)") {
        a |= 0b0101; REQUIRE(a.get_value() == 0b1111); // 15
        a &= 0b0011; REQUIRE(a.get_value() == 0b0011); // 3
        a ^= 0b1111; REQUIRE(a.get_value() == 0b1100); // 12
        a <<= 2;     REQUIRE(a.get_value() == 0b110000); // 48
        a >>= 1;     REQUIRE(a.get_value() == 0b011000); // 24
    }

    SECTION("Binary bitwise operators (&, |, ^, <<, >>, ~)") {
        REQUIRE((a | 0b0101) == 0b1111);
        REQUIRE((a & 0b1000) == 0b1000);
        REQUIRE((a ^ 0b1111) == 0b0101);
        REQUIRE((a << 2) == 40);
        REQUIRE((a >> 1) == 5);
        REQUIRE(static_cast<uint16_t>(~a) == static_cast<uint16_t>(~0b1010));
    }
}

// =========================================================================
// 浮点数运算与 fmod 支持
// =========================================================================
TEST_CASE("arithmetic_member floating point operations", "[endian][floating]") {
    mgpp::endian::arithmetic_member<double, mgpp::endian_t::little_byte> f;
    f.set_value(5.5);

    SECTION("Basic floating point arithmetic") {
        f += 2.0; 
        REQUIRE_THAT(f.get_value(), Catch::Matchers::WithinAbs(7.5, 1e-5));
        
        f -= 1.5;
        REQUIRE_THAT(f.get_value(), Catch::Matchers::WithinAbs(6.0, 1e-5));
        
        f *= 2.0;
        REQUIRE_THAT(f.get_value(), Catch::Matchers::WithinAbs(12.0, 1e-5));
        
        f /= 3.0;
        REQUIRE_THAT(f.get_value(), Catch::Matchers::WithinAbs(4.0, 1e-5));
    }

    SECTION("Floating point modulo (fmod)") {
        // 测试你代码中通过 SFINAE 区分的 fmod 逻辑
        f.set_value(5.3);
        f %= 2.0; 
        REQUIRE_THAT(f.get_value(), Catch::Matchers::WithinAbs(1.3, 1e-5));

        REQUIRE_THAT(f % 1.0, Catch::Matchers::WithinAbs(0.3, 1e-5));
    }
}

// =========================================================================
// 比较操作符与类型转换
// =========================================================================
TEST_CASE("arithmetic_member comparisons and casting", "[endian][comparison]") {
    mgpp::endian::arithmetic_member<int, mgpp::endian_t::little_byte> a;
    a.set_value(100);

    SECTION("Comparisons against primitive types") {
        REQUIRE(a == 100);
        REQUIRE(a != 50);
        REQUIRE(a > 50);
        REQUIRE(a < 200);
        REQUIRE(a >= 100);
        REQUIRE(a <= 100);
    }

    SECTION("Comparisons between arithmetic_members") {
        mgpp::endian::arithmetic_member<int, mgpp::endian_t::big_byte> b;
        b.set_value(100);
        
        mgpp::endian::arithmetic_member<int, mgpp::endian_t::little_byte> c;
        c.set_value(50);

        // 跨端序比较 (测试 operator== 模板)
        REQUIRE(a == b);
        REQUIRE(a != c);
    }

    SECTION("Type casting operators") {
        // 测试 explicit operator type()
        int raw_val = static_cast<int>(a);
        REQUIRE(raw_val == 100);

        // 测试 explicit operator bool()
        REQUIRE(static_cast<bool>(a) == true);
        
        mgpp::endian::arithmetic_member<int, mgpp::endian_t::little_byte> zero;
        zero.set_value(0);
        REQUIRE(static_cast<bool>(zero) == false);

        // 测试 as<_Result>()
        REQUIRE(a.as<double>() == 100.0);
    }
}

// =========================================================================
// 全局操作符重载 (Primitive + arithmetic_member)
// =========================================================================
TEST_CASE("arithmetic_member global operators (LHS primitive)", "[endian][global]") {
    mgpp::endian::arithmetic_member<int, mgpp::endian_t::little_byte> a;
    a.set_value(10);

    REQUIRE((5 + a) == 15);
    REQUIRE((20 - a) == 10);
    REQUIRE((3 * a) == 30);
    REQUIRE((100 / a) == 10);
    REQUIRE((23 % a) == 3);
    
    REQUIRE((10 == a));
    REQUIRE((5 != a));
    REQUIRE((5 < a));
    REQUIRE((20 > a));
    REQUIRE((10 <= a));
    REQUIRE((10 >= a));
}
