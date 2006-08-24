/* This file is automatically generated by the importing utility.  See import.txt. */

/* Interface includes for basic data types */
#include "itf_cpp/data_type_itf.h"
#include "itf_cpp/cpp_primitive_itf.h"
#include "itf_cpp/cstring_itf.h"
#include "itf_cpp/bool_itf.h"
#include "itf_cpp/char_itf.h"
#include "itf_cpp/int_itf.h"
#include "itf_cpp/double_itf.h"
#include "itf_cpp/color_itf.h"
#include "itf_cpp/SDL_Rect_itf.h"
#include "itf_cpp/SDL_Surface_itf.h"

/* Interface includes for preserializing data types */
#include "itf_cpp/key_map_itf.h"
#include "itf_cpp/bitmap_itf.h"
#include "itf_cpp/GUI_itf.h"
#include "itf_cpp/array_itf.h"
#include "itf_cpp/associative_array_itf.h"
#include "itf_cpp/expression_itf.h"
#include "itf_cpp/lookup_table_itf.h"
#include "itf_cpp/ternary_associative_array_itf.h"

/* Interface includes for primitives */
#include "snpl.h"
#include "miscprims.h"
#include "miscprims.h"
#include "snpl.h"
#include "snpl.h"
#include "miscprims.h"
#include "miscprims.h"
#include "miscprims.h"
#include "miscprims.h"
#include "snpl.h"
#include "snpl.h"
#include "miscprims.h"
#include "miscprims.h"
#include "miscprims.h"
#include "itf_cpp/char_itf.h"
#include "miscprims.h"
#include "itf_cpp/double_itf.h"
#include "itf_cpp/double_itf.h"
#include "itf_cpp/double_itf.h"
#include "itf_cpp/double_itf.h"
#include "itf_cpp/int_itf.h"
#include "itf_cpp/int_itf.h"
#include "itf_cpp/int_itf.h"
#include "itf_cpp/int_itf.h"
#include "itf_cpp/int_itf.h"
#include "miscprims.h"
#include "miscprims.h"
#include "miscprims.h"
#include "miscprims.h"
#include "miscprims.h"
#include "miscprims.h"
#include "miscprims.h"
#include "miscprims.h"
#include "miscprims.h"
#include "miscprims.h"
#include "miscprims.h"
#include "miscprims.h"
#include "SDL/SDL_image.h"
#include "type/key_map.h"
#include "type/key_map.h"
#include "type/key_map.h"
#include "type/color.h"
#include "SDLprims.h"
#include "SDLprims.h"
#include "SDLprims.h"
#include "SDLprims.h"
#include "type/gui.h"
#include "type/gui.h"
#include "type/gui.h"
#include "type/gui.h"
#include "type/gui.h"
#include "type/gui.h"
#include "type/gui.h"
#include "type/gui.h"
#include "type/gui.h"
#include "type/gui.h"
#include "type/gui.h"
#include "type/gui.h"
#include "type/gui.h"
#include "type/gui.h"
#include "type/gui.h"
#include "itf_cpp/lookup_table_itf.h"
#include "itf_cpp/lookup_table_itf.h"
#include "itf_cpp/lookup_table_itf.h"
#include "../SParse0/sparse.h"
#include "../SParse0/sparse.h"
#include "../SParse0/sparse.h"
#include "../SParse0/sparse.h"



