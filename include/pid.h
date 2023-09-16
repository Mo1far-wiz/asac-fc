#ifndef PID_H
#define PID_H

#include "asac_fc.h"

typedef struct {
    float Kp;
    float Ki;
    float Kd;
    float Kff;
    float p;
    float i;
    float d;
    float ff;   // Feed Forward
    float err;
    float err_integral;
    float last_err;
    float d_err;
    float pid;
    float integral_limit_threshold;
    bool  integral_disabled;
    uint32_t integral_disabled_timestamp;
} pid_state_t;


// Rotation rates in degrees per second (deg/s)
typedef struct {
    float roll;
    float pitch;
    float yaw;
} rates_t;


// The pid adjust are used to know how much to adjust each motor speed, depending
// on the pid parameter values, and the error. This is the result of a pid update loop.
typedef struct {
    float roll;
    float pitch;
    float yaw;
} pid_adjust_t;


float pid_update(pid_state_t* pid, const float measured, const float desired, const uint16_t throttle, const float dt_s);


#endif /* PID_H */
