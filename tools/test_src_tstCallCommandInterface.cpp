

#include <iostream>
#include <unistd.h>
#include "tstCallCommandInterface.h"
#include "tstGetString.h"
#include "tstDefine.h"

namespace am
{
tstCallCommandInterface::tstCallCommandInterface()
{
    dbus_error_init (&dberr);
    connect  = dbus_bus_get (DBUS_BUS_SESSION, &dberr);
    if (dbus_error_is_set (&dberr))
    {
        cout << "getting session bus failed: " << dberr.message << endl;
        dbus_error_free (&dberr);
    }
    if(connect == NULL)
    {
        cout << "connect bus failed: " << dberr.message  << endl;
        dbus_error_free (&dberr);
    }
    msg = NULL;
    reply = NULL;
    getString = new tstGetString();
    sourceID = 0;
    sinkID = 0;
    mainConnectionID = {0 , 0 , 0, 0, 0};
    lastConnectID = 0;
    returnCode = -1;
    primarySinkToneVolume = 0;
    primarySinkMuteState = 0;
    secondarySinkToneVolume = 0;
    secondarySinkMuteState = 0;
    isInnerCall = false;
}

tstCallCommandInterface::~tstCallCommandInterface()
{
    if(NULL != connect)
    {
        connect = NULL;
    }
    if(NULL != msg)
    {
        msg = NULL;
    }
    if(NULL != reply)
    {
        reply = NULL;
    }
    if(NULL != getString)
    {
        delete getString;
        getString = NULL;
    }
}

int tstCallCommandInterface::connectSource()
{
    returnCode = -1;
    if(false == isInnerCall)
    {
        getListMainConnections();
        isInnerCall = true;
    }
    int selectConnectSources = getString->selectConnectSource();
    switch (selectConnectSources)
    {
    case 1:    //Media
    {
        if(0 != mainConnectionID.mediaConnectionID)
            cout << "already existing media connection!" << endl;
        sourceID = MEDIA_SOURCE_ID;
        sinkID = PRIMARY_SINK_ID;
        break;
    }
    case 2:    //Navi
    {
        if(0 != mainConnectionID.naviConnectionID)
            cout << "already existing navi connection!" << endl;
        sourceID = NAVI_SOURCE_ID;
        sinkID = SECONDARY_SINK_ID;
        break;
    }
    case 3:    //Phone
    {
        if(0 != mainConnectionID.phoneConnectionID)
            cout << "already existing phone connection!" << endl;
        sourceID = PHONE_SOURCE_ID;
        sinkID= PRIMARY_SINK_ID;
        break;
    }
    case 4:    //TTS
    {
        if(0 != mainConnectionID.ttsConnectionID)
            cout << "already existing tts connection!" << endl;
        sourceID = TTS_SOURCE_ID;
        sinkID= SECONDARY_SINK_ID;
        break;
    }
    case 5:    //Reverse
    {
        if(0 != mainConnectionID.reverseConnectionID)
            cout << "already existing reverse connection!" << endl;
        sourceID = REVERSE_SOURCE_ID;
        sinkID= SECONDARY_SINK_ID;
        break;
    }
    default:
    {
        return RETURN_EXIT_FAILURE;
        break;
    }
    }
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "Connect");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_append_args ( msg, DBUS_TYPE_UINT16, &sourceID, DBUS_TYPE_UINT16, &sinkID, DBUS_TYPE_INVALID);
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_get_args (reply, &dberr, DBUS_TYPE_INT16 , &returnCode, DBUS_TYPE_UINT16, &lastConnectID, DBUS_TYPE_INVALID);
    if (dbus_error_is_set (&dberr))
    {
        cout << "get message args failed!" << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    cout << "returnCode = " << getString->getStringReturnCode(returnCode) << endl;
    if(returnCode == am::am_Error_e::E_OK)
    {
        switch (selectConnectSources) {
        case 1:
        {
            mainConnectionID.mediaConnectionID = lastConnectID;
            cout << "mainConnectionID.mediaConnectionID = " << mainConnectionID.mediaConnectionID << endl;
            break;
        }
        case 2:
        {
            mainConnectionID.naviConnectionID = lastConnectID;
            cout << "mainConnectionID.naviConnectionID = " << mainConnectionID.naviConnectionID << endl;
            break;
        }
        case 3:
        {
            mainConnectionID.phoneConnectionID = lastConnectID;
            cout << "mainConnectionID.phoneConnectionID = " << mainConnectionID.phoneConnectionID << endl;
            break;
        }
        case 4:
        {
            mainConnectionID.ttsConnectionID = lastConnectID;
            cout << "mainConnectionID.ttsConnectionID = " << mainConnectionID.ttsConnectionID << endl;
            break;
        }
        case 5:
        {
            mainConnectionID.reverseConnectionID = lastConnectID;
            cout << "mainConnectionID.reverseConnectionID = " << mainConnectionID.reverseConnectionID << endl;
            break;
        }
        default:
            break;
        }
    }
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::disConnectSource()
{
    returnCode = -1;
    if(false == isInnerCall)
    {
        getListMainConnections();
        isInnerCall = true;
    }
    int selectDisconnectSources = getString->selectConnectSource();
    am_connectionID_t connectID = 0;
    switch (selectDisconnectSources) {
    case 1:    //Media
    {
        if(0 == mainConnectionID.mediaConnectionID)
            cout << "non mediaConnectionID" << endl;
        connectID = mainConnectionID.mediaConnectionID;
        break;
    }
    case 2:    //Navi
    {
        if(0 == mainConnectionID.naviConnectionID)
            cout << "non naviConnectionID" << endl;
        connectID = mainConnectionID.naviConnectionID;
        break;
    }
    case 3:    //Phone
    {
        if(0 == mainConnectionID.phoneConnectionID)
            cout << "non phoneConnectionID" << endl;
        connectID = mainConnectionID.phoneConnectionID;
        break;
    }
    case 4:    //TTS
    {
        if(0 == mainConnectionID.ttsConnectionID)
            cout << "non ttsConnectionID" << endl;
        connectID = mainConnectionID.ttsConnectionID;
        break;
    }
    case 5:    //Reverse
    {
        if(0 == mainConnectionID.reverseConnectionID)
            cout << "non reverseConnectionID" << endl;
        connectID = mainConnectionID.reverseConnectionID;
        break;
    }
    default:
    {
        return RETURN_EXIT_FAILURE;
        break;
    }
    }   //end of switch
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "Disconnect");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_append_args ( msg, DBUS_TYPE_UINT16, &connectID, DBUS_TYPE_INVALID);
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_get_args (reply, &dberr, DBUS_TYPE_INT16 , &returnCode, DBUS_TYPE_INVALID);
    if (dbus_error_is_set (&dberr))
    {
        cout << "get message args failed!" << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    cout << "returnCode = " << getString->getStringReturnCode(returnCode) << endl;
    if(returnCode == am::am_Error_e::E_OK)
    {
        switch (selectDisconnectSources) {
        case 1:
        {
            cout << "Media source disconnect!" << endl;
            cout << "MediaConnectionID = " << connectID << endl;
            mainConnectionID.mediaConnectionID = 0;
            break;
        }
        case 2:
        {
            cout << "Navi source disconnect!" << endl;
            cout << "Navi ConnectionID = " << connectID << endl;
            mainConnectionID.naviConnectionID = 0;
            break;
        }
        case 3:
        {
            cout << "Phone source disconnect!" << endl;
            cout << "Phone ConnectionID = " << connectID << endl;
            mainConnectionID.phoneConnectionID = 0;
            break;
        }
        case 4:
        {
            cout << "TTS source disconnect!" << endl;
            cout << "TTS ConnectionID = " << connectID << endl;
            mainConnectionID.ttsConnectionID = 0;
            break;
        }
        case 5:
        {
            cout << "Reverse source disconnect!" << endl;
            cout << "Reverse ConnectionID = " << connectID << endl;
            mainConnectionID.reverseConnectionID = 0;
            break;
        }
        default:
            break;
        }
    }
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::setPrimarySinkVoulme()
{
    returnCode = -1;
    sinkID = PRIMARY_SINK_ID;
    primarySinkToneVolume ++;
    am_volume_t volume = (primarySinkToneVolume % 6) * INIT_VOLUME / 5;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "SetVolume");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_append_args ( msg, DBUS_TYPE_UINT16, &sinkID, DBUS_TYPE_INT16, &volume, DBUS_TYPE_INVALID);
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_get_args (reply, &dberr, DBUS_TYPE_INT16 , &returnCode, DBUS_TYPE_INVALID);
    if (dbus_error_is_set (&dberr))
    {
        cout << "get message args failed!" << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode) << endl;
    cout << "volume : " << volume << endl;
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::primarySinkVolumeStep()
{
    returnCode = -1;
    sinkID = PRIMARY_SINK_ID;
    int16_t volumeStep = VOLUME_STEP;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "VolumeStep");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_append_args ( msg, DBUS_TYPE_UINT16, &sinkID, DBUS_TYPE_INT16, &volumeStep, DBUS_TYPE_INVALID);
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_get_args (reply, &dberr, DBUS_TYPE_INT16 , &returnCode, DBUS_TYPE_INVALID);
    if (dbus_error_is_set (&dberr))
    {
        cout << "get message args failed!" << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode) << endl;
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::setPrimarySinkMuteState()
{
    returnCode = -1;
    sinkID = PRIMARY_SINK_ID;
    primarySinkMuteState ++ ;
    int16_t muteState = (primarySinkMuteState % 2) + 1;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "SetSinkMuteState");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_append_args ( msg, DBUS_TYPE_UINT16, &sinkID, DBUS_TYPE_INT16, &muteState, DBUS_TYPE_INVALID);
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_get_args (reply, &dberr, DBUS_TYPE_INT16 , &returnCode, DBUS_TYPE_INVALID);
    if (dbus_error_is_set (&dberr))
    {
        cout << "get message args failed!" << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode)  << endl;
    cout << "muteState : " <<  getString->getStringMuteState(muteState) << endl;
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::setSecondarySinkVoulme()
{
    returnCode = -1;
    sinkID = SECONDARY_SINK_ID;
    secondarySinkToneVolume ++;
    am_volume_t volume = (secondarySinkToneVolume % 6) * INIT_VOLUME / 5;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "SetVolume");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_append_args ( msg, DBUS_TYPE_UINT16, &sinkID, DBUS_TYPE_INT16, &volume, DBUS_TYPE_INVALID);
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_get_args (reply, &dberr, DBUS_TYPE_INT16 , &returnCode, DBUS_TYPE_INVALID);
    if (dbus_error_is_set (&dberr))
    {
        cout << "get message args failed!" << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode) << endl;
    cout << "volume : " << volume << endl;
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::secondarySinkVolumeStep()
{
    returnCode = -1;
    sinkID = SECONDARY_SINK_ID;
    int16_t volumeStep = VOLUME_STEP;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "VolumeStep");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_append_args ( msg, DBUS_TYPE_UINT16, &sinkID, DBUS_TYPE_INT16, &volumeStep, DBUS_TYPE_INVALID);
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_get_args (reply, &dberr, DBUS_TYPE_INT16 , &returnCode, DBUS_TYPE_INVALID);
    if (dbus_error_is_set (&dberr))
    {
        cout << "get message args failed!" << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode) << endl;
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::setSecondarySinkMuteState()
{
    returnCode = -1;
    sinkID = SECONDARY_SINK_ID;
    secondarySinkMuteState ++ ;
    int16_t muteState = (secondarySinkMuteState % 2) + 1;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "SetSinkMuteState");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_append_args ( msg, DBUS_TYPE_UINT16, &sinkID, DBUS_TYPE_INT16, &muteState, DBUS_TYPE_INVALID);
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_get_args (reply, &dberr, DBUS_TYPE_INT16 , &returnCode, DBUS_TYPE_INVALID);
    if (dbus_error_is_set (&dberr))
    {
        cout << "get message args failed!" << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode)  << endl;
    cout << "muteState : " <<  getString->getStringMuteState(muteState) << endl;
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::setMainSinkSoundProperty()
{
    returnCode = -1;
    if((ALL_SINKS + 1)==  getString->selectSink(sinkID))
        return RETURN_EXIT_FAILURE;
    DBusMessageIter iter;
    DBusMessageIter subIter;
    am_MainSoundProperty_s mainSoundProperty;
    mainSoundProperty.type = MSP_GENIVI_MID;
    mainSoundProperty.value = 5;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "SetMainSinkSoundProperty");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_append_args ( msg, DBUS_TYPE_UINT16, &sinkID, DBUS_TYPE_INVALID);
    dbus_message_iter_init_append(msg, &iter);
    dbus_message_iter_open_container(&iter, DBUS_TYPE_STRUCT, NULL, &subIter);
    dbus_message_iter_append_basic(&subIter, DBUS_TYPE_INT16, &mainSoundProperty.type);
    dbus_message_iter_append_basic(&subIter, DBUS_TYPE_INT16, &mainSoundProperty.value);
    dbus_message_iter_close_container(&iter, &subIter);
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_get_args (reply, &dberr, DBUS_TYPE_INT16 , &returnCode, DBUS_TYPE_INVALID);
    if (dbus_error_is_set (&dberr))
    {
        cout << "get message args failed!" << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode) << endl;
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::setMainSourceSoundProperty()
{
    returnCode = -1;
    if((ALL_SOURCES + 1)== getString->selectSource(sourceID))
    {
        return RETURN_EXIT_FAILURE;
    }
    DBusMessageIter iter;
    DBusMessageIter subIter;
    am_MainSoundProperty_s mainSoundProperty;
    mainSoundProperty.type = MSP_GENIVI_TREBLE;
    mainSoundProperty.value = 5;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "SetMainSourceSoundProperty");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_append_args ( msg, DBUS_TYPE_UINT16, &sourceID, DBUS_TYPE_INVALID);
    dbus_message_iter_init_append(msg, &iter);
    dbus_message_iter_open_container(&iter, DBUS_TYPE_STRUCT, NULL, &subIter);
    dbus_message_iter_append_basic(&subIter, DBUS_TYPE_INT16, &mainSoundProperty.type);
    dbus_message_iter_append_basic(&subIter, DBUS_TYPE_INT16, &mainSoundProperty.value);
    dbus_message_iter_close_container(&iter, &subIter);
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_get_args (reply, &dberr, DBUS_TYPE_INT16 , &returnCode, DBUS_TYPE_INVALID);
    if (dbus_error_is_set (&dberr))
    {
        cout << "get message args failed!" << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode) << endl;
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::setSystemProperty()
{
    returnCode = -1;
    DBusMessageIter iter;
    DBusMessageIter subIter;
    am_SystemProperty_s systemProperty;
    systemProperty.type = SYP_UNKNOWN;
    systemProperty.value = 5;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "SetSystemProperty");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_append_args ( msg, DBUS_TYPE_INVALID);
    dbus_message_iter_init_append(msg, &iter);
    dbus_message_iter_open_container(&iter, DBUS_TYPE_STRUCT, NULL, &subIter);
    dbus_message_iter_append_basic(&subIter, DBUS_TYPE_INT16, &systemProperty.type);
    dbus_message_iter_append_basic(&subIter, DBUS_TYPE_INT16, &systemProperty.value);
    dbus_message_iter_close_container(&iter, &subIter);
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_get_args (reply, &dberr, DBUS_TYPE_INT16 , &returnCode, DBUS_TYPE_INVALID);
    if (dbus_error_is_set (&dberr))
    {
        cout << "get message args failed!" << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode) << endl;
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::setSinkMainNotificationConfiguration()
{
    returnCode = -1;
    if((ALL_SINKS + 1)== getString->selectSink(sinkID))
    {
        return RETURN_EXIT_FAILURE;
    }
    DBusMessageIter iter;
    DBusMessageIter subIter;
    am_NotificationConfiguration_s mainNotificationConfiguration;
    mainNotificationConfiguration.type = NT_UNKNOWN;
    mainNotificationConfiguration.status = am::am_NotificationStatus_e::NS_OFF;
    mainNotificationConfiguration.parameter = 5;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "setSinkMainNotificationConfiguration");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_append_args ( msg, DBUS_TYPE_UINT16, &sinkID, DBUS_TYPE_INVALID);
    dbus_message_iter_init_append(msg, &iter);
    dbus_message_iter_open_container(&iter, DBUS_TYPE_STRUCT, NULL, &subIter);
    dbus_message_iter_append_basic(&subIter, DBUS_TYPE_INT16, &mainNotificationConfiguration.type);
    dbus_message_iter_append_basic(&subIter, DBUS_TYPE_INT16, &mainNotificationConfiguration.status);
    dbus_message_iter_append_basic(&subIter, DBUS_TYPE_INT16, &mainNotificationConfiguration.parameter);
    dbus_message_iter_close_container(&iter, &subIter);
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_get_args (reply, &dberr, DBUS_TYPE_INT16 , &returnCode, DBUS_TYPE_INVALID);
    if (dbus_error_is_set (&dberr))
    {
        cout << "get message args failed!" << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode) << endl;
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::setSourceMainNotificationConfiguration()
{
    returnCode = -1;
    if((ALL_SOURCES + 1)== getString->selectSource(sourceID))
    {
        return RETURN_EXIT_FAILURE;
    }
    DBusMessageIter iter;
    DBusMessageIter subIter;
    am_NotificationConfiguration_s mainNotificationConfiguration;
    mainNotificationConfiguration.type = NT_UNKNOWN;
    mainNotificationConfiguration.status = am::am_NotificationStatus_e::NS_PERIODIC;
    mainNotificationConfiguration.parameter = 20;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "setSourceMainNotificationConfiguration");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_append_args ( msg, DBUS_TYPE_UINT16, &sourceID, DBUS_TYPE_INVALID);
    dbus_message_iter_init_append(msg, &iter);
    dbus_message_iter_open_container(&iter, DBUS_TYPE_STRUCT, NULL, &subIter);
    dbus_message_iter_append_basic(&subIter, DBUS_TYPE_INT16, &mainNotificationConfiguration.type);
    dbus_message_iter_append_basic(&subIter, DBUS_TYPE_INT16, &mainNotificationConfiguration.status);
    dbus_message_iter_append_basic(&subIter, DBUS_TYPE_INT16, &mainNotificationConfiguration.parameter);
    dbus_message_iter_close_container(&iter, &subIter);
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_get_args (reply, &dberr, DBUS_TYPE_INT16 , &returnCode, DBUS_TYPE_INVALID);
    if (dbus_error_is_set (&dberr))
    {
        cout << "get message args failed!" << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode) << endl;
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::getListMainConnections()
{
    returnCode = -1;
    std::vector<am_MainConnectionType_s> listConnections;
    DBusMessageIter rootIter;
    DBusMessageIter arrayIter;
    DBusMessageIter structIter;
    am_MainConnectionType_s mainConnectionType;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "GetListMainConnections");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_iter_init(reply, &rootIter);
    dbus_message_iter_get_basic(&rootIter, &returnCode);
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode) << endl;
    dbus_message_iter_next(&rootIter);
    if(am_Error_e::E_OK == returnCode)
    {
        dbus_message_iter_recurse(&rootIter, &arrayIter);
        while(dbus_message_iter_get_arg_type(&arrayIter))
        {
            dbus_message_iter_recurse(&arrayIter, &structIter);
            dbus_message_iter_get_basic(&structIter, &mainConnectionType.mainConnectionID);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_get_basic(&structIter, &mainConnectionType.sourceID);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_get_basic(&structIter, &mainConnectionType.sinkID);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_get_basic(&structIter, &mainConnectionType.delay);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_get_basic(&structIter, &mainConnectionType.connectionState);
            dbus_message_iter_next(&arrayIter);
            listConnections.push_back(mainConnectionType);
        }
        std::vector<am::am_MainConnectionType_s>::const_iterator listIterator = listConnections.begin();
        for(u_int16_t size = 1; listIterator < listConnections.end(); ++listIterator)
        {
            cout << "    CONNECTION : " << size << endl;
            cout << "mainConnectionID : " << listIterator->mainConnectionID << endl;
            cout << "sourceID : " << listIterator->sourceID << endl;
            cout << "sinkID : " << listIterator->sinkID << endl;
            cout << "delay : " << listIterator->delay << endl;
            cout << "connectionState : " <<  getString->getStringConnectionState(listIterator->connectionState) << endl;
            cout << endl;
            switch (listIterator->sourceID)
            {
            case MEDIA_SOURCE_ID:
            {
                mainConnectionID.mediaConnectionID = listIterator->mainConnectionID;
                break;
            }
            case NAVI_SOURCE_ID:
            {
                mainConnectionID.naviConnectionID = listIterator->mainConnectionID;
                break;
            }
            case PHONE_SOURCE_ID:
            {
                mainConnectionID.phoneConnectionID = listIterator->mainConnectionID;
                break;
            }
            case TTS_SOURCE_ID:
            {
                mainConnectionID.ttsConnectionID = listIterator->mainConnectionID;
                break;
            }
            case REVERSE_SOURCE_ID:
            {
                mainConnectionID.reverseConnectionID = listIterator->mainConnectionID;
                break;
            }
            default:
                break;
            }
            ++ size;
        }
    }
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::getListMainSinks()
{
    returnCode = -1;
    std::vector<am_SinkType_s> listSinks;
    DBusMessageIter rootIter;
    DBusMessageIter arrayIter;
    DBusMessageIter structIter;
    DBusMessageIter availIter;
    am_SinkType_s sinkType;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "GetListMainSinks");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_iter_init(reply, &rootIter);
    dbus_message_iter_get_basic(&rootIter, &returnCode);
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode) << endl;
    dbus_message_iter_next(&rootIter);
    if(am_Error_e::E_OK == returnCode)
    {
        dbus_message_iter_recurse(&rootIter, &arrayIter);
        while(dbus_message_iter_get_arg_type(&arrayIter))
        {
            dbus_message_iter_recurse(&arrayIter, &structIter);
            dbus_message_iter_get_basic(&structIter, &sinkType.sinkID);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_get_basic(&structIter, &sinkType.name);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_recurse(&structIter, &availIter);
            dbus_message_iter_get_basic(&availIter, &sinkType.availability.availability);
            dbus_message_iter_next(&availIter);
            dbus_message_iter_get_basic(&availIter, &sinkType.availability.availabilityReason);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_get_basic(&structIter, &sinkType.volume);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_get_basic(&structIter, &sinkType.muteState);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_get_basic(&structIter, &sinkType.sinkClassID);
            dbus_message_iter_next(&arrayIter);
            listSinks.push_back(sinkType);
        }
        std::vector<am::am_SinkType_s>::const_iterator listIterator = listSinks.begin();
        for(u_int16_t size = 1; listIterator < listSinks.end(); ++listIterator)
        {
            cout << "    SINK : " << size << endl;
            cout << "sinkID : " << listIterator->sinkID << endl;
            cout << "name : " << listIterator->name.c_str() << endl;
            cout << "availability : " <<  getString->getStringAvailability(listIterator->availability.availability) << endl;
            cout << "availabilityReason : " <<  getString->getStringCustomAvailabilityReason(listIterator->availability.availabilityReason) << endl;
            cout << "volume : " << listIterator->volume << endl;
            cout << "muteState : " <<  getString->getStringMuteState(listIterator->muteState) << endl;
            cout << "sinkClassID : " << listIterator->sinkClassID << endl;
            cout << endl;
            ++ size;
        }
    }
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::getListMainSources()
{
    returnCode = -1;
    std::vector<am_SourceType_s> listSources;
    DBusMessageIter rootIter;
    DBusMessageIter arrayIter;
    DBusMessageIter structIter;
    DBusMessageIter availIter;
    am_SourceType_s sourceType;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "GetListMainSources");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_iter_init(reply, &rootIter);
    dbus_message_iter_get_basic(&rootIter, &returnCode);
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode) << endl;
    dbus_message_iter_next(&rootIter);
    if(am_Error_e::E_OK == returnCode)
    {
        dbus_message_iter_recurse(&rootIter, &arrayIter);
        while(dbus_message_iter_get_arg_type(&arrayIter))
        {
            dbus_message_iter_recurse(&arrayIter, &structIter);
            dbus_message_iter_get_basic(&structIter, &sourceType.sourceID);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_get_basic(&structIter, &sourceType.name);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_recurse(&structIter, &availIter);
            dbus_message_iter_get_basic(&availIter, &sourceType.availability.availability);
            dbus_message_iter_next(&availIter);
            dbus_message_iter_get_basic(&availIter, &sourceType.availability.availabilityReason);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_get_basic(&structIter, &sourceType.sourceClassID);
            dbus_message_iter_next(&arrayIter);
            listSources.push_back(sourceType);
        }
        std::vector<am::am_SourceType_s>::const_iterator listIterator = listSources.begin();
        for(u_int16_t size = 1; listIterator < listSources.end(); ++listIterator)
        {
            cout << "    SOURCE : " << size << endl;
            cout << "sourceID : " << listIterator->sourceID << endl;
            cout << "name : " << listIterator->name.c_str() << endl;
            cout << "availability : " <<  getString->getStringAvailability(listIterator->availability.availability) << endl;
            cout << "availabilityReason : " <<  getString->getStringCustomAvailabilityReason(listIterator->availability.availabilityReason) << endl;
            cout << "sourceClassID : " << listIterator->sourceClassID << endl;
            cout << endl;
            ++ size;
        }
    }
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::getListMainSinkSoundProperties()
{
    returnCode = -1;
    if((ALL_SINKS + 1)== getString->selectSink(sinkID))
        return RETURN_EXIT_FAILURE;
    std::vector<am_MainSoundProperty_s> listSinkSoundProperties;
    DBusMessageIter rootIter;
    DBusMessageIter arrayIter;
    DBusMessageIter structIter;
    am_MainSoundProperty_s mainSoundProperty;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "GetListMainSinkSoundProperties");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_append_args ( msg, DBUS_TYPE_UINT16, &sinkID, DBUS_TYPE_INVALID);
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_iter_init(reply, &rootIter);
    dbus_message_iter_get_basic(&rootIter, &returnCode);
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode) << endl;
    dbus_message_iter_next(&rootIter);
    if(am_Error_e::E_OK == returnCode)
    {
        dbus_message_iter_recurse(&rootIter, &arrayIter);
        while(dbus_message_iter_get_arg_type(&arrayIter))
        {
            dbus_message_iter_recurse(&arrayIter, &structIter);
            dbus_message_iter_get_basic(&structIter, &mainSoundProperty.type);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_get_basic(&structIter, &mainSoundProperty.value);
            dbus_message_iter_next(&arrayIter);
            listSinkSoundProperties.push_back(mainSoundProperty);
        }
        std::vector<am::am_MainSoundProperty_s>::const_iterator listIterator = listSinkSoundProperties.begin();
        for(u_int16_t size = 1; listIterator < listSinkSoundProperties.end(); ++listIterator)
        {
            cout << "    SINK_SOUND_PROPERTIES : " << size << endl;
            cout << "type : " <<  getString->getStringCustomMainSoundPropertyType(listIterator->type) << endl;
            cout << "value : " << listIterator->value << endl;
            cout << endl;
            ++ size;
        }
    }
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::getListMainSourceSoundProperties()
{
    returnCode = -1;
    if((ALL_SOURCES + 1)== getString->selectSource(sourceID))
        return RETURN_EXIT_FAILURE;
    std::vector<am_MainSoundProperty_s> listSourceSoundProperties;
    DBusMessageIter rootIter;
    DBusMessageIter arrayIter;
    DBusMessageIter structIter;
    am_MainSoundProperty_s mainSoundProperty;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "GetListMainSourceSoundProperties");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_append_args ( msg, DBUS_TYPE_UINT16, &sourceID, DBUS_TYPE_INVALID);
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_iter_init(reply, &rootIter);
    dbus_message_iter_get_basic(&rootIter, &returnCode);
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode) << endl;
    dbus_message_iter_next(&rootIter);
    if(am_Error_e::E_OK == returnCode)
    {
        dbus_message_iter_recurse(&rootIter, &arrayIter);
        while(dbus_message_iter_get_arg_type(&arrayIter))
        {
            dbus_message_iter_recurse(&arrayIter, &structIter);
            dbus_message_iter_get_basic(&structIter, &mainSoundProperty.type);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_get_basic(&structIter, &mainSoundProperty.value);
            dbus_message_iter_next(&arrayIter);
            listSourceSoundProperties.push_back(mainSoundProperty);
        }
        std::vector<am::am_MainSoundProperty_s>::const_iterator listIterator = listSourceSoundProperties.begin();
        for(u_int16_t size = 1; listIterator < listSourceSoundProperties.end(); ++listIterator)
        {
            cout << "    SOURCE_SOUND_PROPERTIES : " << size << endl;
            cout << "type : " <<  getString->getStringCustomMainSoundPropertyType(listIterator->type) << endl;
            cout << "value : " << listIterator->value << endl;
            cout << endl;
            ++ size;
        }
    }
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::getListSinkClasses()
{
    returnCode = -1;
    std::vector<am::am_SinkClass_s> listSinkClasses;
    DBusMessageIter rootIter;
    DBusMessageIter arrayIter;
    DBusMessageIter structIter;
    DBusMessageIter propIter;
    DBusMessageIter innerIter;
    am_SinkClass_s sinkClass;
    std::vector<am::am_ClassProperty_s> listCP;
    am_ClassProperty_s classProperty;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "GetListSinkClasses");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_iter_init(reply, &rootIter);
    dbus_message_iter_get_basic(&rootIter, &returnCode);
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode) << endl;
    dbus_message_iter_next(&rootIter);
    if(am_Error_e::E_OK == returnCode)
    {
        dbus_message_iter_recurse(&rootIter, &arrayIter);
        while(dbus_message_iter_get_arg_type(&arrayIter))
        {
            dbus_message_iter_recurse(&arrayIter, &structIter);
            dbus_message_iter_get_basic(&structIter, &sinkClass.sinkClassID);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_get_basic(&structIter, &sinkClass.name);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_recurse(&structIter, &innerIter);
            while(dbus_message_iter_get_arg_type(&innerIter))
            {
                dbus_message_iter_recurse(&innerIter, &propIter);
                dbus_message_iter_get_basic(&propIter, &classProperty.classProperty);
                dbus_message_iter_next(&propIter);
                dbus_message_iter_get_basic(&propIter, &classProperty.value);
                listCP.push_back(classProperty);
                dbus_message_iter_next(&innerIter);
            }
            dbus_message_iter_next(&arrayIter);
            sinkClass.listClassProperties = listCP;
            listSinkClasses.push_back(sinkClass);
        }
        std::vector<am::am_SinkClass_s>::const_iterator listIterator = listSinkClasses.begin();
        for(u_int16_t size = 1; listIterator < listSinkClasses.end(); ++listIterator)
        {
            cout << "    SINK_CLASS : " << size << endl;
            cout << "sinkClassID : " << listIterator->sinkClassID << endl;
            cout << "name : " << listIterator->name.c_str() << endl;
            std::vector<am::am_ClassProperty_s>::const_iterator listInnerIterator = listIterator->listClassProperties.begin();
            for (; listInnerIterator < listIterator->listClassProperties.end(); ++listInnerIterator)
            {
                cout << "    CLASS_PROPERTIES     " << endl;
                cout << "classProperty : " <<  getString->getStringCustomClassProperty(listInnerIterator->classProperty) << endl;
                cout << "value : " << listInnerIterator->value << endl;
            }
            cout << endl;
            ++ size;
        }
    }
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::getListSourceClasses()
{
    returnCode = -1;
    std::vector<am_SourceClass_s> listSourceClasses;
    DBusMessageIter rootIter;
    DBusMessageIter arrayIter;
    DBusMessageIter structIter;
    DBusMessageIter propIter;
    DBusMessageIter innerIter;
    am_SourceClass_s sourceClass;
    std::vector<am::am_ClassProperty_s> listCP;
    am_ClassProperty_s classProperty;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "GetListSourceClasses");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_iter_init(reply, &rootIter);
    dbus_message_iter_get_basic(&rootIter, &returnCode);
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode) << endl;
    dbus_message_iter_next(&rootIter);
    if(am_Error_e::E_OK == returnCode)
    {
        dbus_message_iter_recurse(&rootIter, &arrayIter);
        while(dbus_message_iter_get_arg_type(&arrayIter))
        {
            dbus_message_iter_recurse(&arrayIter, &structIter);
            dbus_message_iter_get_basic(&structIter, &sourceClass.sourceClassID);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_get_basic(&structIter, &sourceClass.name);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_recurse(&structIter, &innerIter);
            while(dbus_message_iter_get_arg_type(&innerIter))
            {
                dbus_message_iter_recurse(&innerIter, &propIter);
                dbus_message_iter_get_basic(&propIter, &classProperty.classProperty);
                dbus_message_iter_next(&propIter);
                dbus_message_iter_get_basic(&propIter, &classProperty.value);
                listCP.push_back(classProperty);
                dbus_message_iter_next(&innerIter);
            }
            dbus_message_iter_next(&arrayIter);
            sourceClass.listClassProperties = listCP;
            listSourceClasses.push_back(sourceClass);
        }
        std::vector<am::am_SourceClass_s>::const_iterator listIterator = listSourceClasses.begin();
        for(u_int16_t size = 1; listIterator < listSourceClasses.end(); ++listIterator)
        {
            cout << "    SOURCE_CLASS : " << size << endl;
            cout << "sourceClassID : " << listIterator->sourceClassID << endl;
            cout << "name : " << listIterator->name.c_str() << endl;
            std::vector<am::am_ClassProperty_s>::const_iterator listInnerIterator = listIterator->listClassProperties.begin();
            for (; listInnerIterator < listIterator->listClassProperties.end(); ++listInnerIterator)
            {
                cout << "     CLASS_PROPERTIES     " << endl;
                cout << "classProperty : " <<  getString->getStringCustomClassProperty(listInnerIterator->classProperty) << endl;
                cout << "value : " << listInnerIterator->value << endl;
            }
            cout << endl;
            ++ size;
        }
    }
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::getListSystemProperties()
{
    returnCode = -1;
    std::vector<am_SystemProperty_s> listSystemProperties;
    DBusMessageIter rootIter;
    DBusMessageIter arrayIter;
    DBusMessageIter structIter;
    am_SystemProperty_s systemProperty;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "GetListSystemProperties");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_iter_init(reply, &rootIter);
    dbus_message_iter_get_basic(&rootIter, &returnCode);
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode) << endl;
    dbus_message_iter_next(&rootIter);
    if(am_Error_e::E_OK == returnCode)
    {
        dbus_message_iter_recurse(&rootIter, &arrayIter);
        while(dbus_message_iter_get_arg_type(&arrayIter))
        {
            dbus_message_iter_recurse(&arrayIter, &structIter);
            dbus_message_iter_get_basic(&structIter, &systemProperty.type);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_get_basic(&structIter, &systemProperty.value);
            dbus_message_iter_next(&arrayIter);
            listSystemProperties.push_back(systemProperty);
        }
        std::vector<am::am_SystemProperty_s>::const_iterator listIterator = listSystemProperties.begin();
        for(u_int16_t size = 1; listIterator < listSystemProperties.end(); ++listIterator)
        {
            cout << "     SYSTEM_PROPERTY : " << size << endl;
            cout << "type : " << (listIterator->type == 0 ? "SYP_UNKNOWN" : "SYP_ERROR") << endl;
            cout << "value : " << listIterator->value << endl;
            cout << endl;
            ++ size;
        }
    }
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::getTimingInformation()
{
    returnCode = -1;
    am_timeSync_t delay = 0;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "GetTimingInformation");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_append_args ( msg, DBUS_TYPE_UINT16, &lastConnectID, DBUS_TYPE_INVALID);
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_get_args (reply, &dberr, DBUS_TYPE_INT16 , &returnCode, DBUS_TYPE_INT16 , &delay, DBUS_TYPE_INVALID);
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode) << endl;
    cout << "delay : " << delay << endl;
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::getListSinkMainNotificationConfigurations()
{
    returnCode = -1;
    if((ALL_SINKS + 1)== getString->selectSink(sinkID))
        return RETURN_EXIT_FAILURE;
    std::vector<am_NotificationConfiguration_s> listNotificationConfigurations;
    DBusMessageIter rootIter;
    DBusMessageIter arrayIter;
    DBusMessageIter structIter;
    am_NotificationConfiguration_s notificationConfiguration;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "getListSinkMainNotificationConfigurations");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_append_args ( msg, DBUS_TYPE_UINT16, &sinkID, DBUS_TYPE_INVALID);
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_iter_init(reply, &rootIter);
    dbus_message_iter_get_basic(&rootIter, &returnCode);
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode) << endl;
    dbus_message_iter_next(&rootIter);
    if(am_Error_e::E_OK == returnCode)
    {
        dbus_message_iter_recurse(&rootIter, &arrayIter);
        while(dbus_message_iter_get_arg_type(&arrayIter))
        {
            dbus_message_iter_recurse(&arrayIter, &structIter);
            dbus_message_iter_get_basic(&structIter, &notificationConfiguration.type);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_get_basic(&structIter, &notificationConfiguration.status);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_get_basic(&structIter, &notificationConfiguration.parameter);
            dbus_message_iter_next(&arrayIter);
            listNotificationConfigurations.push_back(notificationConfiguration);
        }
        std::vector<am::am_NotificationConfiguration_s>::const_iterator listIterator = listNotificationConfigurations.begin();
        for(u_int16_t size = 1; listIterator < listNotificationConfigurations.end(); ++listIterator)
        {
            cout << "     SINK_NOTIFICATION_CONFIGURATION : " << size << endl;
            cout << "type : " << (listIterator->type == 0 ? "NT_UNKNOWN" : "NT_ERROR") << endl;
            cout << "status : " <<  getString->getStringNotificationStatus(listIterator->status) << endl;
            cout << "parameter : " << listIterator->parameter << endl;
            cout << endl;
            ++ size;
        }
    }
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}

