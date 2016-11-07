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
	${OBJECTDIR}/DTW.o \
	${OBJECTDIR}/DTWInference.o \
	${OBJECTDIR}/Skeleton.o \
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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/inference_dtw_rawdata

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/inference_dtw_rawdata: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/inference_dtw_rawdata ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/412621687/BehaviourSequence.o: ../Clustering/BehaviourSequence.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/412621687/BehaviourSequence.o ../Clustering/BehaviourSequence.cpp

${OBJECTDIR}/_ext/412621687/ClusterData.o: ../Clustering/ClusterData.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/412621687/ClusterData.o ../Clustering/ClusterData.cpp

${OBJECTDIR}/_ext/412621687/Commons.o: ../Clustering/Commons.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/412621687/Commons.o ../Clustering/Commons.cpp

${OBJECTDIR}/_ext/412621687/KMeans.o: ../Clustering/KMeans.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/412621687/KMeans.o ../Clustering/KMeans.cpp

${OBJECTDIR}/_ext/412621687/PAA.o: ../Clustering/PAA.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/412621687/PAA.o ../Clustering/PAA.cpp

${OBJECTDIR}/_ext/412621687/SimpleSkeleton.o: ../Clustering/SimpleSkeleton.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/412621687/SimpleSkeleton.o ../Clustering/SimpleSkeleton.cpp

${OBJECTDIR}/_ext/412621687/config.o: ../Clustering/config.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/412621687
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/412621687/config.o ../Clustering/config.cpp

${OBJECTDIR}/DTW.o: DTW.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/DTW.o DTW.cpp

${OBJECTDIR}/DTWInference.o: DTWInference.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/DTWInference.o DTWInference.cpp

${OBJECTDIR}/Skeleton.o: Skeleton.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/Skeleton.o Skeleton.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/inference_dtw_rawdata

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
