#ifndef __IR_RECV_H__
#define __IR_RECV_H__

#include "platform.h"
#include "ringbuffer.h"

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus


enum class IrDataType : std::uint8_t {
  Pulse = 0,
  Space = 1,
  Empty = 2
};

class IrData : public Printable {
public:

  IrDataType type;
  uint32_t   duration;
  virtual size_t printTo(Print& p) const;
};

class IrReceiver {
public:

  IrReceiver(pin_t    recv_pin,
             uint16_t buffer_size = kDefaultBufferSize);
  volatile bool HasData(void);
  void          Put(IrData data);
  IrData        Get(void);
  virtual void  AttachInterrupt(void);
  virtual void  DetachInterrupt(void);
  uint32_t      LastStateChange() {
    return last_time_;
  }

private:

  static const uint16_t kDefaultBufferSize { 256 };
  uint8_t  recv_pin_;
  uint16_t buffer_size_;
  RingBuffer<IrData> buffer_ { 256 };
  IrDataType last_state_ { IrDataType::Empty };
  uint32_t   last_time_
  { 0 };
  virtual void Update();
};

#ifdef __cplusplus
}
#endif // ifdef __cplusplus
#endif // ifndef __IR_RECV_H__
