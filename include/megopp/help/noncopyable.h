
#ifndef MEGOPP_AUXILIARY_NONCOPYABLE_HPP_INCLUDED
#define MEGOPP_AUXILIARY_NONCOPYABLE_HPP_INCLUDED

#include <megopp/predef/namespace_alias.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

namespace mgpp {
namespace help {

class noncopyable
{
protected:
  noncopyable() {}
  ~noncopyable() {}
  noncopyable(noncopyable&&) = default;
  noncopyable& operator=(noncopyable&&) = default;
private:
  noncopyable(const noncopyable&) = delete;
  noncopyable& operator=(const noncopyable&) = delete;
};

} 
} // namespace megopp


#endif // MEGOPP_AUXILIARY_NONCOPYABLE_HPP_INCLUDED
