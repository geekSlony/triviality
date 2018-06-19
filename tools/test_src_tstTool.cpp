
#include <iostream>
#include <iomanip>
#include "ncapp/NCApplicationProcess.h"
#include "nceventsys/NCRunnableThread.h"
#include "NMRunnableLooper.h"
#include "NMRunnableTimer.h"
#include "NMTask.h"
#include "PulsePlayerDef.h"
#include "PusePlayerCallback.h"
#include "PulsePlayerAPI.h"
//#include "tstCallCommandInterface.h"
#include "tstCallCommandInterfaceCAPI.h"
#include "tstCallRoutingInterfaceCAPI.h"
#include "tstCallMMInterface.h"
#include "tstCallIVIRadio.h"
//#include "tstCommandReceiver.h"
//#include "tstCallIVIRadio.h"
#include "tstDefine.h"

using namespace nutshell;
using namespace am;

/**
 * NCTestProcess
 *
 * define the class of RSE Test  Process
 */
class NCTestProcess : public NCApplicationProcess
{
public:
    NCTestProcess() : NCApplicationProcess("tst_Tool") {}
    virtual ~NCTestProcess() {}
    VOID OnInitialize() {}
    VOID OnStart() {}
    VOID OnStop() {}
    VOID OnDeinitialize() {}
};

#define FTESTRSE_COMMAND_TIPS_MAX_LEN 255           ///< the max length of command
typedef void (*SFT_callback)();                                             ///< the callback function pointer
media::PusePlayerCallback* cbMedia = new media::PusePlayerCallback();
media::PulsePlayerAPI* apiMedia = new media::PulsePlayerAPI(cbMedia, media::PULSE_AUDIO_PLAYER_TYPE_MEDIA);

media::PusePlayerCallback* cbNavi = new media::PusePlayerCallback();
media::PulsePlayerAPI* apiNavi = new media::PulsePlayerAPI(cbNavi, media::PULSE_AUDIO_PLAYER_TYPE_NAVI);

media::PusePlayerCallback* cbTts = new media::PusePlayerCallback();
media::PulsePlayerAPI* apiTts = new media::PulsePlayerAPI(cbTts, media::PULSE_AUDIO_PLAYER_TYPE_TTS);

media::PusePlayerCallback* cbTel = new media::PusePlayerCallback();
media::PulsePlayerAPI* apiTel = new media::PulsePlayerAPI(cbTel, media::PULSE_AUDIO_PLAYER_TYPE_TEL);

media::PusePlayerCallback* cbBeep = new media::PusePlayerCallback();
media::PulsePlayerAPI* apiBeep = new media::PulsePlayerAPI(cbBeep, media::PULSE_AUDIO_PLAYER_TYPE_BEEP);

//tstCallCommandInterface* callCommandInterface = new tstCallCommandInterface();
tstCallCommandInterfaceCAPI* callCommandInterfaceCAPI = new tstCallCommandInterfaceCAPI();
tstCallMMInterface* callMMInterface = new tstCallMMInterface();
tstCallRoutingInterfaceCAPI* callRoutingInterfaceCAPI = new tstCallRoutingInterfaceCAPI();
tstCallIVIRadio* callIVIRaido = new tstCallIVIRadio();

/*
TCLAP::SwitchArg dltEnable ("e", "dltEnable", "Enables or disables dlt logging. Default = enabled", true);
TCLAP::ValueArg<std::string> dltLogFilename("F", "dltLogFilename", "the name of the logfile, absolute path. Only if logging is et to file", false, " ", "string");
TCLAP::ValueArg<unsigned int> dltOutput ("O", "dltOutput", "defines where logs are written. 0=dlt-daemon(default), 1=command line, 2=file ", false, 0, "int");
TCLAP::SwitchArg currentSettings("i", "currentSettings", "print current settings and exit", false);
TCLAP::SwitchArg daemonizeAM("d", "daemonize", "daemonize Audiomanager. Better use systemd...", false);
*/

/*---------------------------------------------------------------------------*/
// structure of  CmdTips
//
/*---------------------------------------------------------------------------*/
struct CmdTips
{
    //char id[4];     ///< the id
    char tips[FTESTRSE_COMMAND_TIPS_MAX_LEN];   ///< the tips of command
    SFT_callback callback;                                               ///<  the callback
};

void printCmdTips(CmdTips* cmds, unsigned int num);
void printMenuCmdTips(CmdTips* cmds, unsigned int num);
int getUserInput(int num);
void exeCmdTips(CmdTips* cmds, unsigned int maxSelNum);

