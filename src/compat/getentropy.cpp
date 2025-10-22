#include <esp_system.h>
#include <esp_random.h>
#include <errno.h>

extern "C" int getentropy(void* buf, size_t buflen) {
    // POSIX getentropy limits to 256 bytes per call
    if (buf == nullptr) {
        errno = EFAULT;
        return -1;
    }
    if (buflen > 256) {
        errno = EIO;
        return -1;
    }
    // try esp_fill_random if available, otherwise use esp_random to fill
#ifdef __has_include
#if __has_include(<esp_fill_random.h>)
    // unlikely, but try
    esp_fill_random(buf, buflen);
#else
    uint8_t *p = (uint8_t*)buf;
    size_t remaining = buflen;
    while (remaining >= 4) {
        uint32_t r = esp_random();
        *p++ = (r >> 0) & 0xFF;
        *p++ = (r >> 8) & 0xFF;
        *p++ = (r >> 16) & 0xFF;
        *p++ = (r >> 24) & 0xFF;
        remaining -= 4;
    }
    if (remaining > 0) {
        uint32_t r = esp_random();
        for (size_t i = 0; i < remaining; ++i) {
            *p++ = (r >> (8*i)) & 0xFF;
        }
    }
#endif
#else
    // fallback
    uint8_t *p = (uint8_t*)buf;
    size_t remaining = buflen;
    while (remaining >= 4) {
        uint32_t r = esp_random();
        *p++ = (r >> 0) & 0xFF;
        *p++ = (r >> 8) & 0xFF;
        *p++ = (r >> 16) & 0xFF;
        *p++ = (r >> 24) & 0xFF;
        remaining -= 4;
    }
    if (remaining > 0) {
        uint32_t r = esp_random();
        for (size_t i = 0; i < remaining; ++i) {
            *p++ = (r >> (8*i)) & 0xFF;
        }
    }
#endif
    return 0;
}
