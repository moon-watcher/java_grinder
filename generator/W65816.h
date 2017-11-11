/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2017 by Michael Kohn
 *
 * W65816 written by Joe Davisson
 *
 */

#ifndef _W65816_H
#define _W65816_H

#include "Generator.h"

class W65816 : public Generator
{
public:
  W65816();
  virtual ~W65816();

  virtual int open(const char *filename);
  virtual int start_init();
  virtual int insert_static_field_define(const char *name, const char *type, int index);
  virtual int init_heap(int field_count);
  virtual int field_init_int(char *name, int index, int value);
  virtual int field_init_ref(char *name, int index);
  virtual void method_start(int local_count, int max_stack, int param_count, const char *name);
  virtual void method_end(int local_count);
  virtual int push_local_var_int(int index);
  virtual int push_local_var_ref(int index);
  virtual int push_ref_static(const char *name, int index);
  virtual int push_fake();
  virtual int push_int(int32_t n);
  virtual int push_long(int64_t n);
  virtual int push_float(float f);
  virtual int push_double(double f);
  virtual int push_ref(char *name);
  virtual int pop_local_var_int(int index);
  virtual int pop_local_var_ref(int index);
  virtual int pop();
  virtual int dup();
  virtual int dup2();
  virtual int swap();
  virtual int add_integer();
  virtual int add_integer(int num);
  virtual int sub_integer();
  virtual int sub_integer(int num);
  virtual int mul_integer();
  virtual int div_integer();
  virtual int mod_integer();
  virtual int neg_integer();
  virtual int shift_left_integer();
  virtual int shift_left_integer(int num);
  virtual int shift_right_integer();
  virtual int shift_right_integer(int num);
  virtual int shift_right_uinteger();
  virtual int shift_right_uinteger(int num);
  virtual int and_integer();
  virtual int and_integer(int num);
  virtual int or_integer();
  virtual int or_integer(int num);
  virtual int xor_integer();
  virtual int xor_integer(int num);
  virtual int inc_integer(int index, int num);
  virtual int integer_to_byte();
  virtual int integer_to_short();
  virtual int jump_cond(const char *label, int cond, int distance);
  virtual int jump_cond_integer(const char *label, int cond, int distance);
  virtual int ternary(int cond, int value_true, int value_false);
  virtual int ternary(int cond, int compare, int value_true, int value_false);
  virtual int return_local(int index, int local_count);
  virtual int return_integer(int local_count);
  virtual int return_void(int local_count);
  virtual int jump(const char *name, int distance);
  virtual int call(const char *name);
  virtual int invoke_static_method(const char *name, int params, int is_void);
  virtual int put_static(const char *name, int index);
  virtual int get_static(const char *name, int index);
  virtual int brk();
  virtual int new_array(uint8_t type);
  virtual int insert_array(const char *name, int32_t *data, int len, uint8_t type);
  virtual int insert_string(const char *name, uint8_t *bytes, int len);
  virtual int push_array_length();
  virtual int push_array_length(const char *name, int field_id);
  virtual int array_read_byte();
  virtual int array_read_short();
  virtual int array_read_int();
  virtual int array_read_byte(const char *name, int field_id);
  virtual int array_read_short(const char *name, int field_id);
  virtual int array_read_int(const char *name, int field_id);
  virtual int array_write_byte();
  virtual int array_write_short();
  virtual int array_write_int();
  virtual int array_write_byte(const char *name, int field_id);
  virtual int array_write_short(const char *name, int field_id);
  virtual int array_write_int(const char *name, int field_id);

  virtual int get_values_from_stack(int num);

  // Memory API
  virtual int memory_read8_I();
  virtual int memory_write8_IB();
  virtual int memory_read16_I();
  virtual int memory_write16_IS();

protected:
  int stack;
  int start_org;
  int java_stack;
  int ram_start;
  int label_count;

  bool is_main : 1;

  bool need_swap:1;
  bool need_add_integer:1;
  bool need_sub_integer:1;
  bool need_mul_integer:1;
  bool need_div_integer:1;
  bool need_neg_integer:1;
  bool need_shift_left_integer:1;
  bool need_shift_right_integer:1;
  bool need_shift_right_uinteger:1;
  bool need_and_integer:1;
  bool need_or_integer:1;
  bool need_xor_integer:1;
  bool need_integer_to_byte:1;
  bool need_dup:1;
  bool need_dup2:1;
  bool need_push_array_length:1;
  bool need_push_array_length2:1;
  bool need_array_byte_support:1;
  bool need_array_int_support:1;

  bool need_memory_read8:1;
  bool need_memory_write8:1;
  bool need_memory_read16:1;
  bool need_memory_write16:1;

  void insert_swap();
  void insert_add_integer();
  void insert_sub_integer();
  void insert_mul_integer();
  void insert_div_integer();
  void insert_neg_integer();
  void insert_shift_left_integer();
  void insert_shift_right_integer();
  void insert_shift_right_uinteger();
  void insert_and_integer();
  void insert_or_integer();
  void insert_xor_integer();
  void insert_integer_to_byte();
  void insert_dup();
  void insert_dup2();
  void insert_push_array_length();
  void insert_push_array_length2();
  void insert_array_byte_support();
  void insert_array_int_support();

  void insert_memory_read8();
  void insert_memory_write8();
  void insert_memory_read16();
  void insert_memory_write16();
};

#endif

