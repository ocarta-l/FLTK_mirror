//
// "$Id: Fl_GDI_Copy_Surface_Driver.cxx 12298 2017-07-07 15:06:51Z manolo $"
//
// Copy-to-clipboard code for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2017 by Bill Spitzak and others.
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

#ifdef FL_CFG_GFX_GDI
#include <FL/Fl_Copy_Surface.H>
#include <FL/x.H>
#include "Fl_GDI_Graphics_Driver.H"
#include "../WinAPI/Fl_WinAPI_Screen_Driver.H"
#include <windows.h>

class Fl_GDI_Copy_Surface_Driver : public Fl_Copy_Surface_Driver {
  friend class Fl_Copy_Surface_Driver;
protected:
  HDC oldgc;
  HDC gc;
  Fl_GDI_Copy_Surface_Driver(int w, int h);
  ~Fl_GDI_Copy_Surface_Driver();
  void set_current();
  void translate(int x, int y);
  void untranslate();
  int w() {return width;}
  int h() {return height;}
  int printable_rect(int *w, int *h) {*w = width; *h = height; return 0;}
};


Fl_Copy_Surface_Driver *Fl_Copy_Surface_Driver::newCopySurfaceDriver(int w, int h)
{
  return new Fl_GDI_Copy_Surface_Driver(w, h);
}


Fl_GDI_Copy_Surface_Driver::Fl_GDI_Copy_Surface_Driver(int w, int h) : Fl_Copy_Surface_Driver(w, h) {
  driver(new Fl_GDI_Graphics_Driver);
  oldgc = (HDC)Fl_Surface_Device::surface()->driver()->gc();
  // exact computation of factor from screen units to EnhMetaFile units (0.01 mm)
  HDC hdc = GetDC(NULL);
  int hmm = GetDeviceCaps(hdc, HORZSIZE);
  int hdots = GetDeviceCaps(hdc, HORZRES);
  int vmm = GetDeviceCaps(hdc, VERTSIZE);
  int vdots = GetDeviceCaps(hdc, VERTRES);
  ReleaseDC(NULL, hdc);
  float factorw = (100.f * hmm) / hdots;
  float factorh = (100.f * vmm) / vdots;
  // Global display scaling factor: 1, 1.25, 1.5, 1.75, etc...
#ifdef FLTK_HIDPI_SUPPORT
  float scaling = Fl_Graphics_Driver::default_driver().scale();
  driver()->scale(scaling);
#else
  float scaling = 1/((Fl_WinAPI_Screen_Driver*)Fl::screen_driver())->DWM_scaling_factor();
#endif
  RECT rect; rect.left = 0; rect.top = 0; rect.right = (LONG)((w*scaling) * factorw); rect.bottom = (LONG)((h*scaling) * factorh);
  gc = CreateEnhMetaFile (NULL, NULL, &rect, NULL);
  if (gc != NULL) {
    SetTextAlign(gc, TA_BASELINE|TA_LEFT);
    SetBkMode(gc, TRANSPARENT);
  }
}


Fl_GDI_Copy_Surface_Driver::~Fl_GDI_Copy_Surface_Driver() {
  if (oldgc == (HDC)Fl_Surface_Device::surface()->driver()->gc()) oldgc = NULL;
  HENHMETAFILE hmf = CloseEnhMetaFile (gc);
  if ( hmf != NULL ) {
    if ( OpenClipboard (NULL) ){
      EmptyClipboard ();
      SetClipboardData (CF_ENHMETAFILE, hmf);
      CloseClipboard ();
    }
    DeleteEnhMetaFile(hmf);
  }
  DeleteDC(gc);
  Fl_Surface_Device::surface()->driver()->gc(oldgc);
  delete driver();
}


void Fl_GDI_Copy_Surface_Driver::set_current() {
  driver()->gc(gc);
  fl_window = (Window)1;
  Fl_Surface_Device::set_current();
}


void Fl_GDI_Copy_Surface_Driver::translate(int x, int y) {
  ((Fl_GDI_Graphics_Driver*)driver())->translate_all(x, y);
}


void Fl_GDI_Copy_Surface_Driver::untranslate() {
  ((Fl_GDI_Graphics_Driver*)driver())->untranslate_all();
}
#endif // FL_CFG_GFX_GDI

//
// End of "$Id: Fl_GDI_Copy_Surface_Driver.cxx 12298 2017-07-07 15:06:51Z manolo $".
//
