#ifndef CONFIG_H
#define CONFIG_H

////////////////////////////////////
// PLAYER DEFINITIONS
////////////////////////////////////
// PLAYER 1 : YELLOW TRIANGLE
// PLAYER 2 : GREEN SQUARE
// PLAYER 3 : RED CIRCLE
// PLAYER 4 : BLUE CROSS

////////////////////////////////////
// LINEAR ACTUATOR SETTINGS
////////////////////////////////////
#define STEPS_PER_REVOLUTION              200   // steps
#define PULLEY_TEETH                       25   // teeth of the pulley
#define BELT_PITCH                          2   // mm
#define MICROSTEP_POWER_OF_2                5   // 2^5 = 32 microsteps
#define LINEAR_ACTUATOR_MAX_SPEED        200.0f // mm/s
#define LINEAR_ACTUATOR_MAX_ACCELERATION 10000.0f // mm/s²
#define RMS_CURRENT                       100   // percent of the max current

// Calibration Settings
#define COARSE_CALIBRATION_SPEED           82   // mm/s
#define COARSE_CALIBRATION_STALL_VALUE     200
#define FINE_CALIBRATION_SPEED             44   // mm/s
#define FINE_CALIBRATION_STALL_VALUE       185
#define FINE_CALIBRATION_SAMPLES            2   // good samples needed
#define FINE_CALIBRATION_ERROR_THRESHOLD    1   // mm
#define FINE_CALIBRATION_WITHDRAWAL_DISTANCE 10  // mm

// TMC Serial Port Settings
#define TMC_SERIAL_PORT Serial2
#define TMC_SERIAL_BAUD_RATE 115200

// TMC Addresses
#define TMC1_ADDRESS TMC2209::SERIAL_ADDRESS_0
#define TMC2_ADDRESS TMC2209::SERIAL_ADDRESS_1
#define TMC3_ADDRESS TMC2209::SERIAL_ADDRESS_2
#define TMC4_ADDRESS TMC2209::SERIAL_ADDRESS_3

// TMC Pins
#define P1_STEP_PIN GPIO_NUM_22
#define P1_DIR_PIN  GPIO_NUM_23
#define P2_STEP_PIN GPIO_NUM_19
#define P2_DIR_PIN  GPIO_NUM_21
#define P3_STEP_PIN GPIO_NUM_4
#define P3_DIR_PIN  GPIO_NUM_18
#define P4_STEP_PIN GPIO_NUM_33
#define P4_DIR_PIN  GPIO_NUM_32

#define TMC_R_SENSE 0.11f

////////////////////////////////////
// SOLENOID SETTINGS
////////////////////////////////////
#define ANALOG_WRITE_RESOLUTION 8
#define P1_SOLENOID_PIN GPIO_NUM_25
#define P2_SOLENOID_PIN GPIO_NUM_26
#define P3_SOLENOID_PIN GPIO_NUM_27
#define P4_SOLENOID_PIN GPIO_NUM_14

////////////////////////////////////
// BEAM SWITCH SETTINGS
////////////////////////////////////
#define IR_PWM_FREQUENCY       38000
#define IR_PWM_CHANNEL             0
#define BEAM_SWITCH_TIMEOUT_MS    20
#define BEAM_T_PIN         GPIO_NUM_25

#define P1_BEAM_R_PIN     GPIO_NUM_12
#define P2_BEAM_R_PIN     GPIO_NUM_15
#define P3_BEAM_R_PIN     GPIO_NUM_34
#define P4_BEAM_R_PIN     GPIO_NUM_13

////////////////////////////////////
// COMMUNICATION SETTINGS (UART)
////////////////////////////////////
#define RASP_BAUD_RATE 115200

////////////////////////////////////
// TASK CORE ASSIGNMENT
////////////////////////////////////
// Default tasks use core 1 (e.g., setup and loop functions in main.cpp)
#define TASK_BEAM_CHECK_CORE        1
#define TASK_LINEAR_ACTUATOR_CORE   0
#define TASK_SOLENOID_OVERTEMP_CORE 1
// #define TASK_RASP_COMMUNICATION_CORE 1

////////////////////////////////////
// TASK PRIORITY ASSIGNMENT
////////////////////////////////////
// 0 is the lowest priority (inactive task)
#define TASK_BEAM_CHECK_PRIORITY        2
#define TASK_LINEAR_ACTUATOR_PRIORITY   3
#define TASK_SOLENOID_OVERTEMP_PRIORITY 3
// #define TASK_RASP_COMMUNICATION_PRIORITY 1

////////////////////////////////////
// TASK DELAY SETTINGS (MS)
////////////////////////////////////
#define TASK_BEAM_CHECK_DELAY_MS       5
#define TASK_SOLENOID_OVERTEMP_DELAY_MS 100

#endif // CONFIG_H
