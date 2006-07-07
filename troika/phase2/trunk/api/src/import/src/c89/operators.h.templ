/*******************************************************************************

Phase2 language API, Copyright (C) 2006 Joshua Shinavier.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA

*******************************************************************************/

/*
Phase2 has no pointer types (except for the hidden type indirection_type), so
it has no operators which consume or produce pointer values.

The same is true of structure, union, enumeration and array types.

Arithmetic operators with side-effects, namely assignment, increment/decrement
and bit shift operators, have no equivalent in Phase2.  Syntactic sugar such as
the conditional and comma operators are absent as well.
*/


/* 6.3.2.1 Array subscripting -- no equivalent ********************************/


/* 6.3.2.2 Function calls -- no equivalent ************************************/


/* 6.3.2.3 Postfix increment and decrement operators -- no equivalent *********/


/* 6.3.3.2 Address and indirection operators -- no equivalent *****************/


/* 6.3.3.3 Unary arithmetic operators *****************************************/

/*
INDICATION Plus_Indication: Plus_Op;
OPER Plus_Op, Minus_Op(TypeIs_ArithPromoted): TypeIs_ArithPromoted;
*/
#define OP_PLUS_DEC(type)           type type##__plus( type a )
#define OP_PLUS(type)               OP_PLUS_DEC(type) { return +a; }

/*
INDICATION Minus_Indication: Minus_Op;
OPER Plus_Op, Minus_Op(TypeIs_ArithPromoted): TypeIs_ArithPromoted;
*/
#define OP_MINUS_DEC(type)          type type##__minus( type a )
#define OP_MINUS(type)              OP_MINUS_DEC(type) { return -a; }

/*
INDICATION Bitwise_Not_Indication: Bitwise_Not_Op;
OPER Bitwise_Not_Op(TypeIs_IntegralPromoted): TypeIs_IntegralPromoted;
*/
#define OP_BITWISE_NOT_DEC(type)    type type##__bitwise_not( type a )
#define OP_BITWISE_NOT(type)        OP_BITWISE_NOT_DEC(type) { return ~a; }

/*
INDICATION Not_Indication: Not_Op, Void_Ptr_Not_Op, Ptr_Not_Op;
OPER Not_Op(TypeIs_Scalar): TypeIs_int;
*/
#define OP_NOT_DEC(type)            int type##__not( type a )
#define OP_NOT(type)                OP_NOT_DEC(type) { return !a; }


/* 6.3.4 Cast operators *******************************************************/

/*
SET TypeIs_CastResult = TypeIs_Scalar;
OPER Cast_Op(TypeIs_Scalar): TypeIs_CastResult;
INDICATION Cast_Indication: Cast_Op, Cast_IntegraltoPtr, Cast_VoidPtrtoPtr;
*/
#define OP_CAST_DEC(type, totype)   totype type##__cast_to__##totype( type a )
#define OP_CAST(type, totype)       OP_CAST_DEC(type, totype) { return ( totype ) a; }


/* 6.3.5 Multiplicative operators *********************************************/

/*
INDICATION Multiplication_Indication: MulOp;
OPER MulOp, DivOp(TypeIs_Arithmetic, TypeIs_Arithmetic): TypeIs_Arithmetic;
*/
#define OP_MUL_DEC(type)            type type##_##type##__mul( type a, type b )
#define OP_MUL(type)                OP_MUL_DEC(type) { return a * b; }

/*
INDICATION Division_Indication: DivOp;
OPER MulOp, DivOp(TypeIs_Arithmetic, TypeIs_Arithmetic): TypeIs_Arithmetic;
*/
/* TODO: check for division by zero */
#define OP_DIV_DEC(type)            type type##_##type##__div( type a, type b )
#define OP_DIV(type)                OP_DIV_DEC(type) { return a / b; }

/*
INDICATION Mod_Indication: ModOp;
OPER ModOp(TypeIs_Integral, TypeIs_Integral): TypeIs_Integral;
*/
/* TODO: check for mod by zero */
#define OP_MOD_DEC(type)            type type##_##type##__mod( type a, type b )
#define OP_MOD(type)                OP_MOD_DEC(type) { return a % b; }


/* 6.3.6 Additive operators ***************************************************/

/*
INDICATION Addition_Indication:
  AddOp, Void_Ptr_Add_Op, Void_Ptr_Rev_Add_Op, Ptr_Add_Op, Ptr_Rev_Add_Op;
OPER AddOp, SubOp(TypeIs_Arithmetic, TypeIs_Arithmetic): TypeIs_Arithmetic;
*/
#define OP_ADD_DEC(type)            type type##_##type##__add( type a, type b )
#define OP_ADD(type)                OP_ADD_DEC(type) { return a + b; }

