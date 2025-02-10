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
#define MICROSTEP_POWER_OF_2                0   // 2^5 = 32 microsteps
#define LINEAR_ACTUATOR_MAX_SPEED        1600.0f // mm/s
#define LINEAR_ACTUATOR_MAX_ACCELERATION 10000.0f // mm/s²
#define RMS_CURRENT                       100   // percent of the max current
#define COMPUTE_ACCELERATION_PERIOD_MS       20   // ms

// Calibration Settings
#define COARSE_CALIBRATION_SPEED           76   // mm/s
#define COARSE_CALIBRATION_STALL_VALUE     220
#define FINE_CALIBRATION_SPEED             44   // mm/s
#define FINE_CALIBRATION_STALL_VALUE       200
#define FINE_CALIBRATION_SAMPLES            2   // good samples needed
#define FINE_CALIBRATION_ERROR_THRESHOLD    3   // mm
#define FINE_CALIBRATION_WITHDRAWAL_DISTANCE 20  // mm

// TMC Serial Port Settings
#define TMC_SERIAL_PORT Serial2
#define TMC_SERIAL_BAUD_RATE 115200

// TMC Addresses
#define TMC1_ADDRESS 0b00
#define TMC2_ADDRESS 0b01
#define TMC3_ADDRESS 0b10
#define TMC4_ADDRESS 0b11

// TMC Pins
#define P1_STEP_PIN GPIO_NUM_19
#define P1_DIR_PIN  GPIO_NUM_21
#define P2_STEP_PIN GPIO_NUM_4
#define P2_DIR_PIN  GPIO_NUM_18
#define P3_STEP_PIN GPIO_NUM_33
#define P3_DIR_PIN  GPIO_NUM_32
#define P4_STEP_PIN GPIO_NUM_22
#define P4_DIR_PIN  GPIO_NUM_23

// MOTOR DIRECTION INVERSION
#define P1_INVERT_DIR true
#define P2_INVERT_DIR true
#define P3_INVERT_DIR true
#define P4_INVERT_DIR true

#define LINEAR_ACTUATOR_RAILHEAD 357.6
#define MIN_BUMPER_WIDTH 72
#define MAX_BUMPER_WIDTH 120
#define NO_COLLISION_MARGIN 3

#define TMC_R_SENSE 0.98f
#define TMC_MAX_CURRENT 1000 // mA

////////////////////////////////////
// SOLENOID SETTINGS
////////////////////////////////////
#define ANALOG_WRITE_RESOLUTION 8
#define P1_SOLENOID_PIN GPIO_NUM_25
#define P2_SOLENOID_PIN GPIO_NUM_26
#define P3_SOLENOID_PIN GPIO_NUM_27
#define P4_SOLENOID_PIN GPIO_NUM_12
#define MIN_SOLENOID_MIN_POWER 0.82

////////////////////////////////////
// BEAM SWITCH SETTINGS
////////////////////////////////////
#define IR_PWM_FREQUENCY       38000
#define IR_PWM_CHANNEL             0
#define BEAM_SWITCH_TIMEOUT_MS    20
#define BEAM_T_PIN         GPIO_NUM_5

#define P1_BEAM_R_PIN     GPIO_NUM_14
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
// Default tasks use core 0 (e.g., setup and loop functions in main.cpp)
#define TASK_BEAM_CHECK_CORE        0
#define TASK_MOTOR_RUN_CORE   0
#define TASK_SOLENOID_OVERTEMP_CORE 0
#define TASK_RASP_COMMUNICATION_CORE 0
#define TASK_STALLGUARD_CORE        0

////////////////////////////////////
// TASK PRIORITY ASSIGNMENT
////////////////////////////////////
// 0 is the lowest priority (inactive task)
#define TASK_BEAM_CHECK_PRIORITY        2
#define TASK_MOTOR_RUN_PRIORITY   3
#define TASK_SOLENOID_OVERTEMP_PRIORITY 3
#define TASK_RASP_COMMUNICATION_PRIORITY 1
#define TASK_STALLGUARD_PRIORITY        3

////////////////////////////////////
// TASK DELAY SETTINGS (MS)
////////////////////////////////////
#define TASK_BEAM_CHECK_DELAY_MS       5
#define TASK_SOLENOID_OVERTEMP_DELAY_MS 100
#define TASK_RASP_COMMUNICATION_DELAY_MS 10
#define TASK_STALLGUARD_DELAY_MS       3


////////////////////////////////////
// RASP COMMUNICATION SETTINGS
////////////////////////////////////
#define KEY_SEP '/'
#define PARAM_BEGIN_SEP '{'
#define PARAM_END_SEP '}'
#define LINE_SEP '\n'
#define PLAYER_KEY String("P")

#define MOVE_TO_KEY String("MT")
#define CURRENT_POSITION_KEY String("CP")
#define CURRENT_SPEED_KEY String("CS")
#define CURRENT_ACCELERATION_KEY String("CA")
#define BEAM_STATE_KEY String("BS")

#define SET_KEY String("S")
#define MAX_SPEED_KEY String("MS")
#define MAX_ACCELERATION_KEY String("MA")
#define CALIBRATION_KEY String("C")
#define RIGHT_LIMIT_KEY String("RL")
#define LEFT_LIMIT_KEY String("LL")
#define SOL_SATE_KEY String("SS")
#define STOP_KEY String("S")
#define BUSY_KEY String("B")

#define MOVE_TO_LEFT_LIMIT_KEY (MOVE_TO_KEY+LEFT_LIMIT_KEY)
#define MOVE_TO_RIGHT_LIMIT_KEY (MOVE_TO_KEY+RIGHT_LIMIT_KEY)
#define SET_MAX_SPEED_KEY (SET_KEY+MAX_SPEED_KEY)
#define SET_MAX_ACCELERATION_KEY (SET_KEY+MAX_ACCELERATION_KEY)
#define SET_SOL_STATE_KEY (SET_KEY+SOL_SATE_KEY)

#define ASK_KEY String("A")
#define ASK_CURRENT_POSITION_KEY (ASK_KEY+CURRENT_POSITION_KEY)
#define ASK_CURRENT_SPEED_KEY (ASK_KEY+CURRENT_SPEED_KEY)
#define ASK_CURRENT_ACCELERATION_KEY (ASK_KEY+CURRENT_ACCELERATION_KEY)
#define ASK_MAX_SPEED_KEY (ASK_KEY+MAX_SPEED_KEY)
#define ASK_CALIBRATED (ASK_KEY+CALIBRATION_KEY)
#define ASK_RIGHT_LIMIT_KEY (ASK_KEY+RIGHT_LIMIT_KEY)
#define ASK_LEFT_LIMIT_KEY (ASK_KEY+LEFT_LIMIT_KEY)
#define ASK_SOL_STATE_KEY (ASK_KEY+SOL_SATE_KEY)


#endif // CONFIG_H
