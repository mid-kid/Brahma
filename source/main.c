#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include "brahma.h"
#include "hid.h"

#ifndef LAUNCHER_PATH
#define LAUNCHER_PATH "Cakes.dat"
#endif

int main (void) {
    // Initialize services
    gfxInitDefault();

    // Make sure the settings applied by gfxInitDefault come into effect
    gfxSwapBuffers();

    // Load arm9 payload from file
    const u32 max_size = 0x10000;
    const u32 offset = 0x12000;
    if (brahma_init()) {
        int rc = load_arm9_payload("/" LAUNCHER_PATH, offset, max_size);
        if (rc != 1) goto error;
        firm_reboot();
        brahma_exit();
    }

    gfxExit();
    // Return to hbmenu
    return 0;

error:
    consoleInit(GFX_BOTTOM, NULL);
    printf("An error occurred while loading the payload.\n"
            "Make sure your launcher is located at:\n"
            "/" LAUNCHER_PATH);
    wait_any_key();
    gfxExit();
    return 1;
}
