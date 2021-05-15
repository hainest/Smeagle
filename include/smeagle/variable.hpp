// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include <string>

namespace smeagle {
  struct variable {
    variable() = default;
    variable(std::string n, std::string tn, std::string l, bool e)
        : name{std::move(n)}, type_name{std::move(tn)}, location{std::move(l)}, exported{e} {}
    const std::string name;
    const std::string type_name;
    const std::string location;
    const bool exported{true};
  };
}  // namespace smeagle
