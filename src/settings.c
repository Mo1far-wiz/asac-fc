#include "settings.h"
#include "machine.h"
#include "asac_fc.h"

#include <hardware/sync.h>
#include <hardware/flash.h>
#include <pico/multicore.h>

// Inspired by: https://www.makermatrix.com/blog/read-and-write-data-with-the-pi-pico-onboard-flash/


#define FLASH_TARGET_OFFSET    (ASAC_FC_FLASH_SIZE - FLASH_SECTOR_SIZE)
#define FLASH_PAGES_PER_SECTOR (FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE)

#define SETTINGS_NBR_OF_FLASH_PAGES ((sizeof(system_settings_t) / FLASH_PAGE_SIZE) + 1)
#define SETTINGS_FLASH_SIZE (SETTINGS_NBR_OF_FLASH_PAGES * FLASH_PAGE_SIZE)


static int* settings_hash_ptr     = (int*)        (XIP_BASE + FLASH_TARGET_OFFSET);
static system_settings_t* flash_settings = (system_settings_t*) (XIP_BASE + FLASH_TARGET_OFFSET + 4);

static uint8_t flash_buf[SETTINGS_FLASH_SIZE];

static int calculate_simple_hash(const system_settings_t* settings);

system_settings_t system_settings;

//#define USE_DEFAULT_SETTINGS


int settings_init() {
    #ifdef USE_DEFAULT_SETTINGS
        printf("Using default settings!\n");
        settings_reset_default();
    #else
        int flash_settings_hash = *settings_hash_ptr;
        int hash = calculate_simple_hash(flash_settings);


        if (hash != flash_settings_hash) {
            printf("settings.c\t-41: Hash calculation for settings failed, using default...!\n");
            printf("settings.c\t-42: Expected %d, got: %d\n", hash, flash_settings_hash);
            settings_reset_default();
        } else {
            // Copy settings from FLASH to RAM
            memcpy(&system_settings, flash_settings, sizeof(system_settings_t));
        }
    #endif

        printf("settings.c\t-50: Returning 0\n");
    return 0;
}

void settings_write_to_flash(const system_settings_t* settings) {
    printf("settings.c\t-54: Writing settings to flash\n");

    int flash_settings_hash = calculate_simple_hash(settings);
    printf("settings.c\t-57: Setting hash calculated : %d\n", flash_settings_hash);

    // Prepare settings buffer
    memcpy(&flash_buf[0], &flash_settings_hash, 4);
    memcpy(&flash_buf[4], settings, sizeof(system_settings_t));

    // Put primary core in blocked state before we write the flash
    // Inspired by this post: https://forums.raspberrypi.com/viewtopic.php?t=311709
    printf("settings.c\t-65: Multicore lockout blocking start\n");
    multicore_lockout_start_blocking();

    uint32_t uint31_status = save_and_disable_interrupts();
    int int_status = (int)uint31_status;
    // For some reason we need to erase the flash before writing, not sure why.
    flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
    flash_range_program(FLASH_TARGET_OFFSET, flash_buf, SETTINGS_FLASH_SIZE);
    restore_interrupts(int_status);

    // Put primary core to running again
    printf("settings.c\t-76: Put primary core to running again\n");
    multicore_lockout_end_blocking();
}

void settings_reset_default() {
    memcpy(&system_settings, &default_system_params, sizeof(system_settings_t));
    settings_write_to_flash(&system_settings);
}

static int calculate_simple_hash(const system_settings_t* settings) {
    int hash = 0;
    for (int byte = 0; byte < sizeof(system_settings_t); byte++) {
        hash ^= ((uint8_t*) settings)[byte];
    }
    return hash;
}