/*
INDICATION Subtraction_Indication:
  SubOp, Void_Ptr_Sub_Op, Ptr_Sub_Op, Ptr_Ptr_Sub_Op;
OPER AddOp, SubOp(TypeIs_Arithmetic, TypeIs_Arithmetic): TypeIs_Arithmetic;
*/
#define OP_SUB_DEC(type)            type type##_##type##__sub( type a, type b )
#define OP_SUB(type)                OP_SUB_DEC(type) { return a - b; }


/* 6.3.7 Bitwise shift operators -- no equivalent *****************************/


/* 6.3.8 Relational operators *************************************************/

/*
INDICATION LessThan_Indication: LessThan_Op, Ptr_LT_Op;
OPER Greater_Op, LessThan_Op, Greater_Equal_Op, LessThan_Equal_Op
  (TypeIs_Scalar, TypeIs_Scalar): TypeIs_int;
*/
#define OP_LESSTHAN_DEC(type)       int type##_##type##__lessthan( type a, type b )
#define OP_LESSTHAN(type)           OP_LESSTHAN_DEC(type) { return a < b; }

/*
INDICATION Greater_Indication: Greater_Op, Ptr_GT_Op;
OPER Greater_Op, LessThan_Op, Greater_Equal_Op, LessThan_Equal_Op
  (TypeIs_Scalar, TypeIs_Scalar): TypeIs_int;
*/
#define OP_GREATER_DEC(type)        int type##_##type##__greater( type a, type b )
#define OP_GREATER(type)            OP_GREATER_DEC(type) { return a > b; }

/*
INDICATION LessThan_Equal_Indication: LessThan_Equal_Op, Ptr_LTE_Op;
OPER Greater_Op, LessThan_Op, Greater_Equal_Op, LessThan_Equal_Op
  (TypeIs_Scalar, TypeIs_Scalar): TypeIs_int;
*/
#define OP_LESSTHAN_EQUAL_DEC(type) int type##_##type##__lessthan_equal( type a, type b )
#define OP_LESSTHAN_EQUAL(type)     OP_LESSTHAN_EQUAL_DEC(type) { return a <= b; }

/*
INDICATION Greater_Equal_Indication: Greater_Equal_Op, Ptr_GTE_Op;
OPER Greater_Op, LessThan_Op, Greater_Equal_Op, LessThan_Equal_Op
  (TypeIs_Scalar, TypeIs_Scalar): TypeIs_int;
*/
#define OP_GREATER_EQUAL_DEC(type)  int type##_##type##__greater_equal( type a, type b )
#define OP_GREATER_EQUAL(type)      OP_GREATER_EQUAL_DEC(type) { return a >= b; }


/* 6.3.9 Equality operators ***************************************************/

/*
INDICATION Equality_Indication: Equality_Op, Ptr_Eq_Op;
OPER Equality_Op, Not_Equal_Op(TypeIs_Scalar, TypeIs_Scalar): TypeIs_int;
*/
#define OP_EQUALITY_DEC(type)       int type##_##type##__equality( type a, type b )
#define OP_EQUALITY(type)           OP_EQUALITY_DEC(type) { return a == b; }

/*
INDICATION Not_Equal_Indication: Not_Equal_Op, Ptr_NEq_Op;
OPER Equality_Op, Not_Equal_Op(TypeIs_Scalar, TypeIs_Scalar): TypeIs_int;
*/
#define OP_NOT_EQUAL_DEC(type)      int type##_##type##__not_equal( type a, type b )
#define OP_NOT_EQUAL(type)          OP_NOT_EQUAL_DEC(type) { return a != b; }


/* 6.3.10 Bitwise AND operator ************************************************/

/*
INDICATION Bitwise_And_Indication: Bitwise_And_Op;
OPER Bitwise_And_Op(TypeIs_Integral, TypeIs_Integral): TypeIs_Integral;
*/
#define OP_BITWISE_AND_DEC(type)    type type##_##type##__bitwise_and( type a, type b )
#define OP_BITWISE_AND(type)        OP_BITWISE_AND_DEC(type) { return a & b; }


/* 6.3.11 Bitwise exclusive OR operator ***************************************/

/*
INDICATION Bitwise_XOr_Indication: Bitwise_XOr_Op;
OPER Bitwise_XOr_Op(TypeIs_Integral, TypeIs_Integral): TypeIs_Integral;
*/
#define OP_BITWISE_XOR_DEC(type)    type type##_##type##__bitwise_xor( type a, type b )
#define OP_BITWISE_XOR(type)        OP_BITWISE_XOR_DEC(type) { return a ^ b; }


/* 6.3.12 Bitwise inclusive OR operator ***************************************/

