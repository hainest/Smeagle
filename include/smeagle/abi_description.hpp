// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include "smeagle/variable.hpp"

namespace smeagle {

  struct abi_description {
    abi_description() = default;
    abi_description(std::vector<variable> &&p, variable &&rv, std::string &&fn)
        : parameters{std::move(p)}, return_value{std::move(rv)}, function_name{std::move(fn)} {}
    const std::vector<variable> parameters;
    variable return_value;
    const std::string function_name;
  };

}  // namespace smeagle
