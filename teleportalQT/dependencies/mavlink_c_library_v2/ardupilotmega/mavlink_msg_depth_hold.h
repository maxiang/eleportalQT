#pragma once
// MESSAGE DEPTH_HOLD PACKING

#define MAVLINK_MSG_ID_DEPTH_HOLD 11065

MAVPACKED(
typedef struct __mavlink_depth_hold_t {
 float depth; /*< [m] desire depth.*/
 uint8_t cmd; /*<  1 for enable, 0 for disable.*/
}) mavlink_depth_hold_t;

#define MAVLINK_MSG_ID_DEPTH_HOLD_LEN 5
#define MAVLINK_MSG_ID_DEPTH_HOLD_MIN_LEN 5
#define MAVLINK_MSG_ID_11065_LEN 5
#define MAVLINK_MSG_ID_11065_MIN_LEN 5

#define MAVLINK_MSG_ID_DEPTH_HOLD_CRC 46
#define MAVLINK_MSG_ID_11065_CRC 46



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_DEPTH_HOLD { \
    11065, \
    "DEPTH_HOLD", \
    2, \
    {  { "cmd", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_depth_hold_t, cmd) }, \
         { "depth", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_depth_hold_t, depth) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_DEPTH_HOLD { \
    "DEPTH_HOLD", \
    2, \
    {  { "cmd", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_depth_hold_t, cmd) }, \
         { "depth", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_depth_hold_t, depth) }, \
         } \
}
#endif

/**
 * @brief Pack a depth_hold message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param cmd  1 for enable, 0 for disable.
 * @param depth [m] desire depth.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_depth_hold_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t cmd, float depth)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEPTH_HOLD_LEN];
    _mav_put_float(buf, 0, depth);
    _mav_put_uint8_t(buf, 4, cmd);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DEPTH_HOLD_LEN);
#else
    mavlink_depth_hold_t packet;
    packet.depth = depth;
    packet.cmd = cmd;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DEPTH_HOLD_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_DEPTH_HOLD;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_DEPTH_HOLD_MIN_LEN, MAVLINK_MSG_ID_DEPTH_HOLD_LEN, MAVLINK_MSG_ID_DEPTH_HOLD_CRC);
}

/**
 * @brief Pack a depth_hold message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param cmd  1 for enable, 0 for disable.
 * @param depth [m] desire depth.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_depth_hold_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t cmd,float depth)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEPTH_HOLD_LEN];
    _mav_put_float(buf, 0, depth);
    _mav_put_uint8_t(buf, 4, cmd);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_DEPTH_HOLD_LEN);
#else
    mavlink_depth_hold_t packet;
    packet.depth = depth;
    packet.cmd = cmd;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_DEPTH_HOLD_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_DEPTH_HOLD;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_DEPTH_HOLD_MIN_LEN, MAVLINK_MSG_ID_DEPTH_HOLD_LEN, MAVLINK_MSG_ID_DEPTH_HOLD_CRC);
}

/**
 * @brief Encode a depth_hold struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param depth_hold C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_depth_hold_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_depth_hold_t* depth_hold)
{
    return mavlink_msg_depth_hold_pack(system_id, component_id, msg, depth_hold->cmd, depth_hold->depth);
}

/**
 * @brief Encode a depth_hold struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param depth_hold C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_depth_hold_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_depth_hold_t* depth_hold)
{
    return mavlink_msg_depth_hold_pack_chan(system_id, component_id, chan, msg, depth_hold->cmd, depth_hold->depth);
}

/**
 * @brief Send a depth_hold message
 * @param chan MAVLink channel to send the message
 *
 * @param cmd  1 for enable, 0 for disable.
 * @param depth [m] desire depth.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_depth_hold_send(mavlink_channel_t chan, uint8_t cmd, float depth)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_DEPTH_HOLD_LEN];
    _mav_put_float(buf, 0, depth);
    _mav_put_uint8_t(buf, 4, cmd);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEPTH_HOLD, buf, MAVLINK_MSG_ID_DEPTH_HOLD_MIN_LEN, MAVLINK_MSG_ID_DEPTH_HOLD_LEN, MAVLINK_MSG_ID_DEPTH_HOLD_CRC);
#else
    mavlink_depth_hold_t packet;
    packet.depth = depth;
    packet.cmd = cmd;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEPTH_HOLD, (const char *)&packet, MAVLINK_MSG_ID_DEPTH_HOLD_MIN_LEN, MAVLINK_MSG_ID_DEPTH_HOLD_LEN, MAVLINK_MSG_ID_DEPTH_HOLD_CRC);
#endif
}

/**
 * @brief Send a depth_hold message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_depth_hold_send_struct(mavlink_channel_t chan, const mavlink_depth_hold_t* depth_hold)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_depth_hold_send(chan, depth_hold->cmd, depth_hold->depth);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEPTH_HOLD, (const char *)depth_hold, MAVLINK_MSG_ID_DEPTH_HOLD_MIN_LEN, MAVLINK_MSG_ID_DEPTH_HOLD_LEN, MAVLINK_MSG_ID_DEPTH_HOLD_CRC);
#endif
}

#if MAVLINK_MSG_ID_DEPTH_HOLD_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_depth_hold_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t cmd, float depth)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, depth);
    _mav_put_uint8_t(buf, 4, cmd);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEPTH_HOLD, buf, MAVLINK_MSG_ID_DEPTH_HOLD_MIN_LEN, MAVLINK_MSG_ID_DEPTH_HOLD_LEN, MAVLINK_MSG_ID_DEPTH_HOLD_CRC);
#else
    mavlink_depth_hold_t *packet = (mavlink_depth_hold_t *)msgbuf;
    packet->depth = depth;
    packet->cmd = cmd;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_DEPTH_HOLD, (const char *)packet, MAVLINK_MSG_ID_DEPTH_HOLD_MIN_LEN, MAVLINK_MSG_ID_DEPTH_HOLD_LEN, MAVLINK_MSG_ID_DEPTH_HOLD_CRC);
#endif
}
#endif

#endif

// MESSAGE DEPTH_HOLD UNPACKING


/**
 * @brief Get field cmd from depth_hold message
 *
 * @return  1 for enable, 0 for disable.
 */
static inline uint8_t mavlink_msg_depth_hold_get_cmd(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field depth from depth_hold message
 *
 * @return [m] desire depth.
 */
static inline float mavlink_msg_depth_hold_get_depth(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Decode a depth_hold message into a struct
 *
 * @param msg The message to decode
 * @param depth_hold C-struct to decode the message contents into
 */
static inline void mavlink_msg_depth_hold_decode(const mavlink_message_t* msg, mavlink_depth_hold_t* depth_hold)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    depth_hold->depth = mavlink_msg_depth_hold_get_depth(msg);
    depth_hold->cmd = mavlink_msg_depth_hold_get_cmd(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_DEPTH_HOLD_LEN? msg->len : MAVLINK_MSG_ID_DEPTH_HOLD_LEN;
        memset(depth_hold, 0, MAVLINK_MSG_ID_DEPTH_HOLD_LEN);
    memcpy(depth_hold, _MAV_PAYLOAD(msg), len);
#endif
}
