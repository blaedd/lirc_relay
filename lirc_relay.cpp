#include "lirc_relay.h"


LircRelay::LircRelay(const char *kLircHost,
                     uint16_t    port,
                     uint8_t     driver_resolution,
                     bool        lirc_long_udp)
  : lirc_host_(kLircHost),
    lirc_port_(port),
    driver_resolution_(driver_resolution),
    lirc_long_udp_(lirc_long_udp) {
  Init();
}

void LircRelay::Init(void) {
  lirc_host_ip_ = IPAddress(192, 168, 1, 193);
}

void LircRelay::Send(IrData data) {
  uint8_t  buffer[6];
  uint32_t duration = data.duration;
  uint8_t  mask     = 0;

  switch (data.type) {
  case IrDataType::Pulse:
    mask = 0;
    break;

  case IrDataType::Space:
    mask = 0x80;
    break;

  default:
    return;
  }
  duration /= driver_resolution_;
  udp_.begin(lirc_port_);
  udp_.beginPacket(lirc_host_ip_, lirc_port_);

  if (!lirc_long_udp_) {
    duration = duration & 0x7fff;
  }

  if (duration <= 0x8000) {
    buffer[0] = 0x00ff & duration;
    buffer[1] = (duration >> 8) | mask;
    udp_.write(buffer, 2);
  } else {
    buffer[0] = 0; // duration 0, indicates a 32bit value follows;
    buffer[1] = mask;
    memcpy(&buffer[2], &duration, 4);
    udp_.write(buffer, 6);
  }
  udp_.endPacket();
  udp_.stop();
}
