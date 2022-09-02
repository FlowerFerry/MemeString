
#ifndef MEGOPP_AUXILIARY_NONCOPYABLE_HPP_INCLUDED
#define MEGOPP_AUXILIARY_NONCOPYABLE_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

namespace megopp {
namespace auxiliary {

class noncopyable
{
protected:
  noncopyable() {}
  ~noncopyable() {}
private:
  noncopyable(const noncopyable&);
  const noncopyable& operator=(const noncopyable&);
};

} // namespace auxiliary
} // namespace megopp


#endif // MEGOPP_AUXILIARY_NONCOPYABLE_HPP_INCLUDED
