######################################################################
# Automatically generated by qmake (2.00a) Sat 30. Dec 18:36:32 2006
######################################################################

TEMPLATE = app
TARGET += 
DEPENDPATH += . fmod release sha1 fmod\api\inc
INCLUDEPATH += .

# Input
HEADERS += config.h \
           playback.h \
           recorder.h \
           vumeter.h \
           fmod/BladeMP3EncDLL.h \
           fmod/fmod.h \
           sha1/sha1.h \
           fmod/api/inc/fmod.h \
           fmod/api/inc/fmod.hpp \
           fmod/api/inc/fmod_codec.h \
           fmod/api/inc/fmod_dsp.h \
           fmod/api/inc/fmod_errors.h \
           fmod/api/inc/fmod_output.h
SOURCES += config.cpp \
           main.cpp \
           playback.cpp \
           recorder.cpp \
           vumeter.cpp \
           fmod/fmod.cpp \
           sha1/sha1.cpp
RESOURCES += voice_std.qrc 
RC_FILE = appIcon.qrc
