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
LDLIBSOPTIONS=-lnestk -lmysqlcppconn `pkg-config --libs QtCore` `pkg-config --libs QtGui` `pkg-config --libs QtNetwork` `pkg-config --libs opencv` `pkg-config --libs openni-dev` `pkg-config --libs pcl_common-1.6` `pkg-config --libs pcl_features-1.6` `pkg-config --libs pcl_filters-1.6` `pkg-config --libs pcl_io-1.6` `pkg-config --libs pcl_kdtree-1.6` `pkg-config --libs pcl_keypoints-1.6` `pkg-config --libs pcl_octree-1.6` `pkg-config --libs pcl_registration-1.6` `pkg-config --libs pcl_sample_consensus-1.6` `pkg-config --libs pcl_segmentation-1.6` `pkg-config --libs pcl_surface-1.6` `pkg-config --libs pcl_visualization-1.6` `pkg-config --libs flann` -lflann_cpp -lflann_cpp_s -lXnCore -lXnDDK -lXnDeviceFile -lXnDeviceSensorV2 -lXnVCNITE_1_5_2 -lXnVFeatures_1_5_2 -lXnVHandGenerator_1_5_2 -lXnVNite_1_5_2 `pkg-config --libs libfreenect`  

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/behavioursdetection

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/behavioursdetection: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/behavioursdetection ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/412621687/ClusterData.o: ../Clustering/ClusterData.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/local/include -I/usr/include/nite `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork` `pkg-config --cflags opencv` `pkg-config --cflags openni-dev` `pkg-config --cflags pcl_common-1.6` `pkg-config --cflags pcl_features-1.6` `pkg-config --cflags pcl_filters-1.6` `pkg-config --cflags pcl_io-1.6` `pkg-config --cflags pcl_kdtree-1.6` `pkg-config --cflags pcl_keypoints-1.6` `pkg-config --cflags pcl_octree-1.6` `pkg-config --cflags pcl_registration-1.6` `pkg-config --cflags pcl_sample_consensus-1.6` `pkg-config --cflags pcl_segmentation-1.6` `pkg-config --cflags pcl_surface-1.6` `pkg-config --cflags pcl_visualization-1.6` `pkg-config --cflags flann` `pkg-config --cflags libfreenect`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/412621687/ClusterData.o ../Clustering/ClusterData.cpp

${OBJECTDIR}/_ext/412621687/PAA.o: ../Clustering/PAA.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/local/include -I/usr/include/nite `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork` `pkg-config --cflags opencv` `pkg-config --cflags openni-dev` `pkg-config --cflags pcl_common-1.6` `pkg-config --cflags pcl_features-1.6` `pkg-config --cflags pcl_filters-1.6` `pkg-config --cflags pcl_io-1.6` `pkg-config --cflags pcl_kdtree-1.6` `pkg-config --cflags pcl_keypoints-1.6` `pkg-config --cflags pcl_octree-1.6` `pkg-config --cflags pcl_registration-1.6` `pkg-config --cflags pcl_sample_consensus-1.6` `pkg-config --cflags pcl_segmentation-1.6` `pkg-config --cflags pcl_surface-1.6` `pkg-config --cflags pcl_visualization-1.6` `pkg-config --cflags flann` `pkg-config --cflags libfreenect`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/412621687/PAA.o ../Clustering/PAA.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/local/include -I/usr/include/nite `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork` `pkg-config --cflags opencv` `pkg-config --cflags openni-dev` `pkg-config --cflags pcl_common-1.6` `pkg-config --cflags pcl_features-1.6` `pkg-config --cflags pcl_filters-1.6` `pkg-config --cflags pcl_io-1.6` `pkg-config --cflags pcl_kdtree-1.6` `pkg-config --cflags pcl_keypoints-1.6` `pkg-config --cflags pcl_octree-1.6` `pkg-config --cflags pcl_registration-1.6` `pkg-config --cflags pcl_sample_consensus-1.6` `pkg-config --cflags pcl_segmentation-1.6` `pkg-config --cflags pcl_surface-1.6` `pkg-config --cflags pcl_visualization-1.6` `pkg-config --cflags flann` `pkg-config --cflags libfreenect`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/_ext/412621687/BehaviourSequence.o: ../Clustering/BehaviourSequence.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/local/include -I/usr/include/nite `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork` `pkg-config --cflags opencv` `pkg-config --cflags openni-dev` `pkg-config --cflags pcl_common-1.6` `pkg-config --cflags pcl_features-1.6` `pkg-config --cflags pcl_filters-1.6` `pkg-config --cflags pcl_io-1.6` `pkg-config --cflags pcl_kdtree-1.6` `pkg-config --cflags pcl_keypoints-1.6` `pkg-config --cflags pcl_octree-1.6` `pkg-config --cflags pcl_registration-1.6` `pkg-config --cflags pcl_sample_consensus-1.6` `pkg-config --cflags pcl_segmentation-1.6` `pkg-config --cflags pcl_surface-1.6` `pkg-config --cflags pcl_visualization-1.6` `pkg-config --cflags flann` `pkg-config --cflags libfreenect`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/412621687/BehaviourSequence.o ../Clustering/BehaviourSequence.cpp

