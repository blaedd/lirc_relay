#ifndef RING_BUFFER
#define RING_BUFFER

#include <stdlib.h>
#include <math.h>
#include <cstdint>
#include "syslog.h"
template<typename T>
class RingBuffer {
private:

  uint16_t buffer_size_;
  T *volatile buffer_;
  volatile uint16_t head_ { 0 }; // next position to write to.
  volatile uint16_t tail_;       // last position read from
  volatile uint16_t overflow_pos_ { 0 };
  volatile bool     overflow_ { false };

public:

  static const uint16_t kDefaultBufferSize = 256;

  RingBuffer(const uint16_t buffer_size = kDefaultBufferSize) :
    buffer_size_(buffer_size), buffer_(new T[buffer_size]),
    tail_(buffer_size - 1)
  {}

  RingBuffer(const RingBuffer& rhs)
  {
    *this = rhs;
  }

  RingBuffer& operator=(const RingBuffer& rhs)
  {
    if (this != &rhs)
    {
      head_        = rhs.head_;
      tail_        = rhs.tail_;
      buffer_size_ = rhs.buffer_size_;
      delete buffer_;
      buffer_ = new T[buffer_size_];

      for (uint16_t i = 0; i < buffer_size_; i++) buffer_[i] = rhs.buffer_[i];
    }
    return *this;
  }

  ~RingBuffer()
  {
    free((void *)buffer_);
  }

  bool Empty()
  {
    return (head_ == ((tail_ + 1) % buffer_size_)) && !overflow_;
  }

  void Put(const T value)
  {
    buffer_[head_] = value;
    uint16_t old_head = head_;

    head_ = (head_ + 1) % buffer_size_;

    if (old_head == tail_)
    {
      overflow_pos_ = old_head;
      overflow_     = true;
    }
  }

  T Get()
  {
    T value;

    if (overflow_)
    {
      tail_     = overflow_pos_;
      overflow_ = false;
    }
    tail_ = (tail_ + 1) % buffer_size_;
    syslog.start(Facility::daemon, Severity::debug);
    syslog.write("tail: ");
    syslog.print((uint16_t)tail_);
    syslog.write(" overflow:");
    syslog.print(overflow_);
    syslog.write(" buffer_size: ");
    syslog.print(buffer_size_);
    syslog.send();

    value = buffer_[tail_];
    return value;
  }

  uint16_t buffer_size() const
  {
    return buffer_size_;
  }
};
#endif // ifndef RING_BUFFER

// vim:ft=arduino
