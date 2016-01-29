#include "ir_recv.h"

size_t IrData::printTo(Print& p) const
{
  size_t n = 0;
  char   buffer[32];
  String s;

  switch (type)
  {
  case IrDataType::Space:
    s = String("space ") + String(duration);
    break;

  case IrDataType::Pulse:
    s = String("pulse ") + String(duration);
    break;

  default:
    return 0;
  }
  s.toCharArray(buffer, 32);
  n = p.print(buffer);
  return n;
}

IrReceiver::IrReceiver(pin_t    recv_pin,
                       uint16_t buffer_size)
  :
    recv_pin_(recv_pin),
    buffer_size_(buffer_size),

//  buffer_(new RingBuffer<IrData>[buffer_size]),
    last_time_(micros())
{
  pinMode(recv_pin, INPUT);
}

volatile bool IrReceiver::HasData(void) {
  return !buffer_.Empty();
}

void IrReceiver::Put(IrData data) {
  buffer_.Put(data);
}

IrData IrReceiver::Get(void) {
  return buffer_.Get();
}

void IrReceiver::Update(void) {
  IrData data;
  uint32_t   now           = micros();
  IrDataType current_state = (IrDataType)digitalRead(recv_pin_);

  if (current_state != last_state_) {
    data.duration = now - last_time_;
    data.type     = (IrDataType)last_state_;
    buffer_.Put(data);
    last_state_ = current_state;
    last_time_  = now;
  }
}

void IrReceiver::AttachInterrupt(void) {
  auto irInterruptHandler = std::bind(&IrReceiver::Update, this);

  attachInterrupt(recv_pin_, irInterruptHandler, CHANGE);
}

void IrReceiver::DetachInterrupt(void) {
  detachInterrupt(recv_pin_);
}
