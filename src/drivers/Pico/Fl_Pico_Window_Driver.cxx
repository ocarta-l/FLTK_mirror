//
// "$Id: Fl_Pico_Window_Driver.cxx 12180 2017-02-25 19:31:36Z AlbrechtS $"
//
// Definition of SDL Window interface based on Pico
//
// Copyright 1998-2016 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     http://www.fltk.org/COPYING.php
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//


#include "../../config_lib.h"
#include "Fl_Pico_Window_Driver.H"

#include <FL/x.H>
#include <FL/Fl_Window.H>
#include <FL/fl_draw.H>


Fl_Pico_Window_Driver::Fl_Pico_Window_Driver(Fl_Window *win)
: Fl_Window_Driver(win)
{
}


Fl_Pico_Window_Driver::~Fl_Pico_Window_Driver()
{
}


int Fl_Pico_Window_Driver::decorated_w()
{
  return w();
}


int Fl_Pico_Window_Driver::decorated_h()
{
  return h();
}

//
// End of "$Id: Fl_Pico_Window_Driver.cxx 12180 2017-02-25 19:31:36Z AlbrechtS $".
//