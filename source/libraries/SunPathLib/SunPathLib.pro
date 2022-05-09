TEMPLATE = lib
#TARGET = SunPathLib # SunPathLib.dll
DEFINES += SUNPATHLIB_EXPORT

QT -= gui
CONFIG += eigen
include(../config.pri)

HEADERS += \
    SunPathLib.h \
    calculators/Location.h \
    calculators/MB/SunPos.h \
    calculators/NREL/spa.h \
    calculators/RG/Sun_position_algorithms.h \
    calculators/SunCalculator.h \
    calculators/SunCalculatorET.h \
    calculators/SunCalculatorMB.h \
    calculators/SunCalculatorNREL.h \
    calculators/SunCalculatorRG.h \
    data/FormatTMY.h \
    data/FormatWSN.h \
    data/SkyModel.h \
    data/SkyModelPI.h \
    data/SunFunctor.h \
    math/geometry/Interval.h \
    math/geometry/vec2d.h \
    math/geometry/vec3d.h \
    math/math.h \
    math/matrices/Matrix.h \
    math/matrices/MatrixNR.h \
    math/sampling/ErrorAnalysis.h \
    math/sampling/Interpolator.h \
    math/sampling/Summator.h \
    samplers/SkySampler.h \
    samplers/SunSpatial.h \
    samplers/SunTemporal.h \
    samplers/TimeSampler.h

SOURCES += \
    calculators/Location.cpp \
    calculators/MB/SunPos.cpp \
    calculators/MB/SunPos2.cpp \
    calculators/NREL/spa.cpp \
    calculators/SunCalculator.cpp \
    calculators/SunCalculatorET.cpp \
    calculators/SunCalculatorMB.cpp \
    calculators/SunCalculatorNREL.cpp \
    calculators/SunCalculatorRG.cpp \
    data/FormatTMY.cpp \
    data/FormatWSN.cpp \
    data/SkyModel.cpp \
    data/SkyModelPI.cpp \
    data/SunFunctor.cpp \
    math/geometry/Interval.cpp \
    math/geometry/vec2d.cpp \
    math/geometry/vec3d.cpp \
    math/math.cpp \
    math/matrices/MatrixNR.cpp \
    math/sampling/ErrorAnalysis.cpp \
    math/sampling/Interpolator.cpp \
    math/sampling/Summator.cpp \
    samplers/SkySampler.cpp \
    samplers/SunSpatial.cpp \
    samplers/SunTemporal.cpp \
    samplers/TimeSampler.cpp


# Build Settings
# Add Build Step: Make,
# Make Arguments: install

INSTALL_HEADERS = $$HEADERS
INSTALL_HEADERS -= calculators/MB/SunPos.h
INSTALL_HEADERS -= calculators/NREL/spa.h
INSTALL_HEADERS -= calculators/RG/Sun_position_algorithms.h

for(header, $$list($$INSTALL_HEADERS)) {
    pathRel = $$dirname(header)
    pathAbs = $$INSTALL_PREFIX/include/SunPathLib/$$pathRel
    temp = headers_$$replace(pathRel,/,)
    $${temp}.files += $$header
    $${temp}.path = $$pathAbs
    INSTALLS *= $$temp
}

lib.files += libSunPathLib.a
lib.path = $$INSTALL_PREFIX/lib
INSTALLS *= lib

bin.files += SunPathLib.dll
bin.path = $$INSTALL_PREFIX/bin
INSTALLS += bin
