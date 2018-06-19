
/**
   * @file tstCallCommandInterface.h
   * @brief Declaration file of class tstCallCommandInterface.
   *
   */

#ifndef tstCallCommandInterface_H
#define tstCallCommandInterface_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <dbus/dbus.h>
#include "audiomanagertypes.h"

namespace am
{
class tstGetString;

struct sourceConnectID_s
{
public:
    am_connectionID_t mediaConnectionID;
    am_connectionID_t naviConnectionID;
    am_connectionID_t phoneConnectionID;
    am_connectionID_t ttsConnectionID;
    am_connectionID_t reverseConnectionID;
};

class tstCallCommandInterface
{
public:
    tstCallCommandInterface();
    ~tstCallCommandInterface();

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

private:
    DBusConnection *connect;
    DBusError dberr;
    DBusMessage *msg;
    DBusMessage *reply;
    am_sourceID_t  sourceID;
    am_sinkID_t  sinkID;
    sourceConnectID_s mainConnectionID;
    am_connectionID_t lastConnectID;
    tstGetString* getString;
    int16_t returnCode;
    int16_t primarySinkToneVolume;
    int16_t primarySinkMuteState;
    int16_t secondarySinkToneVolume;
    int16_t secondarySinkMuteState;
    bool isInnerCall;
};
}

#endif // tstCallCommandInterface_H
/* EOF */
