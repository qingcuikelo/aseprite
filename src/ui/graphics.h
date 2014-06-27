// Aseprite UI Library
// Copyright (C) 2001-2014  David Capello
//
// This file is released under the terms of the MIT license.
// Read LICENSE.txt for more information.

#ifndef UI_GRAPHICS_H_INCLUDED
#define UI_GRAPHICS_H_INCLUDED
#pragma once

#include "base/shared_ptr.h"
#include "base/disable_copying.h"
#include "gfx/point.h"
#include "gfx/rect.h"
#include "gfx/size.h"
#include "ui/color.h"

#include <string>

namespace gfx {
  class Region;
}

namespace she {
  class Font;
  class Surface;
}

namespace ui {

  // Class to render a widget in the screen.
  class Graphics {
  public:
    Graphics(she::Surface* surface, int dx, int dy);
    ~Graphics();

    she::Surface* getInternalSurface() { return m_surface; }
    int getInternalDeltaX() { return m_dx; }
    int getInternalDeltaY() { return m_dy; }

    gfx::Rect getClipBounds() const;
    void setClipBounds(const gfx::Rect& rc);
    bool intersectClipRect(const gfx::Rect& rc);

    void drawHLine(ui::Color color, int x, int y, int w);
    void drawVLine(ui::Color color, int x, int y, int h);
    void drawLine(ui::Color color, const gfx::Point& a, const gfx::Point& b);

    void drawRect(ui::Color color, const gfx::Rect& rc);
    void fillRect(ui::Color color, const gfx::Rect& rc);
    void fillRegion(ui::Color color, const gfx::Region& rgn);
    void fillAreaBetweenRects(ui::Color color,
      const gfx::Rect& outer, const gfx::Rect& inner);

    void drawSurface(she::Surface* surface, int x, int y);
    void drawRgbaSurface(she::Surface* surface, int x, int y);

    void blit(she::Surface* src, int srcx, int srcy, int dstx, int dsty, int w, int h);

    // ======================================================================
    // FONT & TEXT
    // ======================================================================

    void setFont(she::Font* font);
    she::Font* getFont() { return m_font; }

    void drawChar(int chr, Color fg, Color bg, int x, int y);
    void drawString(const std::string& str, Color fg, Color bg, const gfx::Point& pt);
    void drawUIString(const std::string& str, Color fg, Color bg, const gfx::Point& pt);
    void drawAlignedUIString(const std::string& str, Color fg, Color bg, const gfx::Rect& rc, int align);

    gfx::Size measureChar(int chr);
    gfx::Size measureUIString(const std::string& str);
    static int measureUIStringLength(const std::string& str, she::Font* font);
    gfx::Size fitString(const std::string& str, int maxWidth, int align);

  private:
    gfx::Size doUIStringAlgorithm(const std::string& str, Color fg, Color bg, const gfx::Rect& rc, int align, bool draw);

    she::Surface* m_surface;
    int m_dx;
    int m_dy;
    gfx::Rect m_clipBounds;
    she::Font* m_font;
  };

  // Class to draw directly in the screen.
  class ScreenGraphics : public Graphics {
  public:
    ScreenGraphics();
    virtual ~ScreenGraphics();
  };

  // Class to temporary set the Graphics' clip region to a sub-rectangle
  // (in the life-time of the IntersectClip instance).
  class IntersectClip {
  public:
    IntersectClip(Graphics* g, const gfx::Rect& rc)
      : m_graphics(g)
      , m_oldClip(g->getClipBounds())
    {
      m_notEmpty = m_graphics->intersectClipRect(rc);
    }

    ~IntersectClip()
    {
      m_graphics->setClipBounds(m_oldClip);
    }

    operator bool() const { return m_notEmpty; }

  private:
    Graphics* m_graphics;
    gfx::Rect m_oldClip;
    bool m_notEmpty;

    DISABLE_COPYING(IntersectClip);
  };

  typedef SharedPtr<Graphics> GraphicsPtr;

} // namespace ui

#endif
