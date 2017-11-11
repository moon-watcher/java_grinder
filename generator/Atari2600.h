/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2017 by Michael Kohn, Joe Davisson
 *
 */

#ifndef _ATARI_2600_H
#define _ATARI_2600_H

#include "M6502_8.h"

class Atari2600 : public M6502_8
{
public:
  Atari2600();
  virtual ~Atari2600();

  virtual int open(const char *filename);
  virtual int atari2600_waitHsync_I();
  virtual int atari2600_waitHsync_I(int lines);
  virtual int atari2600_waitHsync();
  virtual int atari2600_startVblank();
  virtual int atari2600_waitVblank();
  virtual int atari2600_startOverscan();
  virtual int atari2600_waitOverscan();
  virtual int atari2600_setColorPlayer0_I();
  virtual int atari2600_setColorPlayer0_I(int color);
  virtual int atari2600_setColorPlayer1_I();
  virtual int atari2600_setColorPlayer1_I(int color);
  virtual int atari2600_setColorPlayfield_I();
  virtual int atari2600_setColorPlayfield_I(int color);
  virtual int atari2600_setColorBackground_I();
  virtual int atari2600_setColorBackground_I(int color);

  virtual int atari2600_resetPlayer0();
  virtual int atari2600_resetPlayer1();
  virtual int atari2600_resetMissile0();
  virtual int atari2600_resetMissile1();
  virtual int atari2600_resetBall();
  virtual int atari2600_resetMissile0ToPlayer0On();
  virtual int atari2600_resetMissile1ToPlayer1On();
  virtual int atari2600_resetMissile0ToPlayer0Off();
  virtual int atari2600_resetMissile1ToPlayer1Off();
  virtual int atari2600_applyHorizontalMotion();
  virtual int atari2600_clearMotionRegisters();
  virtual int atari2600_clearCollisionLatches();

  virtual int atari2600_enableMissile0();
  virtual int atari2600_enableMissile1();
  virtual int atari2600_enableBall();
  virtual int atari2600_disableMissile0();
  virtual int atari2600_disableMissile1();
  virtual int atari2600_disableBall();

  virtual int atari2600_enablePlayer0Reflect();
  virtual int atari2600_enablePlayer1Reflect();
  virtual int atari2600_disablePlayer0Reflect();
  virtual int atari2600_disablePlayer1Reflect();

  virtual int atari2600_setPlayfieldMode_I();
  virtual int atari2600_setPlayfieldMode_I(int mode);

  virtual int atari2600_setPlayfield0_B();
  virtual int atari2600_setPlayfield0_B(int value);
  virtual int atari2600_setPlayfield1_B();
  virtual int atari2600_setPlayfield1_B(int value);
  virtual int atari2600_setPlayfield2_B();
  virtual int atari2600_setPlayfield2_B(int value);

  virtual int atari2600_setPlayfieldData_aB();
  virtual int atari2600_setPlayfieldLength_B();

  virtual int atari2600_setPlayer0Position_BB();
  virtual int atari2600_setPlayer1Position_BB();
  virtual int atari2600_setMissile0Position_BB();
  virtual int atari2600_setMissile1Position_BB();
  virtual int atari2600_setBallPosition_BB();

  virtual int atari2600_setPlayer0Sprite_aB();
  virtual int atari2600_setPlayer1Sprite_aB();
  virtual int atari2600_setMissile0Sprite_aB();
  virtual int atari2600_setMissile1Sprite_aB();
  virtual int atari2600_setBallSprite_aB();

  virtual int atari2600_drawScreen();

  virtual int atari2600_isCollisionMissile0Player1();
  virtual int atari2600_isCollisionMissile0Player0();
  virtual int atari2600_isCollisionMissile1Player0();
  virtual int atari2600_isCollisionMissile1Player1();
  virtual int atari2600_isCollisionPlayer0PlayField();
  virtual int atari2600_isCollisionPlayer0Ball();
  virtual int atari2600_isCollisionPlayer1PlayField();
  virtual int atari2600_isCollisionPlayer1Ball();
  virtual int atari2600_isCollisionMissile0Playfield();
  virtual int atari2600_isCollisionMissile0Ball();
  virtual int atari2600_isCollisionMissile1Playfield();
  virtual int atari2600_isCollisionMissile1Ball();
  virtual int atari2600_isCollisionBallPlayfield();
  virtual int atari2600_isCollisionPlayer0Player1();
  virtual int atari2600_isCollisionMissile0Missile1();

  virtual int atari2600_setAudioControl0_B();
  virtual int atari2600_setAudioControl0_B(int value);
  virtual int atari2600_setAudioControl1_B();
  virtual int atari2600_setAudioControl1_B(int value);
  virtual int atari2600_setAudioFrequency0_B();
  virtual int atari2600_setAudioFrequency0_B(int value);
  virtual int atari2600_setAudioFrequency1_B();
  virtual int atari2600_setAudioFrequency1_B(int value);
  virtual int atari2600_setAudioVolume0_B();
  virtual int atari2600_setAudioVolume0_B(int value);
  virtual int atari2600_setAudioVolume1_B();
  virtual int atari2600_setAudioVolume1_B(int value);

  virtual int atari2600_isJoystick0Right();
  virtual int atari2600_isJoystick0Left();
  virtual int atari2600_isJoystick0Down();
  virtual int atari2600_isJoystick0Up();
  virtual int atari2600_isJoystick0ButtonDown();
  virtual int atari2600_isJoystick1Right();
  virtual int atari2600_isJoystick1Left();
  virtual int atari2600_isJoystick1Down();
  virtual int atari2600_isJoystick1Up();
  virtual int atari2600_isJoystick1ButtonDown();

  virtual int atari2600_getSwitches();
  virtual int atari2600_isSwitchResetDown();
  virtual int atari2600_isSwitchSelectDown();
  virtual int atari2600_isSwitchColorSet();
  virtual int atari2600_isSwitchDifficultyP0Down();
  virtual int atari2600_isSwitchDifficultyP1Down();

  virtual int atari2600_setScore0_B();
  virtual int atari2600_setScore1_B();

  virtual int atari2600_setTitlePos_I();
  virtual int atari2600_setTitleColor_I();
  virtual int atari2600_drawTitleScreen();

  virtual int atari2600_setBank_B();
  virtual int atari2600_setBank_B(int value);

private:
  bool need_game_draw:1;
  bool need_title_draw:1;
  bool need_set_bank:1;
  int bank_index;

  void insert_game_draw();
  void insert_title_draw();
  void insert_set_bank();
  void insert_functions();
  void insert_variables();
};

#endif

