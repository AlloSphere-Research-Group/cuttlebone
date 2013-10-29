#include <cassert>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

#include <string>

int main() {
  sockaddr_in si_me, si_other;
  int s;
  assert((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) != -1);
  int port = 8888;
  int broadcast = 1;

  setsockopt(s, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast);

  memset(&si_me, 0, sizeof(si_me));
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(port);
  si_me.sin_addr.s_addr = INADDR_ANY;

  assert(::bind(s, (sockaddr *)&si_me, sizeof(sockaddr)) != -1);

  while (1) {
    char buf[10000];
    unsigned slen = sizeof(sockaddr);
    recvfrom(s, buf, sizeof(buf) - 1, 0, (sockaddr *)&si_other, &slen);

    printf("recv: %s\n", buf);
  }
}
