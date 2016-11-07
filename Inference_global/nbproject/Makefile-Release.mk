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
	${OBJECTDIR}/_ext/1902552228/BehaviourSequence.o \
	${OBJECTDIR}/_ext/1902552228/ClusterData.o \
	${OBJECTDIR}/_ext/1902552228/Commons.o \
	${OBJECTDIR}/_ext/1902552228/KMeans.o \
	${OBJECTDIR}/_ext/1902552228/config.o \
	${OBJECTDIR}/ClusterDistribution.o \
	${OBJECTDIR}/InferenceClassInterface.o \
	${OBJECTDIR}/KFoldCrossValidation.o \
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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/inference_global

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/inference_global: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/inference_global ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/_ext/1902552228/BehaviourSequence.o: /home/olmin/NetBeansProjects/Clustering/BehaviourSequence.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1902552228
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1902552228/BehaviourSequence.o /home/olmin/NetBeansProjects/Clustering/BehaviourSequence.cpp

${OBJECTDIR}/_ext/1902552228/ClusterData.o: /home/olmin/NetBeansProjects/Clustering/ClusterData.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1902552228
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1902552228/ClusterData.o /home/olmin/NetBeansProjects/Clustering/ClusterData.cpp

${OBJECTDIR}/_ext/1902552228/Commons.o: /home/olmin/NetBeansProjects/Clustering/Commons.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1902552228
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1902552228/Commons.o /home/olmin/NetBeansProjects/Clustering/Commons.cpp

${OBJECTDIR}/_ext/1902552228/KMeans.o: /home/olmin/NetBeansProjects/Clustering/KMeans.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1902552228
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1902552228/KMeans.o /home/olmin/NetBeansProjects/Clustering/KMeans.cpp

${OBJECTDIR}/_ext/1902552228/config.o: /home/olmin/NetBeansProjects/Clustering/config.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1902552228
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1902552228/config.o /home/olmin/NetBeansProjects/Clustering/config.cpp

${OBJECTDIR}/ClusterDistribution.o: ClusterDistribution.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/ClusterDistribution.o ClusterDistribution.cpp

${OBJECTDIR}/InferenceClassInterface.o: InferenceClassInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/InferenceClassInterface.o InferenceClassInterface.cpp

${OBJECTDIR}/KFoldCrossValidation.o: KFoldCrossValidation.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/KFoldCrossValidation.o KFoldCrossValidation.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/inference_global

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
