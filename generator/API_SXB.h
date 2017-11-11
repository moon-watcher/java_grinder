/**
 *  Java Grinder
 *  Author: Michael Kohn
 *   Email: mike@mikekohn.net
 *     Web: http://www.mikekohn.net/
 * License: GPL
 *
 * Copyright 2014-2017 by Michael Kohn
 *
 * Western Design Center SXB boards by Joe Davisson
 *
 */

#ifndef _API_SXB_H
#define _API_SXB_H

class API_SXB
{
public:
  // SXB
  virtual int sxb_getChar() { return -1; }
  virtual int sxb_putChar_C() { return -1; }
  virtual int sxb_getInt() { return -1; }
  virtual int sxb_putInt_I() { return -1; }
  virtual int sxb_getString() { return -1; }
  virtual int sxb_putString_X() { return -1; }
  virtual int sxb_controlTones_IIZZ() { return -1; }
};

#endif

