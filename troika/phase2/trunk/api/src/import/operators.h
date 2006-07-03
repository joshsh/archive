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
#define OP_PLUS(type)  type type##__plus( type a ) { return +a; }

/*
INDICATION Minus_Indication: Minus_Op;
OPER Plus_Op, Minus_Op(TypeIs_ArithPromoted): TypeIs_ArithPromoted;
*/
#define OP_MINUS(type)  type type##__minus( type a ) { return -a; }

/*
INDICATION Bitwise_Not_Indication: Bitwise_Not_Op;
OPER Bitwise_Not_Op(TypeIs_IntegralPromoted): TypeIs_IntegralPromoted;
*/
#define OP_BITWISE_NOT(type)  type type##__bitwise_not( type a ) { return ~a; }

/*
INDICATION Not_Indication: Not_Op, Void_Ptr_Not_Op, Ptr_Not_Op;
OPER Not_Op(TypeIs_Scalar): TypeIs_int;
*/
#define OP_NOT(type)  int type##__not( type a ) { return !a; }


/* 6.3.4 Cast operators *******************************************************/

/*
SET TypeIs_CastResult = TypeIs_Scalar;
OPER Cast_Op(TypeIs_Scalar): TypeIs_CastResult;
INDICATION Cast_Indication: Cast_Op, Cast_IntegraltoPtr, Cast_VoidPtrtoPtr;
*/
#define OP_CAST(type, totype)  totype type##__cast_to__##totype( type a ) { return ( totype ) a; }


/* 6.3.5 Multiplicative operators *********************************************/

/*
INDICATION Multiplication_Indication: MulOp;
OPER MulOp, DivOp(TypeIs_Arithmetic, TypeIs_Arithmetic): TypeIs_Arithmetic;
*/
#define OP_MUL(type)  type type##_##type##__mul( type a, type b ) { return a * b; }

/*
INDICATION Division_Indication: DivOp;
OPER MulOp, DivOp(TypeIs_Arithmetic, TypeIs_Arithmetic): TypeIs_Arithmetic;
*/
/* TODO: check for division by zero */
#define OP_DIV(type)  type type##_##type##__div( type a, type b ) { return a / b; }

/*
INDICATION Mod_Indication: ModOp;
OPER ModOp(TypeIs_Integral, TypeIs_Integral): TypeIs_Integral;
*/
/* TODO: check for mod by zero */
#define OP_MOD(type)  type type##_##type##__mod( type a, type b ) { return a % b; }


/* 6.3.6 Additive operators ***************************************************/

/*
INDICATION Addition_Indication:
  AddOp, Void_Ptr_Add_Op, Void_Ptr_Rev_Add_Op, Ptr_Add_Op, Ptr_Rev_Add_Op;
OPER AddOp, SubOp(TypeIs_Arithmetic, TypeIs_Arithmetic): TypeIs_Arithmetic;
*/
#define OP_ADD(type)  type type##_##type##__add( type a, type b ) { return a + b; }

/*
INDICATION Subtraction_Indication:
  SubOp, Void_Ptr_Sub_Op, Ptr_Sub_Op, Ptr_Ptr_Sub_Op;
OPER AddOp, SubOp(TypeIs_Arithmetic, TypeIs_Arithmetic): TypeIs_Arithmetic;
*/
#define OP_SUB(type)  type type##_##type##__sub( type a, type b ) { return a - b; }


/* 6.3.7 Bitwise shift operators -- no equivalent *****************************/


/* 6.3.8 Relational operators *************************************************/

/*
INDICATION LessThan_Indication: LessThan_Op, Ptr_LT_Op;
OPER Greater_Op, LessThan_Op, Greater_Equal_Op, LessThan_Equal_Op
  (TypeIs_Scalar, TypeIs_Scalar): TypeIs_int;
*/
#define OP_LESSTHAN(type)  int type##_##type##__lessthan( type a, type b ) { return a < b; }