void SNPL_construct_types()
{
  /* Basic data types */

  SNPL_register_type(
    SNPL_data_type_constructor(
      "data_type",
      SNPL_data_type_serializer,
      SNPL_data_type_deserializing_constructor,
      SNPL_data_type_destructor,
      NULL));

  SNPL_register_type(
    SNPL_data_type_constructor(
      "cpp_primitive",
      SNPL_cpp_primitive_serializer,
      SNPL_cpp_primitive_deserializing_constructor,
      SNPL_cpp_primitive_destructor,
      NULL));

  SNPL_register_type(
    SNPL_data_type_constructor(
      "cstring",
      SNPL_cstring_serializer,
      SNPL_cstring_deserializing_constructor,
      SNPL_cstring_destructor,
      NULL));

  SNPL_register_type(
    SNPL_data_type_constructor(
      "bool",
      SNPL_bool_serializer,
      SNPL_bool_deserializing_constructor,
      SNPL_bool_destructor,
      NULL));

  SNPL_register_type(
    SNPL_data_type_constructor(
      "char",
      SNPL_char_serializer,
      SNPL_char_deserializing_constructor,
      SNPL_char_destructor,
      NULL));

  SNPL_register_type(
    SNPL_data_type_constructor(
      "int",
      SNPL_int_serializer,
      SNPL_int_deserializing_constructor,
      SNPL_int_destructor,
      NULL));

  SNPL_register_type(
    SNPL_data_type_constructor(
      "double",
      SNPL_double_serializer,
      SNPL_double_deserializing_constructor,
      SNPL_double_destructor,
      NULL));

  SNPL_register_type(
    SNPL_data_type_constructor(
      "color",
      SNPL_color_serializer,
      SNPL_color_deserializing_constructor,
      SNPL_color_destructor,
      NULL));

  SNPL_register_type(
    SNPL_data_type_constructor(
      "SDL_Rect",
      SDL_Rect_serializer,
      SDL_Rect_deserializing_constructor,
      SDL_Rect_destructor,
      NULL));

  SNPL_register_type(
    SNPL_data_type_constructor(
      "SDL_Surface",
      SDL_Surface_serializer,
      SDL_Surface_deserializing_constructor,
      SDL_Surface_destructor,
      NULL));

  /* Preserializing data types */

  SNPL_register_type(
    SNPL_data_type_constructor(
      "key_map",
      SNPL_key_map_serializer,
      SNPL_key_map_deserializing_constructor,
      SNPL_key_map_destructor,
      SNPL_key_map_preserializer));

  SNPL_register_type(
    SNPL_data_type_constructor(
      "bitmap",
      SNPL_bitmap_serializer,
      SNPL_bitmap_deserializing_constructor,
      SNPL_bitmap_destructor,
      SNPL_bitmap_preserializer));

  SNPL_register_type(
    SNPL_data_type_constructor(
      "GUI",
      SNPL_GUI_serializer,
      SNPL_GUI_deserializing_constructor,
      SNPL_GUI_destructor,
      SNPL_GUI_preserializer));

  SNPL_register_type(
    SNPL_data_type_constructor(
      "array",
      SNPL_array_serializer,
      SNPL_array_deserializing_constructor,
      SNPL_array_destructor,
      SNPL_array_preserializer));

  SNPL_register_type(
    SNPL_data_type_constructor(
      "associative_array",
      SNPL_associative_array_serializer,
      SNPL_associative_array_deserializing_constructor,
      SNPL_associative_array_destructor,
      SNPL_associative_array_preserializer));

  SNPL_register_type(
    SNPL_data_type_constructor(
      "expression",
      SNPL_expression_serializer,
      SNPL_expression_deserializing_constructor,
      SNPL_expression_destructor,
      SNPL_expression_preserializer));

  SNPL_register_type(
    SNPL_data_type_constructor(
      "lookup_table",
      SNPL_lookup_table_serializer,
      SNPL_lookup_table_deserializing_constructor,
      SNPL_lookup_table_destructor,
      SNPL_lookup_table_preserializer));

  SNPL_register_type(
    SNPL_data_type_constructor(
      "ternary_associative_array",
      SNPL_ternary_associative_array_serializer,
      SNPL_ternary_associative_array_deserializing_constructor,
      SNPL_ternary_associative_array_destructor,
      SNPL_ternary_associative_array_preserializer));

  SNPL_register_all_types();}



