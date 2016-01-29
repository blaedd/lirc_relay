#ifndef __SYSLOG_H__
#define __SYSLOG_H__

#include "application.h"


class Buffer : public Stream {
public:

  Buffer(size_t s = 128) :
    _buffer(new uint8_t[s]),
    size(s) {}

  size_t write(uint8_t v);
  size_t write(const uint8_t *str,
               size_t         len);

  void   clear();
  size_t length();
  int    available();
  int    read();
  int    peek();
  void   flush();
  using Stream::write;
  operator uint8_t *const() {
    return _buffer;
  }

private:

  uint8_t *_buffer;
  size_t   n { 0 };
  size_t   size;
};

extern UDP syslogUDP;
static const IPAddress SYSLOG_HOST = IPAddress(192, 168, 1, 131);
static const uint16_t  SYSLOG_PORT = 514;
enum class Facility : uint8_t {
  kern     = 0,
  user     = 1,
  mail     = 2,
  daemon   = 3,
  auth     = 4,
  syslog   = 5,
  lpr      = 6,
  news     = 7,
  uucp     = 8,
  authpriv = 10,
  ftp      = 11,
  cron     = 15,
  local0   = 16,
};

enum class Severity : uint8_t {
  emerg   = 0,
  alert   = 1,
  crit    = 2,
  err     = 3,
  warning = 4,
  notice  = 5,
  info    = 6,
  debug   = 7
};


class Syslog {
public:

  Syslog(IPAddress   host,
         uint16_t    port = 514,
         const char *process = "particle",
         uint8_t     pid = 1);
  void begin();
  bool start(Facility fac,
             Severity sev);
  IPAddress syslog_host_;
  const uint16_t syslog_port_;
  const char    *process_;
  uint8_t pid_;
  void   send();
  size_t write(uint8_t n);
  size_t write(const char *str);
  size_t write(const char *str,
               size_t      n);
  size_t print(const Printable& p);
  size_t print(const char[]);
  size_t print(char);
  size_t print(unsigned char,
               int = DEC);
  size_t print(int,
               int = DEC);
  size_t print(unsigned int,
               int = DEC);
  size_t print(long,
               int = DEC);
  size_t print(unsigned long,
               int = DEC);
  size_t print(double,
               int = 2);
  void   debug(const char *str,
               Facility    fac = Facility::daemon);
  void   debug(Printable& p,
               Facility   fac = Facility::daemon);

  /*void   info(const char *str,
              Facility    fac = Facility::daemon);
     void   error(const char *str,
               Facility    fac = Facility::daemon);
     void   warning(const char *str,
                 Facility    fac = Facility::daemon);

     void   info(Printable& p,
              Facility   fac = Facility::daemon);
     void   error(Printable& p,
               Facility   fac = Facility::daemon);
     void   warning(Printable& p,
                 Facility   fac = Facility::daemon);
   */

private:

  Buffer buffer_;
  UDP    udp_;
};
extern Syslog syslog;


#endif // ifndef __SYSLOG_H__