int tstCallCommandInterface::getListSourceMainNotificationConfigurations()
{
    returnCode = -1;
    if((ALL_SOURCES + 1)== getString->selectSource(sourceID))
        return RETURN_EXIT_FAILURE;
    std::vector<am_NotificationConfiguration_s> listNotificationConfigurations;
    DBusMessageIter rootIter;
    DBusMessageIter arrayIter;
    DBusMessageIter structIter;
    am_NotificationConfiguration_s notificationConfiguration;
    msg = dbus_message_new_method_call ("org.genivi.audiomanager", "/org/genivi/audiomanager/commandinterface",
                                        "org.genivi.audiomanager.commandinterface", "getListSourceMainNotificationConfigurations");
    if(msg == NULL)
    {
        cout << "Message NULL!" << endl;
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_append_args ( msg, DBUS_TYPE_UINT16, &sourceID, DBUS_TYPE_INVALID);
    reply = dbus_connection_send_with_reply_and_block (connect, msg, -1, &dberr);
    if (!reply)
    {
        cout << "couldn't send message: " << dberr.message << endl;
        dbus_error_free (&dberr);
        return RETURN_EXIT_FAILURE;
    }
    dbus_message_iter_init(reply, &rootIter);
    dbus_message_iter_get_basic(&rootIter, &returnCode);
    cout << "returnCode = " <<  getString->getStringReturnCode(returnCode) << endl;
    dbus_message_iter_next(&rootIter);
    if(am_Error_e::E_OK == returnCode)
    {
        dbus_message_iter_recurse(&rootIter, &arrayIter);
        while(dbus_message_iter_get_arg_type(&arrayIter))
        {
            dbus_message_iter_recurse(&arrayIter, &structIter);
            dbus_message_iter_get_basic(&structIter, &notificationConfiguration.type);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_get_basic(&structIter, &notificationConfiguration.status);
            dbus_message_iter_next(&structIter);
            dbus_message_iter_get_basic(&structIter, &notificationConfiguration.parameter);
            dbus_message_iter_next(&arrayIter);
            listNotificationConfigurations.push_back(notificationConfiguration);
        }
        std::vector<am::am_NotificationConfiguration_s>::const_iterator listIterator = listNotificationConfigurations.begin();
        for(u_int16_t size = 1; listIterator < listNotificationConfigurations.end(); ++listIterator)
        {
            cout << "     SOURCE_NOTIFICATION_CONFIGURATION : " << size << endl;
            cout << "type : " << (listIterator->type == 0 ? "NT_UNKNOWN" : "NT_ERROR") << endl;
            cout << "status : " <<  getString->getStringNotificationStatus(listIterator->status) << endl;
            cout << "parameter : " << listIterator->parameter << endl;
            cout << endl;
            ++ size;
        }
    }
    dbus_message_unref (msg);
    dbus_message_unref (reply);
    return RETURN_EXIT_SUCCESS;
}
}
/* EOF */

