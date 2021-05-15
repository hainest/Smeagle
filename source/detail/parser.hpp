// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include <smeagle/abi_description.hpp>
#include <vector>

#include "Symtab.h"
#include "parser/aarch64.hpp"
#include "parser/ppc64le.hpp"
#include "parser/x86_64.hpp"

namespace smeagle {

  namespace st = Dyninst::SymtabAPI;
  inline std::vector<abi_description> parse_abi(std::vector<st::Symbol*> const& symbols,
                                         Dyninst::Architecture arch) {
    std::vector<abi_description> functions;

    for (auto* s : symbols) {
      switch (arch) {
        case Dyninst::Architecture::Arch_x86_64:
          functions.emplace_back(smeagle::x86_64::parse_parameters(s),
                                 smeagle::x86_64::parse_return_value(s), s->getMangledName());
          break;
        case Dyninst::Architecture::Arch_aarch64:
          return {};
        case Dyninst::Architecture::Arch_ppc64:
          return {};
        default:
          throw std::runtime_error{"Unsupported architecture: " + std::to_string(arch)};
          break;
      }
    }
    return functions;
  }
}  // namespace smeagle
