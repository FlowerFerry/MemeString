
#ifndef MEGOPP_AUXILIARY_NULL_MUTEX_HPP_INCLUDED
#define MEGOPP_AUXILIARY_NULL_MUTEX_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "megopp/help/noncopyable.h"

namespace megopp {
namespace help {

class null_mutex
  : private noncopyable
{
public:

  // Constructor.
  null_mutex()
  {
  }

  // Destructor.
  ~null_mutex()
  {
  }

  // Lock the mutex.
  void lock()
  {
  }

  // Unlock the mutex.
  void unlock()
  {
  }
};

} 
} // namespace megopp

#endif // MEGOPP_AUXILIARY_NULL_MUTEX_HPP_INCLUDED
