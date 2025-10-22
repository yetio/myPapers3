#ifndef SYNC_MANAGER_H
#define SYNC_MANAGER_H

#include <Arduino.h>

enum SyncState {
    SYNC_IDLE,
    SYNC_RUNNING,
    SYNC_ERROR
};

void sync_manager_init();
void sync_manager_loop(); // call periodically from main loop
SyncState sync_manager_get_state();
String sync_manager_get_last_error();

#endif