CmdTips cmdsOfHmi[] =
{
    { "Back to Menu", NULL },
    { "Show Commadn Plugin Of Command List", NULL },
    /*
    {"connectSource", [&](){cout << "connect source" << endl; callCommandInterface->connectSource();}},
    {"disConnectSource", [&](){cout << "disconnect source" << endl; callCommandInterface->disConnectSource();}},
    {"setPrimarySinkVoulme", [&](){callCommandInterface->setPrimarySinkVoulme();}},
    {"primarySinkVolumeStep", [&](){callCommandInterface->primarySinkVolumeStep();}},
    {"setPrimarySinkMuteState", [&](){callCommandInterface->setPrimarySinkMuteState();}},
    {"setSecondarySinkVoulme", [&](){callCommandInterface->setSecondarySinkVoulme();}},
    {"secondarySinkVolumeStep", [&](){callCommandInterface->secondarySinkVolumeStep();}},
    {"setSecondarySinkMuteState", [&](){callCommandInterface->setSecondarySinkMuteState();}},
    {"setMainSinkSoundProperty", [&](){callCommandInterface->setMainSinkSoundProperty();}},
    {"setMainSourceSoundProperty", [&](){callCommandInterface->setMainSourceSoundProperty();}},
    {"setSystemProperty", [&](){callCommandInterface->setSystemProperty();}},
    {"setSinkMainNotificationConfiguration", [&](){callCommandInterface->setSinkMainNotificationConfiguration();}},
    {"setSourceMainNotificationConfiguration", [&](){callCommandInterface->setSourceMainNotificationConfiguration();}},
    {"getListMainConnections", [&](){callCommandInterface->getListMainConnections();}},
    {"getListMainSinks", [&](){callCommandInterface->getListMainSinks();}},
    {"getListMainSources", [&](){callCommandInterface->getListMainSources();}},
    {"getListMainSinkSoundProperties", [&](){callCommandInterface->getListMainSinkSoundProperties();}},
    {"getListMainSourceSoundProperties", [&](){callCommandInterface->getListMainSourceSoundProperties();}},
    {"getListSinkClasses", [&](){callCommandInterface->getListSinkClasses();}},
    {"getListSourceClasses", [&](){callCommandInterface->getListSourceClasses();}},
    {"getListSystemProperties", [&](){callCommandInterface->getListSystemProperties();}},
    {"getTimingInformation", [&](){callCommandInterface->getTimingInformation();}},
    {"getListSinkMainNotificationConfigurations", [&](){callCommandInterface->getListSinkMainNotificationConfigurations();}},
    {"getListSourceMainNotificationConfigurations", [&](){callCommandInterface->getListSourceMainNotificationConfigurations();}},
    */
    {   "CMD_connectSource", [&] () {
            cout << "connect source" << endl;
            callCommandInterfaceCAPI->connectSource();
        }
    },

    {   "CMD_disConnectSource", [&] () {
            cout << "disconnect source" << endl;
            callCommandInterfaceCAPI->disConnectSource();
        }
    },

    {   "CMD_setPrimarySinkVoulme", [&] () {
            callCommandInterfaceCAPI->setPrimarySinkVoulme();
        }
    },

    {   "CMD_primarySinkVolumeStep", [&] () {
            callCommandInterfaceCAPI->primarySinkVolumeStep();
        }
    },

    {   "CMD_setPrimarySinkMuteState", [&] () {
            callCommandInterfaceCAPI->setPrimarySinkMuteState();
        }
    },

    {   "CMD_setSecondarySinkVoulme", [&] () {
            callCommandInterfaceCAPI->setSecondarySinkVoulme();
        }
    },

    {   "CMD_secondarySinkVolumeStep", [&] () {
            callCommandInterfaceCAPI->secondarySinkVolumeStep();
        }
    },

    {   "CMD_setSecondarySinkMuteState", [&] () {
            callCommandInterfaceCAPI->setSecondarySinkMuteState();
        }
    },

    {   "CMD_setMainSinkSoundProperty", [&] () {
            callCommandInterfaceCAPI->setMainSinkSoundProperty();
        }
    },

    {   "CMD_setMainSourceSoundProperty", [&] () {
            callCommandInterfaceCAPI->setMainSourceSoundProperty();
        }
    },

    {   "CMD_setSystemProperty", [&] () {
            callCommandInterfaceCAPI->setSystemProperty();
        }
    },

    {   "CMD_setSinkMainNotificationConfiguration", [&] () {
            callCommandInterfaceCAPI->setSinkMainNotificationConfiguration();
        }
    },

    {   "CMD_setSourceMainNotificationConfiguration", [&] () {
            callCommandInterfaceCAPI->setSourceMainNotificationConfiguration();
        }
    },

    {   "CMD_getSinkMuteState", [&] () {
            callCommandInterfaceCAPI->getSinkMuteState();
        }
    },

    {   "CMD_getListMainConnections", [&] () {
            callCommandInterfaceCAPI->getListMainConnections();
        }
    },

    {   "CMD_getListMainSinks", [&] () {
            callCommandInterfaceCAPI->getListMainSinks();
        }
    },

    {   "CMD_getListMainSources", [&] () {
            callCommandInterfaceCAPI->getListMainSources();
        }
    },

    {   "CMD_getListMainSinkSoundProperties", [&] () {
            callCommandInterfaceCAPI->getListMainSinkSoundProperties();
        }
    },

    {   "CMD_getListMainSourceSoundProperties", [&] () {
            callCommandInterfaceCAPI->getListMainSourceSoundProperties();
        }
    },

    {   "CMD_getListSinkClasses", [&] () {
            callCommandInterfaceCAPI->getListSinkClasses();
        }
    },

    {   "CMD_getListSourceClasses", [&] () {
            callCommandInterfaceCAPI->getListSourceClasses();
        }
    },

    {   "CMD_getListSystemProperties", [&] () {
            callCommandInterfaceCAPI->getListSystemProperties();
        }
    },

    {   "CMD_getTimingInformation", [&] () {
            callCommandInterfaceCAPI->getTimingInformation();
        }
    },

    {   "CMD_getListSinkMainNotificationConfigurations", [&] () {
            callCommandInterfaceCAPI->getListSinkMainNotificationConfigurations();
        }
    },

    {   "CMD_getListSourceMainNotificationConfigurations", [&] () {
            callCommandInterfaceCAPI->getListSourceMainNotificationConfigurations();
        }
    },

    {   "CMD_getListOfMainConnectionsAttribute", [&] () {
            callCommandInterfaceCAPI->getListOfMainConnections();
        }
    },

    {   "CMD_getInterfaceVersion", [&] () {
            callCommandInterfaceCAPI->getInterfaceVersion();
        }
    },

    {   "play_Media", [&] () {
            apiMedia->openFile();
            apiMedia->play();
        }
    },

    {   "play_Navi", [&] () {
            apiNavi->openFile();
            apiNavi->play();
        }
    },

    {   "play_Tts", [&] () {
            apiTts->openFile();
            apiTts->play();
        }
    },

    {   "play_Tel", [&] () {
            apiTel->openFile();
            apiTel->play();
        }
    },

    {   "play_Beep", [&] () {
            apiBeep->openFile();
            apiBeep->play();
        }
    },
};

