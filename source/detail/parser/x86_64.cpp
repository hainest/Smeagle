// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <iostream>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "Function.h"
#include "Symtab.h"
#include "parser.hpp"
#include "smeagle/abi_description.hpp"
#include "smeagle/variable.hpp"

namespace smeagle::x86_64 {

  namespace st = Dyninst::SymtabAPI;

  // A register class for AMD64 is defined on page 16 of the System V abi pdf
  enum class RegisterClass {
    INTEGER,      // Integer types that fit into one of the general purpose registers
    SSE,          // Types that fit into an SSE register
    SSEUP,        // ^.. and can ve passed and returned in he most significant half of it
    X87,          // Types that will be returned via the x87 FPU
    X87UP,        // ^
    COMPLEX_X87,  // Types that will be returned via the x87 FPU
    NO_CLASS,     // Initalizer in the algorithms, used for padding and empty
                  // structures and unions
    MEMORY        // Types that will be passed and returned in memory via the stack
  };

  /* --- Internal functions --- */
  static std::pair<RegisterClass, RegisterClass> classify(st::Type *);

  // These are just placeholders. The signatures are not likely correct.
  static void merge(RegisterClass, RegisterClass);
  static void merge_cleanup(RegisterClass, RegisterClass);
  static bool is_aggregate(st::localVar *);
  static std::string allocate(RegisterClass, RegisterClass);
  static bool is_exported(st::localVar *);
  /* -------------------------- */

  // parse a function for parameters and abi location
  std::vector<variable> parse_parameters(st::Symbol const *symbol) {
    st::Function *func = symbol->getFunction();

    std::vector<st::localVar *> params;
    if (!func->getParams(params)) {
      return {};
    }

    std::vector<variable> parameters;

    for (auto *p : params) {
      // classify
      const auto [lo, hi] = classify(p->getType());

      // merge (aggregates only)
      if (is_aggregate(p)) {
        merge(hi, lo);
        merge_cleanup(hi, lo);
      }

      // allocate register/memory space
      auto location = allocate(hi, lo);

      parameters.emplace_back(p->getName(), p->getType()->getName(), std::move(location),
                              is_exported(p));
    }

    return parameters;
  }

  variable parse_return_value(st::Symbol const *symbol) { return {}; }
  // Get register class given the argument type
  static std::pair<RegisterClass, RegisterClass> classify(st::Type *paramType) {
    // We need a string version of the type
    std::string const &paramTypeString = paramType->getName();

    /*
     * 	I'm not sure if these regexes are enough to detect the type names
     *
     * 	Dyninst uses the DWARF DW_AT_name_attribute from the DW_AT_base_type DIE
     * 	as its name string. We should verify that these values reflect what's in
     * 	the DWARF standard (4 and 5) for those attributes.
     *
     */

    // Signed and unsigned Bool,char,short,int,long,long long, and pointers
    std::regex checkinteger("(int|char|short|long|pointer|bool)");

    // float,double,_Decimal32,_Decimal64and__m64are in class SSE.
    std::regex checksse("(double|decimal|float|Decimal|m64)");

    // __float128 and__m128are split into two halves, least significant SSE,
    // most significant in SSEUP.
    std::regex checksseup("(m128|float128)");

    // The 64-bit mantissa of arguments of type long double belongs to classX87
    // the 16-bit exponent plus 6 bytes of padding belongs to class X87UP
    std::regex checklongdouble("(long|double)");

    // A variable of type complex long double is classified as type COMPLEX_X87
    std::regex checkcomplexlong("(complex long double)");

    // Does the type string match one of the types?
    bool isinteger = (std::regex_search(paramTypeString, checkinteger));
    bool issse = (std::regex_search(paramTypeString, checksse));
    bool issseup = (std::regex_search(paramTypeString, checksseup));
    bool islongdouble = (std::regex_search(paramTypeString, checklongdouble));
    bool iscomplexlong = (std::regex_search(paramTypeString, checkcomplexlong));

    if (issseup) {
      return {RegisterClass::SSE, RegisterClass::SSEUP};
    }
    if (issse) {
      return {RegisterClass::SSE, RegisterClass::NO_CLASS};
    }
    if (isinteger) {
      return {RegisterClass::SSE, RegisterClass::NO_CLASS};
    }

    // These two types can be split, so we need more work here
    if (iscomplexlong) {
      return {RegisterClass::COMPLEX_X87, RegisterClass::NO_CLASS};
    }
    if (islongdouble) {
      return {RegisterClass::X87, RegisterClass::X87UP};
    }

    // The classification of aggregate (structures and arrays) and union types worksas follows:
    // TODO need to look for struct / arrays?
    // page 18 of abi document
    return {RegisterClass::NO_CLASS, RegisterClass::NO_CLASS};
  }

  static void merge(RegisterClass, RegisterClass) {}
  static void merge_cleanup(RegisterClass, RegisterClass) {}
  static std::string allocate(RegisterClass, RegisterClass) { return "?somewhere?"; }
  static bool is_exported(st::localVar *) { return false; }
  static bool is_aggregate(st::localVar *) { return false; }

}  // namespace smeagle::x86_64
