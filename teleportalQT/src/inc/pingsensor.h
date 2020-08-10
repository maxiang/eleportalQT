#ifndef PINGSENSOR_H
#define PINGSENSOR_H

#include <QObject>
#include <abstractlink.h>
#include <parser.h>
#include <ping-message-common.h>
#include <ping-message-ping1d.h>
#include <ping-message.h>
class PingSensor : public QObject
{
    Q_OBJECT
public:
    explicit PingSensor(QObject *parent = nullptr);
    void connectLink();
    void printStatus();
    void printSensorInformation();
    void writeMessage(const ping_message& msg) const;
    void startPreConfigurationProcess();
    struct CommonVariables {
        QString ascii_text;
        uint8_t dstId {0};

        struct {
            bool initialized = false;
            uint8_t device_revision {0};
            uint8_t device_type {0};
            uint8_t firmware_version_major {0};
            uint8_t firmware_version_minor {0};
            uint8_t firmware_version_patch {0};
        } deviceInformation;

        QString nack_msg;
        uint8_t protocol_version_major {0};
        uint8_t protocol_version_minor {0};
        uint8_t protocol_version_patch {0};
        uint8_t srcId {0};

        /**
         * @brief Reset variables to default values
         *
         */
        inline void reset() { *this = {}; }
    } _commonVariables;
    void request(int id);
    void handleMessage(const ping_message& msg);
    uint32_t    getDistance();
    uint16_t    getConfidence();
   static uint16_t _firmwareDefaultPingInterval;
signals:
    void parserErrorsChanged();
    void connectionClose();
    void connectionOpen();
    void connectionChanged();
    void linkChanged();
    void distanceConfidenceChanged();
private slots:
    void handleMessagePrivate(const ping_message& msg);
private:
    QSharedPointer<AbstractLink> _abstractLink;
    Parser*                       _parser;
    ///@{
    // The firmware defaults at boot
    static const bool _firmwareDefaultAutoMode;
    static const int _firmwareDefaultGainSetting;
    static const bool _firmwareDefaultPingEnable;
    static const uint32_t _firmwareDefaultSpeedOfSound;
    static const int _pingMaxFrequency;
    uint16_t _board_voltage;
    uint8_t _device_model;
    uint32_t _distance = 0; // mm
    uint16_t _confidence = 0; // 0-100%
    uint16_t _transmit_duration = 0;
    uint32_t _ping_number = 0;
    uint32_t _scan_start = 0;
    uint32_t _scan_length = 0;
    uint32_t _gain_setting = 0;
    uint32_t _speed_of_sound = 0;
    bool _ping_enable;
    uint16_t _pcb_temperature;
    uint16_t _processor_temperature;
    ///@}
    static const uint16_t _num_points = 200;
    QTimer _periodicRequestTimer;
    struct messageStatus {
        // Requested and acknowledge
        int ack = 0;
        // Requested and not acknowledge
        int nack = 0;
        // Number of waiting replies
        int waiting = 0;
    };
     QHash<uint16_t, messageStatus> requestedIds;

};

#endif // PINGSENSOR_H
