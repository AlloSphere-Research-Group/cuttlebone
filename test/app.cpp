#include "Framework/HostRole.hpp"
#include "Framework/Selector.hpp"
#include "Framework/Broadcaster.hpp"
#include "Framework/Timer.hpp"
#include "Framework/Checksum.hpp"

struct App : HostRole, Selector<App>, Broadcaster, Timer, Checksum {
  unsigned char* buffer;
  unsigned char* b;
  unsigned packetSize;

  void init(unsigned packetSize, unsigned timerPeriod, unsigned selectTimeout,
            const char* ip, unsigned port) {
    HostRole::init();

    this->packetSize = packetSize;
    buffer = new unsigned char[packetSize];
    b = new unsigned char[packetSize];

    if (isSelector) Selector<App>::start(packetSize, selectTimeout, port);
    if (isBroadcaster) {
      Broadcaster::init(packetSize, ip, port);
      Timer::start(timerPeriod);
    }
  }

  void onTimer() {
    printf("send:%03u ", b[0]);
    checksum(b, packetSize);
    print();
    send(b);
    b[0]++;
  }

  void onNewBuffer() {
    printf("recv:%03u ", buffer[0]);
    checksum(buffer, packetSize);
    print();
  }
};

int main(int argc, char* argv[]) {
  unsigned packetSize = 512, timerPeriod = 800000, selectTimeout = 900000;
  const char* ip = "127.0.0.1";
  unsigned port = 8888;
  if (argc > 1) packetSize = atoi(argv[1]);
  if (argc > 2) timerPeriod = atof(argv[2]);
  if (argc > 3) selectTimeout = atoi(argv[3]);
  if (argc > 4) ip = argv[4];
  if (argc > 5) port = atoi(argv[5]);

  printf("packetSize:%u timerPeriod:%u selectTimeout:%u ip:%s port:%u\n",
         packetSize, timerPeriod, selectTimeout, ip, port);

  App app;
  app.init(packetSize, timerPeriod, selectTimeout, ip, port);
  getchar();
}