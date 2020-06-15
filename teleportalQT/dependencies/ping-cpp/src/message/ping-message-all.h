// C++ implementation of the Blue Robotics 'Ping' binary message protocol

//~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!
// THIS IS AN AUTOGENERATED FILE
// DO NOT EDIT
//~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!~!

#pragma once

#include "ping-message-bluebps.h"
#include "ping-message-common.h"
#include "ping-message-ping1d.h"
#include "ping-message-ping360.h"

// Declare an empty Q_NAMESPACE and Q_ENUM_NS if it's not a Qt project, otherwise include QObject
#ifndef QT_CORE_LIB
#define Q_NAMESPACE
#define Q_ENUM_NS(x)
#else
#include <QObject>
#endif

namespace PingEnumNamespace {
Q_NAMESPACE
enum class PingDeviceType {
    UNKNOWN = 0,
    PING1D = 1,
    PING360 = 2,
    PINGHF = 3,
};
Q_ENUM_NS(PingDeviceType)

enum class PingMessageId {
    BLUEBPS_SET_CELL_VOLTAGE_MINIMUM = 9000,
    BLUEBPS_SET_CELL_VOLTAGE_TIMEOUT = 9001,
    BLUEBPS_SET_CURRENT_MAX = 9002,
    BLUEBPS_SET_CURRENT_TIMEOUT = 9003,
    BLUEBPS_SET_TEMPERATURE_MAX = 9004,
    BLUEBPS_SET_TEMPERATURE_TIMEOUT = 9005,
    BLUEBPS_SET_STREAM_RATE = 9006,
    BLUEBPS_SET_LPF_SAMPLE_FREQUENCY = 9007,
    BLUEBPS_SET_LPF_SETTING = 9008,
    BLUEBPS_CELL_VOLTAGE_MIN = 9100,
    BLUEBPS_CELL_TIMEOUT = 9101,
    BLUEBPS_CURRENT_MAX = 9102,
    BLUEBPS_CURRENT_TIMEOUT = 9103,
    BLUEBPS_TEMPERATURE_MAX = 9104,
    BLUEBPS_TEMPERATURE_TIMEOUT = 9105,
    BLUEBPS_STATE = 9106,
    BLUEBPS_EVENTS = 9107,
    BLUEBPS_REBOOT = 9200,
    BLUEBPS_ERASE_FLASH = 9201,
    BLUEBPS_RESET_DEFAULTS = 9202,
    COMMON_ACK = 1,
    COMMON_NACK = 2,
    COMMON_ASCII_TEXT = 3,
    COMMON_GENERAL_REQUEST = 6,
    COMMON_DEVICE_INFORMATION = 4,
    COMMON_PROTOCOL_VERSION = 5,
    PING1D_SET_DEVICE_ID = 1000,
    PING1D_SET_RANGE = 1001,
    PING1D_SET_SPEED_OF_SOUND = 1002,
    PING1D_SET_MODE_AUTO = 1003,
    PING1D_SET_PING_INTERVAL = 1004,
    PING1D_SET_GAIN_SETTING = 1005,
    PING1D_SET_PING_ENABLE = 1006,
    PING1D_FIRMWARE_VERSION = 1200,
    PING1D_DEVICE_ID = 1201,
    PING1D_VOLTAGE_5 = 1202,
    PING1D_SPEED_OF_SOUND = 1203,
    PING1D_RANGE = 1204,
    PING1D_MODE_AUTO = 1205,
    PING1D_PING_INTERVAL = 1206,
    PING1D_GAIN_SETTING = 1207,
    PING1D_TRANSMIT_DURATION = 1208,
    PING1D_GENERAL_INFO = 1210,
    PING1D_DISTANCE_SIMPLE = 1211,
    PING1D_DISTANCE = 1212,
    PING1D_PROCESSOR_TEMPERATURE = 1213,
    PING1D_PCB_TEMPERATURE = 1214,
    PING1D_PING_ENABLE = 1215,
    PING1D_PROFILE = 1300,
    PING1D_GOTO_BOOTLOADER = 1100,
    PING1D_CONTINUOUS_START = 1400,
    PING1D_CONTINUOUS_STOP = 1401,
    PING360_DEVICE_ID = 2000,
    PING360_DEVICE_DATA = 2300,
    PING360_AUTO_DEVICE_DATA = 2301,
    PING360_RESET = 2600,
    PING360_TRANSDUCER = 2601,
    PING360_AUTO_TRANSMIT = 2602,
    PING360_MOTOR_OFF = 2903,
};
Q_ENUM_NS(PingMessageId)
}

