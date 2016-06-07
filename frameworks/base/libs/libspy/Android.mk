LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_LDLIBS    := -llog -ldl -landroid_runtime
LOCAL_CFLAGS    := -O2 -pie -fPIE


LOCAL_SRC_FILES :=  Substrate/SubstrateHook.cpp \
                    Substrate/SubstrateDebug.cpp \
                    Substrate/SubstratePosixMemory.cpp \
                    Substrate/hde64.c\
                    HookLib/HookBase.cpp\
                    HookLib/HookFileOp.cpp\
                    HookLib/HookKill.cpp\
                    HookLib/HookLoadDex.cpp\
                    HookLib/HookMain.cpp\
                    HookLib/HookDvmInterp.cpp\
                    HookLib/HookDvmCallJNIMethod.cpp\
                    dalvikWatcher/invokeWatcher.cpp\
                    dalvikWatcher/main.cpp\
                    spy.cpp\
                    socket_client.cpp\
                    utils.cpp\


LOCAL_SHARED_LIBRARIES := \
    libcutils \
    libutils \
    libstlport \
    liblog \
    libdvm \
    libdl \
    libandroid_runtime
    

LOCAL_C_INCLUDES := \
    bionic \
    bionic/libstdc++/include \
    external/stlport/stlport \
    dalvik/ \
    dalvik/vm/ \


LOCAL_MODULE    := libspy



# build an executable
#include $(BUILD_EXECUTABLE)
include $(BUILD_SHARED_LIBRARY)
