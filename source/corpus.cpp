// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include "smeagle/corpus.hpp"

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace smeagle {

  std::string corpus::to_asp() const {
    std::stringstream str;
    str << "corpus(" << file_name << ")," << '\n';

    for (auto const& f : functions) {
      str << f.function_name << '\n';
      for (auto const& p : f.parameters) {
        str << '\t' << p.name << "(" << p.type_name << ")(" << p.location << ")\n";
      }
      str << '\n';
    }
    return str.str();
  }

  std::string corpus::to_yaml() const {
    std::stringstream str;

    //    std::cout << "library: \"" << library << "\"\nlocations: " << std::endl;
    //    for (auto &typeloc : typelocs) {
    //
    //        std::cout << " - library: " << library << "\n   parent: " << typeloc.parent
    //                  << "\n   name: " << typeloc.name << "\n   type: " << typeloc.type
    //                  << "\n   location: " <<  typeloc.location << "\n   register: "
    //                  << typeloc.reg << "\n" << std::endl;
    //    }
    return str.str();
  }

  std::string corpus::to_json() const {
    std::stringstream str;

    //    std::cout << "{ \"library\": \"" << library << "\", \"locations\": [" << std::endl;
    //    for (auto &typeloc : typelocs) {
    //
    //        // Check if we are at the last entry (no comma) or not
    //        std::string endcomma;
    //        if (&typeloc == &typelocs.back())
    //            endcomma = "";
    //        else {
    //            endcomma = ",";
    //        }
    //        std::cout << "{\"library\": \"" << library << "\", \"parent\": \""
    //                  << typeloc.parent << "\", \"name\": \"" << typeloc.name << "\", \"type\":
    //                  \""
    //                  << typeloc.type << "\", \"location\": \"" << typeloc.location << "\", "
    //                  << "\"register\": \"" << typeloc.reg << "\"}"
    //                  << endcomma << std::endl;
    //    }
    //    std::cout << "]}" << std::endl;
    return str.str();
  }
}  // namespace smeagle
