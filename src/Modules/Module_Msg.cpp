// This file is part of RLVM, a RealLive virtual machine clone.
//
// -----------------------------------------------------------------------
//
// Copyright (C) 2006 Elliot Glaysher
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

#include "Precompiled.hpp"

// -----------------------------------------------------------------------

/**
 * @file   Module_Msg.cpp
 * @author Elliot Glaysher
 * @date   Sun Oct  1 22:18:39 2006
 * 
 * @brief  Implements many textout related operations.
 */

#include "Modules/Module_Msg.hpp"
#include "MachineBase/RLOperation.hpp"
#include "MachineBase/RLMachine.hpp"
#include "MachineBase/RLModule.hpp"
//#include "GeneralOperations.hpp"

#include "libReallive/gameexe.h"
#include "Modules/PauseLongOperation.hpp"

#include "Systems/Base/System.hpp"
#include "Systems/Base/EventSystem.hpp"
#include "Systems/Base/GraphicsSystem.hpp"
#include "Systems/Base/TextSystem.hpp"
#include "Systems/Base/TextPage.hpp"
#include "Systems/Base/TextWindow.hpp"

#include "Modules/cp932toUnicode.hpp"

using namespace std;

/**
 * @defgroup ModuleMessage The Message and Textout module (mod<0:3>).
 * @ingroup ModulesOpcodes
 *
 * @{
 */

struct Msg_par : public RLOp_Void_Void {
  void operator()(RLMachine& machine) {
    TextPage& page = machine.system().text().currentPage(machine);
    page.resetIndentation();
    page.hardBrake();
  }
};

// -----------------------------------------------------------------------

/** 
 * Implements op<0:Msg:17, 0>, fun pause().
 *
 */
struct Msg_pause : public RLOp_Void_Void
{
  void operator()(RLMachine& machine)
  {
    TextSystem& text = machine.system().text();
    int windowNum = text.activeWindow();
    TextWindow& textWindow = text.textWindow(machine, windowNum);

    if(textWindow.actionOnPause())
    {
      machine.pushLongOperation(
        new HardBrakeAfterLongop(new PauseLongOperation(machine)));
    }
    else
    {
      machine.pushLongOperation(
        new NewPageAfterLongop(new PauseLongOperation(machine)));
    }
  }
};

// -----------------------------------------------------------------------

struct Msg_TextWindow : public RLOp_Void_1< DefaultIntValue_T< 0 > >
{
  void operator()(RLMachine& machine, int window)
  {
    machine.system().text().setActiveWindow(window);
  }
};

// -----------------------------------------------------------------------

struct Msg_FontColour : public RLOp_Void_2< DefaultIntValue_T< 0 >,
                                            DefaultIntValue_T< 0 > >
{
  void operator()(RLMachine& machine, int textColorNum, int shadowColorNum)
  {
    machine.system().text().currentPage(machine).fontColour(textColorNum);
  }
};

// -----------------------------------------------------------------------

struct Msg_doruby_display : public RLOp_Void_1< StrConstant_T >
{
  void operator()(RLMachine& machine, std::string cpStr)
  {
    std::string utf8str = cp932toUTF8(cpStr, machine.getTextEncoding());
    machine.system().text().currentPage(machine).displayRubyText(utf8str);
  }
};

// -----------------------------------------------------------------------

struct Msg_doruby_mark : public RLOp_Void_Void
{
  void operator()(RLMachine& machine)
  {
    machine.system().text().currentPage(machine).markRubyBegin();
  }
};

// -----------------------------------------------------------------------

struct Msg_msgHide : public RLOp_Void_1< DefaultIntValue_T< 0 > >
{
  void operator()(RLMachine& machine, int unknown)
  {
    int winNum = machine.system().text().activeWindow();
    machine.system().text().hideTextWindow(winNum);
    machine.system().text().newPageOnWindow(machine, winNum);
  }
};

// -----------------------------------------------------------------------

struct Msg_msgClear : public RLOp_Void_Void {
  void operator()(RLMachine& machine) {
    TextSystem& text = machine.system().text();
    int activeWindow = text.activeWindow();
    text.textWindow(machine, activeWindow).clearWin();
    text.newPageOnWindow(machine, activeWindow);
  }
};

// -----------------------------------------------------------------------

struct Msg_msgClearAll : public RLOp_Void_Void {
  void operator()(RLMachine& machine) {
    TextSystem& text = machine.system().text();
    vector<int> activeWindows = text.activeWindows();
    int activeWindow = text.activeWindow();

    for(vector<int>::const_iterator it = activeWindows.begin(); 
        it != activeWindows.end(); ++it)
    {
      text.textWindow(machine, activeWindow).clearWin();
      text.newPageOnWindow(machine, *it);
    }
  }
};

// -----------------------------------------------------------------------

struct Msg_br : public RLOp_Void_Void {
  void operator()(RLMachine& machine) {
    machine.system().text().currentPage(machine).hardBrake();    
  }
};

// -----------------------------------------------------------------------

struct Msg_spause : public RLOp_Void_Void {
  void operator()(RLMachine& machine) {
    machine.pushLongOperation(new PauseLongOperation(machine));
  }
};

// -----------------------------------------------------------------------

struct Msg_page : public RLOp_Void_Void {
  void operator()(RLMachine& machine) {
    machine.pushLongOperation(
      new NewPageAfterLongop(new PauseLongOperation(machine)));
  }
};

// -----------------------------------------------------------------------

MsgModule::MsgModule()
  : RLModule("Msg", 0, 003)
{
  addOpcode(3, 0, new Msg_par);
//  addOpcode(15, 0, /* spause3 */ );
  addOpcode(17, 0, new Msg_pause);

  addOpcode(102, 0, new Msg_TextWindow);
  addOpcode(102, 1, new Msg_TextWindow);

  addOpcode(105, 0, new Msg_FontColour);
  addOpcode(105, 1, new Msg_FontColour);
  addOpcode(105, 2, new Msg_FontColour);

  addOpcode(120, 0, new Msg_doruby_display);
  addOpcode(120, 1, new Msg_doruby_mark);

  addOpcode(151, 0, new Msg_msgHide);

  addOpcode(152, 0, new Msg_msgClear);

  addOpcode(162, 0, new Msg_msgClearAll);

  addOpcode(201, 0, new Msg_br);
  addOpcode(205, 0, new Msg_spause);

  addOpcode(210, 0, new Msg_page);
}

// @}
