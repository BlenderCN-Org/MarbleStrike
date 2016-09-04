import os
import sys
import distutils.dir_util
from stat import *
from shutil import *

#################################################################################
#
#################################################################################

def ChangeSystemSlash( filename ):
	newFilename = filename
	newFilename = filename.replace("\\","/")
	return newFilename

#################################################################################
# Walktree Function
#################################################################################

def Walktree( top, filenameList ):
    for f in os.listdir( top ):
        if f != ".svn":
            pathname = os.path.join( top, f )
            mode = os.stat( pathname )[ST_MODE]
            if S_ISDIR( mode ):
                Walktree( pathname, filenameList )
            elif S_ISREG( mode ):                
                filenameList.append( pathname );
            else:
                print( "Skipping %s" % pathname )

directoryFilters = [
    "code/common/engine/",
    "code/common/game/bowlergame/",
    "code/android/",
    "sdk/tinyxml2/",
    "sdk/jsoncpp-src-0.6.0-rc2/src/lib_json"    
    ]
    
includeDirectories = [
    "code",
    "sdk/tinyxml2",
    "sdk/bullet-2.81-rev2613/src",
    "sdk/jsoncpp-src-0.6.0-rc2/include",
    "sdk/libogg-1.3.0/include",
    "sdk/libvorbis-1.3.3/include"
    ]

extensionFilters = [
    ".cpp"
    ]
    
doNotIncludeFiles = [
    "sdk/tinyxml2/xmltest.cpp"
    ]

filenameList = []                
Walktree( "..\\code\\", filenameList )
Walktree( "..\\sdk\\", filenameList )

finalFileList = []

for filename in filenameList:
    filename = ChangeSystemSlash( filename )
    filename = filename.replace( "../", "" )
    
    includeFile = True
    for file in doNotIncludeFiles:
        findResult = filename.find( file )
        if findResult != -1:
            includeFile = False
            break;
            
    if includeFile:    
        directoryFound = False
        for filter in directoryFilters:
            findResult = filename.find( filter )
            if findResult != -1:
                directoryFound = True
                break;
                
        extensionFound = False
        for extension in extensionFilters:
            findResult = filename.find( extension )
            if findResult != -1:
                extensionFound = True
                break;
        
        if directoryFound and extensionFound:
            finalFileList.append( filename )

f = open( 'jni/Android.mk', 'w+' )

f.write( 'LOCAL_PATH := $(call my-dir)\\..\\..\n' )
f.write( '\n' )
f.write( 'include $(call all-subdir-makefiles)\n' )
f.write( '\n' )
f.write( 'include $(CLEAR_VARS)\n' )
f.write( '\n' )
f.write( 'LOCAL_SHORT_COMMANDS := true\n' )
f.write( '\n' )
f.write( 'LOCAL_MODULE := buildandroid_mod\n' )
f.write( '\n' )

f.write( 'LOCAL_C_INCLUDES := \\\n' )
for dir in includeDirectories:
    f.write( '    $(LOCAL_PATH)/%s \\\n' % dir )
f.write( '\n' )
f.write( '#LOCAL_CFLAGS := -Werror -DPLAT_ANDROID -DRENDER_PLAT_OGLES2 -DAUDIO_OPENSL -O3\n' )
f.write( 'LOCAL_CFLAGS := -Werror -DPLAT_ANDROID -DRENDER_PLAT_OGLES2 -D_DEBUG -DAUDIO_OPENSL -g\n' )
f.write( 'LOCAL_CFLAGS += -std=gnu++11\n' )
f.write( '\n' )

f.write( 'LOCAL_SRC_FILES := \\\n' )
for file in finalFileList:
    f.write( '    %s \\\n' % file )

f.write( '\n' )    
f.write( 'LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog\n' )
f.write( 'LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -landroid\n' )
f.write( 'LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -lEGL\n' )
f.write( 'LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -lGLESv1_CM\n' )
f.write( 'LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -lGLESv2\n' )
f.write( 'LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -lOpenSLES\n' )
f.write( '\n' )
f.write( 'LOCAL_STATIC_LIBRARIES := android_native_app_glue bullet_android vorbis_android ogg_android\n' )
f.write( '\n' )
f.write( 'include $(BUILD_SHARED_LIBRARY)\n' )
f.write( '\n' )
f.write( '$(call import-module,android/native_app_glue)' )