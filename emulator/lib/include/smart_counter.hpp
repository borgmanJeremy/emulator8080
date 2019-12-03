#include <exception>
#ifndef SMART_COUNTER_HPP_
#define SMART_COUNTER_HPP_

// hastily written to help debug....needs way more testing, asserts, and to
// actually be finished
template <class T>
class SmartCounter
{
 public:
  SmartCounter() { upper_limit_ = 0; };
  SmartCounter(T limit) { upper_limit_ = limit; }
  void setLimit(T limit) { upper_limit_ = limit; }

  void operator=(const T& rhs)
  {
    if (rhs < upper_limit_)
    {
      value_ = rhs;
    }
    else
    {
      throw std::runtime_error("Counter Exceeded Max Value");
    }
  }
  void operator+=(const T& rhs)
  {
    if ((value_ + rhs) < upper_limit_)
    {
      value_ += rhs;
    }
    else
    {
      throw std::runtime_error("Counter Exceeded Max Value");
    }
  }

  T value_ = 0;
  T upper_limit_;
};

#endif  // SMART_COUNTER_HPP_