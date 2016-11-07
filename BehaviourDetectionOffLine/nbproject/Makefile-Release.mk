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
	${OBJECTDIR}/_ext/1760650638/DTW.o \
	${OBJECTDIR}/_ext/1760650638/DTWInference.o \
	${OBJECTDIR}/_ext/1760647021/HMM.o \
	${OBJECTDIR}/_ext/1529386596/OlmoAutomata.o \
	${OBJECTDIR}/_ext/377540840/ClusterDistribution.o \
	${OBJECTDIR}/_ext/377540840/InferenceClassInterface.o \
	${OBJECTDIR}/_ext/377540840/KFoldCrossValidation.o \
	${OBJECTDIR}/_ext/1902552228/BehaviourSequence.o \
	${OBJECTDIR}/_ext/1902552228/ClusterData.o \
	${OBJECTDIR}/_ext/1902552228/Commons.o \
	${OBJECTDIR}/_ext/1902552228/KMeans.o \
	${OBJECTDIR}/_ext/1902552228/PAA.o \
	${OBJECTDIR}/_ext/1902552228/SimpleSkeleton.o \
	${OBJECTDIR}/_ext/1902552228/config.o \
	${OBJECTDIR}/main.o


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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/behaviourdetectionoffline

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/behaviourdetectionoffline: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/behaviourdetectionoffline ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/1760650638/DTW.o: ../Inference_DTW/DTW.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1760650638
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1760650638/DTW.o ../Inference_DTW/DTW.cpp

${OBJECTDIR}/_ext/1760650638/DTWInference.o: ../Inference_DTW/DTWInference.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1760650638
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1760650638/DTWInference.o ../Inference_DTW/DTWInference.cpp

${OBJECTDIR}/_ext/1760647021/HMM.o: ../Inference_HMM/HMM.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1760647021
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1760647021/HMM.o ../Inference_HMM/HMM.cpp

${OBJECTDIR}/_ext/1529386596/OlmoAutomata.o: ../Inference_OlmoAutomata/OlmoAutomata.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1529386596
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1529386596/OlmoAutomata.o ../Inference_OlmoAutomata/OlmoAutomata.cpp

${OBJECTDIR}/_ext/377540840/ClusterDistribution.o: ../Inference_global/ClusterDistribution.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/377540840
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/377540840/ClusterDistribution.o ../Inference_global/ClusterDistribution.cpp

${OBJECTDIR}/_ext/377540840/InferenceClassInterface.o: ../Inference_global/InferenceClassInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/377540840
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/377540840/InferenceClassInterface.o ../Inference_global/InferenceClassInterface.cpp

${OBJECTDIR}/_ext/377540840/KFoldCrossValidation.o: ../Inference_global/KFoldCrossValidation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/377540840
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/377540840/KFoldCrossValidation.o ../Inference_global/KFoldCrossValidation.cpp

${OBJECTDIR}/_ext/1902552228/BehaviourSequence.o: /home/olmin/NetBeansProjects/Clustering/BehaviourSequence.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1902552228
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1902552228/BehaviourSequence.o /home/olmin/NetBeansProjects/Clustering/BehaviourSequence.cpp

${OBJECTDIR}/_ext/1902552228/ClusterData.o: /home/olmin/NetBeansProjects/Clustering/ClusterData.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1902552228
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1902552228/ClusterData.o /home/olmin/NetBeansProjects/Clustering/ClusterData.cpp

${OBJECTDIR}/_ext/1902552228/Commons.o: /home/olmin/NetBeansProjects/Clustering/Commons.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1902552228
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1902552228/Commons.o /home/olmin/NetBeansProjects/Clustering/Commons.cpp

${OBJECTDIR}/_ext/1902552228/KMeans.o: /home/olmin/NetBeansProjects/Clustering/KMeans.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1902552228
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1902552228/KMeans.o /home/olmin/NetBeansProjects/Clustering/KMeans.cpp

${OBJECTDIR}/_ext/1902552228/PAA.o: /home/olmin/NetBeansProjects/Clustering/PAA.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1902552228
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1902552228/PAA.o /home/olmin/NetBeansProjects/Clustering/PAA.cpp

${OBJECTDIR}/_ext/1902552228/SimpleSkeleton.o: /home/olmin/NetBeansProjects/Clustering/SimpleSkeleton.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1902552228
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1902552228/SimpleSkeleton.o /home/olmin/NetBeansProjects/Clustering/SimpleSkeleton.cpp

${OBJECTDIR}/_ext/1902552228/config.o: /home/olmin/NetBeansProjects/Clustering/config.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1902552228
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1902552228/config.o /home/olmin/NetBeansProjects/Clustering/config.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/behaviourdetectionoffline

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
