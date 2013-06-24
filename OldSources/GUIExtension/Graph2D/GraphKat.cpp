#include "stdafx.h"
#include "GraphKat.h"

CGraphGUI::CGraphGUI(wchar_t* _caption, IrrlichtDevice* device, core::rect<s32> Rect) {
  driver = device->getVideoDriver();

  L = Rect.UpperLeftCorner.X;
  T = Rect.UpperLeftCorner.Y;
  W = Rect.LowerRightCorner.X;
  H = Rect.LowerRightCorner.Y;

  white = SColor(255, 255,255,255);
  start = SColor(255, 255,170,120);
  end   = SColor(255, 85 ,32 ,50 );

  caption = _caption;
  IGUIEnvironment* guienv = device->getGUIEnvironment();
  font = guienv->getFont("Lucida8.bmp");
  guienv->getSkin()->setFont(font);

  u8 nbVals = W - L;
  Max = H - T - 10;

  for (u8 x=0; x<nbVals; x++)
    t.push_back(rand()%Max);
}

void CGraphGUI::addValue(u8 val) {
  t.push_back(val);
  t.erase(0);
}

void CGraphGUI::draw() {
  drawBorder();
 
  for (u8 x=1; x<t.size(); x++)
    driver->draw2DLine (position2d<s32>(L+x, H-t[x-1]),
                        position2d<s32>(L+x, H-t[x]  ),   white);
}

void CGraphGUI::drawBorder() {
  driver->draw2DLine (position2d<s32>(L,T), position2d<s32>(W,T), white);
  driver->draw2DLine (position2d<s32>(L,T), position2d<s32>(L,H), white);
  driver->draw2DLine (position2d<s32>(L,H), position2d<s32>(W,H), white);
  driver->draw2DLine (position2d<s32>(W,T), position2d<s32>(W,H), white);
}

void CGraphGUI::drawOrange() {
  drawBorder();

  static u8 nb = W - L - 10;

  static f32 Rstep = (start.getRed()   - end.getRed()  ) / nb;
  static f32 Gstep = (start.getGreen() - end.getGreen()) / nb;
  static f32 Bstep = (start.getBlue()  - end.getBlue() ) / nb;

  font->draw (caption, core::rect<s32>(L+2,T+1,W,H), start);

  for (u8 x=1; x<t.size(); x++)
    driver->draw2DLine (position2d<s32>(L+x, H-t[x-1]),
                        position2d<s32>(L+x, H-t[x]  ),
                        SColor (255, 255-(u8)(x*Rstep), (u8)(x*Gstep), (u8)(x*Bstep)) );
}

void CGraphGUI::drawGreen() {
  drawBorder();

  static u8 nb = W - L - 10;

  static f32 Rstep = (start.getRed() - end.getRed()) / nb;

  font->draw (caption, core::rect<s32>(L+2,T+1,W,H), start);

  for (u8 x=1; x<t.size(); x++)
    driver->draw2DLine (position2d<s32>(L+x, H-t[x-1]),
                        position2d<s32>(L+x, H-t[x]  ),
                        SColor(255,(u8)(x*Rstep), 100, 10) );
}

void CGraphGUI::drawBlue() {
  //drawBorder();
  driver->draw2DLine (position2d<s32>(L,H), position2d<s32>(W,H), white);

  static u8 nb = W - L - 10; // -10 to draw text above

  static f32 Rstep = (start.getRed() - end.getRed()) / nb;

  font->draw (caption, core::rect<s32>(L+2,T-1,W,H), start);

  //values
  for (u8 x=1; x<t.size(); x++)
    driver->draw2DLine (position2d<s32>(L+x, H-t[x-1]),
                        position2d<s32>(L+x, H-t[x]  ),
                        SColor(255,(u8)(x*Rstep), 100, 100) );
}

void CGraphGUI::drawCrazy() {
  drawBorder();

  static u8 nb = W - L-10;

  //static SColor start (255, 255,170,120);
  //static SColor end   (255, 85 ,32 ,50);

  //static f32 Rstep = (f32)(start.getRed()  - end.getRed() ) / nb;
  static f32 Gstep = (f32)(start.getGreen() - end.getGreen()) / nb;
  static f32 Bstep = (f32)(start.getBlue() - end.getBlue()) / nb;

  //static f32 red = 0.f;
  static f32 green = 0.f;
  static f32 blue= 0.f;

  font->draw (caption, core::rect<s32>(L+2,T+1,W,H), start);

  for (u8 x=1; x<t.size(); x++) {
    driver->draw2DLine (position2d<s32>(L+x, H-t[x-1]),
                        position2d<s32>(L+x, H-t[x]  ),
                        SColor(255, 100, (u8)green, (u8)blue) );

    green += Gstep; if (green > 255.f) green=0.f;
    blue  += Bstep; if (blue  > 255.f) blue=0.f;
  }
}

void CGraphGUI::drawStrange() {
  static u8 nb = W - L - 10;

  static f32 Rstep = (f32)(start.getRed()  - end.getRed() ) / nb;

  //font->draw (caption, core::rect<s32>(L+2,T+1,W,H), start);

  for (u8 x=0; x<t.size(); x++)
    driver->draw2DLine (position2d<s32>(L+1, H-t[x-1]),
                        position2d<s32>(L+x  , H-t[x]),
                        SColor(255,100, 255-(u8)(x*Rstep), 100) );
}

u8 CGraphGUI::getMax() {
  return Max;
}

void CGraphGUI::setText(wchar_t* text) {
  //if (caption) delete caption;
  caption = text;
}

CGraphGUI::~CGraphGUI() {

}
