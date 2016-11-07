#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/412621687/ClusterData.o \
	${OBJECTDIR}/_ext/412621687/PAA.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/_ext/412621687/BehaviourSequence.o \
	${OBJECTDIR}/_ext/2027473190/OlmoAutomata.o \
	${OBJECTDIR}/_ext/412621687/Commons.o \
	${OBJECTDIR}/BehaviourDetection.o \
	${OBJECTDIR}/_ext/412621687/KMeans.o \
	${OBJECTDIR}/_ext/412621687/config.o \
	${OBJECTDIR}/_ext/1387288982/ClusterDistribution.o \
	${OBJECTDIR}/_ext/1387288982/HMM.o \
	${OBJECTDIR}/_ext/412621687/SimpleSkeleton.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lmysqlcppconn -lnestk `pkg-config --libs QtCore` `pkg-config --libs QtGui` `pkg-config --libs QtNetwork` -lnimMockNodes -lnimRecorder -lXnDevicesSensorV2 -lnimCodecs -lXnDeviceFile -lXnVFeatures -lXnVHandGenerator -lboost_date_time -lflann_cpp_s -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_video  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/behavioursdetection

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/behavioursdetection: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/behavioursdetection ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/412621687/ClusterData.o: ../Clustering/ClusterData.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/local/include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps/openni/Nite/Include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps -I/home/olmin/kinect/OpenCV-2.3.0/include `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/412621687/ClusterData.o ../Clustering/ClusterData.cpp

${OBJECTDIR}/_ext/412621687/PAA.o: ../Clustering/PAA.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/local/include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps/openni/Nite/Include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps -I/home/olmin/kinect/OpenCV-2.3.0/include `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/412621687/PAA.o ../Clustering/PAA.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/local/include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps/openni/Nite/Include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps -I/home/olmin/kinect/OpenCV-2.3.0/include `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/_ext/412621687/BehaviourSequence.o: ../Clustering/BehaviourSequence.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/local/include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps/openni/Nite/Include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps -I/home/olmin/kinect/OpenCV-2.3.0/include `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/412621687/BehaviourSequence.o ../Clustering/BehaviourSequence.cpp

${OBJECTDIR}/_ext/2027473190/OlmoAutomata.o: ../BehaviourInferenceOlmoAutomata/OlmoAutomata.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2027473190
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/local/include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps/openni/Nite/Include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps -I/home/olmin/kinect/OpenCV-2.3.0/include `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2027473190/OlmoAutomata.o ../BehaviourInferenceOlmoAutomata/OlmoAutomata.cpp

${OBJECTDIR}/_ext/412621687/Commons.o: ../Clustering/Commons.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/local/include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps/openni/Nite/Include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps -I/home/olmin/kinect/OpenCV-2.3.0/include `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/412621687/Commons.o ../Clustering/Commons.cpp

${OBJECTDIR}/BehaviourDetection.o: BehaviourDetection.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/local/include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps/openni/Nite/Include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps -I/home/olmin/kinect/OpenCV-2.3.0/include `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/BehaviourDetection.o BehaviourDetection.cpp

${OBJECTDIR}/_ext/412621687/KMeans.o: ../Clustering/KMeans.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/local/include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps/openni/Nite/Include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps -I/home/olmin/kinect/OpenCV-2.3.0/include `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/412621687/KMeans.o ../Clustering/KMeans.cpp

${OBJECTDIR}/_ext/412621687/config.o: ../Clustering/config.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/local/include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps/openni/Nite/Include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps -I/home/olmin/kinect/OpenCV-2.3.0/include `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/412621687/config.o ../Clustering/config.cpp

${OBJECTDIR}/_ext/1387288982/ClusterDistribution.o: ../Inference/ClusterDistribution.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1387288982
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/local/include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps/openni/Nite/Include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps -I/home/olmin/kinect/OpenCV-2.3.0/include `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1387288982/ClusterDistribution.o ../Inference/ClusterDistribution.cpp

${OBJECTDIR}/_ext/1387288982/HMM.o: ../Inference/HMM.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1387288982
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/local/include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps/openni/Nite/Include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps -I/home/olmin/kinect/OpenCV-2.3.0/include `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1387288982/HMM.o ../Inference/HMM.cpp

${OBJECTDIR}/_ext/412621687/SimpleSkeleton.o: ../Clustering/SimpleSkeleton.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} $@.d
	$(COMPILE.cc) -O2 -I/usr/local/include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps/openni/Nite/Include -I/home/olmin/kinect/RGBDemo-0.6.1-Source/nestk/deps -I/home/olmin/kinect/OpenCV-2.3.0/include `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/412621687/SimpleSkeleton.o ../Clustering/SimpleSkeleton.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/behavioursdetection

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
