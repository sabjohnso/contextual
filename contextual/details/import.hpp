#pragma once

//
// ... Standard header files
//
#include <utility>
#include <variant>
#include <type_traits>
#include <concepts>

//
// ... External header files
//
#include <type_utility/type_utility.hpp>
#include <function_utility/function_utility.hpp>

namespace Contextual::Details
{
  // concepts
  using std::invocable;

  using std::variant;

  // traits
  using std::false_type;
  using std::true_type;
  using std::decay_t;
  using std::invoke_result_t;
  using std::is_same_v;
  using std::conditional_t;

  using std::pair;
  using std::move;
  using std::forward;

  using TypeUtility::Nat;
  using TypeUtility::nat;
  using TypeUtility::count_types;
  using TypeUtility::Type;
  using TypeUtility::type;

  using FunctionUtility::curry;
  using FunctionUtility::compose;
  using FunctionUtility::part;
  using FunctionUtility::rpart;
  using FunctionUtility::Static_callable;
  using FunctionUtility::Static_curried;

} // end of namespace Contextual::Details
