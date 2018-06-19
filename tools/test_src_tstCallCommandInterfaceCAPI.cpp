

#include <iostream>
#include <unistd.h>
#include "tstCallCommandInterfaceCAPI.h"
#include "tstGetString.h"
#include "tstDefine.h"

namespace am
{
tstCallCommandInterfaceCAPI::tstCallCommandInterfaceCAPI()
{
    domain = "local";
    instance = "nutshell_genivi_audiomanager";
    cmdControlProxy = NULL;
    getString = new tstGetString();
    m_sourceID = 0;
    m_sinkID = 0;
    m_mainConnectionID = {0 , 0 , 0, 0, 0};
    m_lastConnectID = 0;
    primarySinkToneVolume = 0;
    primarySinkMuteState = 0;
    secondarySinkToneVolume = 0;
    secondarySinkMuteState = 0;
    isInnerCall = false;
    mIsServiceAvailable = false;
    m_stopListen = false;
}

tstCallCommandInterfaceCAPI::~tstCallCommandInterfaceCAPI()
{
    cmdControlProxy = NULL;
    if (NULL != getString)
    {
        delete getString;
        getString = NULL;
    }
}

int tstCallCommandInterfaceCAPI::startProxy()
{
    cout << "tstCallCommandInterfaceCAPI startProxy called" << endl;
    std::shared_ptr < CommonAPI::Runtime > runtime = CommonAPI::Runtime::get();

    cmdControlProxy = runtime->buildProxy<cmd_interface::CommandControlProxy>(domain, instance);
    if (!cmdControlProxy) {
        std::cerr << "Error: Unable to build command control proxy!\n";
    }
    return RETURN_EXIT_SUCCESS;
}

void tstCallCommandInterfaceCAPI::runListen()
{
    cout << "tstCallCommandInterfaceCAPI runListen called" << endl;
    cmdControlProxy->getProxyStatusEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onServiceStatusEvent, this, std::placeholders::_1));
//    iSocketHandler.start_listenting();
    while (!m_stopListen) {
        sleep(1);
        continue;
    }
}

void tstCallCommandInterfaceCAPI::stopListen()
{
    cout << "tstCallCommandInterfaceCAPI stopListen called" << endl;
    m_stopListen = true;
}

void tstCallCommandInterfaceCAPI::onServiceStatusEvent(const CommonAPI::AvailabilityStatus& serviceStatus)
{
    cout << "tstCallCommandInterfaceCAPI onServiceStatusEvent called" << endl;
    mIsServiceAvailable = (serviceStatus == CommonAPI::AvailabilityStatus::AVAILABLE);
    if (mIsServiceAvailable) {
        cmdControlProxy->getNumberOfSourceClassesChangedEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onNumberOfSourceClassesChangedEvent, this));
        cmdControlProxy->getNewMainConnectionEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onNewMainConnection, this, std::placeholders::_1));
        cmdControlProxy->getRemovedMainConnectionEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::removedMainConnection, this, std::placeholders::_1));
        cmdControlProxy->getMainConnectionStateChangedEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onMainConnectionStateChangedEvent, this, std::placeholders::_1, std::placeholders::_2));
        cmdControlProxy->getNewSourceEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onSourceAddedEvent, this, std::placeholders::_1));
        cmdControlProxy->getRemovedSourceEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onSourceRemovedEvent, this, std::placeholders::_1));
        cmdControlProxy->getMainSourceSoundPropertyChangedEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onMainSourceSoundPropertyChangedEvent, this, std::placeholders::_1, std::placeholders::_2));
        cmdControlProxy->getSourceAvailabilityChangedEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onSourceAvailabilityChangedEvent, this, std::placeholders::_1, std::placeholders::_2));
        cmdControlProxy->getNumberOfSinkClassesChangedEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onNumberOfSinkClassesChangedEvent, this));
        cmdControlProxy->getNewSinkEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onSinkAddedEvent, this, std::placeholders::_1));
        cmdControlProxy->getRemovedSinkEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onSinkRemovedEvent, this, std::placeholders::_1));
        cmdControlProxy->getMainSinkSoundPropertyChangedEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onMainSinkSoundPropertyChangedEvent, this, std::placeholders::_1, std::placeholders::_2));
        cmdControlProxy->getSinkAvailabilityChangedEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onSinkAvailabilityChangedEvent, this, std::placeholders::_1, std::placeholders::_2));
        cmdControlProxy->getVolumeChangedEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onVolumeChangedEvent, this, std::placeholders::_1, std::placeholders::_2));
        cmdControlProxy->getSinkMuteStateChangedEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onSinkMuteStateChangedEvent, this, std::placeholders::_1, std::placeholders::_2));
        cmdControlProxy->getSystemPropertyChangedEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onSystemPropertyChangedEvent, this, std::placeholders::_1));
        cmdControlProxy->getTimingInformationChangedEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onTimingInformationChangedEvent, this,
                std::placeholders::_1, std::placeholders::_2));
        cmdControlProxy->getSinkUpdatedEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onSinkUpdatedEvent, this,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        cmdControlProxy->getSourceUpdatedEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onSourceUpdatedEvent, this,
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        cmdControlProxy->getSinkNotificationEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onSinkNotificationEvent, this,
                std::placeholders::_1, std::placeholders::_2));
        cmdControlProxy->getSourceNotificationEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onSourceNotificationEvent, this,
                std::placeholders::_1, std::placeholders::_2));
        cmdControlProxy->getMainSinkNotificationConfigurationChangedEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onMainSinkNotificationConfigurationChangedEvent, this,
                std::placeholders::_1, std::placeholders::_2));
        cmdControlProxy->getMainSourceNotificationConfigurationChangedEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onMainSourceNotificationConfigurationChangedEvent, this,
                std::placeholders::_1, std::placeholders::_2));
        cmdControlProxy->getListOfMainConnectionsAttribute().getChangedEvent().subscribe(std::bind(&tstCallCommandInterfaceCAPI::onListOfMainConnections, this, std::placeholders::_1));
    }
}

