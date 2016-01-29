
#include "ir_recv.h"
#include "lirc_relay.h"
#include "syslog.h"

SYSTEM_MODE(SEMI_AUTOMATIC);
IrReceiver *lirc_receiver;
LircRelay  *lirc_sender;

#define MYDEBUG syslog.debug

const uint8_t ir_port       = D2;
const char *const kLircHost = "YOUR_LIRC_HOST";
const uint16_t    kLircPort = 5000;
const IPAddress kSyslogHost(192, 168, 1, 1);
const uint16_t kSyslogPort    = 514;
const char *const kSyslogName = "lirc_relay";

Syslog syslog =
  Syslog(kSyslogHost, kSyslogPort, kSyslogName, random(1, 32678));

UDP ctrl;

void system_event_handler(system_event_t event, uint32_t value, void *data)
{
  if (event & wifi_listen_end) {
    WiFi.listen(false);
  }
}

bool cloud = false;

void setup(void) {
  char   buffer[128];
  String s;

  System.on(all_events, system_event_handler);
  Serial.begin(115200);

  WiFi.connect();

  while (!WiFi.ready()) {
    delay(10);
  }
  Particle.connect();

  while (!Particle.connected()) {
    delay(10);
  }
  MYDEBUG("Syncing time");
  Particle.syncTime();
  delay(2000);
  Particle.disconnect();
    MYDEBUG("Disconnected from cloud");

  if (!ctrl.begin(5001)) {
    MYDEBUG("Unable to bind to control port");
  }
  pinMode(ir_port, INPUT_PULLUP);
  lirc_receiver = new IrReceiver(ir_port);
  lirc_receiver->AttachInterrupt();
  lirc_sender = new LircRelay(kLircHost, kLircPort);
  lirc_sender->Init();

  s = "System memory: " + String(System.freeMemory());
  s.toCharArray(buffer, 127);
        MYDEBUG(buffer);
}

void loop(void) {
  IrData   data;
  uint32_t now = millis();

  while (lirc_receiver->HasData()) {
    data = lirc_receiver->Get();

    if (data.type != IrDataType::Empty) {
        MYDEBUG(data);
      lirc_sender->Send(data);
    }
  }
  ctrl.parsePacket();

  if (ctrl.available() > 0) {
        MYDEBUG("Control packet received");
    char c;

    while (ctrl.available() > 0) {
      c = ctrl.read();

      if ((c == 'c') && !cloud) {
        MYDEBUG("connecting to cloud");
        Particle.connect();
        cloud = true;
      } else if ((c == 'd') && cloud) {
        MYDEBUG("disconnecting from cloud");
        Particle.disconnect();
        cloud = false;
      }
    }
  }
}
