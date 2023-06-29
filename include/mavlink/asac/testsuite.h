/** @file
 *    @brief MAVLink comm protocol testsuite generated from asac.xml
 *    @see https://mavlink.io/en/
 */
#pragma once
#ifndef ASAC_TESTSUITE_H
#define ASAC_TESTSUITE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAVLINK_TEST_ALL
#define MAVLINK_TEST_ALL
static void mavlink_test_common(uint8_t, uint8_t, mavlink_message_t *last_msg);
static void mavlink_test_asac(uint8_t, uint8_t, mavlink_message_t *last_msg);

static void mavlink_test_all(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_common(system_id, component_id, last_msg);
    mavlink_test_asac(system_id, component_id, last_msg);
}
#endif

#include "../common/testsuite.h"



static void mavlink_test_asac(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{

}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // ASAC_TESTSUITE_H