${OBJECTDIR}/_ext/2027473190/OlmoAutomata.o: ../BehaviourInferenceOlmoAutomata/OlmoAutomata.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/2027473190
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/local/include -I/usr/include/nite `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork` `pkg-config --cflags opencv` `pkg-config --cflags openni-dev` `pkg-config --cflags pcl_common-1.6` `pkg-config --cflags pcl_features-1.6` `pkg-config --cflags pcl_filters-1.6` `pkg-config --cflags pcl_io-1.6` `pkg-config --cflags pcl_kdtree-1.6` `pkg-config --cflags pcl_keypoints-1.6` `pkg-config --cflags pcl_octree-1.6` `pkg-config --cflags pcl_registration-1.6` `pkg-config --cflags pcl_sample_consensus-1.6` `pkg-config --cflags pcl_segmentation-1.6` `pkg-config --cflags pcl_surface-1.6` `pkg-config --cflags pcl_visualization-1.6` `pkg-config --cflags flann` `pkg-config --cflags libfreenect`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/2027473190/OlmoAutomata.o ../BehaviourInferenceOlmoAutomata/OlmoAutomata.cpp

${OBJECTDIR}/_ext/412621687/Commons.o: ../Clustering/Commons.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/local/include -I/usr/include/nite `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork` `pkg-config --cflags opencv` `pkg-config --cflags openni-dev` `pkg-config --cflags pcl_common-1.6` `pkg-config --cflags pcl_features-1.6` `pkg-config --cflags pcl_filters-1.6` `pkg-config --cflags pcl_io-1.6` `pkg-config --cflags pcl_kdtree-1.6` `pkg-config --cflags pcl_keypoints-1.6` `pkg-config --cflags pcl_octree-1.6` `pkg-config --cflags pcl_registration-1.6` `pkg-config --cflags pcl_sample_consensus-1.6` `pkg-config --cflags pcl_segmentation-1.6` `pkg-config --cflags pcl_surface-1.6` `pkg-config --cflags pcl_visualization-1.6` `pkg-config --cflags flann` `pkg-config --cflags libfreenect`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/412621687/Commons.o ../Clustering/Commons.cpp

${OBJECTDIR}/BehaviourDetection.o: BehaviourDetection.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/local/include -I/usr/include/nite `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork` `pkg-config --cflags opencv` `pkg-config --cflags openni-dev` `pkg-config --cflags pcl_common-1.6` `pkg-config --cflags pcl_features-1.6` `pkg-config --cflags pcl_filters-1.6` `pkg-config --cflags pcl_io-1.6` `pkg-config --cflags pcl_kdtree-1.6` `pkg-config --cflags pcl_keypoints-1.6` `pkg-config --cflags pcl_octree-1.6` `pkg-config --cflags pcl_registration-1.6` `pkg-config --cflags pcl_sample_consensus-1.6` `pkg-config --cflags pcl_segmentation-1.6` `pkg-config --cflags pcl_surface-1.6` `pkg-config --cflags pcl_visualization-1.6` `pkg-config --cflags flann` `pkg-config --cflags libfreenect`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/BehaviourDetection.o BehaviourDetection.cpp

${OBJECTDIR}/_ext/412621687/KMeans.o: ../Clustering/KMeans.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/local/include -I/usr/include/nite `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork` `pkg-config --cflags opencv` `pkg-config --cflags openni-dev` `pkg-config --cflags pcl_common-1.6` `pkg-config --cflags pcl_features-1.6` `pkg-config --cflags pcl_filters-1.6` `pkg-config --cflags pcl_io-1.6` `pkg-config --cflags pcl_kdtree-1.6` `pkg-config --cflags pcl_keypoints-1.6` `pkg-config --cflags pcl_octree-1.6` `pkg-config --cflags pcl_registration-1.6` `pkg-config --cflags pcl_sample_consensus-1.6` `pkg-config --cflags pcl_segmentation-1.6` `pkg-config --cflags pcl_surface-1.6` `pkg-config --cflags pcl_visualization-1.6` `pkg-config --cflags flann` `pkg-config --cflags libfreenect`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/412621687/KMeans.o ../Clustering/KMeans.cpp

