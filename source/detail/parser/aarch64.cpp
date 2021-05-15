// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include "aarch64.hpp"

namespace smeagle::aarch64 {

  namespace st = Dyninst::SymtabAPI;
  std::vector<variable> parse_parameters(st::Symbol const* symbol) { return {}; }
  variable parse_return_value(st::Symbol const* symbol) { return {}; }
}  // namespace smeagle::aarch64
