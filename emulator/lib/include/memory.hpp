#ifndef MEMORY_HPP_
#define MEMORY_HPP_

#include <exception>
#include <stdexcept>
#include <string>
#include <vector>
namespace smart_memory
{
enum class MemoryAttribute
{
  READ_ONLY,
  WRITE_ONLY,
  READ_WRITE
};

struct MemorySegment
{
  std::string name;
  unsigned int size;
  MemoryAttribute mem_attribute;
};

template <class T>
class Memory
{
 public:
  Memory() = default;
  Memory(std::vector<MemorySegment> mem_segs);
  ~Memory() = default;

  void pushSegment(MemorySegment const &mem_seg);

  T readMemory(unsigned int address) const;
  void writeMemory(unsigned int address, T value);

  void flashReadOnlySegment(std::string const &name,
                            std::vector<T> const &value);

  // Proxy class to control assignment operation
  class Proxy
  {
    Memory &mem;
    unsigned int idx;

   public:
    Proxy(Memory<T> &mem_val, unsigned int index) : mem(mem_val), idx(index) {}
    T &operator=(T val)
    {
      mem.writeMemory(idx, val);
      return mem.raw_memory_[idx];
    }
    operator T() const { return mem.readMemory(idx); }
  };

  T operator[](unsigned int index) const { return readMemory(index); }
  Proxy operator[](unsigned int index) { return Proxy(*this, index); }

 private:
  unsigned int findSegmentIndex(unsigned int address) const;
  unsigned int findSegmentIndex(std::string const &name) const;

  std::vector<T> raw_memory_;
  std::vector<MemorySegment> memory_layout;
};

template <typename T>
void swap(typename Memory<T>::Proxy a, T &b)
{
  auto temp = static_cast<T>(a);
  a = b;
  b = temp;
}

template <typename T>
void swap(T &a, typename Memory<T>::Proxy b)
{
  auto temp = static_cast<T>(b);
  b = a;
  a = temp;
}

template <typename T1, typename T2>
void adl_swap(T1 &&a, T2 &&b)
{
  using std::swap;
  swap(std::forward<T1>(a), std::forward<T2>(b));
}

template <class T>
Memory<T>::Memory(std::vector<MemorySegment> mem_segs)
{
  unsigned int size = 0;
  for (auto const &seg : mem_segs)
  {
    memory_layout.emplace_back(seg);
    size += seg.size;
  }
  raw_memory_.resize(size);

  for (auto &val : raw_memory_)
  {
    val = 0;
  }
}

template <class T>
void Memory<T>::flashReadOnlySegment(std::string const &name,
                                     std::vector<T> const &value)
{
  auto index = findSegmentIndex(name);
  if (value.size() > memory_layout[index].size)
  {
    throw std::runtime_error("Memory access out of bounds");
  }

  unsigned int base = 0;

  for (unsigned int i = 0; i < index; i++)
  {
    base += memory_layout[i].size;
  }

  for (unsigned int i = 0; i < memory_layout[index].size; i++)
  {
    raw_memory_[i + base] = 0;
  }

  for (unsigned int i = 0; i < value.size(); i++)
  {
    raw_memory_[i + base] = value[i];
  }
}

template <class T>
void Memory<T>::pushSegment(MemorySegment const &mem_seg)
{
  memory_layout.emplace_back(mem_seg);
  auto old_size = raw_memory_.size();
  raw_memory_.resize(raw_memory_.size() + mem_seg.size);

  for (auto i = old_size; i < raw_memory_.size(); i++)
  {
    raw_memory_[i] = 0;
  }
}

template <class T>
unsigned int Memory<T>::findSegmentIndex(std::string const &name) const
{
  for (unsigned int i = 0; i < memory_layout.size(); i++)
  {
    if (name == memory_layout[i].name)
    {
      return i;
    }
  }
  throw std::runtime_error("Memory Segment not found.");
}

template <class T>
unsigned int Memory<T>::findSegmentIndex(unsigned int address) const
{
  unsigned int base = 0;
  for (unsigned int i = 0; i < memory_layout.size(); i++)
  {
    if (address < (base + memory_layout[i].size))
    {
      return i;
    }
    base += memory_layout[i].size;
  }
  throw std::runtime_error("Memory Segment not found.");
}

template <class T>
T Memory<T>::readMemory(unsigned int address) const
{
  if (address >= raw_memory_.size())
  {
    throw std::runtime_error("Memory access out of bounds");
  }

  if (memory_layout.at(findSegmentIndex(address)).mem_attribute ==
      MemoryAttribute::WRITE_ONLY)
  {
    throw std::runtime_error("Memory is Write Only");
  }

  return raw_memory_[address];
}

template <class T>
void Memory<T>::writeMemory(unsigned int address, T value)
{
  if (address >= raw_memory_.size())
  {
    throw std::runtime_error("Memory access out of bounds");
  }

  if (memory_layout.at(findSegmentIndex(address)).mem_attribute ==
      MemoryAttribute::READ_ONLY)
  {
    throw std::runtime_error("Memory is Read Only");
  }

  raw_memory_[address] = value;
}

}  // namespace smart_memory
#endif  // MEMORY_HPP_