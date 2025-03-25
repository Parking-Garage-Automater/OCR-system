#ifndef ESP32EYE_RESOLUTION_HPP
#define ESP32EYE_RESOLUTION_HPP

#include <Printable.h>
#include <iterator>

namespace esp32eye {

class ResolutionList;

class Resolution : public Printable
{
public:
  static ResolutionList list();

  static Resolution find(int minWidth, int minHeight);

  explicit Resolution(int frameSize = -1);

  bool isValid() const;

  int getWidth() const;

  int getHeight() const;

  size_t printTo(Print& p) const override;

  template<typename T>
  T as() const
  {
    return static_cast<T>(m_frameSize);
  }

private:
  int m_frameSize;

  friend bool operator==(const Resolution& lhs, const Resolution& rhs)
  {
    return (!lhs.isValid() && !rhs.isValid()) || (lhs.m_frameSize == rhs.m_frameSize);
  }

  friend bool operator!=(const Resolution& lhs, const Resolution& rhs)
  {
    return !(lhs == rhs);
  }

  friend bool operator<(const Resolution& lhs, const Resolution& rhs)
  {
    return lhs.m_frameSize < rhs.m_frameSize;
  }

  friend bool operator>(const Resolution& lhs, const Resolution& rhs)
  {
    return lhs.m_frameSize > rhs.m_frameSize;
  }
};

class ResolutionList
{
public:
  class Iterator
  {
  public:
    using iterator_catagory = std::forward_iterator_tag;
    using value_type = const Resolution;
    using difference_type = std::ptrdiff_t;
    using pointer = void;
    using reference = value_type;

    explicit Iterator(int value = -1)
      : m_frameSize(value)
    {
    }

    Iterator& operator++()
    {
      ++m_frameSize;
      return *this;
    }

    Iterator operator++(int)
    {
      Iterator copy(*this);
      ++*this;
      return copy;
    }

    reference operator*()
    {
      return Resolution(m_frameSize);
    }

  private:
    int m_frameSize;

    friend bool operator==(const Iterator& lhs, const Iterator& rhs)
    {
      return lhs.m_frameSize == rhs.m_frameSize;
    }

    friend bool operator!=(const Iterator& lhs, const Iterator& rhs)
    {
      return !(lhs == rhs);
    }
  };

  explicit ResolutionList(int max = 0);

  Iterator begin() const;

  Iterator end() const;

  Resolution find(int minWidth, int minHeight) const;

private:
  int m_max;
};

} // namespace esp32eye

#endif // ESP32EYE_RESOLUTION_HPP
