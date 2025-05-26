#ifndef SD_GATEWAY_H
#define SD_GATEWAY_H

#include <stdint.h>
class WebServer;

namespace sd_gateway {
    bool isActive();
    void toggleOrShow();
    void startServer();
    void stopServer();
    uint16_t getPort();
    void loop();
}

#endif // SD_GATEWAY_H 