void tstCallCommandInterfaceCAPI::onNewMainConnection(const am_types::am_MainConnectionType_s& mainConnections)
{
    cout << "tstCallCommandInterfaceCAPI onNewMainConnection called" << endl;
    cout << "new mainConnectionID : " << mainConnections.getMainConnectionID() << endl;
    cout << "sourceID : " << mainConnections.getSourceID() << endl;
    cout << "sinkID : " << mainConnections.getSinkID() << endl;
    cout << "delay : " << mainConnections.getDelay() << endl;
    cout << "connectionState : " << getString->getStringConnectionState(mainConnections.getConnectionState()) << endl;
}
void tstCallCommandInterfaceCAPI::removedMainConnection(am_types::am_mainConnectionID_t mainConnection)
{
    cout << "Call back removedMainConnection called" << endl;
    cout << "reomove mainConnectionID : " << mainConnection << endl;
}
void tstCallCommandInterfaceCAPI::onNumberOfSourceClassesChangedEvent()
{
    cout << "Call back onNumberOfSourceClassesChangedEvent called" << endl;
}
void tstCallCommandInterfaceCAPI::onMainConnectionStateChangedEvent(am_types::am_mainConnectionID_t connectionID, am_types::am_ConnectionState_e connectionState)
{
    cout << "Call back onMainConnectionStateChangedEvent called" << endl;
    cout << "connectionID : " << connectionID << endl;
    cout << "connectionState : " << getString->getStringConnectionState(connectionState) << endl;
}
void tstCallCommandInterfaceCAPI::onSourceAddedEvent(const am_types::am_SourceType_s& sources)
{
    cout << "Call back onSourceAddedEvent called" << endl;
    cout << "Add sourceID : " << sources.getSourceID() << endl;
    cout << "name : " << sources.getName().c_str() << endl;
    cout << "availability : " << getString->getStringAvailability(sources.getAvailability().getAvailability()) << endl;
    cout << "availabilityReason : " << getString->getStringCustomAvailabilityReason(sources.getAvailability().getAvailabilityReason()) << endl;
    cout << "sourceClassID : " << sources.getSourceClassID() << endl;
}
void tstCallCommandInterfaceCAPI::onSourceRemovedEvent(am_types::am_sourceID_t source)
{
    cout << "Call back onSourceRemovedEvent called" << endl;
    cout << "Remove sourceID : " << source << endl;
}
void tstCallCommandInterfaceCAPI::onMainSourceSoundPropertyChangedEvent(am_types::am_sourceID_t sourceID, const am_types::am_MainSoundProperty_s& soundProperty)
{
    cout << "Call back onMainSourceSoundPropertyChangedEvent called" << endl;
    cout << "sourceID : " << sourceID << endl;
    cout << "type : " << getString->getStringCustomMainSoundPropertyType(soundProperty.getType()) << endl;
    cout << "value : " << soundProperty.getValue() << endl;
}
void tstCallCommandInterfaceCAPI::onSourceAvailabilityChangedEvent(am_types::am_sourceID_t sourceID, const am_types::am_Availability_s& availability)
{
    cout << "Call back onSourceAvailabilityChangedEvent called" << endl;
    cout << "sourceID : " << sourceID << endl;
    cout << "availability : " << getString->getStringAvailability(availability.getAvailability()) << endl;
    cout << "availabilityReason : " << getString->getStringCustomAvailabilityReason(availability.getAvailabilityReason()) << endl;
}
void tstCallCommandInterfaceCAPI::onNumberOfSinkClassesChangedEvent()
{
    cout << "Call back onNumberOfSinkClassesChangedEvent called" << endl;
}
void tstCallCommandInterfaceCAPI::onSinkAddedEvent(const am_types::am_SinkType_s& sinks)
{
    cout << "Call back onSinkAddedEvent called" << endl;
    cout << "Add sinkID : " << sinks.getSinkID() << endl;
    cout << "name : " << sinks.getName().c_str() << endl;
    cout << "availability : " << getString->getStringAvailability(sinks.getAvailability().getAvailability()) << endl;
    cout << "availabilityReason : " << getString->getStringCustomAvailabilityReason(sinks.getAvailability().getAvailabilityReason()) << endl;
    cout << "volume : " << sinks.getVolume() << endl;
    cout << "muteState : " << getString->getStringMuteState(sinks.getMuteState()) << endl;
    cout << "sinkClassID : " << sinks.getSinkClassID() << endl;
}
void tstCallCommandInterfaceCAPI::onSinkRemovedEvent(am_types::am_sinkID_t sink)
{
    cout << "Call back onSinkRemovedEvent called" << endl;
    cout << "Remove sinkID : " << sink << endl;
}
void tstCallCommandInterfaceCAPI::onMainSinkSoundPropertyChangedEvent(am_types::am_sinkID_t sinkID, const am_types::am_MainSoundProperty_s& soundProperty)
{
    cout << "Call back onMainSinkSoundPropertyChangedEvent called" << endl;
    cout << "sinkID : " << sinkID << endl;
    cout << "type : " << getString->getStringCustomMainSoundPropertyType(soundProperty.getType()) << endl;
    cout << "value : " << soundProperty.getValue() << endl;
}
void tstCallCommandInterfaceCAPI::onSinkAvailabilityChangedEvent(am_types::am_sinkID_t sinkID, const am_types::am_Availability_s& availability)
{
    cout << "Call back onSinkAvailabilityChangedEvent called" << endl;
}
void tstCallCommandInterfaceCAPI::onVolumeChangedEvent(am_types::am_sinkID_t sinkID, am_types::am_mainVolume_t volume)
{
    cout << "Call back onVolumeChangedEvent called" << endl;
    cout << "sinkID : " << sinkID << endl;
    cout << "volume : " << volume << endl;
}
void tstCallCommandInterfaceCAPI::onSinkMuteStateChangedEvent(am_types::am_sinkID_t sinkID, am_types::am_MuteState_e muteState)
{
    cout << "Call back onSinkMuteStateChangedEvent called" << endl;
    cout << "sinkID : " << sinkID << endl;
    cout << "muteState : " <<  getString->getStringMuteState(muteState) << endl;
}
void tstCallCommandInterfaceCAPI::onSystemPropertyChangedEvent(const am_types::am_SystemProperty_s& systemProperty)
{
    cout << "Call back onSystemPropertyChangedEvent called" << endl;
    cout << "type : " << (systemProperty.getType() == 0 ? "SYP_UNKNOWN" : "SYP_ERROR")  << endl;
    cout << "value : " << systemProperty.getValue() << endl;
}
void tstCallCommandInterfaceCAPI::onTimingInformationChangedEvent(am_types::am_mainConnectionID_t mainConnectionID, am_types::am_timeSync_t time)
{
    cout << "Call back onTimingInformationChangedEvent called" << endl;
    cout << "mainConnectionID : " << mainConnectionID << endl;
    cout << "time : " << time << endl;
}
void tstCallCommandInterfaceCAPI::onSinkUpdatedEvent(am_types::am_sinkID_t sinkID, am_types::am_sinkClass_t sinkClassID, const am_types::am_MainSoundProperty_L& listMainSoundProperties)
{
    cout << "Call back onSinkUpdatedEvent called" << endl;
    cout << "sinkID : " << sinkID << endl;
    cout << "sinkClassID : " << sinkClassID << endl;
    std::vector<am_types::am_MainSoundProperty_s>::const_iterator listIterator = listMainSoundProperties.begin();
    for (; listIterator < listMainSoundProperties.end(); ++listIterator)
    {
        cout << "type : " << getString->getStringCustomMainSoundPropertyType(listIterator->getType()) << endl;
        cout << "value : " << listIterator->getValue() << endl;
    }
}
void tstCallCommandInterfaceCAPI::onSourceUpdatedEvent(am_types::am_sourceID_t sourceID, am_types::am_sourceClass_t sourceClassID, const am_types::am_MainSoundProperty_L& listMainSoundProperties)
{
    cout << "Call back onSourceUpdatedEvent called" << endl;
    cout << "sourceID : " << sourceID << endl;
    cout << "sourceClassID : " << sourceClassID << endl;
    std::vector<am_types::am_MainSoundProperty_s>::const_iterator listIterator = listMainSoundProperties.begin();
    for (; listIterator < listMainSoundProperties.end(); ++listIterator)
    {
        cout << "type : " << getString->getStringCustomMainSoundPropertyType(listIterator->getType()) << endl;
        cout << "value : " << listIterator->getValue() << endl;
    }
}
void tstCallCommandInterfaceCAPI::onSinkNotificationEvent(am_types::am_sinkID_t sinkID, const am_types::am_NotificationPayload_s& notification)
{
    cout << "Call back onSinkNotificationEvent called" << endl;
    cout << "sinkID : " << sinkID << endl;
    cout << "type : " << (notification.getType() == 0 ? "NT_UNKNOWN" : "NT_ERROR") << endl;
    cout << "value : " << notification.getValue() << endl;
}
void tstCallCommandInterfaceCAPI::onSourceNotificationEvent(am_types::am_sourceID_t sourceID, const am_types::am_NotificationPayload_s& notification)
{
    cout << "Call back onSourceNotificationEvent called" << endl;
    cout << "sourceID : " << sourceID << endl;
    cout << "type : " << (notification.getType() == 0 ? "NT_UNKNOWN" : "NT_ERROR") << endl;
    cout << "value : " << notification.getValue() << endl;
}
void tstCallCommandInterfaceCAPI::onMainSinkNotificationConfigurationChangedEvent(am_types::am_sinkID_t sinkID, const am_types::am_NotificationConfiguration_s& mainNotificationConfiguration)
{
    cout << "Call back onMainSinkNotificationConfigurationChangedEvent called" << endl;
    cout << "sinkID : " << sinkID << endl;
    cout << "type : " << (mainNotificationConfiguration.getType() == 0 ? "NT_UNKNOWN" : "NT_ERROR") << endl;
    cout << "status : " <<  getString->getStringNotificationStatus(mainNotificationConfiguration.getStatus()) << endl;
    cout << "parameter : " << mainNotificationConfiguration.getParameter() << endl;
}
void tstCallCommandInterfaceCAPI::onMainSourceNotificationConfigurationChangedEvent(am_types::am_sourceID_t sourceID, const am_types::am_NotificationConfiguration_s& mainNotificationConfiguration)
{
    cout << "Call back onMainSourceNotificationConfigurationChangedEvent called" << endl;
    cout << "sourceID : " << sourceID << endl;
    cout << "type : " << (mainNotificationConfiguration.getType() == 0 ? "NT_UNKNOWN" : "NT_ERROR") << endl;
    cout << "status : " <<  getString->getStringNotificationStatus(mainNotificationConfiguration.getStatus()) << endl;
    cout << "parameter : " << mainNotificationConfiguration.getParameter() << endl;
}