CmdTips cmdsOfRoutingHmi[] =
{
    { "Back to Menu", NULL },
    { "Show Routing Plugin Command List", NULL },

    {   "RT_asyncAbort", [&] () {
            callRoutingInterfaceCAPI->callAsyncAbort();
        }
    },

    {   "RT_asyncConnect", [&] () {
            callRoutingInterfaceCAPI->callAsyncConnect();
        }
    },

    {   "RT_asyncDisconnect", [&] () {
            callRoutingInterfaceCAPI->callAsyncDisconnect();
        }
    },

    {   "RT_asyncSetVolumes", [&] () {
            callRoutingInterfaceCAPI->callAsyncSetVolumes();
        }
    },

    {   "RT_asyncCrossFade", [&] () {
            callRoutingInterfaceCAPI->callAsyncCrossFade();
        }
    },

    {   "RT_asyncSetSinkVolume", [&] () {
            callRoutingInterfaceCAPI->callAsyncSetSinkVolume();
        }
    },

    {   "RT_asyncSetSourceVolume", [&] () {
            callRoutingInterfaceCAPI->callAsyncSetSourceVolume();
        }
    },

    {   "RT_asyncSetSinkSoundProperty", [&] () {
            callRoutingInterfaceCAPI->callAsyncSetSinkSoundProperty();
        }
    },

    {   "RT_asyncSetSinkSoundProperties", [&] () {
            callRoutingInterfaceCAPI->callAsyncSetSinkSoundProperties();
        }
    },

    {   "RT_asyncSetSourceSoundProperty", [&] () {
            callRoutingInterfaceCAPI->callAsyncSetSourceSoundProperty();
        }
    },

    {   "RT_asyncSetSourceSoundProperties", [&] () {
            callRoutingInterfaceCAPI->callAsyncSetSourceSoundProperties();
        }
    },

    {   "RT_asyncSetSourceState", [&] () {
            callRoutingInterfaceCAPI->callAsyncSetSourceState();
        }
    },

    {   "RT_asyncSetSinkNotificationConfiguration", [&] () {
            callRoutingInterfaceCAPI->callAsyncSetSinkNotificationConfiguration();
        }
    },

    {   "RT_asyncSetSourceNotificationConfiguration", [&] () {
            callRoutingInterfaceCAPI->callAsyncSetSourceNotificationConfiguration();
        }
    },

    {   "RT_setDomainState", [&] () {
            callRoutingInterfaceCAPI->callSetDomainState();
        }
    },

    {   "RT_peekDomain", [&] () {
            callRoutingInterfaceCAPI->peekDomain();
        }
    },

    {   "RT_registerDomain", [&] () {
            callRoutingInterfaceCAPI->registerDomain();
        }
    },

    {   "RT_deregisterDomain", [&] () {
            callRoutingInterfaceCAPI->deregisterDomain();
        }
    },

    {   "RT_registerGateway", [&] () {
            callRoutingInterfaceCAPI->registerGateway();
        }
    },

    {   "RT_deregisterGateway", [&] () {
            callRoutingInterfaceCAPI->deregisterGateway();
        }
    },

    {   "RT_registerConverter", [&] () {
            callRoutingInterfaceCAPI->registerConverter();
        }
    },

    {   "RT_deregisterConverter", [&] () {
            callRoutingInterfaceCAPI->deregisterConverter();
        }
    },

    {   "RT_peekSink", [&] () {
            callRoutingInterfaceCAPI->peekSink();
        }
    },

    {   "RT_registerSink", [&] () {
            callRoutingInterfaceCAPI->registerSink();
        }
    },

    {   "RT_deregisterSink", [&] () {
            callRoutingInterfaceCAPI->deregisterSink();
        }
    },

    {   "RT_peekSource", [&] () {
            callRoutingInterfaceCAPI->peekSource();
        }
    },

    {   "RT_registerSource", [&] () {
            callRoutingInterfaceCAPI->registerSource();
        }
    },

    {   "RT_deregisterSource", [&] () {
            callRoutingInterfaceCAPI->deregisterSource();
        }
    },

    {   "RT_registerCrossfader", [&] () {
            callRoutingInterfaceCAPI->registerCrossfader();
        }
    },

    {   "RT_deregisterCrossfader", [&] () {
            callRoutingInterfaceCAPI->deregisterCrossfader();
        }
    },

    {   "RT_peekSourceClassID", [&] () {
            callRoutingInterfaceCAPI->peekSourceClassID();
        }
    },

    {   "RT_peekSinkClassID", [&] () {
            callRoutingInterfaceCAPI->peekSinkClassID();
        }
    },

    {   "RT_hookInterruptStatusChange", [&] () {
            callRoutingInterfaceCAPI->hookInterruptStatusChange();
        }
    },

    {   "RT_hookDomainRegistrationComplete", [&] () {
            callRoutingInterfaceCAPI->hookDomainRegistrationComplete();
        }
    },

    {   "RT_hookSinkAvailablityStatusChange", [&] () {
            callRoutingInterfaceCAPI->hookSinkAvailablityStatusChange();
        }
    },

    {   "RT_hookSourceAvailablityStatusChange", [&] () {
            callRoutingInterfaceCAPI->hookSourceAvailablityStatusChange();
        }
    },

    {   "RT_hookDomainStateChange", [&] () {
            callRoutingInterfaceCAPI->hookDomainStateChange();
        }
    },

    {   "RT_hookTimingInformationChanged", [&] () {
            callRoutingInterfaceCAPI->hookTimingInformationChanged();
        }
    },

    {   "RT_sendChangedData", [&] () {
            callRoutingInterfaceCAPI->sendChangedData();
        }
    },

    {   "RT_updateGateway", [&] () {
            callRoutingInterfaceCAPI->updateGateway();
        }
    },

    {   "RT_updateConverter", [&] () {
            callRoutingInterfaceCAPI->updateConverter();
        }
    },

    {   "RT_updateSink", [&] () {
            callRoutingInterfaceCAPI->updateSink();
        }
    },

    {   "RT_updateSource", [&] () {
            callRoutingInterfaceCAPI->updateSource();
        }
    },

    {   "RT_hookSinkNotificationDataChange", [&] () {
            callRoutingInterfaceCAPI->hookSinkNotificationDataChange();
        }
    },

    {   "RT_hookSourceNotificationDataChange", [&] () {
            callRoutingInterfaceCAPI->hookSourceNotificationDataChange();
        }
    },

    {   "RT_confirmRoutingRundown", [&] () {
            callRoutingInterfaceCAPI->confirmRoutingRundown();
        }
    },

    {   "RT_getRoutingInterfaceVersion", [&] () {
            callRoutingInterfaceCAPI->getRoutingInterfaceVersion();
        }
    },

    {   "RT_getRoutingObserverInterfaceVersion", [&] () {
            callRoutingInterfaceCAPI->getRoutingObserverInterfaceVersion();
        }
    },

    {   "play_Media", [&] () {
            apiMedia->openFile();
            apiMedia->play();
        }
    },

    {   "play_Navi", [&] () {
            apiNavi->openFile();
            apiNavi->play();
        }
    },

    {   "play_Tts", [&] () {
            apiTts->openFile();
            apiTts->play();
        }
    },

    {   "play_Tel", [&] () {
            apiTel->openFile();
            apiTel->play();
        }
    },

    {   "play_Beep", [&] () {
            apiBeep->openFile();
            apiBeep->play();
        }
    },
};

