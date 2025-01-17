// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#pragma once

#include <string>
#include <utility>

#include "Type.h"
#include "register_class.hpp"

namespace smeagle::x86_64 {

  struct classification {
    RegisterClass lo, hi;
    std::string name;
    int pointer_indirections;
  };

  namespace st = Dyninst::SymtabAPI;

  inline classification classify_pointer(int ptr_cnt) {
    return {RegisterClass::INTEGER, RegisterClass::NO_CLASS, "Pointer", ptr_cnt};
  }

  inline classification classify(st::typeScalar *t) {
    // paramType properties have booleans to indicate types
    auto const &props = t->properties();

    // size in BITS
    const auto size = t->getSize() * 8;

    // Integral types
    if (props.is_integral || props.is_UTF) {
      if (size > 128) {
        return {RegisterClass::SSE, RegisterClass::SSEUP, "IntegerVec"};
      }
      if (size == 128) {
        // __int128 is treated as struct{long,long};
        // This is NOT correct, but we don't handle aggregates yet.
        // How do we differentiate between __int128 and __m128i?
        return {RegisterClass::MEMORY, RegisterClass::NO_CLASS, "Integer"};
      }

      // _Decimal32, _Decimal64, and __m64 are supposed to be SSE.
      // TODO How can we differentiate them here?
      return {RegisterClass::INTEGER, RegisterClass::NO_CLASS, "Integer"};
    }

    if (props.is_floating_point) {
      if (props.is_complex_float) {
        if (size == 128) {
          // x87 `complex long double`
          return {RegisterClass::COMPLEX_X87, RegisterClass::NO_CLASS, "CplxFloat"};
        }
        // This is NOT correct.
        // TODO It should be struct{T r,i;};, but we don't handle aggregates yet
        return {RegisterClass::MEMORY, RegisterClass::NO_CLASS, "CplxFloat"};
      }
      if (size <= 64) {
        // 32- or 64-bit floats
        return {RegisterClass::SSE, RegisterClass::SSEUP, "Float"};
      }
      if (size == 128) {
        // x87 `long double` OR __m128[d]
        // TODO: How do we differentiate the vector type here? Dyninst should help us
        return {RegisterClass::X87, RegisterClass::X87UP, "Float"};
      }
      if (size > 128) {
        return {RegisterClass::SSE, RegisterClass::SSEUP, "FloatVec"};
      }
    }

    // TODO we will eventually want to throw this
    // throw std::runtime_error{"Unknown scalar type"};
    return {RegisterClass::NO_CLASS, RegisterClass::NO_CLASS, "Unknown"};
  }

  inline classification classify(st::typeStruct *) { return {}; }
  inline classification classify(st::typeUnion *) { return {}; }
  inline classification classify(st::typeArray *) { return {}; }
  inline classification classify(st::typeEnum *) {
    return {RegisterClass::INTEGER, RegisterClass::NO_CLASS, "Integer"};
  }
  inline classification classify(st::typeFunction *) { return {}; }

}  // namespace smeagle::x86_64
