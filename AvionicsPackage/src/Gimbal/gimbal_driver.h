#include "../main.h"

void gimbal_setup(void);
void gimbal_loop(float yaw, float pitch);
void center_gimbal(void);

#define YAW_SERVO_PIN 3
#define PITCH_SERVO_PIN 20

// extern int yawPin, pitchPin, pwmBitDepth, pwmFrequency;