CmdTips cmdsOfMMHmi[] =
{
    { "Back to Menu", NULL },
    { "Show Media Manager Command List", NULL },

    {   "MM_indexer.startIndexing", [&] () {
            callMMInterface->callStartIndexing();
        }
    },

    {   "MM_indexer.stopIndexing", [&] () {
            callMMInterface->callStopIndexing();
        }
    },

    {   "MM_indexer.getDatabasePath", [&] () {
            callMMInterface->callgetDatabasePath();
        }
    },

    {   "MM_indexer.getIndexerStatusAttribute", [&] () {
            callMMInterface->getIndexerStatusAttribute();
        }
    },

    {   "MM_browser.listIndexes", [&] () {
            callMMInterface->callBrowserListIndexes();
        }
    },

    {   "MM_browser.discoverMediaManagers", [&] () {
            callMMInterface->callBrowserDiscoverMediaManagers();
        }
    },

    {   "MM_browser.listContainers", [&] () {
            callMMInterface->callBrowserListContainers();
        }
    },

    {   "MM_browser.listContainersEx", [&] () {
            callMMInterface->callBrowserListContainersEx();
        }
    },

    {   "MM_browser.listChildren", [&] () {
            callMMInterface->callBrowserListChildren();
        }
    },

    {   "MM_browser.listChildrenEx", [&] () {
            callMMInterface->callBrowserListChildrenEx();
        }
    },

    {   "MM_browser.listItems", [&] () {
            callMMInterface->callBrowserListItems();
        }
    },

    {   "MM_browser.listItemsEx", [&] () {
            callMMInterface->callBrowserListItemsEx();
        }
    },

    {   "MM_browser.createReference", [&] () {
            callMMInterface->callBrowserCreateReference();
        }
    },

    {   "MM_browser.createContainer", [&] () {
            callMMInterface->callBrowserCreateContainer();
        }
    },

    {   "MM_browser.searchObjects", [&] () {
            callMMInterface->callBrowserSearchObjects();
        }
    },

    {   "MM_browser.searchObjectsEx", [&] () {
            callMMInterface->callBrowserSearchObjectsEx();
        }
    },

    {   "MM_player .setMuteAttribute", [&] () {
            callMMInterface->setMuteAttribute();
        }
    },

    {   "MM_player .getMuteAttribute", [&] () {
            callMMInterface->getMuteAttribute();
        }
    },

    {   "MM_player .setShuffleAttribute", [&] () {
            callMMInterface->setShuffleAttribute();
        }
    },

    {   "MM_player .getShuffleAttribute", [&] () {
            callMMInterface->getShuffleAttribute();
        }
    },

    {   "MM_player .setRepeatAttribute", [&] () {
            callMMInterface->setRepeatAttribute();
        }
    },

    {   "MM_player .getRepeatAttribute", [&] () {
            callMMInterface->getRepeatAttribute();
        }
    },

    {   "MM_player .setRateAttribute", [&] () {
            callMMInterface->setRateAttribute();
        }
    },

    {   "MM_player .getRateAttribute", [&] () {
            callMMInterface->getRateAttribute();
        }
    },

    {   "MM_player .setVolumeAttribute", [&] () {
            callMMInterface->setVolumeAttribute();
        }
    },

    {   "MM_player .getVolumeAttribute", [&] () {
            callMMInterface->getVolumeAttribute();
        }
    },

    {   "MM_player .getCanGoNextAttribute", [&] () {
            callMMInterface->getCanGoNextAttribute();
        }
    },

    {   "MM_player .getCanGoPreviousAttribute", [&] () {
            callMMInterface->getCanGoPreviousAttribute();
        }
    },

    {   "MM_player .getCanPauseAttribute", [&] () {
            callMMInterface->getCanPauseAttribute();
        }
    },

    {   "MM_player .getCanPlayAttribute", [&] () {
            callMMInterface->getCanPlayAttribute();
        }
    },

    {   "MM_player .getCanSeekAttribute", [&] () {
            callMMInterface->getCanSeekAttribute();
        }
    },

    {   "MM_player .getCurrentTrackAttribute", [&] () {
            callMMInterface->getCurrentTrackAttribute();
        }
    },

    {   "MM_player .getPlaybackStatusAttribute", [&] () {
            callMMInterface->getPlaybackStatusAttribute();
        }
    },

    {   "MM_player .getPositionAttribute", [&] () {
            callMMInterface->getPositionAttribute();
        }
    },

    {   "MM_player .getDurationAttribute", [&] () {
            callMMInterface->getDurationAttribute();
        }
    },

    {   "MM_player .next", [&] () {
            callMMInterface->callPlayerNext();
        }
    },

    {   "MM_player .openUri", [&] () {
            callMMInterface->callPlayerOpenUri();
        }
    },

    {   "MM_player .enqueueUri", [&] () {
            callMMInterface->callPlayerEnqueueUri();
        }
    },

    {   "MM_player .dequeueIndex", [&] () {
            callMMInterface->callPlayerDequeueIndex();
        }
    },

    {   "MM_player .getCurrentPlayQueue", [&] () {
            callMMInterface->callPlayerGetCurrentPlayQueue();
        }
    },

    {   "MM_player .dequeueAll", [&] () {
            callMMInterface->callPlayerDequeueAll();
        }
    },

    {   "MM_player .openPlaylist", [&] () {
            callMMInterface->callPlayerOpenPlaylist();
        }
    },

    {   "MM_player .pause", [&] () {
            callMMInterface->callPlayerPause();
        }
    },

    {   "MM_player .play", [&] () {
            callMMInterface->callPlayerPlay();
        }
    },

    {   "MM_player .playPause", [&] () {
            callMMInterface->callPlayerPlayPause();
        }
    },

    {   "MM_player .previous", [&] () {
            callMMInterface->callPlayerPrevious();
        }
    },

    {   "MM_player .seek", [&] () {
            callMMInterface->callPlayerSeek();
        }
    },

    {   "MM_player .setPosition", [&] () {
            callMMInterface->callPlayerSetPosition();
        }
    },
};

