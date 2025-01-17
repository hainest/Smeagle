from optparse import OptionParser
parser = OptionParser()
parser.add_option('-t', '--tests', dest='test_filename', help='Write test cases to FILE', metavar='FILE')
parser.add_option('-f', '--funcs', dest='func_filename', help='Write test functions to FILE', metavar='FILE')
(options, args) = parser.parse_args()

if options.test_filename is None:
    parser.print_help()
    exit()
if options.func_filename is None:
    parser.print_help()
    exit()

# ------------------------------------------ #

types = [
    {
        'category': "Integral Types",
        'types': [
            {'name':'bool',      'type':'Integer',  'res':'%rdi'},
            {'name':'char',      'type':'Integer',  'res':'%rdi'},
            {'name':'short',     'type':'Integer', 'res':'%rdi'},
            {'name':'int',       'type':'Integer', 'res':'%rdi'},
            {'name':'long',      'type':'Integer', 'res':'%rdi'},
            {'name':'long long', 'type':'Integer', 'res':'%rdi'}
        ]
    },
    {
        'category': "Signed Integral Types",
        'types': [
            {'name':'signed',           'type':'Integer', 'res':'%rdi'},
            {'name':'signed char',      'type':'Integer',  'res':'%rdi'},
            {'name':'signed short',     'type':'Integer', 'res':'%rdi'},
            {'name':'signed int',       'type':'Integer', 'res':'%rdi'},
            {'name':'signed long',      'type':'Integer', 'res':'%rdi'},
            {'name':'signed long long', 'type':'Integer', 'res':'%rdi'}
        ]
    },
    {
        'category': "Unsigned Integral Types",
        'types': [
            {'name':'unsigned',           'type':'Integer', 'res':'%rdi'},
            {'name':'unsigned char',      'type':'Integer',  'res':'%rdi'},
            {'name':'unsigned short',     'type':'Integer', 'res':'%rdi'},
            {'name':'unsigned int',       'type':'Integer', 'res':'%rdi'},
            {'name':'unsigned long',      'type':'Integer', 'res':'%rdi'},
            {'name':'unsigned long long', 'type':'Integer', 'res':'%rdi'}
        ]
    },
    {
        'category': "Floating Point Types",
        'types': [
            {'name':'float',                'type':'Float',      'res':'%xmm0'},
            {'name':'double',               'type':'Float',      'res':'%xmm0'},
            {'name':'long double',          'type':'Float',     'res':'framebase+8'},
            {'name':'float _Complex',       'type':'CplxFloat',  'res':'framebase+8'},
            {'name':'double _Complex',      'type':'CplxFloat', 'res':'framebase+8'},
            {'name':'long double _Complex', 'type':'CplxFloat', 'res':'framebase+8'}
        ]
    },
    {
        'category': "UTF Types",
        'types': [
            {'name':'wchar_t',  'type':'Integer', 'res':'%rdi'},
            {'name':'char16_t', 'type':'Integer', 'res':'%rdi'},
            {'name':'char32_t', 'type':'Integer', 'res':'%rdi'}
        ]
    },
    {
        'category': "Size Types",
        'types': [
            {'name':'size_t',    'type':'Integer', 'res':'%rdi'},
            {'name':'intmax_t',  'type':'Integer', 'res':'%rdi'},
            {'name':'uintmax_t', 'type':'Integer', 'res':'%rdi'},
            {'name':'intptr_t',  'type':'Integer', 'res':'%rdi'},
            {'name':'uintptr_t', 'type':'Integer', 'res':'%rdi'}
        ]
    },
    {
        'category': "Fixed-width Integral Types",
        'types': [
            {'name':'int8_t',        'type':'Integer', 'res':'%rdi'},
            {'name':'int16_t',       'type':'Integer', 'res':'%rdi'},
            {'name':'int32_t',       'type':'Integer', 'res':'%rdi'},
            {'name':'int64_t',       'type':'Integer', 'res':'%rdi'},
            {'name':'int_fast8_t',   'type':'Integer', 'res':'%rdi'},
            {'name':'int_fast16_t',  'type':'Integer', 'res':'%rdi'},
            {'name':'int_fast32_t',  'type':'Integer', 'res':'%rdi'},
            {'name':'int_fast64_t',  'type':'Integer', 'res':'%rdi'},
            {'name':'int_least8_t',  'type':'Integer', 'res':'%rdi'},
            {'name':'int_least16_t', 'type':'Integer', 'res':'%rdi'},
            {'name':'int_least32_t', 'type':'Integer', 'res':'%rdi'},
            {'name':'int_least64_t', 'type':'Integer', 'res':'%rdi'}
        ]
    },
    {
        'category': "Unsigned Fixed-width Integral Types",
        'types': [
            {'name':'uint8_t',        'type':'Integer', 'res':'%rdi'},
            {'name':'uint16_t',       'type':'Integer', 'res':'%rdi'},
            {'name':'uint32_t',       'type':'Integer', 'res':'%rdi'},
            {'name':'uint64_t',       'type':'Integer', 'res':'%rdi'},
            {'name':'uint_fast8_t',   'type':'Integer', 'res':'%rdi'},
            {'name':'uint_fast16_t',  'type':'Integer', 'res':'%rdi'},
            {'name':'uint_fast32_t',  'type':'Integer', 'res':'%rdi'},
            {'name':'uint_fast64_t',  'type':'Integer', 'res':'%rdi'},
            {'name':'uint_least8_t',  'type':'Integer', 'res':'%rdi'},
            {'name':'uint_least16_t', 'type':'Integer', 'res':'%rdi'},
            {'name':'uint_least32_t', 'type':'Integer', 'res':'%rdi'},
            {'name':'uint_least64_t', 'type':'Integer', 'res':'%rdi'}
        ]
    }
]

