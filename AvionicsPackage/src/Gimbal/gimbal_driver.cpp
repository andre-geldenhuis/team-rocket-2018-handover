#include "gimbal_driver.h"

// int yawPin, pitchPin, pwmBitDepth, pwmFrequency;

// Big end gimbal is J3 and YAW
// Small end is J2 and pitch

#define YAW_CENTER 10480
#define YAW_MAX 11480
#define YAW_MIN 9500

#define PITCH_CENTER 8500
#define PITCH_MAX 9300
#define PITCH_MIN 7600

void gimbal_setup(void)
{
    debugger.println("Gimbal Init Started");
    analogWriteFrequency(YAW_SERVO_PIN, 100);
    analogWriteFrequency(PITCH_SERVO_PIN, 100);
    analogWriteResolution(16);

    analogWrite(YAW_SERVO_PIN, YAW_CENTER);
    analogWrite(PITCH_SERVO_PIN, PITCH_CENTER);

    
    // delay(2000);
    // analogWrite(YAW_SERVO_PIN, YAW_MIN);
    // analogWrite(PITCH_SERVO_PIN, PITCH_MIN);

    // delay(2000);
    // analogWrite(YAW_SERVO_PIN, YAW_MAX);
    // analogWrite(PITCH_SERVO_PIN, PITCH_MAX);
    // while (1);

    delay(1000);
    debugger.println("Gimbal Init Finished");
}

void gimbal_loop(float yaw, float pitch)
{
    // analogWrite(YAW_SERVO_PIN, YAW_CENTER + yaw * 1000);
    // analogWrite(PITCH_SERVO_PIN, PITCH_CENTER + pitch * 1000);
    analogWrite(YAW_SERVO_PIN, max(min(YAW_CENTER + yaw * 1000, YAW_MAX), YAW_MIN));
    analogWrite(PITCH_SERVO_PIN, max(min(PITCH_CENTER + pitch * 1000, PITCH_MAX), PITCH_MIN));
}

void center_gimbal(void)
{
    analogWrite(YAW_SERVO_PIN, YAW_CENTER);
    analogWrite(PITCH_SERVO_PIN, PITCH_CENTER);
}