void tstCallCommandInterfaceCAPI::onListOfMainConnections(const am_types::am_MainConnection_L& listMainConnections)
{
    cout << "Call back tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    std::vector<am_types::am_MainConnectionType_s>::const_iterator listIterator = listMainConnections.begin();
    for (u_int16_t size = 1; listIterator < listMainConnections.end(); ++listIterator)
    {
        cout << "    CONNECTION : " << size << endl;
        cout << "mainConnectionID : " << listIterator->getMainConnectionID() << endl;
        cout << "sourceID : " << listIterator->getSourceID() << endl;
        cout << "sinkID : " << listIterator->getSinkID() << endl;
        cout << "delay : " << listIterator->getDelay() << endl;
        cout << "connectionState : " <<  getString->getStringConnectionState(listIterator->getConnectionState()) << endl;
        cout << endl;
        switch (listIterator->getSourceID())
        {
        case MEDIA_SOURCE_ID:
        {
            m_mainConnectionID.mediaConnectionID = listIterator->getMainConnectionID();
            break;
        }
        case NAVI_SOURCE_ID:
        {
            m_mainConnectionID.naviConnectionID = listIterator->getMainConnectionID();
            break;
        }
        case PHONE_SOURCE_ID:
        {
            m_mainConnectionID.phoneConnectionID = listIterator->getMainConnectionID();
            break;
        }
        case TTS_SOURCE_ID:
        {
            m_mainConnectionID.ttsConnectionID = listIterator->getMainConnectionID();
            break;
        }
        case REVERSE_SOURCE_ID:
        {
            m_mainConnectionID.reverseConnectionID = listIterator->getMainConnectionID();
            break;
        }
        default:
            break;
        }
        ++ size;
    }
}