/*
INDICATION Greater_Indication: Greater_Op, Ptr_GT_Op;
OPER Greater_Op, LessThan_Op, Greater_Equal_Op, LessThan_Equal_Op
  (TypeIs_Scalar, TypeIs_Scalar): TypeIs_int;
*/
#define OP_GREATER(type)  int type##_##type##__greater( type a, type b ) { return a > b; }

/*
INDICATION LessThan_Equal_Indication: LessThan_Equal_Op, Ptr_LTE_Op;
OPER Greater_Op, LessThan_Op, Greater_Equal_Op, LessThan_Equal_Op
  (TypeIs_Scalar, TypeIs_Scalar): TypeIs_int;
*/
#define OP_LESSTHAN_EQUAL(type)  int type##_##type##__lessthan_equal( type a, type b ) { return a <= b; }

/*
INDICATION Greater_Equal_Indication: Greater_Equal_Op, Ptr_GTE_Op;
OPER Greater_Op, LessThan_Op, Greater_Equal_Op, LessThan_Equal_Op
  (TypeIs_Scalar, TypeIs_Scalar): TypeIs_int;
*/
#define OP_GREATER_EQUAL(type)  int type##_##type##__greater_equal( type a, type b ) { return a >= b; }


/* 6.3.9 Equality operators ***************************************************/

/*
INDICATION Equality_Indication: Equality_Op, Ptr_Eq_Op;
OPER Equality_Op, Not_Equal_Op(TypeIs_Scalar, TypeIs_Scalar): TypeIs_int;
*/
#define OP_EQUALITY(type)  int type##_##type##__equality( type a, type b ) { return a == b; }

/*
INDICATION Not_Equal_Indication: Not_Equal_Op, Ptr_NEq_Op;
OPER Equality_Op, Not_Equal_Op(TypeIs_Scalar, TypeIs_Scalar): TypeIs_int;
*/
#define OP_NOT_EQUAL(type)  int type##_##type##__not_equal( type a, type b ) { return a != b; }


/* 6.3.10 Bitwise AND operator ************************************************/

/*
INDICATION Bitwise_And_Indication: Bitwise_And_Op;
OPER Bitwise_And_Op(TypeIs_Integral, TypeIs_Integral): TypeIs_Integral;
*/
#define OP_BITWISE_AND(type)  type type##_##type##__bitwise_and( type a, type b ) { return a & b; }


/* 6.3.11 Bitwise exclusive OR operator ***************************************/

/*
INDICATION Bitwise_XOr_Indication: Bitwise_XOr_Op;
OPER Bitwise_XOr_Op(TypeIs_Integral, TypeIs_Integral): TypeIs_Integral;
*/
#define OP_BITWISE_XOR(type)  type type##_##type##__bitwise_xor( type a, type b ) { return a ^ b; }


/* 6.3.12 Bitwise inclusive OR operator ***************************************/

/*
INDICATION Bitwise_Or_Indication: Bitwise_Or_Op;
OPER Bitwise_Or_Op(TypeIs_Integral, TypeIs_Integral): TypeIs_Integral;
*/
#define OP_BITWISE_OR(type)  type type##_##type##__bitwise_or( type a, type b ) { return a | b; }


/* 6.3.13 Logical AND operator ************************************************/

/*
INDICATION And_Indication: And_Op, Ptr_And_Op;
OPER And_Op(TypeIs_Scalar, TypeIs_Scalar): TypeIs_int;
*/
#define OP_AND(type)  int type##_##type##__and( type a, type b ) { return a && b; }


/* 6.3.14 Logical OR operator *************************************************/

/*
INDICATION Or_Indication: Or_Op, Ptr_Or_Op;  
OPER Or_Op(TypeIs_Scalar, TypeIs_Scalar): TypeIs_int;
*/
#define OP_OR(type)  int type##_##type##__or( type a, type b ) { return a || b; }


/* 6.3.15 Conditional operator -- no equivalent *******************************/


/* 6.3.16 Assignment operators -- no equivalent ******************************/


/* 6.3.16.2 Compound assignment -- no equivalent ******************************/


/* 6.3.17 Comma operator -- no equivalent *************************************/

