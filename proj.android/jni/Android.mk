LOCAL_PATH := $(call my-dir)

#prebuild KTPlay library
 include $(CLEAR_VARS)
 LOCAL_MODULE := libKTPlay
 LOCAL_SRC_FILES := armeabi/libKTPlay.so
 LOCAL_MODULE_FILENAME := libKTPlay
 include $(PREBUILT_SHARED_LIBRARY)
 include $(CLEAR_VARS)
 LOCAL_MODULE := libKTAccountmanager
 LOCAL_SRC_FILES := armeabi/libKTAccountmanager.so
 LOCAL_MODULE_FILENAME := libKTAccountmanager
 include $(PREBUILT_SHARED_LIBRARY)
 include $(CLEAR_VARS)
 LOCAL_MODULE := libKTFriendship
 LOCAL_SRC_FILES := armeabi/libKTFriendship.so
 LOCAL_MODULE_FILENAME := libKTFriendship
 include $(PREBUILT_SHARED_LIBRARY)
 include $(CLEAR_VARS)
 LOCAL_MODULE := libKTLeaderboard
 LOCAL_SRC_FILES := armeabi/libKTLeaderboard.so
 LOCAL_MODULE_FILENAME := libKTLeaderboard
 include $(PREBUILT_SHARED_LIBRARY)
 
 include $(CLEAR_VARS)
 LOCAL_MODULE := libumeng_opustool
 LOCAL_SRC_FILES := armeabi/libumeng_opustool.so
 LOCAL_MODULE_FILENAME := libumeng_opustool
 include $(PREBUILT_SHARED_LIBRARY)
 
  include $(CLEAR_VARS)
 LOCAL_MODULE := libBugly
 LOCAL_SRC_FILES := armeabi/libBugly.so
 LOCAL_MODULE_FILENAME := libBugly
 include $(PREBUILT_SHARED_LIBRARY)
 

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

FILE_LIST := hellocpp/main.cpp \
		../../Classes/C2DXShareSDK/Android/ShareSDKUtils.cpp \
   		../../Classes/C2DXShareSDK/C2DXShareSDK.cpp \
   		../../Classes/C2DXShareSDK/Android/JSON/CCJSONConverter.cpp \
   		../../Classes/C2DXShareSDK/Android/JSON/cJSON/cJSON.c
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/tinyXML/*.cpp)


LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
					$(LOCAL_PATH)/../../Classes/C2DXShareSDK \
  					$(LOCAL_PATH)/../../Classes/C2DXShareSDK/Android \
   					$(LOCAL_PATH)/../../Classes/C2DXShareSDK/Android/JSON \
   					$(LOCAL_PATH)/../../Classes/C2DXShareSDK/Android/JSON/cJSON

LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += mobclickcpp_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_curl_static
LOCAL_SHARED_LIBRARIES:= libKTPlay libKTAccountmanager libKTFriendship libKTLeaderboard
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := tpnsSecurity
LOCAL_SRC_FILES := armeabi/libtpnsSecurity.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := tpnsWatchdog
LOCAL_SRC_FILES := armeabi/libtpnsWatchdog.so
include $(PREBUILT_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,libMobClickCpp)
$(call import-module,curl/prebuilt/android)