CmdTips cmdsOfRadioHmi[] =
{
    { "Back to Menu", NULL },
    { "Show IVI Radio Command List", NULL },

    {   "FMSourceOn", [&] () {
            callIVIRaido->callSourceOn();
        }
    },

    {   "IVI.requestDataService", [&] () {
            callIVIRaido->callRequestDataService();
        }
    },

    {   "IVI.requestSubscribedServiceOptions", [&] () {
            callIVIRaido->callRequestSubscribedServiceOptions();
        }
    },

    {   "IVI.abort", [&] () {
            callIVIRaido->callAbort();
        }
    },

    {   "IVI.changeFunctionalityConfiguration", [&] () {
            callIVIRaido->callChangeFunctionalityConfiguration();
        }
    },

    {   "IVI.requestFunctionalityConfiguration", [&] () {
            callIVIRaido->callRequestFunctionalityConfiguration();
        }
    },

    {   "IVI.abortTune", [&] () {
            callIVIRaido->callAbortTune();
        }
    },

    {   "IVI.makePermanent", [&] () {
            callIVIRaido->callMakePermanent();
        }
    },

    {   "IVI.controlUpdate", [&] () {
            callIVIRaido->callControlUpdate();
        }
    },

    {   "IVI.Seek", [&] () {
            callIVIRaido->callSeek();
        }
    },

    {   "IVI.startTune", [&] () {
            callIVIRaido->callStartTune();
        }
    },

};