${OBJECTDIR}/_ext/412621687/config.o: ../Clustering/config.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/local/include -I/usr/include/nite `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork` `pkg-config --cflags opencv` `pkg-config --cflags openni-dev` `pkg-config --cflags pcl_common-1.6` `pkg-config --cflags pcl_features-1.6` `pkg-config --cflags pcl_filters-1.6` `pkg-config --cflags pcl_io-1.6` `pkg-config --cflags pcl_kdtree-1.6` `pkg-config --cflags pcl_keypoints-1.6` `pkg-config --cflags pcl_octree-1.6` `pkg-config --cflags pcl_registration-1.6` `pkg-config --cflags pcl_sample_consensus-1.6` `pkg-config --cflags pcl_segmentation-1.6` `pkg-config --cflags pcl_surface-1.6` `pkg-config --cflags pcl_visualization-1.6` `pkg-config --cflags flann` `pkg-config --cflags libfreenect`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/412621687/config.o ../Clustering/config.cpp

${OBJECTDIR}/_ext/1387288982/ClusterDistribution.o: ../Inference/ClusterDistribution.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1387288982
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/local/include -I/usr/include/nite `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork` `pkg-config --cflags opencv` `pkg-config --cflags openni-dev` `pkg-config --cflags pcl_common-1.6` `pkg-config --cflags pcl_features-1.6` `pkg-config --cflags pcl_filters-1.6` `pkg-config --cflags pcl_io-1.6` `pkg-config --cflags pcl_kdtree-1.6` `pkg-config --cflags pcl_keypoints-1.6` `pkg-config --cflags pcl_octree-1.6` `pkg-config --cflags pcl_registration-1.6` `pkg-config --cflags pcl_sample_consensus-1.6` `pkg-config --cflags pcl_segmentation-1.6` `pkg-config --cflags pcl_surface-1.6` `pkg-config --cflags pcl_visualization-1.6` `pkg-config --cflags flann` `pkg-config --cflags libfreenect`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1387288982/ClusterDistribution.o ../Inference/ClusterDistribution.cpp

${OBJECTDIR}/_ext/1387288982/HMM.o: ../Inference/HMM.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1387288982
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/local/include -I/usr/include/nite `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork` `pkg-config --cflags opencv` `pkg-config --cflags openni-dev` `pkg-config --cflags pcl_common-1.6` `pkg-config --cflags pcl_features-1.6` `pkg-config --cflags pcl_filters-1.6` `pkg-config --cflags pcl_io-1.6` `pkg-config --cflags pcl_kdtree-1.6` `pkg-config --cflags pcl_keypoints-1.6` `pkg-config --cflags pcl_octree-1.6` `pkg-config --cflags pcl_registration-1.6` `pkg-config --cflags pcl_sample_consensus-1.6` `pkg-config --cflags pcl_segmentation-1.6` `pkg-config --cflags pcl_surface-1.6` `pkg-config --cflags pcl_visualization-1.6` `pkg-config --cflags flann` `pkg-config --cflags libfreenect`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1387288982/HMM.o ../Inference/HMM.cpp

${OBJECTDIR}/_ext/412621687/SimpleSkeleton.o: ../Clustering/SimpleSkeleton.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} $@.d
	$(COMPILE.cc) -g -I/usr/local/include -I/usr/include/nite `pkg-config --cflags QtCore` `pkg-config --cflags QtGui` `pkg-config --cflags QtNetwork` `pkg-config --cflags opencv` `pkg-config --cflags openni-dev` `pkg-config --cflags pcl_common-1.6` `pkg-config --cflags pcl_features-1.6` `pkg-config --cflags pcl_filters-1.6` `pkg-config --cflags pcl_io-1.6` `pkg-config --cflags pcl_kdtree-1.6` `pkg-config --cflags pcl_keypoints-1.6` `pkg-config --cflags pcl_octree-1.6` `pkg-config --cflags pcl_registration-1.6` `pkg-config --cflags pcl_sample_consensus-1.6` `pkg-config --cflags pcl_segmentation-1.6` `pkg-config --cflags pcl_surface-1.6` `pkg-config --cflags pcl_visualization-1.6` `pkg-config --cflags flann` `pkg-config --cflags libfreenect`    -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/412621687/SimpleSkeleton.o ../Clustering/SimpleSkeleton.cpp

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
