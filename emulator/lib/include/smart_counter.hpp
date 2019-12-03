#include <exception>
#ifndef SMART_COUNTER_HPP_
#define SMART_COUNTER_HPP_

// hastily written to help debug....needs way more testing, asserts, and to
// actually be finished
#include <exception>
#include <stdexcept>

#include <exception>
#include <iostream>
#include <stdexcept>
template <class T>
class SmartCounter
{
 public:
  SmartCounter() { upper_limit_ = 0; };
  SmartCounter(T limit) { upper_limit_ = limit; }
  void setLimit(T limit) { upper_limit_ = limit; }
  bool operator<(T const &rhs) { return (value_ < rhs); }
  bool operator>(T const &rhs) { return (value_ > rhs); }
  bool operator<=(T const &rhs) { return (value_ <= rhs); }
  bool operator>=(T const &rhs) { return (value_ >= rhs); }

  SmartCounter operator-(const SmartCounter &rhs)
  {
    value_ -= rhs.value_;
    return *this;
  }

  //  T operator=(SmartCounter const &rhs) { return rhs.value_; }

  SmartCounter &operator=(const SmartCounter &rhs)
  {
    if (rhs.value_ < upper_limit_)
    {
      value_ = rhs.value_;
      return *this;
    }
    else
    {
      throw std::runtime_error("Counter Exceeded Max Value");
    }
  }

  SmartCounter &operator=(const T &rhs)
  {
    if (rhs < upper_limit_)
    {
      value_ = rhs;
      return *this;
    }
    else
    {
      throw std::runtime_error("Counter Exceeded Max Value");
    }
  }

  SmartCounter &operator+=(const SmartCounter &rhs)
  {
    if ((value_ + rhs.value_) < upper_limit_)
    {
      value_ += rhs.value_;
      return *this;
    }

    else
    {
      throw std::runtime_error("Counter Exceeded Max Value");
    }
  }

  SmartCounter &operator+=(const T &rhs)
  {
    if ((value_ + rhs) < upper_limit_)
    {
      value_ += rhs;
      return *this;
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