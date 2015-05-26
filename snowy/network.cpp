#include "network.h"
#include "tools.h"

Network::Network(Uint16 portVal, int packetSize) {
    if(SDLNet_Init() < 0) {
        std::string errmsg = "SDLNet_Init error: " + std::string(SDLNet_GetError());
		callError(errmsg);
    }

    if(!(socket = SDLNet_UDP_Open(portVal))) {
        std::string errmsg = "SDLNet_UDP_Open error: " + std::string(SDLNet_GetError());
		callError(errmsg);
    }

    if(!(pack = SDLNet_AllocPacket(packetSize))) {
        std::string errmsg = "SDLNet_AllocPacket error: " + std::string(SDLNet_GetError());
        callError(errmsg);
    }

    struct packageInfo{
        // 2,147,483,647 highest signed number
        int sequenceNum;
        // whether the package got acknowledged or not.
        bool gotAcked;
        // 1000 milliseconds, if it is <=0 on next update resend package and reset timeout to 1000ms
        float timeout;
    };


}

Network::~Network() {
    SDLNet_FreePacket(pack);
    SDLNet_Quit();
}

void Network::updateNetwork() {
    if(SDLNet_UDP_Recv(socket, pack)) {

    }
}
