
#ifndef MEGO_NUMERIC_TYPES_H_INCLUDED
#define MEGO_NUMERIC_TYPES_H_INCLUDED

//! @enum mg_int_neg_mode_e
//! @brief 整数负数模式枚举。
//!
//! 该枚举定义了表示负整数的不同模式。
//! 它提供了两种选择：
//!
//! - `mg_int_neg_mode__standard`: 表示处理负整数的标准方法（值为0）。
//! - `mg_int_neg_mode__sign_bit`: 表示使用符号位处理负整数的替代方法（值为1）。
enum mg_int_neg_mode_e
{
    mg_int_neg_mode__standard = 0, ///< 标准模式。
    mg_int_neg_mode__sign_bit = 1, ///< 符号位模式，最高位为符号位。
};

#endif // !MEGO_NUMERIC_TYPES_H_INCLUDED
