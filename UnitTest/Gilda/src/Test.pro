TEMPLATE = app
TARGET =  
CONFIG       += debug_and_release
CONFIG	-= app_bundle

DEPENDPATH += . \ 
			src
INCLUDEPATH += . \
				src \
				../../TonatiuhProject/Tonatiuh/src \ 
				$$(TDE_ROOT)/local/include 


win32	{
	DEFINES += COIN_DLL SOQT_DLL
		INCLUDEPATH += . \
				src \
				../../TonatiuhProject/Tonatiuh/src \  
				$$(TDE_ROOT)/local/include 
	
	LIBS += -L"$$(TDE_ROOT)/local/lib" -lqxcppunit -lqxrunner
	LIBS += -L"$$(TDE_ROOT)/local/lib" -lcppunit
	LIBS += -L"$$(TDE_ROOT)/local/lib" -lSoQt -lCoin
}

unix {
	INCLUDEPATH += 	src \
				../../TonatiuhProject/Tonatiuh/src \  
				/usr/local/include 

	LIBS +=-L/usr/local/lib  -lSoQt -lCoin -lqxcppunit -lqxrunner -lcppunit
	
}

# Input
HEADERS += *.h \
			../../TonatiuhProject/Tonatiuh/src/BBox.h \
			../../TonatiuhProject/Tonatiuh/src/DifferentialGeometry.h \
			../../TonatiuhProject/Tonatiuh/src/Matrix4x4.h \
			../../TonatiuhProject/Tonatiuh/src/NormalVector.h \
			../../TonatiuhProject/Tonatiuh/src/Point3D.h \
			../../TonatiuhProject/Tonatiuh/src/Ray.h \
			../../TonatiuhProject/Tonatiuh/src/RefCount.h \
			../../TonatiuhProject/Tonatiuh/src/Trace.h \
			../../TonatiuhProject/Tonatiuh/src/Transform.cpp \
			../../TonatiuhProject/Tonatiuh/src/Vector3D.h
		
SOURCES += *.cpp \
			../../TonatiuhProject/Tonatiuh/src/BBox.cpp \
			../../TonatiuhProject/Tonatiuh/src/DifferentialGeometry.cpp \
			../../TonatiuhProject/Tonatiuh/src/Matrix4x4.cpp \
			../../TonatiuhProject/Tonatiuh/src/NormalVector.cpp \
			../../TonatiuhProject/Tonatiuh/src/Point3D.cpp \
			../../TonatiuhProject/Tonatiuh/src/Ray.cpp \
			../../TonatiuhProject/Tonatiuh/src/RefCount.cpp \
			../../TonatiuhProject/Tonatiuh/src/Trace.cpp \
			../../TonatiuhProject/Tonatiuh/src/Transform.cpp \
			../../TonatiuhProject/Tonatiuh/src/Vector3D.cpp 
    
CONFIG(debug, debug|release) {
        DESTDIR = debug/
    }
else {
        DESTDIR  = release/
}