CmdTips cmdsOfMainMenuHmi[] =
{
    { "Exit", NULL },
    { "Show Menu Command List", NULL },

    {   "Show Command Plugin API List", [&] () {
            int maxSelNum = (sizeof(cmdsOfHmi) / sizeof(CmdTips));
            exeCmdTips(cmdsOfHmi, maxSelNum);
        }
    },

    {   "Show Routing Plugin API List", [&] () {
            int maxSelNum = (sizeof(cmdsOfRoutingHmi) / sizeof(CmdTips));
            exeCmdTips(cmdsOfRoutingHmi, maxSelNum);
        }
    },

    {   "Show Media  Manager API List", [&] () {
            int maxSelNum = (sizeof(cmdsOfMMHmi) / sizeof(CmdTips));
            exeCmdTips(cmdsOfMMHmi, maxSelNum);
        }
    },

    {   "Show    IVI   Radio API List", [&] () {
            int maxSelNum = (sizeof(cmdsOfRadioHmi) / sizeof(CmdTips));
            exeCmdTips(cmdsOfRadioHmi, maxSelNum);
        }
    },
};


void printCmdTips(CmdTips* cmds, unsigned int num)
{
    unsigned int midNum = (num / 2) + (num % 2);
    cout << "****************************************************************************************" << endl;
    unsigned int size = 1;
    for (unsigned int i = 0; i < midNum; i++)
    {
        cout << std::setw(3) << std::right << size << " : " << std::setw(50) << std::left << cmds[i].tips;
        if (num == (i + midNum))
        {
            cout << endl;
            break;
        }
        cout << std::setw(3) << std::right << (size + midNum) << " : " << std::setw(50) << std::left << cmds[i + midNum].tips << endl;
        ++size;
    }
    cout << "****************************************************************************************" << endl;
}

void printMenuCmdTips(CmdTips* cmds, unsigned int num)
{
    cout << "*****************************************" << endl;
    unsigned int size = 1;
    for (unsigned int i = 0; i < num; i++)
    {
        cout << std::setw(3) << std::right << size << " : " << std::setw(50) << std::left << cmds[i].tips << endl;
        ++size;
    }
    cout << "*****************************************" << endl;
}

int getUserInput(int num)
{
    int ret = 0;
    cout << "Please input a number between 1 and " << num << endl;

    cin >> ret;
    if (!cin)
    {
        cin.clear();
        cin.get();
        cout << endl;
    }
    return ret;
}

