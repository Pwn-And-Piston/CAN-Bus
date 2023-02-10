#define DEFAULT_CONTROL_ID 7
#define DEFAULT_DOOR_ID 411
#define DEFAULT_DOOR_POS 2
#define DEFAULT_SIGNAL_ID 392
#define DEFAULT_ECU_ID 2016
#define DEFAULT_SIGNAL_POS 0
#define DEFAULT_SPEED_ID 580
#define DEFAULT_SPEED_POS 3
#define DEFAULT_WARNING_ID 1066
#define DEFAULT_WARNING_POS 2
#define DEFAULT_LUMINOSITY_ID 924
#define DEFAULT_LUMINOSITY_POS 3
#define DEFAULT_LIGHT_ID 832
#define DEFAULT_LIGHT_POS 5


#define CAN_DOOR1_LOCK 1
#define CAN_DOOR2_LOCK 2
#define CAN_DOOR3_LOCK 4
#define CAN_DOOR4_LOCK 8
#define CAN_LEFT_SIGNAL 1
#define CAN_RIGHT_SIGNAL 2
#define CAN_WARNING_SIGNAL 3
#define CAN_LIGHT_ON 2
#define CAN_LIGHT_AUTO 1
#define ON 1
#define OFF 0
#define DOOR_LOCKED 0
#define DOOR_UNLOCKED 1

#define UDS_SID_DIAGNOSTIC_CONTROL        0x10 // GMLAN = Initiate Diagnostics
#define UDS_SID_ECU_RESET                 0x11
#define UDS_SID_GM_READ_FAILURE_RECORD    0x12 // GMLAN
#define UDS_SID_CLEAR_DTC                 0x14
#define UDS_SID_READ_DTC                  0x19
#define UDS_SID_GM_READ_DID_BY_ID         0x1A // GMLAN - Read DID By ID
#define UDS_SID_RESTART_COMMUNICATIONS    0x20 // GMLAN - Restart a stopped com
#define UDS_SID_READ_DATA_BY_ID           0x22
#define UDS_SID_READ_MEM_BY_ADDRESS       0x23
#define UDS_SID_READ_SCALING_BY_ID        0x24
#define UDS_SID_SECURITY_ACCESS           0x27
#define UDS_SID_COMMUNICATION_CONTROL     0x28 // GMLAN Stop Communications
#define UDS_SID_READ_DATA_BY_ID_PERIODIC  0x2A
#define UDS_SID_DEFINE_DATA_ID            0x2C
#define UDS_SID_WRITE_DATA_BY_ID          0x2E
#define UDS_SID_IO_CONTROL_BY_ID          0x2F
#define UDS_SID_ROUTINE_CONTROL           0x31
#define UDS_SID_REQUEST_DOWNLOAD          0x34
#define UDS_SID_REQUEST_UPLOAD            0x35
#define UDS_SID_TRANSFER_DATA             0x36
#define UDS_SID_REQUEST_XFER_EXIT         0x37
#define UDS_SID_REQUEST_XFER_FILE         0x38
#define UDS_SID_WRITE_MEM_BY_ADDRESS      0x3D
#define UDS_SID_TESTER_PRESENT            0x3E
#define UDS_SID_ACCESS_TIMING             0x83
#define UDS_SID_SECURED_DATA_TRANS        0x84
#define UDS_SID_CONTROL_DTC_SETTINGS      0x85
#define UDS_SID_RESPONSE_ON_EVENT         0x86
#define UDS_SID_LINK_CONTROL              0x87
#define UDS_SID_GM_PROGRAMMED_STATE       0xA2
#define UDS_SID_GM_PROGRAMMING_MODE       0xA5
#define UDS_SID_GM_READ_DIAG_INFO         0xA9
#define UDS_SID_GM_READ_DATA_BY_ID        0xAA
#define UDS_SID_GM_DEVICE_CONTROL         0xAE

#define PID_INFO                          0x01
#define PID_VEHICLE_INFO                  0x09
#define PID_INFO_VEHICLE_SPEED            0x0D
#define PID_VEHICLE_VIN                   0x02
#define PID_VEHICLE_ECU_NAME              0x0A

#define UDS_ERROR_SERVICE_NOT_SUPPORTED   0x11
#define UDS_ERROR_SUBFUNC_NOT_SUPPORTED   0x12
#define UDS_ERROR_INCORRECT_LENGTH        0x13
#define UDS_ERROR_FUNC_INCORRECT_SESSION  0x7F
#define UDS_ERROR_SEQUENCE_ERROR          0x24
#define UDS_ERROR_SECURITY_DENIED         0x33
#define UDS_ERROR_INVALID_KEY             0x35
#define UDS_ERROR_REQUEST_OUT_RANGE       0x31

#define PENDING_READ_DATA_BY_ID_GM         1

char challenges[6] = {0,0,0,0,0,0};
char challengeValue[6] = {5, 10, 10, 15, 30, 30};
#define CHALLENGE_TURN_SIGNALS            0
#define CHALLENGE_SPOOF_LIGHT             1
#define CHALLENGE_SPOOF_SPEED             2
#define CHALLENGE_REQUEST_VIN             3
#define CHALLENGE_FIND_ROUTINE_CONTROL    4
#define CHALLENGE_SECURITY_ACCESS         5

#define MAX_SPEED 100.0 // Limiter 260.0 is full guage speed
#define CHALLENGE_SPEED 150


#define DEBUG 0
#define VIN "WBARBHACKFA149850"
#define LIGHT_LEVEL 0x60
#define ISOTP_TIMEOUT 600
