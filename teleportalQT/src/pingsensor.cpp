#include "pingsensor.h"
#include <pingparserext.h>
#include <udplink.h>
const int PingSensor::_pingMaxFrequency = 50;

const bool PingSensor::_firmwareDefaultAutoMode = true;
const int PingSensor::_firmwareDefaultGainSetting = 1;
const bool PingSensor::_firmwareDefaultPingEnable = true;
 uint16_t PingSensor::_firmwareDefaultPingInterval = 250;
const uint32_t PingSensor::_firmwareDefaultSpeedOfSound = 1500;
PingSensor::PingSensor(QObject *parent) : QObject(parent)
{
    _abstractLink=QSharedPointer<AbstractLink>(new UDPLink());
    _abstractLink->setParent(this);
    _abstractLink->setName("Ping1D");


    _parser = new PingParserExt();
    connect(dynamic_cast<PingParserExt*>(_parser), &PingParserExt::newMessage, this, &PingSensor::handleMessagePrivate,
        Qt::DirectConnection);
    connect(dynamic_cast<PingParserExt*>(_parser), &PingParserExt::parseError, this, &PingSensor::parserErrorsChanged);
     _periodicRequestTimer.setInterval(PingSensor::_firmwareDefaultPingInterval);

     connect(&_periodicRequestTimer, &QTimer::timeout, this, [this] {
         if (!_abstractLink->isWritable()) {
             qDebug() << "Can't write in this type of link.";
             _periodicRequestTimer.stop();
             return;
         }

         if (!_abstractLink->isOpen()) {
             qDebug()<< "Can't write, port is not open!";
             _periodicRequestTimer.stop();
             return;
         }
         request(Ping1dId::PCB_TEMPERATURE);
         request(Ping1dId::PROCESSOR_TEMPERATURE);
         request(Ping1dId::VOLTAGE_5);
         request(Ping1dId::MODE_AUTO);
         request(Ping1dId::DISTANCE_SIMPLE);
         //request(Ping1dId::PROFILE);
     });



     connect(this, &PingSensor::connectionOpen, this, &PingSensor::startPreConfigurationProcess);
     connectLink();

}

void PingSensor::connectLink()
{
    LinkConfiguration conConf;
    conConf.setName("Ping1D");
    conConf.setType(LinkType::Udp);
   conConf.setArgs({"192.168.2.2","9090"});
    conConf.setDeviceType(PingDeviceType::PING1D);
   // conConf.setArgs({"127.0.0.1","6676"});
    if (_abstractLink->isOpen()) {
        _abstractLink->finishConnection();
    }

    qDebug()<< "Connecting to"<<conConf;

    if (_abstractLink) {
       _abstractLink.clear();
    }

    _abstractLink= QSharedPointer<AbstractLink>(new UDPLink());
    _abstractLink->setParent(this);
    _abstractLink->setConfiguration(conConf);
    _abstractLink->startConnection();

    if (!_abstractLink->isOpen()) {
        qDebug()<< "Connection fail !";
        emit connectionClose();
        return;
    }

    emit linkChanged();

    if (_parser) {
        connect(_abstractLink.get(), &AbstractLink::newData, _parser, &Parser::parseBuffer, Qt::DirectConnection);
    }

    emit connectionOpen();
    startPreConfigurationProcess();
}

void PingSensor::printStatus()
{
    qDebug()<< "PingSensor Status:";
    qDebug()<< "\t- srcId:" << _commonVariables.srcId;
    qDebug()<< "\t- dstID:" << _commonVariables.dstId;
    qDebug()<< "\t- device_type:" << _commonVariables.deviceInformation.device_type;
    qDebug()<< "\t- device_revision:" << _commonVariables.deviceInformation.device_revision;
    qDebug()<< "\t- protocol_version_major:" << _commonVariables.protocol_version_major;
    qDebug()<< "\t- protocol_version_minor:" << _commonVariables.protocol_version_minor;
    qDebug()<< "\t- protocol_version_patch:" << _commonVariables.protocol_version_patch;
    qDebug()<< "\t- firmware_version_major:"
            << _commonVariables.deviceInformation.firmware_version_major;
    qDebug()<< "\t- firmware_version_minor:"
            << _commonVariables.deviceInformation.firmware_version_minor;
    qDebug()<< "\t- firmware_version_patch:"
            << _commonVariables.deviceInformation.firmware_version_patch;
    qDebug()<< "\t- ascii_text:" << _commonVariables.ascii_text;
    qDebug()<< "\t- nack_msg:" << _commonVariables.nack_msg;
    printSensorInformation();

}

void PingSensor::printSensorInformation()
{

}
void PingSensor::request(int id)
{
    if (!_abstractLink->isWritable()) {
        qDebug() << "Can't write in this type of link.";
        return;
    }
    //qDebug() << "Requesting:" << id;

    ping_message m(10);
    m.set_payload_length(0);
    m.set_message_id(id);
    m.updateChecksum();
    writeMessage(m);
}

void PingSensor::handleMessage(const ping_message &msg)
{
    qDebug()<< QStringLiteral("Handling Message: %1 [%2]")
                                       .arg(PingHelper::nameFromMessageId(
                                           static_cast<PingEnumNamespace::PingMessageId>(msg.message_id())))
                                       .arg(msg.message_id());

    auto& requestedId = requestedIds[msg.message_id()];
    if (requestedId.waiting) {
        requestedId.waiting--;
        requestedId.ack++;
    }

    switch (msg.message_id()) {

    // This message is deprecated, it provides no added information because
    // the device id is already supplied in every message header
    case Ping1dId::DEVICE_ID: {
        ping1d_device_id m(msg);
        _commonVariables.srcId = m.source_device_id();
    } break;

    case Ping1dId::DISTANCE: {
        ping1d_distance m(msg);
        _distance = m.distance();
        _confidence = m.confidence();
        _transmit_duration = m.transmit_duration();
        _ping_number = m.ping_number();
        _scan_start = m.scan_start();
        _scan_length = m.scan_length();
        _gain_setting = m.gain_setting();
    } break;

    case Ping1dId::DISTANCE_SIMPLE: {
        ping1d_distance_simple m(msg);
        _distance = m.distance();
        _confidence = m.confidence();
        emit distanceConfidenceChanged();
    } break;

    case Ping1dId::PROFILE: {
        ping1d_profile m(msg);
        _distance = m.distance();
        _confidence = m.confidence();
        _transmit_duration = m.transmit_duration();
        _ping_number = m.ping_number();
        _scan_start = m.scan_start();
        _scan_length = m.scan_length();
        _gain_setting = m.gain_setting();

    } break;

    case Ping1dId::PING_ENABLE: {
        ping1d_ping_enable m(msg);
        _ping_enable = m.ping_enabled();

    } break;

    case Ping1dId::RANGE: {
        ping1d_range m(msg);
        _scan_start = m.scan_start();
        _scan_length = m.scan_length();
    } break;

    case Ping1dId::GENERAL_INFO: {
        ping1d_general_info m(msg);
        _gain_setting = m.gain_setting();
    } break;

    case Ping1dId::GAIN_SETTING: {
        ping1d_gain_setting m(msg);
        _gain_setting = m.gain_setting();
    } break;

    case Ping1dId::SPEED_OF_SOUND: {
        ping1d_speed_of_sound m(msg);
        _speed_of_sound = m.speed_of_sound();
    } break;

    case Ping1dId::PROCESSOR_TEMPERATURE: {
        ping1d_processor_temperature m(msg);
        _processor_temperature = m.processor_temperature();
        break;
    }

    case Ping1dId::PCB_TEMPERATURE: {
        ping1d_pcb_temperature m(msg);
        _pcb_temperature = m.pcb_temperature();
        break;
    }

    case Ping1dId::VOLTAGE_5: {
        ping1d_voltage_5 m(msg);
        _board_voltage = m.voltage_5(); // millivolts
        break;
    }

    default:
        qDebug()<< "UNHANDLED MESSAGE ID:" << msg.message_id();
        break;
    }

}

uint32_t PingSensor::getDistance()
{
    return _distance;
}

uint16_t PingSensor::getConfidence()
{
    return _confidence;
}

void PingSensor::writeMessage(const ping_message &msg) const
{
    if (_abstractLink&&_abstractLink->isOpen() && _abstractLink->isWritable()) {
        _abstractLink->write(reinterpret_cast<const char*>(msg.msgData), msg.msgDataLength());
    }
}

void PingSensor::startPreConfigurationProcess()
{
    qDebug() << "Start pre configuration task and requests.";
    if (!_abstractLink->isWritable()) {
        qDebug()<< "It's only possible to set last configuration when link is writable.";
        return;
    }

    // Request device information
    request(Ping1dId::PING_ENABLE);
    request(Ping1dId::MODE_AUTO);
    request(Ping1dId::PROFILE);
    request(Ping1dId::FIRMWARE_VERSION);
    request(Ping1dId::DEVICE_ID);
    request(Ping1dId::SPEED_OF_SOUND);

    // Start periodic request timer
    _periodicRequestTimer.start();
}

void PingSensor::handleMessagePrivate(const ping_message &msg)
{
    if (_commonVariables.dstId != msg.destination_device_id()) {
        _commonVariables.dstId = msg.destination_device_id();

    }

    if (_commonVariables.srcId != msg.source_device_id()) {
        _commonVariables.srcId = msg.source_device_id();

    }

    switch (msg.message_id()) {

    case CommonId::ACK: {
        common_ack ackMessage {msg};
        qDebug()<< "ACK message:" << ackMessage.acked_id();
        break;
    }

    case CommonId::NACK: {
        common_nack nackMessage {msg};
        qDebug()<< "Sensor NACK!";
        _commonVariables.nack_msg = QString("%1: %2").arg(nackMessage.nack_message()).arg(nackMessage.nacked_id());
        qDebug()<< "NACK message:" << _commonVariables.nack_msg;

        break;
    }

    // needs dynamic-payload patch
    case CommonId::ASCII_TEXT: {
        _commonVariables.ascii_text = common_ascii_text(msg).ascii_message();
        qDebug()<< "Sensor status:" << _commonVariables.ascii_text;
        break;
    }

    case CommonId::DEVICE_INFORMATION: {
        common_device_information m(msg);

        _commonVariables.deviceInformation.initialized = true;
        _commonVariables.deviceInformation.device_type = m.device_type();
        _commonVariables.deviceInformation.device_revision = m.device_revision();
        _commonVariables.deviceInformation.firmware_version_major = m.firmware_version_major();
        _commonVariables.deviceInformation.firmware_version_minor = m.firmware_version_minor();
        _commonVariables.deviceInformation.firmware_version_patch = m.firmware_version_patch();
        break;
    }

    case CommonId::PROTOCOL_VERSION: {
        common_protocol_version m(msg);

        _commonVariables.protocol_version_major = m.version_major();
        _commonVariables.protocol_version_minor = m.version_minor();
        _commonVariables.protocol_version_patch = m.version_patch();
        break;
    }

    // Will be deprecated in future firmware versions of Ping1D
    case Ping1dId::FIRMWARE_VERSION: {
        ping1d_firmware_version m(msg);

        m.device_type();
        // Ping1D uses device_model as device_type to specify which sersion is it
        _commonVariables.deviceInformation.initialized = true;
        _commonVariables.deviceInformation.device_type = m.device_model();
        _commonVariables.deviceInformation.firmware_version_major = m.firmware_version_major();
        _commonVariables.deviceInformation.firmware_version_minor = m.firmware_version_minor();

        // Since this is a specific Ping1D message with an old firmware, we can assume device_type and device_revision
        _commonVariables.deviceInformation.device_type
            = static_cast<uint8_t>(PingDeviceType::PING1D); // move type to enum
        _commonVariables.deviceInformation.device_revision = 0;


        break;
    }

    default:
        // Not a common message
        break;
    }

    handleMessage(msg);
}
