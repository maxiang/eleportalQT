#pragma once
// MESSAGE FLIP_TRICK PACKING

#define MAVLINK_MSG_ID_FLIP_TRICK 11067

MAVPACKED(
typedef struct __mavlink_flip_trick_t {
 float value; /*< [degree] flip value.*/
 uint8_t tpye; /*<  0 for canle.*/
}) mavlink_flip_trick_t;

#define MAVLINK_MSG_ID_FLIP_TRICK_LEN 5
#define MAVLINK_MSG_ID_FLIP_TRICK_MIN_LEN 5
#define MAVLINK_MSG_ID_11067_LEN 5
#define MAVLINK_MSG_ID_11067_MIN_LEN 5

#define MAVLINK_MSG_ID_FLIP_TRICK_CRC 167
#define MAVLINK_MSG_ID_11067_CRC 167



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_FLIP_TRICK { \
    11067, \
    "FLIP_TRICK", \
    2, \
    {  { "tpye", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_flip_trick_t, tpye) }, \
         { "value", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_flip_trick_t, value) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_FLIP_TRICK { \
    "FLIP_TRICK", \
    2, \
    {  { "tpye", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_flip_trick_t, tpye) }, \
         { "value", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_flip_trick_t, value) }, \
         } \
}
#endif

/**
 * @brief Pack a flip_trick message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param tpye  0 for canle.
 * @param value [degree] flip value.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_flip_trick_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint8_t tpye, float value)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FLIP_TRICK_LEN];
    _mav_put_float(buf, 0, value);
    _mav_put_uint8_t(buf, 4, tpye);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_FLIP_TRICK_LEN);
#else
    mavlink_flip_trick_t packet;
    packet.value = value;
    packet.tpye = tpye;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_FLIP_TRICK_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_FLIP_TRICK;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_FLIP_TRICK_MIN_LEN, MAVLINK_MSG_ID_FLIP_TRICK_LEN, MAVLINK_MSG_ID_FLIP_TRICK_CRC);
}

/**
 * @brief Pack a flip_trick message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param tpye  0 for canle.
 * @param value [degree] flip value.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_flip_trick_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint8_t tpye,float value)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FLIP_TRICK_LEN];
    _mav_put_float(buf, 0, value);
    _mav_put_uint8_t(buf, 4, tpye);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_FLIP_TRICK_LEN);
#else
    mavlink_flip_trick_t packet;
    packet.value = value;
    packet.tpye = tpye;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_FLIP_TRICK_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_FLIP_TRICK;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_FLIP_TRICK_MIN_LEN, MAVLINK_MSG_ID_FLIP_TRICK_LEN, MAVLINK_MSG_ID_FLIP_TRICK_CRC);
}

/**
 * @brief Encode a flip_trick struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param flip_trick C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_flip_trick_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_flip_trick_t* flip_trick)
{
    return mavlink_msg_flip_trick_pack(system_id, component_id, msg, flip_trick->tpye, flip_trick->value);
}

/**
 * @brief Encode a flip_trick struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param flip_trick C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_flip_trick_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_flip_trick_t* flip_trick)
{
    return mavlink_msg_flip_trick_pack_chan(system_id, component_id, chan, msg, flip_trick->tpye, flip_trick->value);
}

/**
 * @brief Send a flip_trick message
 * @param chan MAVLink channel to send the message
 *
 * @param tpye  0 for canle.
 * @param value [degree] flip value.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_flip_trick_send(mavlink_channel_t chan, uint8_t tpye, float value)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_FLIP_TRICK_LEN];
    _mav_put_float(buf, 0, value);
    _mav_put_uint8_t(buf, 4, tpye);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FLIP_TRICK, buf, MAVLINK_MSG_ID_FLIP_TRICK_MIN_LEN, MAVLINK_MSG_ID_FLIP_TRICK_LEN, MAVLINK_MSG_ID_FLIP_TRICK_CRC);
#else
    mavlink_flip_trick_t packet;
    packet.value = value;
    packet.tpye = tpye;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FLIP_TRICK, (const char *)&packet, MAVLINK_MSG_ID_FLIP_TRICK_MIN_LEN, MAVLINK_MSG_ID_FLIP_TRICK_LEN, MAVLINK_MSG_ID_FLIP_TRICK_CRC);
#endif
}

/**
 * @brief Send a flip_trick message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_flip_trick_send_struct(mavlink_channel_t chan, const mavlink_flip_trick_t* flip_trick)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_flip_trick_send(chan, flip_trick->tpye, flip_trick->value);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FLIP_TRICK, (const char *)flip_trick, MAVLINK_MSG_ID_FLIP_TRICK_MIN_LEN, MAVLINK_MSG_ID_FLIP_TRICK_LEN, MAVLINK_MSG_ID_FLIP_TRICK_CRC);
#endif
}

#if MAVLINK_MSG_ID_FLIP_TRICK_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_flip_trick_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint8_t tpye, float value)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_float(buf, 0, value);
    _mav_put_uint8_t(buf, 4, tpye);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FLIP_TRICK, buf, MAVLINK_MSG_ID_FLIP_TRICK_MIN_LEN, MAVLINK_MSG_ID_FLIP_TRICK_LEN, MAVLINK_MSG_ID_FLIP_TRICK_CRC);
#else
    mavlink_flip_trick_t *packet = (mavlink_flip_trick_t *)msgbuf;
    packet->value = value;
    packet->tpye = tpye;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_FLIP_TRICK, (const char *)packet, MAVLINK_MSG_ID_FLIP_TRICK_MIN_LEN, MAVLINK_MSG_ID_FLIP_TRICK_LEN, MAVLINK_MSG_ID_FLIP_TRICK_CRC);
#endif
}
#endif

#endif

// MESSAGE FLIP_TRICK UNPACKING


/**
 * @brief Get field tpye from flip_trick message
 *
 * @return  0 for canle.
 */
static inline uint8_t mavlink_msg_flip_trick_get_tpye(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field value from flip_trick message
 *
 * @return [degree] flip value.
 */
static inline float mavlink_msg_flip_trick_get_value(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Decode a flip_trick message into a struct
 *
 * @param msg The message to decode
 * @param flip_trick C-struct to decode the message contents into
 */
static inline void mavlink_msg_flip_trick_decode(const mavlink_message_t* msg, mavlink_flip_trick_t* flip_trick)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    flip_trick->value = mavlink_msg_flip_trick_get_value(msg);
    flip_trick->tpye = mavlink_msg_flip_trick_get_tpye(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_FLIP_TRICK_LEN? msg->len : MAVLINK_MSG_ID_FLIP_TRICK_LEN;
        memset(flip_trick, 0, MAVLINK_MSG_ID_FLIP_TRICK_LEN);
    memcpy(flip_trick, _MAV_PAYLOAD(msg), len);
#endif
}
