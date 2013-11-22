LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := CodePort_Hulcy
LOCAL_SRC_FILES := CodePort_Hulcy.cpp 	\
GameApp.cpp								\
./CoreEngine/Assertion.cpp 				\
./CoreEngine/Clock.cpp					\
./CoreEngine/Colors.cpp					\
./CoreEngine/ConsoleInterface.cpp		\
./CoreEngine/EventSystem.cpp			\
./CoreEngine/EventSystemInterface.cpp	\
./CoreEngine/FileManager.cpp			\
./CoreEngine/InputListener.cpp			\
./CoreEngine/InputSystem.cpp			\
./CoreEngine/Logging.cpp				\
./CoreEngine/MonkyException.cpp			\
./CoreEngine/NamedProperties.cpp		\
./CoreEngine/ProfileData.cpp			\
./CoreEngine/ProfileSection.cpp			\
./CoreEngine/ProfileSystem.cpp			\
./CoreEngine/StringUtils.cpp			\
./CoreEngine/Timer.cpp					\
./CoreEngine/TimeUtils.cpp				\
./CoreEngine/tinyxml2.cpp				\
./CoreEngine/XMLParser.cpp				\
./MathLib/RandNumGen.cpp				\
./MathLib/RandRangeFloat.cpp			\
./MathLib/RandRangeInt.cpp				\
./MonkyRenderer/Buffer.cpp				\
./MonkyRenderer/BufferLayout.cpp		\
./MonkyRenderer/Camera.cpp				\
./MonkyRenderer/Font.cpp				\
./MonkyRenderer/FrameBufferObject.cpp	\
./MonkyRenderer/GLBuffer.cpp			\
./MonkyRenderer/glHelper.cpp			\
./MonkyRenderer/Material.cpp			\
./MonkyRenderer/Mesh.cpp				\
./MonkyRenderer/MeshFactory.cpp			\
./MonkyRenderer/Node.cpp				\
./MonkyRenderer/Object.cpp				\
./MonkyRenderer/Renderer.cpp			\
./MonkyRenderer/Shader.cpp				\
./MonkyRenderer/ShaderProgram.cpp		\
./MonkyRenderer/stb_image.c				\
./MonkyRenderer/Texture.cpp				\
./MonkyRenderer/VtxBufferAccessor.cpp	\
./MonkyRenderer/Actor.cpp				\
./MonkyRenderer/ActorManager.cpp		\
./MonkyRenderer/SpriteAnimation.cpp		\
./MonkyRenderer/SpriteActor.cpp



LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2
LOCAL_STATIC_LIBRARIES := android_native_app_glue
LOCAL_CFILES += -c++11 

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)
