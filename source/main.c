#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include "brahma.h"
#include "hid.h"

#ifndef LAUNCHER_PATH
#define LAUNCHER_PATH "Cakes.dat"
#endif

extern u32 __ctru_heap_size;
void __system_initSyscalls(void);
void __system_allocateHeaps(void);
void __system_initArgv(void);
void __appInit(void);
void (*__system_retAddr)(void);
Result __sync_init(void);

void  __libctru_init(void (*retAddr)(void))
{
     __ctru_heap_size = 24*1024*1024;

    // Store the return address
    __system_retAddr = envIsHomebrew() ? retAddr : NULL;

    // Initialize the synchronization subsystem
    __sync_init();

    // Initialize newlib support system calls
    __system_initSyscalls();

    // Allocate application and linear heaps
    __system_allocateHeaps();

    // Build argc/argv if present
    __system_initArgv();
}

int main (void) {
    if (brahma_init()) {
        if (load_arm9_payload_offset("/" LAUNCHER_PATH, 0x12000, 0x10000) != 1)
            goto error;
        firm_reboot();
        brahma_exit();
    }

    // Return to hbmenu
    return 0;

error:
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);
    printf("An error occurred while loading the payload.\nMake sure your launcher is located at:\n/" LAUNCHER_PATH);
    wait_any_key();

    gfxExit();
    return 1;
}
