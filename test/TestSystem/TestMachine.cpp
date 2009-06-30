// -*- Mode: C++; tab-width:2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi:tw=80:et:ts=2:St's=2
//
// -----------------------------------------------------------------------
//
// This file is part of RLVM, a RealLive virtual machine clone.
//
// -----------------------------------------------------------------------
//
// Copyright (C) 2009 Elliot Glaysher
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
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
// -----------------------------------------------------------------------

#include "TestSystem/TestMachine.hpp"

#include "MachineBase/RLModule.hpp"
#include "MachineBase/RLOperation.hpp"
#include "Utilities/Exception.hpp"
#include "libReallive/defs.h"

using libReallive::insert_i16;
using rlvm::Exception;
using std::make_pair;

TestMachine::TestMachine(System& in_system, libReallive::Archive& in_archive)
    : RLMachine(in_system, in_archive) {
}

void TestMachine::attachModule(RLModule* module) {
  for (RLModule::OpcodeMap::iterator it = module->begin(); it != module->end();
       ++it) {
    int opcode = -1;
    unsigned char overload = 0;
    RLModule::unpackOpcodeNumber(it->first, opcode, overload);

    RLOperation* op = it->second;
    registry_.insert(make_pair(make_pair(it->second->name(), overload), op));
  }

  RLMachine::attachModule(module);
}

void TestMachine::exe(const std::string& name, unsigned char overload) {
  runOpcode(name, overload, 0, "");
}

void TestMachine::runOpcode(const std::string& name, unsigned char overload,
                            int argc, const std::string& argument_string) {
  string repr;
  repr.resize(8, 0);
  repr[0] = '#';
  repr[1] = 0;  // type
  repr[2] = 0;  // module
  insert_i16(repr, 3, 0);  // opcode
  insert_i16(repr, 5, argc);
  repr[7] = overload;

  string full = repr + '(' + argument_string + ')';
  libReallive::FunctionElement element(full.c_str());

  RLOperation* op = registry_[make_pair(name, overload)];
  if (op) {
    op->dispatchFunction(*this, element);
  } else {
    throw rlvm::Exception("Illegal opcode TestMachine::runOpcode");
  }
}