int tstCallCommandInterfaceCAPI::connectSource()
{
    //log(&tstCommandCAPI, DLT_LOG_INFO, "tstCallCommandInterfaceCAPI connectSource called");
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    if (false == isInnerCall)
    {
        getListMainConnections();
        isInnerCall = true;
    }
    int selectConnectSources = getString->selectConnectSource();
    switch (selectConnectSources)
    {
    case 1:    //Media
    {
        if (0 != m_mainConnectionID.mediaConnectionID)
            cout << "already existing media connection!" << endl;
        m_sourceID = MEDIA_SOURCE_ID;
        m_sinkID = PRIMARY_SINK_ID;
        break;
    }
    case 2:    //Navi
    {
        if (0 != m_mainConnectionID.naviConnectionID)
            cout << "already existing navi connection!" << endl;
        m_sourceID = NAVI_SOURCE_ID;
        m_sinkID = SECONDARY_SINK_ID;
        break;
    }
    case 3:    //Phone
    {
        if (0 != m_mainConnectionID.phoneConnectionID)
            cout << "already existing phone connection!" << endl;
        m_sourceID = PHONE_SOURCE_ID;
        m_sinkID = PRIMARY_SINK_ID;
        break;
    }
    case 4:    //TTS
    {
        if (0 != m_mainConnectionID.ttsConnectionID)
            cout << "already existing tts connection!" << endl;
        m_sourceID = TTS_SOURCE_ID;
        m_sinkID = SECONDARY_SINK_ID;
        break;
    }
    case 5:    //Reverse
    {
        if (0 != m_mainConnectionID.reverseConnectionID)
            cout << "already existing reverse connection!" << endl;
        m_sourceID = REVERSE_SOURCE_ID;
        m_sinkID = SECONDARY_SINK_ID;
        break;
    }
    default:
    {
        return RETURN_EXIT_FAILURE;
        break;
    }
    }/*END OF SWITH*/
    cmdControlProxy->connect(m_sourceID, m_sinkID, callStatus, m_lastConnectID, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    if ( am_types::am_Error_e::E_OK == amError)
    {
        switch (selectConnectSources) {
        case 1:
        {
            m_mainConnectionID.mediaConnectionID = m_lastConnectID;
            cout << "mainConnectionID.mediaConnectionID = " << m_mainConnectionID.mediaConnectionID << endl;
            break;
        }
        case 2:
        {
            m_mainConnectionID.naviConnectionID = m_lastConnectID;
            cout << "mainConnectionID.naviConnectionID = " << m_mainConnectionID.naviConnectionID << endl;
            break;
        }
        case 3:
        {
            m_mainConnectionID.phoneConnectionID = m_lastConnectID;
            cout << "mainConnectionID.phoneConnectionID = " << m_mainConnectionID.phoneConnectionID << endl;
            break;
        }
        case 4:
        {
            m_mainConnectionID.ttsConnectionID = m_lastConnectID;
            cout << "mainConnectionID.ttsConnectionID = " << m_mainConnectionID.ttsConnectionID << endl;
            break;
        }
        case 5:
        {
            m_mainConnectionID.reverseConnectionID = m_lastConnectID;
            cout << "mainConnectionID.reverseConnectionID = " << m_mainConnectionID.reverseConnectionID << endl;
            break;
        }
        default:
            break;
        }
    }
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::disConnectSource()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    if (false == isInnerCall)
    {
        getListMainConnections();
        isInnerCall = true;
    }
    int selectDisconnectSources = getString->selectConnectSource();
    am_types::am_mainConnectionID_t connectID = 0;
    switch (selectDisconnectSources) {
    case 1:    //Media
    {
        if (0 == m_mainConnectionID.mediaConnectionID)
            cout << "non mediaConnectionID" << endl;
        connectID = m_mainConnectionID.mediaConnectionID;
        break;
    }
    case 2:    //Navi
    {
        if (0 == m_mainConnectionID.naviConnectionID)
            cout << "non naviConnectionID" << endl;
        connectID = m_mainConnectionID.naviConnectionID;
        break;
    }
    case 3:    //Phone
    {
        if (0 == m_mainConnectionID.phoneConnectionID)
            cout << "non phoneConnectionID" << endl;
        connectID = m_mainConnectionID.phoneConnectionID;
        break;
    }
    case 4:    //TTS
    {
        if (0 == m_mainConnectionID.ttsConnectionID)
            cout << "non ttsConnectionID" << endl;
        connectID = m_mainConnectionID.ttsConnectionID;
        break;
    }
    case 5:    //Reverse
    {
        if (0 == m_mainConnectionID.reverseConnectionID)
            cout << "non reverseConnectionID" << endl;
        connectID = m_mainConnectionID.reverseConnectionID;
        break;
    }
    default:
    {
        return RETURN_EXIT_FAILURE;
        break;
    }
    }   //end of switch
    cmdControlProxy->disconnect(connectID, callStatus, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    if ( am_types::am_Error_e::E_OK == amError)
    {
        switch (selectDisconnectSources)
        {
        case 1:
        {
            cout << "Media source disconnect!" << endl;
            cout << "MediaConnectionID = " << connectID << endl;
            m_mainConnectionID.mediaConnectionID = 0;
            break;
        }
        case 2:
        {
            cout << "Navi source disconnect!" << endl;
            cout << "Navi ConnectionID = " << connectID << endl;
            m_mainConnectionID.naviConnectionID = 0;
            break;
        }
        case 3:
        {
            cout << "Phone source disconnect!" << endl;
            cout << "Phone ConnectionID = " << connectID << endl;
            m_mainConnectionID.phoneConnectionID = 0;
            break;
        }
        case 4:
        {
            cout << "TTS source disconnect!" << endl;
            cout << "TTS ConnectionID = " << connectID << endl;
            m_mainConnectionID.ttsConnectionID = 0;
            break;
        }
        case 5:
        {
            cout << "Reverse source disconnect!" << endl;
            cout << "Reverse ConnectionID = " << connectID << endl;
            m_mainConnectionID.reverseConnectionID = 0;
            break;
        }
        default:
            break;
        }
    }
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::setPrimarySinkVoulme()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    m_sinkID = PRIMARY_SINK_ID;
    primarySinkToneVolume ++;
    am_types::am_mainVolume_t volume = (primarySinkToneVolume % 6) * INIT_VOLUME / 5;
    cmdControlProxy->setVolume(m_sinkID, volume, callStatus, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    cout << "volume : " << volume << endl;
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::primarySinkVolumeStep()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    m_sinkID = PRIMARY_SINK_ID;
    int16_t volumeStep = VOLUME_STEP;
    cmdControlProxy->volumeStep(m_sinkID, volumeStep, callStatus, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::setPrimarySinkMuteState()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    m_sinkID = PRIMARY_SINK_ID;
    primarySinkMuteState ++ ;
    primarySinkMuteState %= 2;
    am_types::am_MuteState_e muteState = am_types::am_MuteState_e::MS_UNKNOWN;
    if (0 == primarySinkMuteState)
    {
        muteState = am_types::am_MuteState_e::MS_MUTED;
    }
    else
    {
        muteState = am_types::am_MuteState_e::MS_UNMUTED;
    }
    cmdControlProxy->setSinkMuteState(m_sinkID, muteState, callStatus, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    cout << "muteState : " <<  getString->getStringMuteState(muteState) << endl;
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::setSecondarySinkVoulme()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    m_sinkID = SECONDARY_SINK_ID;
    secondarySinkToneVolume ++;
    am_types::am_mainVolume_t volume = (secondarySinkToneVolume % 6) * INIT_VOLUME / 5;
    cmdControlProxy->setVolume(m_sinkID, volume, callStatus, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    cout << "volume : " << volume << endl;
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::secondarySinkVolumeStep()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    m_sinkID = SECONDARY_SINK_ID;
    int16_t volumeStep = VOLUME_STEP;
    cmdControlProxy->volumeStep(m_sinkID, volumeStep, callStatus, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::setSecondarySinkMuteState()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    m_sinkID = SECONDARY_SINK_ID;
    secondarySinkMuteState ++ ;
    secondarySinkMuteState %= 2;
    am_types::am_MuteState_e muteState = am_types::am_MuteState_e::MS_UNKNOWN;
    if (0 == secondarySinkMuteState)
    {
        muteState = am_types::am_MuteState_e::MS_MUTED;
    }
    else
    {
        muteState = am_types::am_MuteState_e::MS_UNMUTED;
    }
    cmdControlProxy->setSinkMuteState(m_sinkID, muteState, callStatus, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    cout << "muteState : " <<  getString->getStringMuteState(muteState) << endl;
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::setMainSinkSoundProperty()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    if ((ALL_SINKS + 1) ==  getString->selectSink(m_sinkID))
    {
        return RETURN_EXIT_FAILURE;
    }
    am_types::am_MainSoundProperty_s mainSoundProperty;
    mainSoundProperty.setType(2);
    mainSoundProperty.setValue(5);
    cmdControlProxy->setMainSinkSoundProperty(m_sinkID, mainSoundProperty, callStatus, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::setMainSourceSoundProperty()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    if ((ALL_SOURCES + 1) == getString->selectSource(m_sourceID))
    {
        return RETURN_EXIT_FAILURE;
    }
    am_types::am_MainSoundProperty_s mainSoundProperty;
    mainSoundProperty.setType(3);
    mainSoundProperty.setValue(5);
    cmdControlProxy->setMainSourceSoundProperty(m_sourceID, mainSoundProperty, callStatus, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::setSystemProperty()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    am_types::am_SystemProperty_s systemProperty;
    systemProperty.setType(0);
    systemProperty.setValue(5);
    cmdControlProxy->setSystemProperty(systemProperty, callStatus, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::setSinkMainNotificationConfiguration()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    if ((ALL_SINKS + 1) == getString->selectSink(m_sinkID))
    {
        return RETURN_EXIT_FAILURE;
    }
    am_types::am_NotificationConfiguration_s mainNotificationConfiguration;
    uint16_t type = 0;
    cout << "Please input a number : " << endl;
    type = getString->inputNummber();
    cout << "type : " << type << endl;
    mainNotificationConfiguration.setType(1);
    mainNotificationConfiguration.setStatus(am_types::am_NotificationStatus_e::NS_MINIMUM);
    mainNotificationConfiguration.setParameter(20);
    cmdControlProxy->setMainSinkNotificationConfiguration(m_sinkID, mainNotificationConfiguration, callStatus, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::setSourceMainNotificationConfiguration()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    if ((ALL_SOURCES + 1) == getString->selectSource(m_sourceID))
    {
        return RETURN_EXIT_FAILURE;
    }
    am_types::am_NotificationConfiguration_s mainNotificationConfiguration;
    mainNotificationConfiguration.setType(1);
    mainNotificationConfiguration.setStatus(am_types::am_NotificationStatus_e::NS_CHANGE);
    mainNotificationConfiguration.setParameter(15);
    cmdControlProxy->setMainSourceNotificationConfiguration(m_sourceID, mainNotificationConfiguration, callStatus, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::getSinkMuteState()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    if ((ALL_SINKS + 1) == getString->selectSink(m_sinkID))
    {
        return RETURN_EXIT_FAILURE;
    }
    am_types::am_MuteState_e muteState;
    cmdControlProxy->getSinkMuteState(m_sinkID, callStatus, muteState, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    cout << "muteState : " <<  getString->getStringMuteState(muteState) << endl;
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::getListMainConnections()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    am_types::am_MainConnection_L listConnections;
    cmdControlProxy->getListMainConnections(callStatus, listConnections, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    if ( am_types::am_Error_e::E_OK == amError)
    {
        std::vector<am_types::am_MainConnectionType_s>::const_iterator listIterator = listConnections.begin();
        for (u_int16_t size = 1; listIterator < listConnections.end(); ++listIterator)
        {
            cout << "    CONNECTION : " << size << endl;
            cout << "mainConnectionID : " << listIterator->getMainConnectionID() << endl;
            cout << "sourceID : " << listIterator->getSourceID() << endl;
            cout << "sinkID : " << listIterator->getSinkID() << endl;
            cout << "delay : " << listIterator->getDelay() << endl;
            cout << "connectionState : " <<  getString->getStringConnectionState(listIterator->getConnectionState()) << endl;
            cout << endl;
            switch (listIterator->getSourceID())
            {
            case MEDIA_SOURCE_ID:
            {
                m_mainConnectionID.mediaConnectionID = listIterator->getMainConnectionID();
                break;
            }
            case NAVI_SOURCE_ID:
            {
                m_mainConnectionID.naviConnectionID = listIterator->getMainConnectionID();
                break;
            }
            case PHONE_SOURCE_ID:
            {
                m_mainConnectionID.phoneConnectionID = listIterator->getMainConnectionID();
                break;
            }
            case TTS_SOURCE_ID:
            {
                m_mainConnectionID.ttsConnectionID = listIterator->getMainConnectionID();
                break;
            }
            case REVERSE_SOURCE_ID:
            {
                m_mainConnectionID.reverseConnectionID = listIterator->getMainConnectionID();
                break;
            }
            default:
                break;
            }
            ++ size;
        }
    }
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::getListMainSinks()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    am_types::am_SinkType_L listSinks;
    cmdControlProxy->getListMainSinks(callStatus, listSinks, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    if ( am_types::am_Error_e::E_OK == amError)
    {
        std::vector<am_types::am_SinkType_s>::const_iterator listIterator = listSinks.begin();
        for (u_int16_t size = 1; listIterator < listSinks.end(); ++listIterator)
        {
            cout << "    SINK : " << size << endl;
            cout << "sinkID : " << listIterator->getSinkID() << endl;
            cout << "name : " << listIterator->getName().c_str() << endl;
            cout << "availability : " <<  getString->getStringAvailability(listIterator->getAvailability().getAvailability()) << endl;
            cout << "availabilityReason : " <<  getString->getStringCustomAvailabilityReason(listIterator->getAvailability().getAvailabilityReason()) << endl;
            cout << "volume : " << listIterator->getVolume() << endl;
            cout << "muteState : " <<  getString->getStringMuteState(listIterator->getMuteState()) << endl;
            cout << "sinkClassID : " << listIterator->getSinkClassID() << endl;
            cout << endl;
            ++ size;
        }
    }
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::getListMainSources()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    am_types::am_SourceType_L listSources;
    cmdControlProxy->getListMainSources(callStatus, listSources, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    if ( am_types::am_Error_e::E_OK == amError)
    {
        std::vector<am_types::am_SourceType_s>::const_iterator listIterator = listSources.begin();
        for (u_int16_t size = 1; listIterator < listSources.end(); ++listIterator)
        {
            cout << "    SOURCE : " << size << endl;
            cout << "sourceID : " << listIterator->getSourceID() << endl;
            cout << "name : " << listIterator->getName().c_str() << endl;
            cout << "availability : " <<  getString->getStringAvailability(listIterator->getAvailability().getAvailability()) << endl;
            cout << "availabilityReason : " <<  getString->getStringCustomAvailabilityReason(listIterator->getAvailability().getAvailabilityReason()) << endl;
            cout << "sourceClassID : " << listIterator->getSourceClassID() << endl;
            cout << endl;
            ++ size;
        }
    }
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::getListMainSinkSoundProperties()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    if ((ALL_SINKS + 1) == getString->selectSink(m_sinkID))
    {
        return RETURN_EXIT_FAILURE;
    }
    am_types::am_MainSoundProperty_L listSinkSoundProperties;
    cmdControlProxy->getListMainSinkSoundProperties(m_sinkID, callStatus, listSinkSoundProperties, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    if ( am_types::am_Error_e::E_OK == amError)
    {
        std::vector<am_types::am_MainSoundProperty_s>::const_iterator listIterator = listSinkSoundProperties.begin();
        for (u_int16_t size = 1; listIterator < listSinkSoundProperties.end(); ++listIterator)
        {
            cout << "    SINK_SOUND_PROPERTIES : " << size << endl;
            cout << "type : " <<  getString->getStringCustomMainSoundPropertyType(listIterator->getType()) << endl;
            cout << "value : " << listIterator->getValue() << endl;
            cout << endl;
            ++ size;
        }
    }
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::getListMainSourceSoundProperties()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    if ((ALL_SOURCES + 1) == getString->selectSource(m_sourceID))
    {
        return RETURN_EXIT_FAILURE;
    }
    am_types::am_MainSoundProperty_L listSourceSoundProperties;
    cmdControlProxy->getListMainSourceSoundProperties(m_sourceID, callStatus, listSourceSoundProperties, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    if ( am_types::am_Error_e::E_OK == amError)
    {
        std::vector<am_types::am_MainSoundProperty_s>::const_iterator listIterator = listSourceSoundProperties.begin();
        for (u_int16_t size = 1; listIterator < listSourceSoundProperties.end(); ++listIterator)
        {
            cout << "    SOURCE_SOUND_PROPERTIES : " << size << endl;
            cout << "type : " <<  getString->getStringCustomMainSoundPropertyType(listIterator->getType()) << endl;
            cout << "value : " << listIterator->getValue() << endl;
            cout << endl;
            ++ size;
        }
    }
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::getListSinkClasses()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    am_types::am_SinkClass_L listSinkClasses;
    cmdControlProxy->getListSinkClasses(callStatus, listSinkClasses, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    if ( am_types::am_Error_e::E_OK == amError)
    {
        std::vector<am_types::am_SinkClass_s>::const_iterator listIterator = listSinkClasses.begin();
        for (u_int16_t size = 1; listIterator < listSinkClasses.end(); ++listIterator)
        {
            cout << "    SINK_CLASS : " << size << endl;
            cout << "sinkClassID : " << listIterator->getSinkClassID() << endl;
            cout << "name : " << listIterator->getName().c_str() << endl;
            std::vector<am_types::am_ClassProperty_s>::const_iterator listInnerIterator = listIterator->getListClassProperties().begin();
            for (; listInnerIterator < listIterator->getListClassProperties().end(); ++listInnerIterator)
            {
                cout << "    CLASS_PROPERTIES     " << endl;
                cout << "classProperty : " <<  getString->getStringCustomClassProperty(listInnerIterator->getClassProperty()) << endl;
                cout << "value : " << listInnerIterator->getValue() << endl;
            }
            cout << endl;
            ++ size;
        }
    }
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::getListSourceClasses()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    am_types::am_SourceClass_L listSourceClasses;
    cmdControlProxy->getListSourceClasses(callStatus, listSourceClasses, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    if ( am_types::am_Error_e::E_OK == amError)
    {
        std::vector<am_types::am_SourceClass_s>::const_iterator listIterator = listSourceClasses.begin();
        for (u_int16_t size = 1; listIterator < listSourceClasses.end(); ++listIterator)
        {
            cout << "    SOURCE_CLASS : " << size << endl;
            cout << "sourceClassID : " << listIterator->getSourceClassID() << endl;
            cout << "name : " << listIterator->getName().c_str() << endl;
            std::vector<am_types::am_ClassProperty_s>::const_iterator listInnerIterator = listIterator->getListClassProperties().begin();
            for (; listInnerIterator < listIterator->getListClassProperties().end(); ++listInnerIterator)
            {
                cout << "     CLASS_PROPERTIES     " << endl;
                cout << "classProperty : " <<  getString->getStringCustomClassProperty(listInnerIterator->getClassProperty()) << endl;
                cout << "value : " << listInnerIterator->getValue() << endl;
            }
            cout << endl;
            ++ size;
        }
    }
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::getListSystemProperties()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    am_types::am_SystemProperty_L listSystemProperties;
    cmdControlProxy->getListSystemProperties(callStatus, listSystemProperties, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    if ( am_types::am_Error_e::E_OK == amError)
    {
        std::vector<am_types::am_SystemProperty_s>::const_iterator listIterator = listSystemProperties.begin();
        for (u_int16_t size = 1; listIterator < listSystemProperties.end(); ++listIterator)
        {
            cout << "     SYSTEM_PROPERTY : " << size << endl;
            cout << "type : " << (listIterator->getType() == 0 ? "SYP_UNKNOWN" : "SYP_ERROR") << endl;
            cout << "value : " << listIterator->getValue() << endl;
            cout << endl;
            ++ size;
        }
    }
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::getTimingInformation()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    am_types::am_timeSync_t delay = 0;
    cmdControlProxy->getTimingInformation(m_lastConnectID, callStatus, delay, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    cout << "delay : " << delay << endl;
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::getListSinkMainNotificationConfigurations()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    if ((ALL_SINKS + 1) == getString->selectSink(m_sinkID))
    {
        return RETURN_EXIT_FAILURE;
    }
    am_types::am_NotificationConfiguration_L listNotificationConfigurations;
    cmdControlProxy->getListMainSinkNotificationConfigurations(m_sinkID, callStatus, listNotificationConfigurations, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    if ( am_types::am_Error_e::E_OK == amError)
    {
        std::vector<am_types::am_NotificationConfiguration_s>::const_iterator listIterator = listNotificationConfigurations.begin();
        for (u_int16_t size = 1; listIterator < listNotificationConfigurations.end(); ++listIterator)
        {
            cout << "     SINK_NOTIFICATION_CONFIGURATION : " << size << endl;
            cout << "type : " << (listIterator->getType() == 0 ? "NT_UNKNOWN" : "NT_ERROR") << endl;
            cout << "status : " <<  getString->getStringNotificationStatus(listIterator->getStatus()) << endl;
            cout << "parameter : " << listIterator->getParameter() << endl;
            cout << endl;
            ++ size;
        }
    }
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::getListSourceMainNotificationConfigurations()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    if ((ALL_SOURCES + 1) == getString->selectSource(m_sourceID))
    {
        return RETURN_EXIT_FAILURE;
    }
    am_types::am_NotificationConfiguration_L listNotificationConfigurations;
    cmdControlProxy->getListMainSourceNotificationConfigurations(m_sourceID, callStatus, listNotificationConfigurations, amError);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "amError = " << getString->getStringReturnCode(amError) << endl;
    if ( am_types::am_Error_e::E_OK == amError)
    {
        std::vector<am_types::am_NotificationConfiguration_s>::const_iterator listIterator = listNotificationConfigurations.begin();
        for (u_int16_t size = 1; listIterator < listNotificationConfigurations.end(); ++listIterator)
        {
            cout << "     SINK_NOTIFICATION_CONFIGURATION : " << size << endl;
            cout << "type : " << (listIterator->getType() == 0 ? "NT_UNKNOWN" : "NT_ERROR") << endl;
            cout << "status : " <<  getString->getStringNotificationStatus(listIterator->getStatus()) << endl;
            cout << "parameter : " << listIterator->getParameter() << endl;
            cout << endl;
            ++ size;
        }
    }
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::getInterfaceVersion()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    CommonAPI::Version version;
    cmdControlProxy->getInterfaceVersionAttribute().getValue(callStatus, version);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    cout << "version : " << version.Major << "." << version.Minor << endl;
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterfaceCAPI::getListOfMainConnections()
{
    cout << "Call to tstCallCommandInterfaceCAPI " << __FUNCTION__ << endl;
    am_types::am_MainConnection_L listMainConnections;
    cmdControlProxy->getListOfMainConnectionsAttribute().getValue(callStatus, listMainConnections);
    cout << "callStatus : " << getString->callStatusToStr(callStatus) << endl;
    std::vector<am_types::am_MainConnectionType_s>::const_iterator listIterator = listMainConnections.begin();
    for (u_int16_t size = 1; listIterator < listMainConnections.end(); ++listIterator)
    {
        cout << "    CONNECTION : " << size << endl;
        cout << "mainConnectionID : " << listIterator->getMainConnectionID() << endl;
        cout << "sourceID : " << listIterator->getSourceID() << endl;
        cout << "sinkID : " << listIterator->getSinkID() << endl;
        cout << "delay : " << listIterator->getDelay() << endl;
        cout << "connectionState : " <<  getString->getStringConnectionState(listIterator->getConnectionState()) << endl;
        cout << endl;
        switch (listIterator->getSourceID())
        {
        case MEDIA_SOURCE_ID:
        {
            m_mainConnectionID.mediaConnectionID = listIterator->getMainConnectionID();
            break;
        }
        case NAVI_SOURCE_ID:
        {
            m_mainConnectionID.naviConnectionID = listIterator->getMainConnectionID();
            break;
        }
        case PHONE_SOURCE_ID:
        {
            m_mainConnectionID.phoneConnectionID = listIterator->getMainConnectionID();
            break;
        }
        case TTS_SOURCE_ID:
        {
            m_mainConnectionID.ttsConnectionID = listIterator->getMainConnectionID();
            break;
        }
        case REVERSE_SOURCE_ID:
        {
            m_mainConnectionID.reverseConnectionID = listIterator->getMainConnectionID();
            break;
        }
        default:
            break;
        }
        ++ size;
    }

    return RETURN_EXIT_SUCCESS;
}

}/*END OF NAMESPACE*/