void SNPL_construct_primitives()
{
  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "void scratch(void)",
      (void (*)()) SNPL_scratch));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "int C_&(void)",
      (void (*)()) C_address_operator));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "char C_*(int)",
      (void (*)()) C_indirection_operator));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "associate(void, void, void)",
      (void (*)()) SNPL_associate));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "data_type ?(void)",
      (void (*)()) SNPL_find_type));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "cstring new_cstring(int)",
      (void (*)()) new_cstring));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "println(cstring)",
      (void (*)()) println));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "printstr(cstring)",
      (void (*)()) printstr));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "printchar(char)",
      (void (*)()) printchar));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "strcat(cstring, cstring)",
      (void (*)()) strcat));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "strcpy(cstring, cstring)",
      (void (*)()) strcpy));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "int strlen(cstring)",
      (void (*)()) itf_strlen));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "charcat(cstring, char)",
      (void (*)()) charcat));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "capitalize(cstring)",
      (void (*)()) capitalize));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "char_print(char)",
      (void (*)()) SNPL_char_print));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "char int_to_char(int)",
      (void (*)()) int_to_char));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "double double_add(double, double)",
      (void (*)()) double_add));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "double double_subtract(double, double)",
      (void (*)()) double_subtract));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "double double_multiply(double, double)",
      (void (*)()) double_multiply));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "double double_divide(double, double)",
      (void (*)()) double_divide));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "int int_add(int, int)",
      (void (*)()) int_add));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "int int_subtract(int, int)",
      (void (*)()) int_subtract));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "int int_multiply(int, int)",
      (void (*)()) int_multiply));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "int int_divide(int, int)",
      (void (*)()) int_divide));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "int int_mod(int, int)",
      (void (*)()) int_mod));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "bool int_greaterthan(int, int)",
      (void (*)()) int_greaterthan));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "bool int_lessthan(int, int)",
      (void (*)()) int_lessthan));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "bool int_equals(int, int)",
      (void (*)()) int_equals));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "double sin(double)",
      (void (*)()) math_sin));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "double cos(double)",
      (void (*)()) math_cos));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "double tan(double)",
      (void (*)()) math_tan));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "double asin(double)",
      (void (*)()) math_asin));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "double acos(double)",
      (void (*)()) math_acos));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "double atan(double)",
      (void (*)()) math_atan));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "double log(double)",
      (void (*)()) math_log));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "double exp(double)",
      (void (*)()) math_exp));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "double sqrt(double)",
      (void (*)()) math_sqrt));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "SDL_Surface IMG_Load(cstring)",
      (void (*)()) IMG_Load));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "key_map new_key_map(void)",
      (void (*)()) SNPL_key_map_constructor));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "char map_char(char, key_map)",
      (void (*)()) SNPL_map_char));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "char get_next_keydown(key_map)",
      (void (*)()) SNPL_get_next_keydown));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "color new_color(cstring)",
      (void (*)()) new_color));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "SDL_Flip(SDL_Surface)",
      (void (*)()) SDL_Flip));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "SDL_Rect new_SDL_Rect(int, int, int, int)",
      (void (*)()) new_SDL_Rect));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "SDL_Surface new_SDL_Surface(int, int)",
      (void (*)()) new_SDL_Surface));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "SDL_BlitSurface(SDL_Surface, SDL_Rect, SDL_Surface, SDL_Rect)",
      (void (*)()) SDL_BlitSurface));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "GUI new_GUI(SDL_Surface, color, int, int, int)",
      (void (*)()) new_SNPL_GUI));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "bitmap GUI_copy(GUI)",
      (void (*)()) SNPL_GUI_copy));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "GUI_embed_center(bitmap, GUI)",
      (void (*)()) SNPL_GUI_embed_center));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "GUI_pen_color(color, GUI)",
      (void (*)()) SNPL_GUI_pen_color));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "color GUI_get_color(GUI)",
      (void (*)()) SNPL_GUI_get_color));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "GUI_pen_move(int, int, GUI)",
      (void (*)()) SNPL_GUI_pen_move));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "GUI_cls(GUI)",
      (void (*)()) SNPL_GUI_cls));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "GUI_ECT(GUI)",
      (void (*)()) SNPL_GUI_ECT));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "GUI_poit(int, int, GUI)",
      (void (*)()) SNPL_GUI_poit));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "GUI_draw_outline(color, GUI)",
      (void (*)()) SNPL_GUI_draw_outline));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "GUI_draw_color(GUI)",
      (void (*)()) SNPL_GUI_draw_color));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "GUI_draw_bool(color, color, GUI)",
      (void (*)()) SNPL_GUI_draw_bool));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "GUI_draw_silhouette(color, color, color, GUI)",
      (void (*)()) SNPL_GUI_draw_silhouette));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "GUI_flex(color, color, GUI)",
      (void (*)()) SNPL_GUI_flex));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "GUI_conway(color, GUI)",
      (void (*)()) SNPL_GUI_conway));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "void lookup(cstring, lookup_table)",
      (void (*)()) SNPL_lookup_table_lookup));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "add(void, cstring, lookup_table)",
      (void (*)()) SNPL_lookup_table_add));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "remove(cstring, lookup_table)",
      (void (*)()) SNPL_lookup_table_remove));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "export(void, cstring)",
      (void (*)()) SParse_export));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "void import(cstring)",
      (void (*)()) SParse_import));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "display(lookup_table)",
      (void (*)()) display_lookup_table));

  SNPL_register_primitive(
    SNPL_cpp_primitive_constructor(
      "system(cstring)",
      (void (*)()) SParse_system_call));
}
