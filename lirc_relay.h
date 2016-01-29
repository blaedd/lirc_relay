#ifndef __LIRC_RELAY_H__
#define __LIRC_RELAY_H__

#include "platform.h"
#include "ir_recv.h"

#ifdef __cplusplus
extern "C" {
#endif // ifdef __cplusplus

class LircRelay {
public:

  LircRelay(const char *kLircHost,
            uint16_t    port,
            uint8_t     driver_resolution = kDefaultDriverResolution,
            bool        lirc_long_udp = kLircLongUdpDefault);
  void         Send(IrData data);
  virtual void Init(void);

private:

  static const uint8_t kDefaultDriverResolution { 61 };
  static const bool    kLircLongUdpDefault { false };

  const char *lirc_host_;
  uint16_t    lirc_port_;
  uint8_t     driver_resolution_;
  IPAddress   lirc_host_ip_;
  bool lirc_long_udp_;
  UDP  udp_;
};

#ifdef __cplusplus
}
#endif // ifdef __cplusplus
#endif // ifndef __LIRC_RELAY_H__
