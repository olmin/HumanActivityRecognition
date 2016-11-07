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
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/behaviour.o \
	${OBJECTDIR}/MySQLConnector.o \
	${OBJECTDIR}/controlBehaviour.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-pthread
CXXFLAGS=-pthread

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lnestk -lmysqlcppconn `pkg-config --libs QtCore` `pkg-config --libs QtGui` `pkg-config --libs QtNetwork` `pkg-config --libs opencv` `pkg-config --libs openni-dev` `pkg-config --libs pcl_common-1.6` `pkg-config --libs pcl_features-1.6` `pkg-config --libs pcl_filters-1.6` `pkg-config --libs pcl_io-1.6` `pkg-config --libs pcl_kdtree-1.6` `pkg-config --libs pcl_keypoints-1.6` `pkg-config --libs pcl_octree-1.6` `pkg-config --libs pcl_registration-1.6` `pkg-config --libs pcl_sample_consensus-1.6` `pkg-config --libs pcl_segmentation-1.6` `pkg-config --libs pcl_surface-1.6` `pkg-config --libs pcl_visualization-1.6` `pkg-config --libs flann` -lflann_cpp -lflann_cpp_s -lXnCore -lXnDDK -lXnDeviceFile -lXnDeviceSensorV2 -lXnVCNITE_1_5_2 -lXnVFeatures_1_5_2 -lXnVHandGenerator_1_5_2 -lXnVNite_1_5_2 `pkg-config --libs libfreenect`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/controlBehaviour

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/controlBehaviour: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/controlBehaviour ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/behaviour.o: behaviour.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/local/include -I/usr/include/nite `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork` `pkg-config --cflags opencv` `pkg-config --cflags openni-dev` `pkg-config --cflags pcl_common-1.6` `pkg-config --cflags pcl_features-1.6` `pkg-config --cflags pcl_filters-1.6` `pkg-config --cflags pcl_io-1.6` `pkg-config --cflags pcl_kdtree-1.6` `pkg-config --cflags pcl_keypoints-1.6` `pkg-config --cflags pcl_octree-1.6` `pkg-config --cflags pcl_registration-1.6` `pkg-config --cflags pcl_sample_consensus-1.6` `pkg-config --cflags pcl_segmentation-1.6` `pkg-config --cflags pcl_surface-1.6` `pkg-config --cflags pcl_visualization-1.6` `pkg-config --cflags flann` `pkg-config --cflags libfreenect`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/behaviour.o behaviour.cpp

${OBJECTDIR}/MySQLConnector.o: MySQLConnector.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/local/include -I/usr/include/nite `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork` `pkg-config --cflags opencv` `pkg-config --cflags openni-dev` `pkg-config --cflags pcl_common-1.6` `pkg-config --cflags pcl_features-1.6` `pkg-config --cflags pcl_filters-1.6` `pkg-config --cflags pcl_io-1.6` `pkg-config --cflags pcl_kdtree-1.6` `pkg-config --cflags pcl_keypoints-1.6` `pkg-config --cflags pcl_octree-1.6` `pkg-config --cflags pcl_registration-1.6` `pkg-config --cflags pcl_sample_consensus-1.6` `pkg-config --cflags pcl_segmentation-1.6` `pkg-config --cflags pcl_surface-1.6` `pkg-config --cflags pcl_visualization-1.6` `pkg-config --cflags flann` `pkg-config --cflags libfreenect`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/MySQLConnector.o MySQLConnector.cpp

${OBJECTDIR}/controlBehaviour.o: controlBehaviour.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/local/include -I/usr/include/nite `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork` `pkg-config --cflags opencv` `pkg-config --cflags openni-dev` `pkg-config --cflags pcl_common-1.6` `pkg-config --cflags pcl_features-1.6` `pkg-config --cflags pcl_filters-1.6` `pkg-config --cflags pcl_io-1.6` `pkg-config --cflags pcl_kdtree-1.6` `pkg-config --cflags pcl_keypoints-1.6` `pkg-config --cflags pcl_octree-1.6` `pkg-config --cflags pcl_registration-1.6` `pkg-config --cflags pcl_sample_consensus-1.6` `pkg-config --cflags pcl_segmentation-1.6` `pkg-config --cflags pcl_surface-1.6` `pkg-config --cflags pcl_visualization-1.6` `pkg-config --cflags flann` `pkg-config --cflags libfreenect`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/controlBehaviour.o controlBehaviour.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/controlBehaviour

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
