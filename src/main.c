#include "asac_fc.h"
#include "motor.h"
#include "receiver.h"
#include "imu.h"
#include "vsrtos.h"
#include "controller.h"
#include "led.h"
#include "battery_adc.h"
#include "settings.h"
#include "state.h"
#include "telemetry.h"

#include <pico/stdio.h>
#include <pico/multicore.h>


static void init_driver(int (*init_function)(), const char* name);

static void go_to_error_during_startup();

static void core1_entry();

static int init_result = 0;


int main() {
    stdio_init_all();

    // Initialize all drivers
    state.mode = MODE_BOOTING;
    printf("Booting up...\n");
    printf("Initializing drivers\n");

    // Initialize LED driver and blink led boot-up sequence
    init_driver(led_init, "Led");
    led_run_boot_sequence();

    // Turn RED led high to indicate we're booting
    led_set(LED_RED, 1);

    init_driver(settings_init,    "Settings");
    init_driver(receiver_init,    "Receiver");
    init_driver(battery_adc_init, "Battery ADC");
    init_driver(imu_init,         "IMU");
    init_driver(controller_init,  "Controller");
    init_driver(motors_init,      "Motors");
    init_driver(telemetry_init,   "Telemetry");

    // Done booting
    led_set(LED_RED, 0);

    if (init_result != 0) {
        state.mode = MODE_ERROR;
        go_to_error_during_startup();
    }

    // Create tasks
    //vsrtos_create_task(controller_debug, "Controller debug", 10, 1);
    vsrtos_create_task(controller_update, "Controller", 1000, 1);
    vsrtos_create_task(controller_set_motors, "Motor Controller", 490, 1);

    state.mode = MODE_IDLE;

    multicore_launch_core1(core1_entry);

    // Start scheduler
    vsrtos_scheduler_start();

    // Should never reach this point
    while (1);

    return 0;
}


// -- Helper functions -- //

static void init_driver(int (*init_function)(), const char* name) {
    int res = init_function();
    printf("  Init: %s ", name);
    if (res == 0) {
        printf("OK\n");
    } else {
        printf("Error: %d\n", res);
    }

    init_result |= res;
}

static void core1_entry() {
    while (1) {
        telemetry_update();
    }
}

static void go_to_error_during_startup() {
    while (1) {
        // TODO: Better error handling
        printf("Error occurred during startup: %d\n", init_result);
        led_set(LED_RED, 1);
        sleep_ms(500);
        led_set(LED_RED, 0);
        sleep_ms(500);
    }
}