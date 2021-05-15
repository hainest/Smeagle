// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include "smeagle/smeagle.hpp"

#include <stdexcept>

#include "Symtab.h"
#include "detail/parser.hpp"

namespace smeagle {

  corpus parse(std::string file_name) {
    namespace st = Dyninst::SymtabAPI;

    // Read the library into the Symtab object
    st::Symtab *obj{};
    if (!st::Symtab::openFile(obj, file_name)) {
      throw std::runtime_error{"Could not open file " + file_name};
    }

    // Get all symbols in the library
    std::vector<st::Symbol *> symbols;
    if (!obj->getAllSymbols(symbols)) {
      throw std::runtime_error{"Could not read symbols from " + file_name};
    }

    /* clang-format off */
    // We are only interested in exported (global) functions
    symbols.erase(
      std::remove_if(symbols.begin(), symbols.end(),
        [](st::Symbol *s) {
          return !s->isFunction() || s->getLinkage() != st::Symbol::SL_GLOBAL;
        }
      )
    );
    /* clang-format on */

    return {parse_abi(symbols, obj->getArchitecture()), std::move(file_name)};
  }
}  // namespace smeagle
