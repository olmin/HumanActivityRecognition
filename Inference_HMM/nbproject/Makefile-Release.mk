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
	${OBJECTDIR}/_ext/412621687/BehaviourSequence.o \
	${OBJECTDIR}/_ext/412621687/ClusterData.o \
	${OBJECTDIR}/_ext/412621687/Commons.o \
	${OBJECTDIR}/_ext/412621687/KMeans.o \
	${OBJECTDIR}/_ext/412621687/PAA.o \
	${OBJECTDIR}/_ext/412621687/SimpleSkeleton.o \
	${OBJECTDIR}/_ext/412621687/config.o \
	${OBJECTDIR}/_ext/377540840/ClusterDistribution.o \
	${OBJECTDIR}/_ext/377540840/InferenceClassInterface.o \
	${OBJECTDIR}/_ext/377540840/KFoldCrossValidation.o \
	${OBJECTDIR}/_ext/377540840/main.o \
	${OBJECTDIR}/HMM.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/inference_hmm

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/inference_hmm: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/inference_hmm ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/412621687/BehaviourSequence.o: ../Clustering/BehaviourSequence.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/412621687/BehaviourSequence.o ../Clustering/BehaviourSequence.cpp

${OBJECTDIR}/_ext/412621687/ClusterData.o: ../Clustering/ClusterData.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/412621687/ClusterData.o ../Clustering/ClusterData.cpp

${OBJECTDIR}/_ext/412621687/Commons.o: ../Clustering/Commons.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/412621687/Commons.o ../Clustering/Commons.cpp

${OBJECTDIR}/_ext/412621687/KMeans.o: ../Clustering/KMeans.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/412621687/KMeans.o ../Clustering/KMeans.cpp

${OBJECTDIR}/_ext/412621687/PAA.o: ../Clustering/PAA.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/412621687/PAA.o ../Clustering/PAA.cpp

${OBJECTDIR}/_ext/412621687/SimpleSkeleton.o: ../Clustering/SimpleSkeleton.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/412621687/SimpleSkeleton.o ../Clustering/SimpleSkeleton.cpp

${OBJECTDIR}/_ext/412621687/config.o: ../Clustering/config.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/412621687/config.o ../Clustering/config.cpp

${OBJECTDIR}/_ext/377540840/ClusterDistribution.o: ../Inference_global/ClusterDistribution.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/377540840
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/377540840/ClusterDistribution.o ../Inference_global/ClusterDistribution.cpp

${OBJECTDIR}/_ext/377540840/InferenceClassInterface.o: ../Inference_global/InferenceClassInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/377540840
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/377540840/InferenceClassInterface.o ../Inference_global/InferenceClassInterface.cpp

${OBJECTDIR}/_ext/377540840/KFoldCrossValidation.o: ../Inference_global/KFoldCrossValidation.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/377540840
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/377540840/KFoldCrossValidation.o ../Inference_global/KFoldCrossValidation.cpp

${OBJECTDIR}/_ext/377540840/main.o: ../Inference_global/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/377540840
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/377540840/main.o ../Inference_global/main.cpp

${OBJECTDIR}/HMM.o: HMM.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/HMM.o HMM.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/inference_hmm

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
