#pragma once

//
// ... Contextual header files
//
#include <contextual/details/import.hpp>
#include <contextual/details/Contextual.hpp>
#include <contextual/details/Magma.hpp>

namespace Contextual::Details
{

  class Semigroup : public Magma
  {
  public:
    static constexpr auto op_ =
      []<typename F>
      (F&& f){ return f(f); }(
        []<typename Recur>
        (Recur&& recur){
          return
            [recur = forward<Recur>(recur)]<typename T, typename U, typename ... Vs>
            (T&& x, U&& y, Vs&& ... zs){
            if constexpr (count_types<Vs ...>() == 0){
                return op(forward<T>(x), forward<U>(y));
            } else {
              return recur(recur)(op(forward<T>(x), forward<U>(y)), forward<Vs>(zs) ...);
            }
          };
        });
  }; // end of class Semigroup

} // end of namespace Contextual::Details
