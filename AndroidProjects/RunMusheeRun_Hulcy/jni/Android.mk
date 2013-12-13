LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := RunMusheeRun_Hulcy
LOCAL_SRC_FILES := RunMusheeRun_Hulcy.cpp	\
./Game/GameApp.cpp						\
./Game/MSGame.cpp						\
./Game/TiledMap.cpp						\
./Game/TileSet.cpp						\
./Game/MSCamera.cpp						\
./Game/MSMap.cpp						\
./Game/Button.cpp						\
./Game/SpriteActor.cpp			\
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
./CoreEngine/Timer.cpp					\
./CoreEngine/TimeUtils.cpp				\
./CoreEngine/tinyxml2.cpp				\
./CoreEngine/XMLParser.cpp				\
./CoreEngine/Util/StringUtils.cpp		\
./CoreEngine/Util/Base64.cpp			\
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


LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv2
LOCAL_STATIC_LIBRARIES := android_native_app_glue
LOCAL_CFILES += -c++11 

include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)