/*
INDICATION Bitwise_Or_Indication: Bitwise_Or_Op;
OPER Bitwise_Or_Op(TypeIs_Integral, TypeIs_Integral): TypeIs_Integral;
*/
#define OP_BITWISE_OR_DEC(type)     type type##_##type##__bitwise_or( type a, type b )
#define OP_BITWISE_OR(type)         OP_BITWISE_OR_DEC(type) { return a | b; }


/* 6.3.13 Logical AND operator ************************************************/

/*
INDICATION And_Indication: And_Op, Ptr_And_Op;
OPER And_Op(TypeIs_Scalar, TypeIs_Scalar): TypeIs_int;
*/
#define OP_AND_DEC(type)            int type##_##type##__and( type a, type b )
#define OP_AND(type)                OP_AND_DEC(type) { return a && b; }


/* 6.3.14 Logical OR operator *************************************************/

/*
INDICATION Or_Indication: Or_Op, Ptr_Or_Op;  
OPER Or_Op(TypeIs_Scalar, TypeIs_Scalar): TypeIs_int;
*/
#define OP_OR_DEC(type)             int type##_##type##__or( type a, type b )
#define OP_OR(type)                 OP_OR_DEC(type) { return a || b; }


/* 6.3.15 Conditional operator -- no equivalent *******************************/


/* 6.3.16 Assignment operators -- no equivalent ******************************/


/* 6.3.16.2 Compound assignment -- no equivalent ******************************/


/* 6.3.17 Comma operator -- no equivalent *************************************/


/******************************************************************************/


/*
SET TypeIs_IntegralPromoted=
  [TypeIs_int, TypeIs_unsigned_int, TypeIs_long, TypeIs_unsigned_long];
SET TypeIs_ArithPromoted = TypeIs_IntegralPromoted + TypeIs_Floating;

SET TypeIs_Scalar = TypeIs_Arithmetic + [TypeIs_VoidPointer];
SET TypeIs_Arithmetic = TypeIs_Integral + TypeIs_Floating;
SET TypeIs_Integral=
  [TypeIs_char] + TypeIs_Signed_Integer + TypeIs_Unsigned_Integer;

SET TypeIs_Floating=
  [TypeIs_float, TypeIs_double, TypeIs_long_double];


All C imported types in Phase2 = {char, double, int}
TypeIs_Arithmetic = {char, double, int}
TypeIs_ArithPromoted = {double, int}
TypeIs_Floating = {double}
TypeIs_Integral = {char, int}
TypeIs_IntegralPromoted = {int}
TypeIs_Scalar = {char, double, int}
*/

OP_PLUS_DEC( double );
OP_PLUS_DEC( int );

OP_MINUS_DEC( double );
OP_MINUS_DEC( int );

OP_BITWISE_NOT_DEC( int );

OP_NOT_DEC( char );
OP_NOT_DEC( double );
OP_NOT_DEC( int );

OP_CAST_DEC( char, double );
OP_CAST_DEC( char, int );
OP_CAST_DEC( double, char );
OP_CAST_DEC( double, int );
OP_CAST_DEC( int, char );
OP_CAST_DEC( int, double );

OP_MUL_DEC( char );
OP_MUL_DEC( double );
OP_MUL_DEC( int );

OP_DIV_DEC( char );
OP_DIV_DEC( double );
OP_DIV_DEC( int );

OP_MOD_DEC( char );
OP_MOD_DEC( int );

OP_ADD_DEC( char );
OP_ADD_DEC( double );
OP_ADD_DEC( int );

OP_SUB_DEC( char );
OP_SUB_DEC( double );
OP_SUB_DEC( int );

OP_LESSTHAN_DEC( char );
OP_LESSTHAN_DEC( double );
OP_LESSTHAN_DEC( int );

OP_GREATER_DEC( char );
OP_GREATER_DEC( double );
OP_GREATER_DEC( int );

OP_LESSTHAN_EQUAL_DEC( char );
OP_LESSTHAN_EQUAL_DEC( double );
OP_LESSTHAN_EQUAL_DEC( int );

OP_GREATER_EQUAL_DEC( char );
OP_GREATER_EQUAL_DEC( double );
OP_GREATER_EQUAL_DEC( int );

OP_EQUALITY_DEC( char );
OP_EQUALITY_DEC( double );
OP_EQUALITY_DEC( int );

OP_NOT_EQUAL_DEC( char );
OP_NOT_EQUAL_DEC( double );
OP_NOT_EQUAL_DEC( int );

OP_BITWISE_AND_DEC( char );
OP_BITWISE_AND_DEC( int );

OP_BITWISE_XOR_DEC( char );
OP_BITWISE_XOR_DEC( int );

OP_BITWISE_OR_DEC( char );
OP_BITWISE_OR_DEC( int );

OP_AND_DEC( char );
OP_AND_DEC( double );
OP_AND_DEC( int );

OP_OR_DEC( char );
OP_OR_DEC( double );
OP_OR_DEC( int );


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */
