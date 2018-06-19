
/**
   * @file tstCallCommandInterfaceCAPI.h
   * @brief Declaration file of class tstCallCommandInterfaceCAPI.
   *
   */

#ifndef tstCallCommandInterfaceCAPI_H
#define tstCallCommandInterfaceCAPI_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

//#include <dbus/dbus.h>
#include <CommonAPI/CommonAPI.hpp>
#include <v1/org/genivi/am/commandinterface/CommandControlProxy.hpp>
#include <v1/org/genivi/am/AudioManagerTypes.hpp>
//#include "CAmSocketHandler.h"

#define am_types v1::org::genivi::am::AudioManagerTypes
#define cmd_interface v1::org::genivi::am::commandinterface

namespace am
{
//class tstCAPIWrapper;
class tstGetString;

struct sourceConnectID_s_CAPI
{
public:
    am_types::am_mainConnectionID_t mediaConnectionID;
    am_types::am_mainConnectionID_t naviConnectionID;
    am_types::am_mainConnectionID_t phoneConnectionID;
    am_types::am_mainConnectionID_t ttsConnectionID;
    am_types::am_mainConnectionID_t reverseConnectionID;
};

class tstCallCommandInterfaceCAPI
{
public:
    tstCallCommandInterfaceCAPI();
    ~tstCallCommandInterfaceCAPI();

    int connectSource();
    int disConnectSource();
    int setPrimarySinkVoulme();
    int primarySinkVolumeStep();
    int setPrimarySinkMuteState();
    int setSecondarySinkVoulme();
    int secondarySinkVolumeStep();
    int setSecondarySinkMuteState();
    int setMainSinkSoundProperty();
    int setMainSourceSoundProperty();
    int setSystemProperty();
    int setSinkMainNotificationConfiguration();
    int setSourceMainNotificationConfiguration();
    int getSinkMuteState();
    int getListMainConnections();
    int getListMainSinks();
    int getListMainSources();
    int getListMainSinkSoundProperties();
    int getListMainSourceSoundProperties();
    int getListSinkClasses();
    int getListSourceClasses();
    int getListSystemProperties();
    int getTimingInformation();
    int getListSinkMainNotificationConfigurations();
    int getListSourceMainNotificationConfigurations();
    int getInterfaceVersion();
    int getListOfMainConnections();
    int startProxy();
    void runListen();
    void stopListen();
    void onServiceStatusEvent(const CommonAPI::AvailabilityStatus& serviceStatus);

private:
    void onNewMainConnection(const am_types::am_MainConnectionType_s& mainConnections);
    void removedMainConnection(am_types::am_mainConnectionID_t mainConnection);
    void onNumberOfSourceClassesChangedEvent();
    void onMainConnectionStateChangedEvent(am_types::am_mainConnectionID_t connectionID, am_types::am_ConnectionState_e connectionState);
    void onSourceAddedEvent(const am_types::am_SourceType_s& sources);
    void onSourceRemovedEvent(am_types::am_sourceID_t source);
    void onMainSourceSoundPropertyChangedEvent(am_types::am_sourceID_t sourceID, const am_types::am_MainSoundProperty_s& soundProperty);
    void onSourceAvailabilityChangedEvent(am_types::am_sourceID_t sourceID, const am_types::am_Availability_s& availability);
    void onNumberOfSinkClassesChangedEvent();
    void onSinkAddedEvent(const am_types::am_SinkType_s& sinks);
    void onSinkRemovedEvent(am_types::am_sinkID_t sink);
    void onMainSinkSoundPropertyChangedEvent(am_types::am_sinkID_t sinkID, const am_types::am_MainSoundProperty_s& soundProperty);
    void onSinkAvailabilityChangedEvent(am_types::am_sinkID_t sinkID, const am_types::am_Availability_s& availability);
    void onVolumeChangedEvent(am_types::am_sinkID_t sinkID, am_types::am_mainVolume_t volume);
    void onSinkMuteStateChangedEvent(am_types::am_sinkID_t sinkID, am_types::am_MuteState_e muteState);
    void onSystemPropertyChangedEvent(const am_types::am_SystemProperty_s& systemProperty);
    void onTimingInformationChangedEvent(am_types::am_mainConnectionID_t mainConnectionID, am_types::am_timeSync_t time);
    void onSinkUpdatedEvent(am_types::am_sinkID_t sinkID, am_types::am_sinkClass_t sinkClassID, const am_types::am_MainSoundProperty_L& listMainSoundProperties);
    void onSourceUpdatedEvent(am_types::am_sourceID_t sourceID, am_types::am_sourceClass_t sourceClassID, const am_types::am_MainSoundProperty_L& listMainSoundProperties);
    void onSinkNotificationEvent(am_types::am_sinkID_t sinkID, const am_types::am_NotificationPayload_s& notification);
    void onSourceNotificationEvent(am_types::am_sourceID_t sourceID, const am_types::am_NotificationPayload_s& notification);
    void onMainSinkNotificationConfigurationChangedEvent(am_types::am_sinkID_t sinkID, const am_types::am_NotificationConfiguration_s& mainNotificationConfiguration);
    void onMainSourceNotificationConfigurationChangedEvent(am_types::am_sourceID_t sourceID, const am_types::am_NotificationConfiguration_s& mainNotificationConfiguration);
    void onListOfMainConnections(const am_types::am_MainConnection_L& listMainConnections);

private:
    std::string domain;
    std::string instance;
    std::shared_ptr<cmd_interface::CommandControlProxy<>> cmdControlProxy;
    CommonAPI::CallStatus callStatus;
    am_types::am_Error_e amError;
    am_types::am_sourceID_t  m_sourceID;
    am_types::am_sinkID_t  m_sinkID;
    sourceConnectID_s_CAPI m_mainConnectionID;
    am_types::am_mainConnectionID_t m_lastConnectID;
    tstGetString* getString;
    uint16_t primarySinkToneVolume;
    uint16_t primarySinkMuteState;
    uint16_t secondarySinkToneVolume;
    uint16_t secondarySinkMuteState;
    bool isInnerCall;
    bool m_stopListen;

private:
//    CAmSocketHandler iSocketHandler;
//    tstCAPIWrapper* wrapper;
    bool mIsServiceAvailable;
};
}

#endif // tstCallCommandInterfaceCAPI_H
/* EOF */
