#include "syslog.h"

size_t Buffer::write(uint8_t v) {
  _buffer[n++] =  v;
  return 1;
}

size_t Buffer::write(const uint8_t *str, size_t len) {
  if (len + n > size) {
    len = size - n;
  }
  memcpy(&_buffer[n], str, len);
  n += len;
  return len;
}

void Buffer::clear() {
  memset(_buffer, '\0', size);
  n = 0;
}

size_t Buffer::length() {
  return n;
}

int Buffer::available() {
  return 0;
}

int Buffer::read() {
  return -1;
}

int Buffer::peek() {
  return -1;
}

void Buffer::flush() {}

Syslog::Syslog(IPAddress   host,
               uint16_t    port,
               const char *process,
               uint8_t     pid) :
  syslog_host_(host),
  syslog_port_(port),
  process_(process),
  pid_(pid),
  buffer_(256)   {}

bool Syslog::start(Facility fac, Severity sev) {
  uint8_t priority =  (uint8_t)fac * 8 + (uint8_t)sev;

  buffer_.write('<');
  buffer_.print(priority);
  buffer_.write('>');
  buffer_.print(WiFi.localIP());
  buffer_.write(' ');
  buffer_.write(process_);
  buffer_.write('[');
  buffer_.print(pid_);
  buffer_.write("]: ");
  return true;
}

void Syslog::send() {
  udp_.begin(syslog_port_);
  udp_.beginPacket(syslog_host_, syslog_port_);
  udp_.write((const uint8_t *)buffer_, buffer_.length());
  udp_.endPacket();
  udp_.stop();
  buffer_.clear();
}

size_t Syslog::write(uint8_t n) {
  return buffer_.write(n);
}

size_t Syslog::write(const char *str) {
  return buffer_.write(str);
}

size_t Syslog::write(const char *str, size_t n) {
  return buffer_.write((const uint8_t *)str, n);
}

size_t Syslog::print(const Printable& p) {
  return buffer_.print(p);
}

size_t Syslog::print(const char p[]) {
  return buffer_.print(p);
}

size_t Syslog::print(char p) {
  return buffer_.print(p);
}

size_t Syslog::print(unsigned char p, int base) {
  return buffer_.print(p, base);
}

size_t Syslog::print(int p, int base) {
  return buffer_.print(p, base);
}

size_t Syslog::print(unsigned int p, int base) {
  return buffer_.print(p, base);
}

size_t Syslog::print(long p, int base) {
  return buffer_.print(p, base);
}

size_t Syslog::print(unsigned long p, int base) {
  return buffer_.print(p, base);
}

size_t Syslog::print(double p, int base) {
  return buffer_.print(p, base);
}

void Syslog::debug(const char *str, Facility fac) {
  start(fac, Severity::debug);
  buffer_.write(str);
  send();
}

void Syslog::debug(Printable& p, Facility fac) {
  start(fac, Severity::debug);
  buffer_.print(p);
  send();
}
