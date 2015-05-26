#pragma once
#include <SDL2/SDL_net.h>

class Network {
public:
    /// default port is 27015 and default packet size is 32000 bytes (256 kilobits).
    Network(Uint16 portVal = 27015, int packetSize = 32000);
    ~Network();

    /// network loop containing listeners for packets.
    void updateNetwork();

private:
    Uint16 port;
    int packSize;
    UDPsocket socket;
    UDPpacket* pack;
};
