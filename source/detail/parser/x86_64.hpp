// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include <vector>

#include "Symtab.h"
#include "smeagle/variable.hpp"

namespace smeagle::x86_64 {

  namespace st = Dyninst::SymtabAPI;
  std::vector<variable> parse_parameters(st::Symbol const* symbol);
  variable parse_return_value(st::Symbol const* symbol);
}  // namespace smeagle::x86_64
