
#ifndef MEGO_NUMERIC_TYPES_H_INCLUDED
#define MEGO_NUMERIC_TYPES_H_INCLUDED

//! @enum mg_int_neg_mode_e
//! @brief Integer negative mode enumeration.
//!
//! This enumeration defines different modes for representing negative integers.
//! It provides two options:
//!
//! - `mg_int_neg_mode__standard`: Represents the standard method of handling negative integers (value 0).
//! - `mg_int_neg_mode__sign_bit`: Represents an alternative method of handling negative integers using the sign bit (value 1).
typedef enum mg_int_neg_mode_e
{
    mg_int_neg_mode__standard = 0, //!< Standard mode.
    mg_int_neg_mode__sign_bit = 1, //!< Sign bit mode, the highest bit is the sign bit.
} mg_int_neg_mode_e;

#endif // !MEGO_NUMERIC_TYPES_H_INCLUDED
