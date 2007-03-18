// This file is part of RLVM, a RealLive virtual machine clone.
//
// -----------------------------------------------------------------------
//
// Copyright (C) 2006 El Riot
//  
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//  
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//  
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//  
// -----------------------------------------------------------------------

#ifndef __TextWindow_hpp__
#define __TextWindow_hpp__

#include <vector>

class RLMachine;
class Gameexe;
class GraphicsSystem;

/**
 * Abstract representation of a TextWindow. Aggrigated by TextSystem,
 * and rendered in conjunction with GraphicsSystem.
 *
 * This class has all sorts of complex, rarely used text rendering
 * options, including several co-ordinate systems, which I'm sure was
 * done to give me a headache.
 */
class TextWindow
{
protected:
  /**
   * Describes the origin point of the window
   * 
   * @{
   */
  int m_windowPositionOrigin;
  int m_windowPositionX;
  int m_windowPositionY;
  /// @}

  /** The text insertion point. These two numbers are relative to the
   * text window location. 
   *
   * @{
   */
  int m_textInsertionPointX;
  int m_textInsertionPointY;
  /// @}

  /**
   * Text output properties
   * 
   * @{
   */

  /// The current size of the font
  int m_fontSizeInPixels;

  /// The current size of the ruby text
  int m_rubySize;

  /// Size of the window in characters
  int m_xWindowSizeInChars, m_yWindowSizeInChars;

  /// Spacing between characters
  int m_xSpacing, m_ySpacing;

  /// 
  int m_useIndentation;

  int m_currentIndentationInPixels;

  /// @}

  /**
   * @name Positional data
   * 
   * @{
   */
  int m_origin, m_xDistanceFromOrigin, m_yDistanceFromOrigin;

  /// @}

  int m_upperBoxPadding, m_lowerBoxPadding, m_leftBoxPadding, m_rightBoxPadding;

  /// The window background color
  int m_r, m_g, m_b, m_alpha, m_filter;

  int m_isVisible;

  /**
   * @name Waku (window decorations)
   * 
   * @{
   */
  

  /// @}

protected:

  /// Internal calculations stuff

public:
  TextWindow();

  virtual ~TextWindow() {}

  /**
   * @name Text size and location
   * 
   * Accessors dealing with the size and location of the text
   * window. 
   *
   * @{
   */

  /**
   * Sets the size of the text window in characters. Reprsented by
   * \#WINDOW.xxx.MOJI_CNT.
   */
  void setWindowSizeInCharacters(const std::vector<int>& posData);
  int xWindowSizeInChars() const { return m_xWindowSizeInChars; }
  int yWindowSizeInChars() const { return m_yWindowSizeInChars; }

  /**
   * Sets the size of the spacing between characters. Reprsented by
   * \#WINDOW.xxx.MOJI_REP.
   */
  void setSpacingBetweenCharacters(const std::vector<int>& posData);
  int xSpacing() const { return m_xSpacing; }
  int ySpacing() const { return m_ySpacing; }

  /**
   * Sets the size of the ruby (furigana; pronounciation guide) text
   * in pixels. If zero, ruby text is disabled in this
   * window. Hilariously represented by \#WINDOW.xxx.LUBY_SIZE.
   */
  void setRubyTextSize(const int i) { m_rubySize = i; }
  int rubyTextSize() const { return m_rubySize; }

  /** 
   * Sets the size of the font. Reprsented by \#WINDOW.xxx.MOJI.SIZE.
   */
  void setFontSizeInPixels(int i) { m_fontSizeInPixels = i; }
  int fontSizeInPixels() const { return m_fontSizeInPixels; }

  void setTextboxPadding(const std::vector<int>& posData);

  void setUseIndentation(const int i) { m_useIndentation = i; }

  /// @}

  /**
   * @name Window Positional 
   * 
   * @{
   */
  void setWindowPosition(const std::vector<int>& posData);

  int windowWidth() const;
  int windowHeight() const;

  int boxX1() const;
  int boxY1() const;

  int textX1(RLMachine& machine) const;
  int textY1(RLMachine& machine) const;
  int textX2(RLMachine& machine) const;
  int textY2(RLMachine& machine) const;

  /// @}

  /**
   * @name Waku (Window decoration) Handling Functions
   * 
   * @{
   */

  void setWindowWaku(RLMachine& machine, Gameexe& gexe, const int wakuNo);


  virtual void setWakuMain(RLMachine& machine, const std::string& name) = 0;

  /** 
   * Loads the graphics file name as the backing for the 
   * 
   * @param name 
   */
  virtual void setWakuBacking(RLMachine& machine, const std::string& name) = 0;

  /// @}


  /**
   * @name Window Color Attributes
   * 
   * Accessors regarding the background color of the window.
   *
   * Represents the data parsed from \#WINDOW_ATTR,
   * \#WINDOW.index.ATTR_MOD, and \#WINDOW.index.ATTR
   *
   * @{
   */
  void setR(int i) { m_r = i; }
  void setG(int i) { m_g = i; }
  void setB(int i) { m_b = i; }
  void setAlpha(int i) { m_alpha = i; }
  void setFilter(int i) { m_filter = i; }
  void setRGBAF(const std::vector<int>& rgbaValues);
  
  int r() const { return m_r; }
  int g() const { return m_g; }
  int b() const { return m_b; }
  int alpha() const { return m_alpha; }
  int filter() const { return m_filter; }
  /// @}

  void setVisible(int in) { m_isVisible = in; }
  bool isVisible() const { return m_isVisible; }

  // ------------------------------------------------ [ Abstract interface ]
  virtual void render(RLMachine& machine) = 0;

  /** 
   * Clears the text window of all text and resets the insertion
   * point.
   */
  virtual void clearWin() = 0;

  /** 
   * Displays 1 or more UTF-8 characters in this window.
   */
  virtual void displayText(RLMachine& machine, const std::string& text) = 0;
};

#endif
