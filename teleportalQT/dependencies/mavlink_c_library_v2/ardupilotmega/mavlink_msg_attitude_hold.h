#pragma once
// MESSAGE ATTITUDE_HOLD PACKING

#define MAVLINK_MSG_ID_ATTITUDE_HOLD 11066

MAVPACKED(
typedef struct __mavlink_attitude_hold_t {
 float yaw; /*< [degree] desire yaw.*/
 float pitch; /*< [degree] desire pitch.*/
 float roll; /*< [roll] desire roll.*/
 uint8_t cmd; /*<  1 for enable, 0 for disable.*/
}) mavlink_attitude_hold_t;

#define MAVLINK_MSG_ID_ATTITUDE_HOLD_LEN 13
#define MAVLINK_MSG_ID_ATTITUDE_HOLD_MIN_LEN 13
#define MAVLINK_MSG_ID_11066_LEN 13
#define MAVLINK_MSG_ID_11066_MIN_LEN 13

#define MAVLINK_MSG_ID_ATTITUDE_HOLD_CRC 207
#define MAVLINK_MSG_ID_11066_CRC 207



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_ATTITUDE_HOLD { \
    11066, \
    "ATTITUDE_HOLD", \
    4, \
    {  { "cmd", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_attitude_hold_t, cmd) }, \
         { "yaw", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_attitude_hold_t, yaw) }, \
         { "pitch", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_attitude_hold_t, pitch) }, \
         { "roll", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_attitude_hold_t, roll) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_ATTITUDE_HOLD { \
    "ATTITUDE_HOLD", \
    4, \
    {  { "cmd", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_attitude_hold_t, cmd) }, \
         { "yaw", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_attitude_hold_t, yaw) }, \
         { "pitch", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_attitude_hold_t, pitch) }, \
         { "roll", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_attitude_hold_t, roll) }, \
         } \
}
#endif

