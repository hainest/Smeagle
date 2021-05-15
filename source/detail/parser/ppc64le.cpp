// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include "ppc64le.hpp"

namespace smeagle::ppc64le {

  namespace st = Dyninst::SymtabAPI;
  std::vector<variable> parse_parameters(st::Symbol const* symbol) { return {}; }
  variable parse_return_value(st::Symbol const* symbol) { return {}; }
}  // namespace smeagle::ppc64le
