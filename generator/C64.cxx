/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2017 by Michael Kohn, Joe Davisson
 *
 * C64 written by Joe Davisson
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "C64.h"

#define POKE(dst) \
  POP_HI(); \
  POP_LO(); \
  fprintf(out, "  sta 0x%04x\n", dst)

#define PEEK(src) \
  fprintf(out, "  lda 0x%04x\n", src); \
  PUSH_LO(); \
  fprintf(out, "  lda #0\n"); \
  PUSH_HI()

C64::C64() :
  need_c64_vic_hires_enable(0),
  need_c64_vic_hires_clear(0),
  need_c64_vic_hires_plot(0),
  need_c64_vic_make_hires_tables(0),
  need_c64_vic_text_enable(0),
  need_c64_vic_text_clear(0),
  need_c64_vic_text_plot(0),
  need_c64_vic_color_ram_clear(0)
{
  start_org = 0x07ff;
  java_stack_lo = 0x200;
  java_stack_hi = 0x300;
  ram_start = 0xa000;
}

C64::~C64()
{
  if(need_c64_vic_hires_enable) { insert_c64_vic_hires_enable(); }
  if(need_c64_vic_hires_clear) { insert_c64_vic_hires_clear(); }
  if(need_c64_vic_hires_plot) { insert_c64_vic_hires_plot(); }
  if(need_c64_vic_make_hires_tables) { insert_c64_vic_make_hires_tables(); }
  if(need_c64_vic_text_enable) { insert_c64_vic_text_enable(); }
  if(need_c64_vic_text_clear) { insert_c64_vic_text_clear(); }
  if(need_c64_vic_text_plot) { insert_c64_vic_text_plot(); }
  if(need_c64_vic_color_ram_clear) { insert_c64_vic_color_ram_clear(); }
}

