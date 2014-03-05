QT += widgets

HEADERS += \
        Connector.h \
    MaterialGraphWidget.h \
    Node.h \
    DeleteCommand.h \
    mainwindow.h \
    MoveCommand.h \
    AddLinkCommand.h \
    AddNodeCommand.h \
    BreakLinkCommand.h \
    Link.h \
    NodeType.h \
    XMLNodeGenerator.h \
    CoreEngine/InputSystem.h \
    CoreEngine/InputListener.h \
    CoreEngine/FileManager.h \
    CoreEngine/EventSystemInterface.h \
    CoreEngine/EventSystem.h \
    CoreEngine/EventSubscriberBase.h \
    CoreEngine/EventSubscriber.h \
    CoreEngine/ConsoleInterface.h \
    CoreEngine/CommonCoreEngineIncludes.h \
    CoreEngine/Colors.h \
    CoreEngine/Clock.h \
    CoreEngine/Assertion.h \
    CoreEngine/XMLParser.h \
    CoreEngine/Uncopyable.h \
    CoreEngine/TypedDataBase.h \
    CoreEngine/TypedData.h \
    CoreEngine/tinyxml2.h \
    CoreEngine/TimeUtils.h \
    CoreEngine/Timer.h \
    CoreEngine/StringUtils.h \
    CoreEngine/StdLibraryIncludes.h \
    CoreEngine/StartupCommandsParser.h \
    CoreEngine/Singleton.h \
    CoreEngine/ProfileSystemInterface.h \
    CoreEngine/ProfileSystem.h \
    CoreEngine/ProfileSection.h \
    CoreEngine/ProfileData.h \
    CoreEngine/NamedProperties.h \
    CoreEngine/MonkyMouse.h \
    CoreEngine/MonkyKeys.h \
    CoreEngine/MonkyException.h \
    CoreEngine/Logging.h \
    CoreEngine/StringUtils.inl \
    MathLib/AABB2.h \
    MathLib/CommonTypeDefines.h \
    MathLib/FixedPoint.h \
    MathLib/FixedPointMath.h \
    MathLib/Line.h \
    MathLib/LineSegment.h \
    MathLib/MathFuncs.h \
    MathLib/Matrix2.h \
    MathLib/Matrix3.h \
    MathLib/Matrix4.h \
    MathLib/MatrixStack.h \
    MathLib/Plane.h \
    MathLib/Quaternion.h \
    MathLib/RandItemPool.h \
    MathLib/RandNumGen.h \
    MathLib/RandRangeFloat.h \
    MathLib/RandRangeInt.h \
    MathLib/Ray.h \
    MathLib/StateSpaceNumericalMethods.h \
    MathLib/Vector2.h \
    MathLib/Vector3.h \
    MathLib/Vector4.h \
    MathLib/VectorUtils.h \
    MathLib/WeightedRandPool.h \
    MathLib/Matrix4.inl \
    MathLib/MatrixStack.inl \
    MathLib/Quaternion.inl \
    MathLib/StateSpaceNumericalMethods.inl \
    MathLib/Vector2.inl \
    MathLib/Vector3.inl \
    MathLib/Vector4.inl \
    MathLib/VectorUtils.inl \
    InputXMLNodeGenerator.h \
    VariableXMLNodeGenerator.h \
    TextureSampleXMLNodeGenerator.h \
    BreakAllLinksCommand.h \
    GraphLayoutParser.h \
    GraphLayoutGenerator.h


SOURCES += \
        main.cpp \
        Connector.cpp \
    MaterialGraphWidget.cpp \
    Node.cpp \
    mainwindow.cpp \
    DeleteCommand.cpp \
    MoveCommand.cpp \
    AddNodeCommand.cpp \
    AddLinkCommand.cpp \
    Link.cpp \
    BreakLinkCommand.cpp \
    NodeType.cpp \
    CoreEngine/XMLParser.cpp \
    CoreEngine/tinyxml2.cpp \
    CoreEngine/TimeUtils.cpp \
    CoreEngine/Timer.cpp \
    CoreEngine/StringUtils.cpp \
    CoreEngine/StartupCommandsParser.cpp \
    CoreEngine/ProfileSystem.cpp \
    CoreEngine/ProfileSection.cpp \
    CoreEngine/ProfileData.cpp \
    CoreEngine/NamedProperties.cpp \
    CoreEngine/MonkyException.cpp \
    CoreEngine/Logging.cpp \
    CoreEngine/InputSystem.cpp \
    CoreEngine/InputListener.cpp \
    CoreEngine/FileManager.cpp \
    CoreEngine/EventSystemInterface.cpp \
    CoreEngine/EventSystem.cpp \
    CoreEngine/ConsoleInterface.cpp \
    CoreEngine/Colors.cpp \
    CoreEngine/Clock.cpp \
    CoreEngine/Assertion.cpp \
    MathLib/FixedPoint.cpp \
    MathLib/FixedPointMath.cpp \
    MathLib/RandNumGen.cpp \
    MathLib/RandRangeFloat.cpp \
    MathLib/RandRangeInt.cpp \
    XMLNodeGenerator.cpp \
    InputXMLNodeGenerator.cpp \
    VariableXMLNodeGenerator.cpp \
    TextureSampleXMLNodeGenerator.cpp \
    BreakAllLinksCommand.cpp \
    GraphLayoutParser.cpp \
    GraphLayoutGenerator.cpp


CONFIG += c++11
CONFIG += rtti

# install
target.path = $$./
INSTALLS += target
