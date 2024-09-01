
#include <catch2/catch.hpp>
#include <megopp/endian/bit_field_member.h>

TEST_CASE("bit_field_member", "[bit_field_member]")
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
    } u_1_2;
    u_1_2.v = 0x0650;
    REQUIRE(u_1_2.bfm == 0);
    u_1_2.bfm += 3;
    REQUIRE(u_1_2.bfm == 3);
    REQUIRE(u_1_2.v == 0xC650);
    
    REQUIRE(u_1_2.bfm * 2 == 6);
    REQUIRE(u_1_2.bfm / 2 == 1);
    REQUIRE(u_1_2.bfm + 2 == 5);
    REQUIRE(u_1_2.bfm - 2 == 1);
    REQUIRE(u_1_2.bfm & 2 == 2);
    REQUIRE(u_1_2.bfm | 2 == 3);
    REQUIRE(u_1_2.bfm ^ 2 == 1);
    REQUIRE(~u_1_2.bfm == 0xFFFC);
    REQUIRE(u_1_2.bfm << 2 == 12);
    REQUIRE(u_1_2.bfm >> 2 == 0);
    REQUIRE(!u_1_2.bfm == false);
    
    REQUIRE(u_1_2.bfm++ == 3);
    REQUIRE(u_1_2.bfm == 4);
    REQUIRE(u_1_2.v == 0x0651);
    REQUIRE(u_1_2.bfm-- == 4);
    REQUIRE(u_1_2.bfm == 3);
    REQUIRE(u_1_2.v == 0xC650);

    REQUIRE((u_1_2.bfm *= 2) == 6);
    REQUIRE(u_1_2.v == 0x8651);
    REQUIRE((u_1_2.bfm /= 2) == 3);
    REQUIRE(u_1_2.v == 0xC650);
    REQUIRE((u_1_2.bfm += 2) == 5);
    REQUIRE(u_1_2.v == 0x4651);
    REQUIRE((u_1_2.bfm -= 2) == 3);
    REQUIRE(u_1_2.v == 0xC650);
    REQUIRE((u_1_2.bfm &= 2) == 2);
    REQUIRE(u_1_2.v == 0x8650);
    REQUIRE((u_1_2.bfm |= 1) == 3);
    REQUIRE(u_1_2.v == 0xC650);
    
    REQUIRE((u_1_2.bfm *= 6) == 2);
    REQUIRE(u_1_2.v == 0x8650);
    REQUIRE((u_1_2.bfm += 15) == 1);
    REQUIRE(u_1_2.v == 0x4650);
    REQUIRE((u_1_2.bfm /= 10) == 0);
    REQUIRE(u_1_2.v == 0x0650);
    REQUIRE((u_1_2.bfm -= 1) == 0xF);
    REQUIRE(u_1_2.v == 0xC653);

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
