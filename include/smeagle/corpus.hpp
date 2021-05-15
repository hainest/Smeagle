// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include <string>
#include <vector>

#include "smeagle/abi_description.hpp"

namespace smeagle {
  struct corpus final {
    const std::vector<abi_description> functions;
    const std::string file_name;
    std::string to_asp() const;
    std::string to_yaml() const;
    std::string to_json() const;
  };
}  // namespace smeagle