int C64::open(const char *filename)
{
  if (Generator::open(filename) != 0) { return -1; }

  fprintf(out, ".6502\n");

  // heap
  fprintf(out, "ram_start equ 0x%04x\n", ram_start);
  fprintf(out, "heap_ptr equ ram_start\n");

  // for indirection (2 bytes)
  fprintf(out, "address equ 0xfb\n");

  // java stack
  fprintf(out, "stack_lo equ 0x%04x\n", java_stack_lo);
  fprintf(out, "stack_hi equ 0x%04x\n", java_stack_hi);

  // points to locals
  fprintf(out, "locals equ 0xfe\n");

  // temp variables
  fprintf(out, "result equ 0x20\n");
  fprintf(out, "return equ 0x22\n");
  fprintf(out, "remainder equ 0x24\n");
  fprintf(out, "length equ 0x26\n");
  fprintf(out, "value1 equ 0x2a\n");
  fprintf(out, "value2 equ 0x2c\n");
  fprintf(out, "value3 equ 0x2e\n");

  // sprites
  fprintf(out, "sprite_msb_set equ 0x10\n");
  fprintf(out, "sprite_msb_clear equ 0x11\n");
  fprintf(out, "sprite_x equ 0x12\n");
  fprintf(out, "sprite_y equ 0x13\n");

  // basic loader
  fprintf(out, ".org 0x%04x\n", start_org);

  fprintf(out, "dw 0x0801\n");
  fprintf(out, "dw start\n");
  fprintf(out, "dw 2013\n");
  fprintf(out, "db 0x9e\n");
  fprintf(out, "db (((start / 1000) %% 10) + 0x30)\n");
  fprintf(out, "db (((start / 100) %% 10) + 0x30)\n");
  fprintf(out, "db (((start / 10) %% 10) + 0x30)\n");
  fprintf(out, "db (((start / 1) %% 10) + 0x30)\n");
  fprintf(out, "db ':'\n");
  fprintf(out, "db 0x8f\n");
  fprintf(out, "db \" MORTIS\"\n");
  fprintf(out, "db 0\n");
  fprintf(out, "dw 0\n\n");

  fprintf(out, "start:\n");
  fprintf(out, "  sei\n");
  fprintf(out, "  cld\n");
  fprintf(out, "  lda #0xff\n");
  fprintf(out, "  tax\n");
  fprintf(out, "  txs\n");

  // switch VIC-II to bank 0
  fprintf(out, "  lda #4\n");
  fprintf(out, "  sta 0xdd00\n");

  // put text screen at 0xc000 and charset at 0xc800
  fprintf(out, "  lda #2\n");
  fprintf(out, "  sta 0xd018\n");

  // copy charset from ROM
  fprintf(out, "  lda #50\n");
  fprintf(out, "  sta 0x0001\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "copy_charset_loop:\n");
  fprintf(out, "  lda 0xd000,y\n");
  fprintf(out, "  sta 0xc800,y\n");
  fprintf(out, "  lda 0xd100,y\n");
  fprintf(out, "  sta 0xc900,y\n");
  fprintf(out, "  lda 0xd200,y\n");
  fprintf(out, "  sta 0xca00,y\n");
  fprintf(out, "  lda 0xd300,y\n");
  fprintf(out, "  sta 0xcb00,y\n");
  fprintf(out, "  lda 0xd400,y\n");
  fprintf(out, "  sta 0xcc00,y\n");
  fprintf(out, "  lda 0xd500,y\n");
  fprintf(out, "  sta 0xcd00,y\n");
  fprintf(out, "  lda 0xd600,y\n");
  fprintf(out, "  sta 0xce00,y\n");
  fprintf(out, "  lda 0xd700,y\n");
  fprintf(out, "  sta 0xcf00,y\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne copy_charset_loop\n");

  // turn off ROM chips
  fprintf(out, "  lda #53\n");
  fprintf(out, "  sta 0x0001\n");
  fprintf(out, "\n");

  return 0;
}

int C64::c64_sid_voice1_frequency(/* value */)
{
  POP_HI();
  fprintf(out, "  sta 0xd401\n");
  POP_LO();
  fprintf(out, "  sta 0xd400\n");

  return 0;
}

int C64::c64_sid_voice1_pulse_width(/* value */)
{
  POP_HI();
  fprintf(out, "  sta 0xd403\n");
  POP_LO();
  fprintf(out, "  sta 0xd402\n");

  return 0;
}

int C64::c64_sid_voice1_waveform(/* value */) { POKE(0xd404); return 0; }

int C64::c64_sid_voice1_adsr(/* value */)
{
  POP_HI();
  fprintf(out, "  sta 0xd406\n");
  POP_LO();
  fprintf(out, "  sta 0xd405\n");

  return 0;
}

int C64::c64_sid_voice2_frequency(/* value */)
{
  POP_HI();
  fprintf(out, "  sta 0xd408\n");
  POP_LO();
  fprintf(out, "  sta 0xd407\n");

  return 0;
}

int C64::c64_sid_voice2_pulse_width(/* value */)
{
  POP_HI();
  fprintf(out, "  sta 0xd40a\n");
  POP_LO();
  fprintf(out, "  sta 0xd409\n");

  return 0;
}

int C64::c64_sid_voice2_waveform(/* value */) { POKE(0xd40b); return 0; }

int C64::c64_sid_voice2_adsr(/* value */)
{
  POP_HI();
  fprintf(out, "  sta 0xd40d\n");
  POP_LO();
  fprintf(out, "  sta 0xd40c\n");

  return 0;
}

int C64::c64_sid_voice3_frequency(/* value */)
{
  POP_HI();
  fprintf(out, "  sta 0xd40f\n");
  POP_LO();
  fprintf(out, "  sta 0xd40e\n");

  return 0;
}

int C64::c64_sid_voice3_pulse_width(/* value */)
{
  POP_HI();
  fprintf(out, "  sta 0xd411\n");
  POP_LO();
  fprintf(out, "  sta 0xd410\n");

  return 0;
}

int C64::c64_sid_voice3_waveform(/* value */) { POKE(0xd412); return 0; }

int C64::c64_sid_voice3_adsr(/* value */)
{
  POP_HI();
  fprintf(out, "  sta 0xd414\n");
  POP_LO();
  fprintf(out, "  sta 0xd413\n");

  return 0;
}

int C64::c64_sid_filter_cutoff(/* value */)
{
  POP_HI();
  fprintf(out, "  sta 0xd416\n");
  POP_LO();
  fprintf(out, "  sta 0xd415\n");

  return 0;
}

int C64::c64_sid_filter_resonance(/* value */) { POKE(0xd417); return 0; }
int C64::c64_sid_volume(/* value */) { POKE(0xd418); return 0; }
int C64::c64_sid_voice3_oscillator(/* value */) { POKE(0xd41b); return 0; }
int C64::c64_sid_voice3_envelope(/* value */) { POKE(0xd41c); return 0; }

int C64::c64_sid_clear()
{
  fprintf(out, "; sid clear\n");
  fprintf(out, "  ldy #0x1c\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "  sta 0xd400,y\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bpl #-8\n");

  return 0;
}

int C64::c64_vic_sprite0pos(/* x, y */)
{
  fprintf(out, "; sprite0pos\n");

  // y
  POP_HI();
  POP_LO();
  fprintf(out, "  sta 0xd001\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 1\n");
  fprintf(out, "  sta 0xd010\n");
  POP_HI();
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #(1 << 0)\n");
  fprintf(out, "  sta 0xd010\n");
  POP_LO();
  fprintf(out, "  sta 0xd000\n");

  return 0;
}

int C64::c64_vic_sprite1pos(/* x, y */)
{
  fprintf(out, "; sprite1pos\n");

  // y
  POP_HI();
  POP_LO();
  fprintf(out, "  sta 0xd003\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 2\n");
  fprintf(out, "  sta 0xd010\n");
  POP_HI();
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #2\n");
  fprintf(out, "  sta 0xd010\n");
  POP_LO();
  fprintf(out, "  sta 0xd002\n");

  return 0;
}

int C64::c64_vic_sprite2pos(/* x, y */)
{
  fprintf(out, "; sprite2pos\n");

  // y
  POP_HI();
  POP_LO();
  fprintf(out, "  sta 0xd005\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 4\n");
  fprintf(out, "  sta 0xd010\n");
  POP_HI();
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #4\n");
  fprintf(out, "  sta 0xd010\n");
  POP_LO();
  fprintf(out, "  sta 0xd004\n");

  return 0;
}

int C64::c64_vic_sprite3pos(/* x, y */)
{
  fprintf(out, "; sprite3pos\n");

  // y
  POP_HI();
  POP_LO();
  fprintf(out, "  sta 0xd007\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 8\n");
  fprintf(out, "  sta 0xd010\n");
  POP_HI();
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #8\n");
  fprintf(out, "  sta 0xd010\n");
  POP_LO();
  fprintf(out, "  sta 0xd006\n");

  return 0;
}

int C64::c64_vic_sprite4pos(/* x, y */)
{
  fprintf(out, "; sprite4pos\n");

  // y
  POP_HI();
  POP_LO();
  fprintf(out, "  sta 0xd009\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 16\n");
  fprintf(out, "  sta 0xd010\n");
  POP_HI();
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #16\n");
  fprintf(out, "  sta 0xd010\n");
  POP_LO();
  fprintf(out, "  sta 0xd008\n");

  return 0;
}

int C64::c64_vic_sprite5pos(/* x, y */)
{
  fprintf(out, "; sprite5pos\n");

  // y
  POP_HI();
  POP_LO();
  fprintf(out, "  sta 0xd00b\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 32\n");
  fprintf(out, "  sta 0xd010\n");
  POP_HI();
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #32\n");
  fprintf(out, "  sta 0xd010\n");
  POP_LO();
  fprintf(out, "  sta 0xd00a\n");

  return 0;
}

int C64::c64_vic_sprite6pos(/* x, y */)
{
  fprintf(out, "; sprite6pos\n");

  // y
  POP_HI();
  POP_LO();
  fprintf(out, "  sta 0xd00d\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 64\n");
  fprintf(out, "  sta 0xd010\n");
  POP_HI();
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #64\n");
  fprintf(out, "  sta 0xd010\n");
  POP_LO();
  fprintf(out, "  sta 0xd00c\n");

  return 0;
}

int C64::c64_vic_sprite7pos(/* x, y */)
{
  fprintf(out, "; sprite7pos\n");

  // y
  POP_HI();
  POP_LO();
  fprintf(out, "  sta 0xd00f\n");

  // x
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  and #255 - 128\n");
  fprintf(out, "  sta 0xd010\n");
  POP_HI();
  fprintf(out, "  beq #8\n");
  fprintf(out, "  lda 0xd010\n");
  fprintf(out, "  ora #128\n");
  fprintf(out, "  sta 0xd010\n");
  POP_LO();
  fprintf(out, "  sta 0xd00e\n");

  return 0;
}

int C64::c64_vic_write_control1(/* value */) { POKE(0xd011); return 0; }
int C64::c64_vic_read_control1() { PEEK(0xd011); return 0; }
int C64::c64_vic_wait_raster(/* line */)
{
  fprintf(out, "; wait_raster\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  cmp 0xd012\n");
  fprintf(out, "  bne #-5\n");

  return 0;
}
int C64::c64_vic_sprite_enable(/* value */) { POKE(0xd015); return 0; }
int C64::c64_vic_write_control2(/* value */) { POKE(0xd016); return 0; }
int C64::c64_vic_read_control2() { PEEK(0xd016); return 0; }
int C64::c64_vic_sprite_expandy(/* value */) { POKE(0xd017); return 0; }
int C64::c64_vic_write_pointer(/* value */) { POKE(0xd018); return 0; }
int C64::c64_vic_read_pointer() { PEEK(0xd018); return 0; }
int C64::c64_vic_sprite_priority(/* value */) { POKE(0xd01b); return 0; }
int C64::c64_vic_sprite_multicolor_enable(/* value */) { POKE(0xd01c); return 0; }
int C64::c64_vic_sprite_expandx(/* value */) { POKE(0xd01d); return 0; }
int C64::c64_vic_sprite_collision() { PEEK(0xd01e); return 0; }
int C64::c64_vic_data_collision() { PEEK(0xd01f); return 0; }
int C64::c64_vic_border(/* value */) { POKE(0xd020); return 0; }
int C64::c64_vic_background(/* value */) { POKE(0xd021); return 0; }
int C64::c64_vic_background1(/* value */) { POKE(0xd022); return 0; }
int C64::c64_vic_background2(/* value */) { POKE(0xd023); return 0; }
int C64::c64_vic_background3(/* value */) { POKE(0xd024); return 0; }
int C64::c64_vic_sprite_multicolor0(/* value */) { POKE(0xd025); return 0; }
int C64::c64_vic_sprite_multicolor1(/* value */) { POKE(0xd026); return 0; }
int C64::c64_vic_sprite0color(/* value */) { POKE(0xd027); return 0; }
int C64::c64_vic_sprite1color(/* value */) { POKE(0xd028); return 0; }
int C64::c64_vic_sprite2color(/* value */) { POKE(0xd029); return 0; }
int C64::c64_vic_sprite3color(/* value */) { POKE(0xd02a); return 0; }
int C64::c64_vic_sprite4color(/* value */) { POKE(0xd02b); return 0; }
int C64::c64_vic_sprite5color(/* value */) { POKE(0xd02c); return 0; }
int C64::c64_vic_sprite6color(/* value */) { POKE(0xd02d); return 0; }
int C64::c64_vic_sprite7color(/* value */) { POKE(0xd02e); return 0; }

int C64::c64_vic_hires_enable()
{
  need_c64_vic_hires_enable = 1;
  fprintf(out, "  jsr hires_enable\n");
  return 0;
}

int C64::c64_vic_hires_clear(/* value */)
{
  need_c64_vic_hires_clear = 1;
  fprintf(out, "  jsr hires_clear\n");
  return 0;
}

int C64::c64_vic_hires_plot(/* x, y, value */)
{
  need_c64_vic_hires_plot = 1;
  fprintf(out, "  jsr hires_plot\n");
  return 0;
}

int C64::c64_vic_make_hires_tables()
{
  need_c64_vic_make_hires_tables = 1;
  fprintf(out, "  jsr make_hires_tables\n");
  return 0;
}

int C64::c64_vic_text_enable()
{
  need_c64_vic_text_enable = 1;
  fprintf(out, "  jsr text_enable\n");
  return 0;
}

int C64::c64_vic_text_clear(/* value */)
{
  need_c64_vic_text_clear = 1;
  fprintf(out, "  jsr text_clear\n");
  return 0;
}

int C64::c64_vic_text_plot(/* x, y, value */)
{
  need_c64_vic_text_plot = 1;
  fprintf(out, "  jsr text_plot\n");
  return 0;
}

int C64::c64_vic_color_ram_clear(/* value */)
{
  need_c64_vic_color_ram_clear = 1;
  fprintf(out, "  jsr color_ram_clear\n");
  return 0;
}

void C64::insert_c64_vic_hires_enable()
{
  fprintf(out, "hires_enable:\n");
  fprintf(out, "  lda #8\n");
  fprintf(out, "  sta 0xd018\n");
  fprintf(out, "  lda 0xd011\n");
  fprintf(out, "  ora #32\n");
  fprintf(out, "  sta 0xd011\n");
  fprintf(out, "  rts\n");
}

void C64::insert_c64_vic_hires_clear()
{
  fprintf(out, "hires_clear:\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  ldy #0\n");
  fprintf(out, "hires_clear_loop:\n");
  fprintf(out, "  sta 0xe000,y\n");
  fprintf(out, "  sta 0xe100,y\n");
  fprintf(out, "  sta 0xe200,y\n");
  fprintf(out, "  sta 0xe300,y\n");
  fprintf(out, "  sta 0xe400,y\n");
  fprintf(out, "  sta 0xe500,y\n");
  fprintf(out, "  sta 0xe600,y\n");
  fprintf(out, "  sta 0xe700,y\n");
  fprintf(out, "  sta 0xe800,y\n");
  fprintf(out, "  sta 0xe900,y\n");
  fprintf(out, "  sta 0xea00,y\n");
  fprintf(out, "  sta 0xeb00,y\n");
  fprintf(out, "  sta 0xec00,y\n");
  fprintf(out, "  sta 0xed00,y\n");
  fprintf(out, "  sta 0xee00,y\n");
  fprintf(out, "  sta 0xef00,y\n");
  fprintf(out, "  sta 0xf000,y\n");
  fprintf(out, "  sta 0xf100,y\n");
  fprintf(out, "  sta 0xf200,y\n");
  fprintf(out, "  sta 0xf300,y\n");
  fprintf(out, "  sta 0xf400,y\n");
  fprintf(out, "  sta 0xf500,y\n");
  fprintf(out, "  sta 0xf600,y\n");
  fprintf(out, "  sta 0xf700,y\n");
  fprintf(out, "  sta 0xf800,y\n");
  fprintf(out, "  sta 0xf900,y\n");
  fprintf(out, "  sta 0xfa00,y\n");
  fprintf(out, "  sta 0xfb00,y\n");
  fprintf(out, "  sta 0xfc00,y\n");
  fprintf(out, "  sta 0xfd00,y\n");
  fprintf(out, "  sta 0xfe00,y\n");
  fprintf(out, "  sta 0xff00,y\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne hires_clear_loop\n");
  fprintf(out, "  rts\n");
}

void C64::insert_c64_vic_hires_plot()
{
  fprintf(out, "hires_plot:\n");
  // value
  POP_HI();
  POP_LO();
  // y
  POP_HI();
  POP_LO();
  fprintf(out, "  tay\n");
  // address lo/hi
  fprintf(out, "  lda 0x0400,y\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda 0x0500,y\n");
  fprintf(out, "  sta address + 1\n");
  // x
  POP_HI();
  fprintf(out, "  sta result + 1\n");
  POP_LO();
  fprintf(out, "  sta result + 0\n");
  // x & 7
  fprintf(out, "  and #7\n");
  fprintf(out, "  sta value1 + 0\n");
  // x / 8
  fprintf(out, "  lsr result + 1\n");
  fprintf(out, "  ror result + 0\n");
  fprintf(out, "  lsr result + 1\n");
  fprintf(out, "  ror result + 0\n");
  fprintf(out, "  lsr result + 1\n");
  fprintf(out, "  ror result + 0\n");
  fprintf(out, "  lda result + 0\n");
  fprintf(out, "  tay\n");
  // col
  fprintf(out, "  clc\n");
  fprintf(out, "  lda 0x0600,y\n");
  fprintf(out, "  adc address + 0\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda 0x0640,y\n");
  fprintf(out, "  adc address + 1\n");
  fprintf(out, "  sta address + 1\n");
  // read byte
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda (address),y\n");
  fprintf(out, "  ldy value1 + 0\n");
  fprintf(out, "  ora 0x0700,y\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  sta (address),y\n");

  fprintf(out, "  rts\n");
}

void C64::insert_c64_vic_make_hires_tables()
{
  fprintf(out, "make_hires_tables:\n");
  // row tables
  fprintf(out, "  lda #0x00\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda #0xe0\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  ldy #0\n");
  fprintf(out, "make_hires_tables_loop1:\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda address + 0\n");
  fprintf(out, "  sta 0x0400,y\n");
  fprintf(out, "  adc #1\n");
  fprintf(out, "  sta 0x0401,y\n");
  fprintf(out, "  adc #1\n");
  fprintf(out, "  sta 0x0402,y\n");
  fprintf(out, "  adc #1\n");
  fprintf(out, "  sta 0x0403,y\n");
  fprintf(out, "  adc #1\n");
  fprintf(out, "  sta 0x0404,y\n");
  fprintf(out, "  adc #1\n");
  fprintf(out, "  sta 0x0405,y\n");
  fprintf(out, "  adc #1\n");
  fprintf(out, "  sta 0x0406,y\n");
  fprintf(out, "  adc #1\n");
  fprintf(out, "  sta 0x0407,y\n");
  fprintf(out, "  lda address + 1\n");
  fprintf(out, "  sta 0x0500,y\n");
  fprintf(out, "  sta 0x0501,y\n");
  fprintf(out, "  sta 0x0502,y\n");
  fprintf(out, "  sta 0x0503,y\n");
  fprintf(out, "  sta 0x0504,y\n");
  fprintf(out, "  sta 0x0505,y\n");
  fprintf(out, "  sta 0x0506,y\n");
  fprintf(out, "  sta 0x0507,y\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  lda #0x40\n");
  fprintf(out, "  adc address + 0\n");
  fprintf(out, "  sta address + 0\n");
  fprintf(out, "  lda #0x01\n");
  fprintf(out, "  adc address + 1\n");
  fprintf(out, "  sta address + 1\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  bne make_hires_tables_loop1\n");

  // col table
  fprintf(out, "  ldy #0\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "make_hires_tables_loop3:\n");
  fprintf(out, "  sta 0x0600,y\n");
  fprintf(out, "  clc\n");
  fprintf(out, "  adc #8\n");
  fprintf(out, "  iny\n");
  fprintf(out, "  bne make_hires_tables_loop3\n");

  fprintf(out, "  ldy #40\n");
  fprintf(out, "  lda #0\n");
  fprintf(out, "make_hires_tables_loop4:\n");
  fprintf(out, "  sta 0x0640,y\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne make_hires_tables_loop4\n");

  fprintf(out, "  lda #1\n");
  fprintf(out, "  sta 0x0660\n");
  fprintf(out, "  sta 0x0661\n");
  fprintf(out, "  sta 0x0662\n");
  fprintf(out, "  sta 0x0663\n");
  fprintf(out, "  sta 0x0664\n");
  fprintf(out, "  sta 0x0665\n");
  fprintf(out, "  sta 0x0666\n");
  fprintf(out, "  sta 0x0667\n");

  // bit table
  fprintf(out, "  lda #128\n");
  fprintf(out, "  sta 0x0700\n");
  fprintf(out, "  lda #64\n");
  fprintf(out, "  sta 0x0701\n");
  fprintf(out, "  lda #32\n");
  fprintf(out, "  sta 0x0702\n");
  fprintf(out, "  lda #16\n");
  fprintf(out, "  sta 0x0703\n");
  fprintf(out, "  lda #8\n");
  fprintf(out, "  sta 0x0704\n");
  fprintf(out, "  lda #4\n");
  fprintf(out, "  sta 0x0705\n");
  fprintf(out, "  lda #2\n");
  fprintf(out, "  sta 0x0706\n");
  fprintf(out, "  lda #1\n");
  fprintf(out, "  sta 0x0707\n");
  fprintf(out, "  rts\n");
}

void C64::insert_c64_vic_text_enable()
{
  fprintf(out, "text_enable:\n");
  fprintf(out, "  lda #2\n");
  fprintf(out, "  sta 0xd018\n");
  fprintf(out, "  lda 0xd011\n");
  fprintf(out, "  and #223\n");
  fprintf(out, "  sta 0xd011\n");
  fprintf(out, "  rts\n");
}

void C64::insert_c64_vic_text_clear()
{
  fprintf(out, "text_clear:\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  ldy #0\n");
  fprintf(out, "text_clear_loop:\n");
  fprintf(out, "  sta 0xc000,y\n");
  fprintf(out, "  sta 0xc100,y\n");
  fprintf(out, "  sta 0xc200,y\n");
  fprintf(out, "  sta 0xc300,y\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne text_clear_loop\n");
  fprintf(out, "  rts\n");
}

void C64::insert_c64_vic_text_plot()
{
  fprintf(out, "text_plot:\n");
  fprintf(out, "  rts\n");
}

void C64::insert_c64_vic_color_ram_clear()
{
  fprintf(out, "color_ram_clear:\n");
  POP_HI();
  POP_LO();
  fprintf(out, "  ldy #0\n");
  fprintf(out, "color_ram_clear_loop:\n");
  fprintf(out, "  sta 0xd800,y\n");
  fprintf(out, "  sta 0xd900,y\n");
  fprintf(out, "  sta 0xda00,y\n");
  fprintf(out, "  sta 0xdb00,y\n");
  fprintf(out, "  dey\n");
  fprintf(out, "  bne color_ram_clear_loop\n");
  fprintf(out, "  rts\n");
}

#if 0
void C64::close()
{
}
#endif