/**
 * @brief Pack a attitude_hold message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param cmd  1 for enable, 0 for disable.
 * @param yaw [degree] desire yaw.
 * @param pitch [degree] desire pitch.
 * @param roll [roll] desire roll.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_attitude_hold_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t cmd, float yaw, float pitch, float roll)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ATTITUDE_HOLD_LEN];
    _mav_put_float(buf, 0, yaw);
    _mav_put_float(buf, 4, pitch);
    _mav_put_float(buf, 8, roll);
    _mav_put_uint8_t(buf, 12, cmd);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ATTITUDE_HOLD_LEN);
#else
    mavlink_attitude_hold_t packet;
    packet.yaw = yaw;
    packet.pitch = pitch;
    packet.roll = roll;
    packet.cmd = cmd;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ATTITUDE_HOLD_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ATTITUDE_HOLD;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_ATTITUDE_HOLD_MIN_LEN, MAVLINK_MSG_ID_ATTITUDE_HOLD_LEN, MAVLINK_MSG_ID_ATTITUDE_HOLD_CRC);
}

/**
 * @brief Pack a attitude_hold message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param cmd  1 for enable, 0 for disable.
 * @param yaw [degree] desire yaw.
 * @param pitch [degree] desire pitch.
 * @param roll [roll] desire roll.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_attitude_hold_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t cmd,float yaw,float pitch,float roll)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ATTITUDE_HOLD_LEN];
    _mav_put_float(buf, 0, yaw);
    _mav_put_float(buf, 4, pitch);
    _mav_put_float(buf, 8, roll);
    _mav_put_uint8_t(buf, 12, cmd);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ATTITUDE_HOLD_LEN);
#else
    mavlink_attitude_hold_t packet;
    packet.yaw = yaw;
    packet.pitch = pitch;
    packet.roll = roll;
    packet.cmd = cmd;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ATTITUDE_HOLD_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ATTITUDE_HOLD;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_ATTITUDE_HOLD_MIN_LEN, MAVLINK_MSG_ID_ATTITUDE_HOLD_LEN, MAVLINK_MSG_ID_ATTITUDE_HOLD_CRC);
}

/**
 * @brief Encode a attitude_hold struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param attitude_hold C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_attitude_hold_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_attitude_hold_t* attitude_hold)
{
    return mavlink_msg_attitude_hold_pack(system_id, component_id, msg, attitude_hold->cmd, attitude_hold->yaw, attitude_hold->pitch, attitude_hold->roll);
}

/**
 * @brief Encode a attitude_hold struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param attitude_hold C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_attitude_hold_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_attitude_hold_t* attitude_hold)
{
    return mavlink_msg_attitude_hold_pack_chan(system_id, component_id, chan, msg, attitude_hold->cmd, attitude_hold->yaw, attitude_hold->pitch, attitude_hold->roll);
}

/**
 * @brief Send a attitude_hold message
 * @param chan MAVLink channel to send the message
 *
 * @param cmd  1 for enable, 0 for disable.
 * @param yaw [degree] desire yaw.
 * @param pitch [degree] desire pitch.
 * @param roll [roll] desire roll.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_attitude_hold_send(mavlink_channel_t chan, uint8_t cmd, float yaw, float pitch, float roll)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ATTITUDE_HOLD_LEN];
    _mav_put_float(buf, 0, yaw);
    _mav_put_float(buf, 4, pitch);
    _mav_put_float(buf, 8, roll);
    _mav_put_uint8_t(buf, 12, cmd);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ATTITUDE_HOLD, buf, MAVLINK_MSG_ID_ATTITUDE_HOLD_MIN_LEN, MAVLINK_MSG_ID_ATTITUDE_HOLD_LEN, MAVLINK_MSG_ID_ATTITUDE_HOLD_CRC);
#else
    mavlink_attitude_hold_t packet;
    packet.yaw = yaw;
    packet.pitch = pitch;
    packet.roll = roll;
    packet.cmd = cmd;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ATTITUDE_HOLD, (const char *)&packet, MAVLINK_MSG_ID_ATTITUDE_HOLD_MIN_LEN, MAVLINK_MSG_ID_ATTITUDE_HOLD_LEN, MAVLINK_MSG_ID_ATTITUDE_HOLD_CRC);
#endif
}

/**
 * @brief Send a attitude_hold message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_attitude_hold_send_struct(mavlink_channel_t chan, const mavlink_attitude_hold_t* attitude_hold)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_attitude_hold_send(chan, attitude_hold->cmd, attitude_hold->yaw, attitude_hold->pitch, attitude_hold->roll);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ATTITUDE_HOLD, (const char *)attitude_hold, MAVLINK_MSG_ID_ATTITUDE_HOLD_MIN_LEN, MAVLINK_MSG_ID_ATTITUDE_HOLD_LEN, MAVLINK_MSG_ID_ATTITUDE_HOLD_CRC);
#endif
}

#if MAVLINK_MSG_ID_ATTITUDE_HOLD_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_attitude_hold_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t cmd, float yaw, float pitch, float roll)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, yaw);
    _mav_put_float(buf, 4, pitch);
    _mav_put_float(buf, 8, roll);
    _mav_put_uint8_t(buf, 12, cmd);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ATTITUDE_HOLD, buf, MAVLINK_MSG_ID_ATTITUDE_HOLD_MIN_LEN, MAVLINK_MSG_ID_ATTITUDE_HOLD_LEN, MAVLINK_MSG_ID_ATTITUDE_HOLD_CRC);
#else
    mavlink_attitude_hold_t *packet = (mavlink_attitude_hold_t *)msgbuf;
    packet->yaw = yaw;
    packet->pitch = pitch;
    packet->roll = roll;
    packet->cmd = cmd;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ATTITUDE_HOLD, (const char *)packet, MAVLINK_MSG_ID_ATTITUDE_HOLD_MIN_LEN, MAVLINK_MSG_ID_ATTITUDE_HOLD_LEN, MAVLINK_MSG_ID_ATTITUDE_HOLD_CRC);
#endif
}
#endif

#endif

// MESSAGE ATTITUDE_HOLD UNPACKING


/**
 * @brief Get field cmd from attitude_hold message
 *
 * @return  1 for enable, 0 for disable.
 */
static inline uint8_t mavlink_msg_attitude_hold_get_cmd(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  12);
}

/**
 * @brief Get field yaw from attitude_hold message
 *
 * @return [degree] desire yaw.
 */
static inline float mavlink_msg_attitude_hold_get_yaw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field pitch from attitude_hold message
 *
 * @return [degree] desire pitch.
 */
static inline float mavlink_msg_attitude_hold_get_pitch(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field roll from attitude_hold message
 *
 * @return [roll] desire roll.
 */
static inline float mavlink_msg_attitude_hold_get_roll(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Decode a attitude_hold message into a struct
 *
 * @param msg The message to decode
 * @param attitude_hold C-struct to decode the message contents into
 */
static inline void mavlink_msg_attitude_hold_decode(const mavlink_message_t* msg, mavlink_attitude_hold_t* attitude_hold)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    attitude_hold->yaw = mavlink_msg_attitude_hold_get_yaw(msg);
    attitude_hold->pitch = mavlink_msg_attitude_hold_get_pitch(msg);
    attitude_hold->roll = mavlink_msg_attitude_hold_get_roll(msg);
    attitude_hold->cmd = mavlink_msg_attitude_hold_get_cmd(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_ATTITUDE_HOLD_LEN? msg->len : MAVLINK_MSG_ID_ATTITUDE_HOLD_LEN;
        memset(attitude_hold, 0, MAVLINK_MSG_ID_ATTITUDE_HOLD_LEN);
    memcpy(attitude_hold, _MAV_PAYLOAD(msg), len);
#endif
}
