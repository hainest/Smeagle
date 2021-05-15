// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include <string>

#include "corpus.hpp"

namespace smeagle {

  /**
   * @brief Parse the given file to generate ABI descriptions for each exported function
   * @param file_name the name of the file to parse
   * @return a corpus for the file
   */
  corpus parse(std::string file_name);

}  // namespace smeagle