void exeCmdTips(CmdTips* cmds, unsigned int maxSelNum)
{
    while (1) {
        printCmdTips(cmds, maxSelNum);
        unsigned int info = (unsigned int)getUserInput(maxSelNum);

        // do corresponding operation according user input
        if (info <= 0 || info > maxSelNum) {
            cout << "Warning: Do nothing because your input is out of range." << endl;
            continue;
        }
        else if (1 == info) {
            cout << cmds[0].tips << endl;
            break;
        }
        else if (2 == info) {
            continue;
        }
        else {
            cmds[info - 1].callback();
            cout << CYAN << "Press Enter to Continue!" << RESET << endl;
            if ('\0' == cin.get()) {
                continue;
            }
            else if ('\r' == cin.get()) {
                continue;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    printf("----------process start\n");
    NCTestProcess process;
    process.initialize();
    process.start();

    /*
        try
        {
            TCLAP::CmdLine* cmd(CAmCommandLineSingleton::instanciateOnce("The team of the AudioManager wishes you a nice day!", ' ', DAEMONVERSION, true));
            cmd->add(currentSettings);
            cmd->add(daemonizeAM);
            cmd->add(dltEnable);
            cmd->add(dltLogFilename);
            cmd->add(dltOutput);
        }
        catch (TCLAP::ArgException &e)  // catch any exceptions
        {
            std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        }
        //hen and egg. We need to parse a part of the commandline options to get the paths of the controller and the plugins.
        //So we do some little parsing first and the real parsing later so that the plugins can profit from that.
        CAmCommandLineSingleton::instance()->preparse(argc, argv);
        if (daemonizeAM.getValue())
        {
            //daemonize();
        }
        CAmDltWrapper::instanctiateOnce("AUDI", "AudioManager-Client", dltEnable.getValue(), static_cast<am::CAmDltWrapper::logDestination>(dltOutput.getValue()), dltLogFilename.getValue());
        try
        {
            //parse the commandline options
            CAmCommandLineSingleton::instance()->reset();
            CAmCommandLineSingleton::instance()->parse(argc, argv);
            if (currentSettings.getValue())
            {
                //printCmdInformation();
            }
        }
        catch (TCLAP::ArgException &e)  // catch any exceptions
        {
            std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
        }

        CAmSocketHandler iSocketHandler;
        DBusBusType dbusWrapperType = DBUS_BUS_SESSION;
        CAmDbusWrapper *dbusWrapper = new CAmDbusWrapper(&iSocketHandler, dbusWrapperType, "org.genivi.audiomanager.commandinterface", "/org/genivi/audiomanager/commandinterface");
        tstCommandReceiver* receiver = new tstCommandReceiver();
        receiver->setCommandReceiver(dbusWrapper);
    */

    NCRunnableThread* m_pThread = NULL;
    NMRunnableLooper* m_pNMlooper = NULL;
    NMRunnableTimer*  m_pTimer = NULL;

    NCRunnableThread* m_pRoutingThread = NULL;
    NMRunnableLooper* m_pNMRoutinglooper = NULL;
    NMRunnableTimer*  m_pRoutingTimer = NULL;

    NCRunnableThread* m_pRadioThread = NULL;
    NMRunnableLooper* m_pNMRadiolooper = NULL;
    NMRunnableTimer*  m_pRadioTimer = NULL;

    callMMInterface->startProxy();
    callCommandInterfaceCAPI->startProxy();
    callRoutingInterfaceCAPI->startProxy();
    callIVIRaido->startProxy();

    m_pThread = new NCRunnableThread();
    if (NULL != m_pThread) {
        m_pThread->startThread("tstReceiver");
        m_pNMlooper = new NMRunnableLooper(m_pThread->looper());
    }
    //m_pTimer = new NMRunnableTimer(NMTaskBind(&iSocketHandler, &CAmSocketHandler::start_listenting), 1000, NC_FALSE);   //Dbus Receiver CallBack
    m_pTimer = new NMRunnableTimer(NMTaskBind(callCommandInterfaceCAPI, &tstCallCommandInterfaceCAPI::runListen), 1000, NC_FALSE);
    m_pNMlooper->postRunnable(NMTaskBind(m_pTimer, &NMRunnableTimer::start));
    sleep(1);

    m_pRoutingThread = new NCRunnableThread();
    if (NULL != m_pRoutingThread) {
        m_pRoutingThread->startThread("tstRoutingReceiver");
        m_pNMRoutinglooper = new NMRunnableLooper(m_pRoutingThread->looper());
    }
    m_pRoutingTimer = new NMRunnableTimer(NMTaskBind(callRoutingInterfaceCAPI, &tstCallRoutingInterfaceCAPI::runListen), 1000, NC_FALSE);
    m_pNMRoutinglooper->postRunnable(NMTaskBind(m_pRoutingTimer, &NMRunnableTimer::start));
    sleep(1);

    m_pRadioThread = new NCRunnableThread();
    if (NULL != m_pRadioThread) {
        m_pRadioThread->startThread("tstRadioReceiver");
        m_pNMRadiolooper = new NMRunnableLooper(m_pRadioThread->looper());
    }
    m_pRadioTimer = new NMRunnableTimer(NMTaskBind(callIVIRaido, &tstCallIVIRadio::runListen), 1000, NC_FALSE);
    m_pNMRadiolooper->postRunnable(NMTaskBind(m_pRadioTimer, &NMRunnableTimer::start));
    sleep(1);

    int maxSelNum = (sizeof(cmdsOfMainMenuHmi) / sizeof(CmdTips));
    while (1) {
        printMenuCmdTips(cmdsOfMainMenuHmi, maxSelNum);
        int info = getUserInput(maxSelNum);

        // do corresponding operation according user input
        if (info <= 0 || info > maxSelNum) {
            cout << "Warning: Do nothing because your input is out of range." << endl;
            continue;
        }
        else if (1 == info) {
            cout << "Exit!" << endl;
            break;
        }
        else if (2 == info) {
            continue;
        }
        else {
            cmdsOfMainMenuHmi[info - 1].callback();
        }
    }

    callCommandInterfaceCAPI->stopListen();
    callRoutingInterfaceCAPI->stopListen();
    callIVIRaido->stopListen();

//   iSocketHandler.stop_listening();
    sleep(2);

    if (NULL != cbMedia) {
        delete cbMedia;
        cbMedia = NULL;
    }
    if (NULL != apiMedia) {
        delete apiMedia;
        apiMedia = NULL;
    }
    if (NULL != cbNavi) {
        delete cbNavi;
        cbNavi = NULL;
    }
    if (NULL != apiNavi) {
        delete apiNavi;
        apiNavi = NULL;
    }
    if (NULL != cbTts) {
        delete cbTts;
        cbTts = NULL;
    }
    if (NULL != apiTts) {
        delete apiTts;
        apiTts = NULL;
    }
    if (NULL != cbTel) {
        delete cbTel;
        cbTel = NULL;
    }
    if (NULL != apiTel) {
        delete apiTel;
        apiTel = NULL;
    }
    if (NULL != cbBeep) {
        delete cbBeep;
        cbBeep = NULL;
    }
    if (NULL != apiBeep) {
        delete apiBeep;
        apiBeep = NULL;
    }

    if (NULL != callMMInterface) {
        delete callMMInterface;
        callMMInterface = NULL;
    }

    if (NULL != m_pTimer) {
        delete m_pTimer;
        m_pTimer = NULL;
    }

    if (NULL != m_pNMlooper) {
        delete m_pNMlooper;
        m_pNMlooper = NULL;
    }

    if (NULL != m_pThread) {
        m_pThread->stopThread();
        delete m_pThread;
        m_pThread = NULL;
    }

    if (NULL != m_pRoutingTimer) {
        delete m_pRoutingTimer;
        m_pRoutingTimer = NULL;
    }

    if (NULL != m_pNMRoutinglooper) {
        delete m_pNMRoutinglooper;
        m_pNMRoutinglooper = NULL;
    }

    if (NULL != m_pRoutingThread) {
        m_pRoutingThread->stopThread();
        delete m_pRoutingThread;
        m_pRoutingThread = NULL;
    }

    if (NULL != m_pRadioTimer) {
        delete m_pRadioTimer;
        m_pRadioTimer = NULL;
    }

    if (NULL != m_pNMRadiolooper) {
        delete m_pNMRadiolooper;
        m_pNMRadiolooper = NULL;
    }

    if (NULL != m_pRadioThread) {
        m_pRadioThread->stopThread();
        delete m_pRadioThread;
        m_pRadioThread = NULL;
    }

    /*
        if (NULL != callCommandInterface) {
            delete callCommandInterface;
            callCommandInterface = NULL;
        }
    */

    if (NULL != callCommandInterfaceCAPI) {
        delete callCommandInterfaceCAPI;
        callCommandInterfaceCAPI = NULL;
    }

    if (NULL != callRoutingInterfaceCAPI) {
        delete callRoutingInterfaceCAPI;
        callRoutingInterfaceCAPI = NULL;
    }

    if (NULL != callIVIRaido) {
        delete callIVIRaido;
        callIVIRaido = NULL;
    }

    /*
        if (NULL != receiver)
        {
            delete receiver;
            receiver = NULL;
        }

        if (NULL != dbusWrapper)
        {
            delete dbusWrapper;
            dbusWrapper = NULL;
        }
    */

    sleep(1);

    cout << "Stop process!" << endl;
    process.stop();

    cout << "Deinitialize!" << endl;
    process.deinitialize();

    return 0;
}
/* EOF */
