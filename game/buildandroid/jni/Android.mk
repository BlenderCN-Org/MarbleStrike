LOCAL_PATH := $(call my-dir)\..\..

include $(call all-subdir-makefiles)

include $(CLEAR_VARS)

LOCAL_SHORT_COMMANDS := true

LOCAL_MODULE := buildandroid_mod

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/code \
    $(LOCAL_PATH)/sdk/tinyxml2 \
    $(LOCAL_PATH)/sdk/bullet-2.81-rev2613/src \
    $(LOCAL_PATH)/sdk/jsoncpp-src-0.6.0-rc2/include \
    $(LOCAL_PATH)/sdk/libogg-1.3.0/include \
    $(LOCAL_PATH)/sdk/libvorbis-1.3.3/include \

LOCAL_CFLAGS := -Werror -DPLAT_ANDROID -DRENDER_PLAT_OGLES2 -DAUDIO_OPENSL -O3
#LOCAL_CFLAGS := -Werror -DPLAT_ANDROID -DRENDER_PLAT_OGLES2 -D_DEBUG -DAUDIO_OPENSL -g
LOCAL_CFLAGS += -std=gnu++11

LOCAL_SRC_FILES := \
    code/android/androidassetmanager.cpp \
    code/android/input/androidinputmanager.cpp \
    code/android/main.cpp \
    code/common/engine/animation/animationcontroller.cpp \
    code/common/engine/animation/interpolators.cpp \
    code/common/engine/audio/audiomanager.cpp \
    code/common/engine/audio/audiostaticsource.cpp \
    code/common/engine/audio/audiostreamsource.cpp \
    code/common/engine/audio/format/ogg.cpp \
    code/common/engine/audio/format/ogginterface.cpp \
    code/common/engine/audio/format/oggstream.cpp \
    code/common/engine/audio/format/wave.cpp \
    code/common/engine/audio/format/waveinterface.cpp \
    code/common/engine/audio/format/wavestream.cpp \
    code/common/engine/audio/openal/openalaudiomanager.cpp \
    code/common/engine/audio/openal/openalaudiostaticsource.cpp \
    code/common/engine/audio/openal/openalaudiostreamsource.cpp \
    code/common/engine/audio/openal/openalwrapper.cpp \
    code/common/engine/audio/opensl/openslaudiomanager.cpp \
    code/common/engine/audio/opensl/openslaudiostaticsource.cpp \
    code/common/engine/audio/opensl/openslaudiostreamsource.cpp \
    code/common/engine/audio/xaudio2/xaudio2audiomanager.cpp \
    code/common/engine/audio/xaudio2/xaudio2audiostaticsource.cpp \
    code/common/engine/audio/xaudio2/xaudio2audiostreamsource.cpp \
    code/common/engine/database/database.cpp \
    code/common/engine/game/game.cpp \
    code/common/engine/game/gameapp.cpp \
    code/common/engine/game/resource.cpp \
    code/common/engine/game/resourcemanager.cpp \
    code/common/engine/game/script.cpp \
    code/common/engine/game/scriptmanager.cpp \
    code/common/engine/game/scripttask.cpp \
    code/common/engine/game/timescheduler.cpp \
    code/common/engine/game/timetask.cpp \
    code/common/engine/gui/guibutton.cpp \
    code/common/engine/gui/guicheckbox.cpp \
    code/common/engine/gui/guicontrol.cpp \
    code/common/engine/gui/guiinput.cpp \
    code/common/engine/gui/guimanager.cpp \
    code/common/engine/gui/guiradiobutton.cpp \
    code/common/engine/gui/guiscreen.cpp \
    code/common/engine/gui/guislider.cpp \
    code/common/engine/gui/guisprite.cpp \
    code/common/engine/gui/guistatic.cpp \
    code/common/engine/input/inputmanager.cpp \
    code/common/engine/math/aabb.cpp \
    code/common/engine/math/disc.cpp \
    code/common/engine/math/frustum.cpp \
    code/common/engine/math/gaussjordan.cpp \
    code/common/engine/math/intersections.cpp \
    code/common/engine/math/mathfunctions.cpp \
    code/common/engine/math/matrix.cpp \
    code/common/engine/math/matrix44.cpp \
    code/common/engine/math/noise.cpp \
    code/common/engine/math/plane.cpp \
    code/common/engine/math/quaternion.cpp \
    code/common/engine/math/ray.cpp \
    code/common/engine/math/sphere.cpp \
    code/common/engine/math/triangle.cpp \
    code/common/engine/math/vector3.cpp \
    code/common/engine/math/vector4.cpp \
    code/common/engine/messages/message.cpp \
    code/common/engine/messages/messagehandler.cpp \
    code/common/engine/network/clientsocketmanager.cpp \
    code/common/engine/network/networklistenersocket.cpp \
    code/common/engine/network/networkmanager.cpp \
    code/common/engine/network/networkserver.cpp \
    code/common/engine/network/networksocket.cpp \
    code/common/engine/network/networksocketmanager.cpp \
    code/common/engine/network/packet.cpp \
    code/common/engine/network/serversocketmanager.cpp \
    code/common/engine/physics/physicsconstraintdata.cpp \
    code/common/engine/physics/physicsdata.cpp \
    code/common/engine/physics/physicsmanager.cpp \
    code/common/engine/physics/physicsobject.cpp \
    code/common/engine/render/blenderasset.cpp \
    code/common/engine/render/blenderassetmanager.cpp \
    code/common/engine/render/blendstate.cpp \
    code/common/engine/render/bmpimage.cpp \
    code/common/engine/render/bone.cpp \
    code/common/engine/render/boundingvolume.cpp \
    code/common/engine/render/bsp.cpp \
    code/common/engine/render/bsppolygon.cpp \
    code/common/engine/render/camera.cpp \
    code/common/engine/render/cameravolume.cpp \
    code/common/engine/render/constantbuffer.cpp \
    code/common/engine/render/cubicbezierspline.cpp \
    code/common/engine/render/curve.cpp \
    code/common/engine/render/ddsimage.cpp \
    code/common/engine/render/depthstencilstate.cpp \
    code/common/engine/render/effect.cpp \
    code/common/engine/render/effectmanager.cpp \
    code/common/engine/render/effectpass.cpp \
    code/common/engine/render/effecttechnique.cpp \
    code/common/engine/render/followpathconstraint.cpp \
    code/common/engine/render/font.cpp \
    code/common/engine/render/fontmanager.cpp \
    code/common/engine/render/grid.cpp \
    code/common/engine/render/icosahedronsphere.cpp \
    code/common/engine/render/image.cpp \
    code/common/engine/render/ipocurve.cpp \
    code/common/engine/render/light.cpp \
    code/common/engine/render/material.cpp \
    code/common/engine/render/mesh.cpp \
    code/common/engine/render/oceanwave.cpp \
    code/common/engine/render/octree.cpp \
    code/common/engine/render/octreenode.cpp \
    code/common/engine/render/particle.cpp \
    code/common/engine/render/particlemanager.cpp \
    code/common/engine/render/particlesystem.cpp \
    code/common/engine/render/pixelshader.cpp \
    code/common/engine/render/postfx.cpp \
    code/common/engine/render/rasterizerstate.cpp \
    code/common/engine/render/renderer.cpp \
    code/common/engine/render/rendernode.cpp \
    code/common/engine/render/renderobject.cpp \
    code/common/engine/render/renderobjectdata.cpp \
    code/common/engine/render/renderqueue.cpp \
    code/common/engine/render/renderstatemanager.cpp \
    code/common/engine/render/renderstateshadowing.cpp \
    code/common/engine/render/rendertarget.cpp \
    code/common/engine/render/renderutilities.cpp \
    code/common/engine/render/rgbeimage.cpp \
    code/common/engine/render/samplerstate.cpp \
    code/common/engine/render/shader.cpp \
    code/common/engine/render/shadermanager.cpp \
    code/common/engine/render/shaderparameter.cpp \
    code/common/engine/render/shaderparametermatrix.cpp \
    code/common/engine/render/shaderparametertexture.cpp \
    code/common/engine/render/shaderparametervector.cpp \
    code/common/engine/render/shaderprogram.cpp \
    code/common/engine/render/simpledraw.cpp \
    code/common/engine/render/simplemesh.cpp \
    code/common/engine/render/spriteanimated.cpp \
    code/common/engine/render/texture.cpp \
    code/common/engine/render/texturemanager.cpp \
    code/common/engine/render/textureparameter.cpp \
    code/common/engine/render/trigger.cpp \
    code/common/engine/render/vertexbuffer.cpp \
    code/common/engine/render/vertexdeclaration.cpp \
    code/common/engine/render/vertexelement.cpp \
    code/common/engine/render/vertexshader.cpp \
    code/common/engine/system/debug/console.cpp \
    code/common/engine/system/debug/debugmenu.cpp \
    code/common/engine/system/debug/debugvariable.cpp \
    code/common/engine/system/debug/debugvariablebool.cpp \
    code/common/engine/system/debug/debugvariablefloat.cpp \
    code/common/engine/system/debug/debugvariableint.cpp \
    code/common/engine/system/debug/debugvariablestring.cpp \
    code/common/engine/system/debug/directoryitem.cpp \
    code/common/engine/system/debug/directorysubdir.cpp \
    code/common/engine/system/debug/directoryvariable.cpp \
    code/common/engine/system/fixedstring.cpp \
    code/common/engine/system/hashtable.cpp \
    code/common/engine/system/keyboard.cpp \
    code/common/engine/system/memory/memorydlmalloc.cpp \
    code/common/engine/system/memory/memorylog.cpp \
    code/common/engine/system/memory/memorymalloc.cpp \
    code/common/engine/system/memory/memorymanager.cpp \
    code/common/engine/system/memory/memorysba.cpp \
    code/common/engine/system/memory/memory_unittest.cpp \
    code/common/engine/system/memory.cpp \
    code/common/engine/system/pathutilities.cpp \
    code/common/engine/system/profiler.cpp \
    code/common/engine/system/profilermanager.cpp \
    code/common/engine/system/rawfile.cpp \
    code/common/engine/system/string.cpp \
    code/common/engine/system/stringutilities.cpp \
    code/common/engine/system/task.cpp \
    code/common/engine/system/taskmanager.cpp \
    code/common/engine/system/timer.cpp \
    code/common/engine/system/utilities.cpp \
    code/common/engine/system/variable.cpp \
    code/common/engine/system/variablebool.cpp \
    code/common/engine/system/variablefloat.cpp \
    code/common/engine/system/variableint.cpp \
    code/common/game/bowlergame/bowlerdatabase.cpp \
    code/common/game/bowlergame/bowlergame.cpp \
    code/common/game/bowlergame/bowlerinput.cpp \
    code/common/game/bowlergame/bowlerutilities.cpp \
    code/common/game/bowlergame/gamescript/levelscript.cpp \
    code/common/game/bowlergame/gamescript/levelscript6.cpp \
    code/common/game/bowlergame/gamescript/levelscript7.cpp \
    code/common/game/bowlergame/gui/bowlerguimanager.cpp \
    code/common/game/bowlergame/gui/bowlerloadgui.cpp \
    code/common/game/bowlergame/gui/bowleroptionsgui.cpp \
    code/common/game/bowlergame/gui/bowlerrungui.cpp \
    code/common/game/bowlergame/gui/bowlerselectgui.cpp \
    code/common/game/bowlergame/gui/bowlerstartgui.cpp \
    code/common/game/bowlergame/gui/bowlerstatsgui.cpp \
    code/common/game/bowlergame/messages/bowlereventmessage.cpp \
    code/common/game/bowlergame/messages/bowlerguieventmessage.cpp \
    code/common/game/bowlergame/messages/bowlerguilevelenablemessage.cpp \
    code/common/game/bowlergame/messages/bowlerstatechangemessage.cpp \
    code/common/game/bowlergame/objects/bowlerobject.cpp \
    code/common/game/bowlergame/objects/bowlerskydome.cpp \
    code/common/game/bowlergame/states/bowlerrunstate.cpp \
    code/common/game/bowlergame/states/bowlerselectstate.cpp \
    code/common/game/bowlergame/states/bowlerstartstate.cpp \
    code/common/game/bowlergame/states/bowlerstate.cpp \
    code/common/game/bowlergame/states/bowlerstatemanager.cpp \
    code/common/engine/render/ogles2/oglesblendstate.cpp \
    code/common/engine/render/ogles2/oglesconstantbuffer.cpp \
    code/common/engine/render/ogles2/oglescontext.cpp \
    code/common/engine/render/ogles2/oglesdepthstencilstate.cpp \
    code/common/engine/render/ogles2/oglespixelshader.cpp \
    code/common/engine/render/ogles2/oglesrasterizerstate.cpp \
    code/common/engine/render/ogles2/oglesrenderer.cpp \
    code/common/engine/render/ogles2/oglesrendertarget.cpp \
    code/common/engine/render/ogles2/oglessamplerstate.cpp \
    code/common/engine/render/ogles2/oglesshader.cpp \
    code/common/engine/render/ogles2/oglesshaderprogram.cpp \
    code/common/engine/render/ogles2/oglestexture.cpp \
    code/common/engine/render/ogles2/oglesvertexbuffer.cpp \
    code/common/engine/render/ogles2/oglesvertexdeclaration.cpp \
    code/common/engine/render/ogles2/oglesvertexshader.cpp \
    code/common/engine/render/ogles2/ogleswrapper.cpp \
    sdk/jsoncpp-src-0.6.0-rc2/src/lib_json/json_reader.cpp \
    sdk/jsoncpp-src-0.6.0-rc2/src/lib_json/json_value.cpp \
    sdk/jsoncpp-src-0.6.0-rc2/src/lib_json/json_writer.cpp \
    sdk/tinyxml2/tinyxml2.cpp \

LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -landroid
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -lEGL
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -lGLESv1_CM
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -lGLESv2
LOCAL_LDLIBS += -L$(SYSROOT)/usr/lib -lOpenSLES

LOCAL_STATIC_LIBRARIES := android_native_app_glue bullet_android vorbis_android ogg_android

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)