# ------------------------------------------ #

def make_funcs(file, category, types):
    file.write("\n// {}\n".format(category))
    for t in types:
        name = t['name'].replace(' ', '_')
        file.write('extern "C" void test_{0}({1} x){{}}\n'.format(name, t['name']))
        file.write('extern "C" void test_ptr_{0}({1}* x){{}}\n'.format(name, t['name']))
        file.write('extern "C" void test_ptr_ptr_{0}({1}** x){{}}\n'.format(name, t['name']))

def make_tests(file, category, types):
    file.write('TEST_CASE("Register Allocation - {}") {{'.format(category))
    subcase = \
"""
  SUBCASE("{0}") {{
    auto func = get_one(corpus, "test_{1}");
    auto const& parameters = func.parameters;
    CHECK(parameters[0].location() == "{2}");
"""
    for t in types:
        name = t['name'].replace(' ', '_')
        
        # Plain type test
        file.write(subcase.format(t['name'], name, t['res']))
        file.write('    CHECK(parameters[0].class_name() == "{0}");\n'.format(t['type']))
        file.write('  }')
        
        # Pointer indirection test
        for p in [['*', 'ptr_', 1], ['**','ptr_ptr_', 2]]:
            file.write(subcase.format(t['name']+p[0], p[1]+name, '%rdi'))
            file.write('    CHECK(parameters[0].class_name() == "Pointer");\n')
            file.write('  }')
    
    file.write("\n}\n")

def write_headers(test_file, func_file):
    func_file.write(
"""
// Functions to test register allocation
#include <complex.h>
#include <iostream>
""")
    test_file.write(
"""
// Copyright 2013-2021 Lawrence Livermore National Security, LLC and other
// Spack Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (Apache-2.0 OR MIT)

#include <doctest/doctest.h>
#include <string.h>

#include "smeagle/smeagle.h"

auto const& get_one(smeagle::Corpus const& corpus, char const* name) {
  auto const& funcs = corpus.getFunctions();

  return *std::find_if(funcs.begin(), funcs.end(), [name](smeagle::abi_description const& d) {
    return d.function_name == name;
  });
}

namespace {
  auto corpus = smeagle::Smeagle("liballocation.so").parse();
}

"""
    )    

def write_null_tests(test_file, func_file):
    func_file.write(
"""
// Register Allocation - Null Type
extern "C" void test_void(){}
""")

    test_file.write(
"""
TEST_CASE("Register Allocation - Null Types") {
  SUBCASE("void") {
    auto const& func = get_one(corpus, "test_void");
    auto const& parameters = func.parameters;
    CHECK(parameters.size() == 0UL);
  }
}
""")

# ------------------------------------------ #

with open(options.test_filename, 'w') as test_file:
    with open(options.func_filename, 'w') as func_file:
        write_headers(test_file, func_file)
        for t in types:
            make_funcs(func_file, t['category'], t['types'])
            make_tests(test_file, t['category'], t['types'])
        write_null_tests(test_file, func_file)

