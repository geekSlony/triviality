
LOCAL_PATH:= $(call my-dir)

#################################################################
# Target tsttool
#################################################################
include $(CLEAR_VARS)

LOCAL_MODULE := tsttool

LOCAL_SRC_FILES = src/tstTool.cpp \
                  src/tstCallMMInterface.cpp \
                  src/tstCallCommandInterfaceCAPI.cpp \

LOCAL_SHARED_LIBRARIES := \
                        libncapp \
                        libncore \
                        libnceventsys \
                        libutils \
                        libnmutils \
                        libCommonAPI \
                        libnmaudioclient \
                        libpulse_common \
                        libpulse_client \
                        libpulse_core \
                        libmediamgrproxy \


LOCAL_CFLAGS := \
        -std=gnu++11 -fPIC -Wall -O0 -pthread -Wno-error=non-virtual-dtor \
        -fexceptions -fpermissive\

LOCAL_LDFLAGS := -lpthread -lrt -ldl -Wl,-no-as-needed \

LOCAL_C_INCLUDES := \
            $(call include-path-for, libCommonAPI) \
            ${LOCAL_PATH}/include \
            ${LOCAL_PATH}/include/util/soft_renderer \
            framework/genivi/idl-common \
            framework/api/idl-generated/include/ \
            framework/service/media/MediaManager/include \
            platform/base/AppFramework/include \
            platform/media/mediautils/include \
            platform/service/AudioService/include \
            system/core/include \

LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)

