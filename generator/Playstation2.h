/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2017 by Michael Kohn
 *
 */

#ifndef _PLAYSTATION_2_H
#define _PLAYSTATION_2_H

#include "MIPS64.h"

class Playstation2 : public MIPS64
{
public:
  Playstation2();
  virtual ~Playstation2();

  virtual int open(const char *filename);
  virtual int start_init();

  virtual int playstation2_setVideoMode_III();
  virtual int playstation2_setFrameBuffer1_IIIII();
  virtual int playstation2_setFrameBuffer2_IIIII();
  virtual int playstation2_setDisplay1_IIIIII();
  virtual int playstation2_setDisplay2_IIIIII();
  virtual int playstation2_waitVsync();

private:
  void playstation2_addDMAReset();
  virtual int playstation2_setFrameBuffer(int index);
  virtual int playstation2_setDisplay(int index);
};

#endif