using namespace PingEnumNamespace;

/**
 * @brief Ping Helper class
 *  All functions should be validated in compile time or static
 *
 */
class PingHelper {
public:
    /**
     * @brief Return a human friendly device name
     *
     * @param deviceType
     * @return constexpr const char*
     */
    static constexpr const char* nameFromDeviceType(const PingDeviceType deviceType) {
        switch(deviceType) {
            case(PingDeviceType::UNKNOWN):
                return "UNKNOWN";
            case(PingDeviceType::PING1D):
                return "Ping1D";
            case(PingDeviceType::PING360):
                return "Ping360";
            case(PingDeviceType::PINGHF):
                return "PingHF";
            default:
                return "Unregistered";
        }
    }

    /**
     * @brief Helper function that provides the name of a message from the message ID number
     *
     * @param messageId
     * @return constexpr const char*
     */
    static constexpr const char* nameFromMessageId(const PingEnumNamespace::PingMessageId messageId) {
        switch(messageId) {
            case(PingEnumNamespace::PingMessageId::BLUEBPS_SET_CELL_VOLTAGE_MINIMUM):
                return "SET_CELL_VOLTAGE_MINIMUM";
            case(PingEnumNamespace::PingMessageId::BLUEBPS_SET_CELL_VOLTAGE_TIMEOUT):
                return "SET_CELL_VOLTAGE_TIMEOUT";
            case(PingEnumNamespace::PingMessageId::BLUEBPS_SET_CURRENT_MAX):
                return "SET_CURRENT_MAX";
            case(PingEnumNamespace::PingMessageId::BLUEBPS_SET_CURRENT_TIMEOUT):
                return "SET_CURRENT_TIMEOUT";
            case(PingEnumNamespace::PingMessageId::BLUEBPS_SET_TEMPERATURE_MAX):
                return "SET_TEMPERATURE_MAX";
            case(PingEnumNamespace::PingMessageId::BLUEBPS_SET_TEMPERATURE_TIMEOUT):
                return "SET_TEMPERATURE_TIMEOUT";
            case(PingEnumNamespace::PingMessageId::BLUEBPS_SET_STREAM_RATE):
                return "SET_STREAM_RATE";
            case(PingEnumNamespace::PingMessageId::BLUEBPS_SET_LPF_SAMPLE_FREQUENCY):
                return "SET_LPF_SAMPLE_FREQUENCY";
            case(PingEnumNamespace::PingMessageId::BLUEBPS_SET_LPF_SETTING):
                return "SET_LPF_SETTING";
            case(PingEnumNamespace::PingMessageId::BLUEBPS_CELL_VOLTAGE_MIN):
                return "CELL_VOLTAGE_MIN";
            case(PingEnumNamespace::PingMessageId::BLUEBPS_CELL_TIMEOUT):
                return "CELL_TIMEOUT";
            case(PingEnumNamespace::PingMessageId::BLUEBPS_CURRENT_MAX):
                return "CURRENT_MAX";
            case(PingEnumNamespace::PingMessageId::BLUEBPS_CURRENT_TIMEOUT):
                return "CURRENT_TIMEOUT";
            case(PingEnumNamespace::PingMessageId::BLUEBPS_TEMPERATURE_MAX):
                return "TEMPERATURE_MAX";
            case(PingEnumNamespace::PingMessageId::BLUEBPS_TEMPERATURE_TIMEOUT):
                return "TEMPERATURE_TIMEOUT";
            case(PingEnumNamespace::PingMessageId::BLUEBPS_STATE):
                return "STATE";
            case(PingEnumNamespace::PingMessageId::BLUEBPS_EVENTS):
                return "EVENTS";
            case(PingEnumNamespace::PingMessageId::BLUEBPS_REBOOT):
                return "REBOOT";
            case(PingEnumNamespace::PingMessageId::BLUEBPS_ERASE_FLASH):
                return "ERASE_FLASH";
            case(PingEnumNamespace::PingMessageId::BLUEBPS_RESET_DEFAULTS):
                return "RESET_DEFAULTS";
            case(PingEnumNamespace::PingMessageId::COMMON_ACK):
                return "ACK";
            case(PingEnumNamespace::PingMessageId::COMMON_NACK):
                return "NACK";
            case(PingEnumNamespace::PingMessageId::COMMON_ASCII_TEXT):
                return "ASCII_TEXT";
            case(PingEnumNamespace::PingMessageId::COMMON_GENERAL_REQUEST):
                return "GENERAL_REQUEST";
            case(PingEnumNamespace::PingMessageId::COMMON_DEVICE_INFORMATION):
                return "DEVICE_INFORMATION";
            case(PingEnumNamespace::PingMessageId::COMMON_PROTOCOL_VERSION):
                return "PROTOCOL_VERSION";
            case(PingEnumNamespace::PingMessageId::PING1D_SET_DEVICE_ID):
                return "SET_DEVICE_ID";
            case(PingEnumNamespace::PingMessageId::PING1D_SET_RANGE):
                return "SET_RANGE";
            case(PingEnumNamespace::PingMessageId::PING1D_SET_SPEED_OF_SOUND):
                return "SET_SPEED_OF_SOUND";
            case(PingEnumNamespace::PingMessageId::PING1D_SET_MODE_AUTO):
                return "SET_MODE_AUTO";
            case(PingEnumNamespace::PingMessageId::PING1D_SET_PING_INTERVAL):
                return "SET_PING_INTERVAL";
            case(PingEnumNamespace::PingMessageId::PING1D_SET_GAIN_SETTING):
                return "SET_GAIN_SETTING";
            case(PingEnumNamespace::PingMessageId::PING1D_SET_PING_ENABLE):
                return "SET_PING_ENABLE";
            case(PingEnumNamespace::PingMessageId::PING1D_FIRMWARE_VERSION):
                return "FIRMWARE_VERSION";
            case(PingEnumNamespace::PingMessageId::PING1D_DEVICE_ID):
                return "DEVICE_ID";
            case(PingEnumNamespace::PingMessageId::PING1D_VOLTAGE_5):
                return "VOLTAGE_5";
            case(PingEnumNamespace::PingMessageId::PING1D_SPEED_OF_SOUND):
                return "SPEED_OF_SOUND";
            case(PingEnumNamespace::PingMessageId::PING1D_RANGE):
                return "RANGE";
            case(PingEnumNamespace::PingMessageId::PING1D_MODE_AUTO):
                return "MODE_AUTO";
            case(PingEnumNamespace::PingMessageId::PING1D_PING_INTERVAL):
                return "PING_INTERVAL";
            case(PingEnumNamespace::PingMessageId::PING1D_GAIN_SETTING):
                return "GAIN_SETTING";
            case(PingEnumNamespace::PingMessageId::PING1D_TRANSMIT_DURATION):
                return "TRANSMIT_DURATION";
            case(PingEnumNamespace::PingMessageId::PING1D_GENERAL_INFO):
                return "GENERAL_INFO";
            case(PingEnumNamespace::PingMessageId::PING1D_DISTANCE_SIMPLE):
                return "DISTANCE_SIMPLE";
            case(PingEnumNamespace::PingMessageId::PING1D_DISTANCE):
                return "DISTANCE";
            case(PingEnumNamespace::PingMessageId::PING1D_PROCESSOR_TEMPERATURE):
                return "PROCESSOR_TEMPERATURE";
            case(PingEnumNamespace::PingMessageId::PING1D_PCB_TEMPERATURE):
                return "PCB_TEMPERATURE";
            case(PingEnumNamespace::PingMessageId::PING1D_PING_ENABLE):
                return "PING_ENABLE";
            case(PingEnumNamespace::PingMessageId::PING1D_PROFILE):
                return "PROFILE";
            case(PingEnumNamespace::PingMessageId::PING1D_GOTO_BOOTLOADER):
                return "GOTO_BOOTLOADER";
            case(PingEnumNamespace::PingMessageId::PING1D_CONTINUOUS_START):
                return "CONTINUOUS_START";
            case(PingEnumNamespace::PingMessageId::PING1D_CONTINUOUS_STOP):
                return "CONTINUOUS_STOP";
            case(PingEnumNamespace::PingMessageId::PING360_DEVICE_ID):
                return "DEVICE_ID";
            case(PingEnumNamespace::PingMessageId::PING360_DEVICE_DATA):
                return "DEVICE_DATA";
            case(PingEnumNamespace::PingMessageId::PING360_AUTO_DEVICE_DATA):
                return "AUTO_DEVICE_DATA";
            case(PingEnumNamespace::PingMessageId::PING360_RESET):
                return "RESET";
            case(PingEnumNamespace::PingMessageId::PING360_TRANSDUCER):
                return "TRANSDUCER";
            case(PingEnumNamespace::PingMessageId::PING360_AUTO_TRANSMIT):
                return "AUTO_TRANSMIT";
            case(PingEnumNamespace::PingMessageId::PING360_MOTOR_OFF):
                return "MOTOR_OFF";
            default:
                return "UNKNOWN";
        }
    }

private:
    PingHelper() = default;
    ~PingHelper() = default;
};