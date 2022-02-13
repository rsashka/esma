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
CC=clang
CCC=clang++
CXX=clang++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=CLang-Linux
CND_DLIB_EXT=so
CND_CONF=LLVM
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1e501df/gtest-all.o \
	${OBJECTDIR}/_ext/e16507f5/logger.o \
	${OBJECTDIR}/builtin.o \
	${OBJECTDIR}/context.o \
	${OBJECTDIR}/lexer.o \
	${OBJECTDIR}/lexer.yy.o \
	${OBJECTDIR}/newlang.o \
	${OBJECTDIR}/nlc.o \
	${OBJECTDIR}/object.o \
	${OBJECTDIR}/parser.o \
	${OBJECTDIR}/parser.yy.o \
	${OBJECTDIR}/term.o \
	${OBJECTDIR}/test/eval_test.o \
	${OBJECTDIR}/test/fraction_test.o \
	${OBJECTDIR}/test/lexer_test.o \
	${OBJECTDIR}/test/newlang_test.o \
	${OBJECTDIR}/test/nlc_test.o \
	${OBJECTDIR}/test/object_test.o \
	${OBJECTDIR}/test/parser_test.o \
	${OBJECTDIR}/variable.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=`llvm-config-12 --cppflags` -Wno-trigraphs -Wno-invalid-source-encoding -fno-rtti  -stdlib=libstdc++ 
CXXFLAGS=`llvm-config-12 --cppflags` -Wno-trigraphs -Wno-invalid-source-encoding -fno-rtti  -stdlib=libstdc++ 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=../contrib/PDCurses/x11/libXCurses.a -lXaw -lXmu -lXt -lX11 -lXpm -lclangLex -lclangTooling -lclangFrontendTool -lclangFrontend -lclangSerialization -lclangCodeGen -lclangParse -lclangSema -lclangDriver -lclangStaticAnalyzerFrontend -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore -lclangAnalysis -lclangARCMigrate -lclangRewrite -lclangRewriteFrontend -lclangEdit -lclangAST -lclangLex -lclangBasic -lclang -lLLVM-12 -lstdc++

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/core

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/core: ../contrib/PDCurses/x11/libXCurses.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/core: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/core ${OBJECTFILES} ${LDLIBSOPTIONS} `llvm-config-12 --libs --ldflags` -fno-rtti  -fuse-ld=lld-12 -stdlib=libstdc++

${OBJECTDIR}/_ext/1e501df/gtest-all.o: ../contrib/googletest/googletest/src/gtest-all.cc
	${MKDIR} -p ${OBJECTDIR}/_ext/1e501df
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -DLOG_LEVEL_NORMAL=LOG_LEVEL_DEBUG -DPDC_WIDE -I/usr/lib/llvm-12/lib/clang/12.0.1/include -I/usr/include/x86_64-linux-gnu/c++/10 -I/usr/include/c++/10 -I/usr/local/include -I/usr/include -I../contrib/googletest/googletest -I../contrib/googletest/googletest/include -I../contrib/Tensorflow/bazel-bin/tensorflow/include -I../contrib/Lyra/include -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1e501df/gtest-all.o ../contrib/googletest/googletest/src/gtest-all.cc

${OBJECTDIR}/_ext/e16507f5/logger.o: ../contrib/logger/logger.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/e16507f5
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -DLOG_LEVEL_NORMAL=LOG_LEVEL_DEBUG -DPDC_WIDE -I/usr/lib/llvm-12/lib/clang/12.0.1/include -I/usr/include/x86_64-linux-gnu/c++/10 -I/usr/include/c++/10 -I/usr/local/include -I/usr/include -I../contrib/googletest/googletest -I../contrib/googletest/googletest/include -I../contrib/Tensorflow/bazel-bin/tensorflow/include -I../contrib/Lyra/include -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/e16507f5/logger.o ../contrib/logger/logger.cpp

${OBJECTDIR}/builtin.o: builtin.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -DLOG_LEVEL_NORMAL=LOG_LEVEL_DEBUG -DPDC_WIDE -I/usr/lib/llvm-12/lib/clang/12.0.1/include -I/usr/include/x86_64-linux-gnu/c++/10 -I/usr/include/c++/10 -I/usr/local/include -I/usr/include -I../contrib/googletest/googletest -I../contrib/googletest/googletest/include -I../contrib/Tensorflow/bazel-bin/tensorflow/include -I../contrib/Lyra/include -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/builtin.o builtin.cpp

${OBJECTDIR}/context.o: context.cpp parser.h parser.yy.h
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -DLOG_LEVEL_NORMAL=LOG_LEVEL_DEBUG -DPDC_WIDE -I/usr/lib/llvm-12/lib/clang/12.0.1/include -I/usr/include/x86_64-linux-gnu/c++/10 -I/usr/include/c++/10 -I/usr/local/include -I/usr/include -I../contrib/googletest/googletest -I../contrib/googletest/googletest/include -I../contrib/Tensorflow/bazel-bin/tensorflow/include -I../contrib/Lyra/include -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/context.o context.cpp

${OBJECTDIR}/lexer.o: lexer.cpp parser.yy.h parser.yy.cpp location.hh
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -DLOG_LEVEL_NORMAL=LOG_LEVEL_DEBUG -DPDC_WIDE -I/usr/lib/llvm-12/lib/clang/12.0.1/include -I/usr/include/x86_64-linux-gnu/c++/10 -I/usr/include/c++/10 -I/usr/local/include -I/usr/include -I../contrib/googletest/googletest -I../contrib/googletest/googletest/include -I../contrib/Tensorflow/bazel-bin/tensorflow/include -I../contrib/Lyra/include -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lexer.o lexer.cpp

: lexer.h parser.yy.h parser.yy.cpp location.hh
	@echo Выполнение шага пользовательского сборки
	

.NO_PARALLEL:lexer.yy.cpp lexer.yy.h
lexer.yy.cpp lexer.yy.h: lexer.l parser.y parser.yy.h parser.yy.cpp location.hh term.h
	@echo 
	flex  --outfile=lexer.yy.cpp --header-file=lexer.yy.h --noline  lexer.l

${OBJECTDIR}/lexer.yy.o: lexer.yy.cpp parser.y parser.yy.h parser.yy.cpp location.hh
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -DLOG_LEVEL_NORMAL=LOG_LEVEL_DEBUG -DPDC_WIDE -I/usr/lib/llvm-12/lib/clang/12.0.1/include -I/usr/include/x86_64-linux-gnu/c++/10 -I/usr/include/c++/10 -I/usr/local/include -I/usr/include -I../contrib/googletest/googletest -I../contrib/googletest/googletest/include -I../contrib/Tensorflow/bazel-bin/tensorflow/include -I../contrib/Lyra/include -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lexer.yy.o lexer.yy.cpp

${OBJECTDIR}/newlang.o: newlang.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -DLOG_LEVEL_NORMAL=LOG_LEVEL_DEBUG -DPDC_WIDE -I/usr/lib/llvm-12/lib/clang/12.0.1/include -I/usr/include/x86_64-linux-gnu/c++/10 -I/usr/include/c++/10 -I/usr/local/include -I/usr/include -I../contrib/googletest/googletest -I../contrib/googletest/googletest/include -I../contrib/Tensorflow/bazel-bin/tensorflow/include -I../contrib/Lyra/include -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/newlang.o newlang.cpp

${OBJECTDIR}/nlc.o: nlc.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -DLOG_LEVEL_NORMAL=LOG_LEVEL_DEBUG -DPDC_WIDE -I/usr/lib/llvm-12/lib/clang/12.0.1/include -I/usr/include/x86_64-linux-gnu/c++/10 -I/usr/include/c++/10 -I/usr/local/include -I/usr/include -I../contrib/googletest/googletest -I../contrib/googletest/googletest/include -I../contrib/Tensorflow/bazel-bin/tensorflow/include -I../contrib/Lyra/include -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/nlc.o nlc.cpp

${OBJECTDIR}/object.o: object.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -DLOG_LEVEL_NORMAL=LOG_LEVEL_DEBUG -DPDC_WIDE -I/usr/lib/llvm-12/lib/clang/12.0.1/include -I/usr/include/x86_64-linux-gnu/c++/10 -I/usr/include/c++/10 -I/usr/local/include -I/usr/include -I../contrib/googletest/googletest -I../contrib/googletest/googletest/include -I../contrib/Tensorflow/bazel-bin/tensorflow/include -I../contrib/Lyra/include -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/object.o object.cpp

${OBJECTDIR}/parser.o: parser.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -DLOG_LEVEL_NORMAL=LOG_LEVEL_DEBUG -DPDC_WIDE -I/usr/lib/llvm-12/lib/clang/12.0.1/include -I/usr/include/x86_64-linux-gnu/c++/10 -I/usr/include/c++/10 -I/usr/local/include -I/usr/include -I../contrib/googletest/googletest -I../contrib/googletest/googletest/include -I../contrib/Tensorflow/bazel-bin/tensorflow/include -I../contrib/Lyra/include -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/parser.o parser.cpp

.NO_PARALLEL:parser.yy.h parser.yy.cpp location.hh
parser.yy.h parser.yy.cpp location.hh: parser.y
	@echo ************* Bison compile ************* 
	bison --output-file=parser.yy.cpp --defines=parser.yy.h --warnings=all parser.y

${OBJECTDIR}/parser.yy.o: parser.yy.cpp parser.y
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -DLOG_LEVEL_NORMAL=LOG_LEVEL_DEBUG -DPDC_WIDE -I/usr/lib/llvm-12/lib/clang/12.0.1/include -I/usr/include/x86_64-linux-gnu/c++/10 -I/usr/include/c++/10 -I/usr/local/include -I/usr/include -I../contrib/googletest/googletest -I../contrib/googletest/googletest/include -I../contrib/Tensorflow/bazel-bin/tensorflow/include -I../contrib/Lyra/include -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/parser.yy.o parser.yy.cpp

: parser.yy.h parser.y
	@echo Выполнение шага пользовательского сборки
	

${OBJECTDIR}/term.o: term.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -DLOG_LEVEL_NORMAL=LOG_LEVEL_DEBUG -DPDC_WIDE -I/usr/lib/llvm-12/lib/clang/12.0.1/include -I/usr/include/x86_64-linux-gnu/c++/10 -I/usr/include/c++/10 -I/usr/local/include -I/usr/include -I../contrib/googletest/googletest -I../contrib/googletest/googletest/include -I../contrib/Tensorflow/bazel-bin/tensorflow/include -I../contrib/Lyra/include -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/term.o term.cpp

: term.h parser.yy.cpp location.hh
	@echo Выполнение шага пользовательского сборки
	

${OBJECTDIR}/test/eval_test.o: test/eval_test.cpp
	${MKDIR} -p ${OBJECTDIR}/test
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -DLOG_LEVEL_NORMAL=LOG_LEVEL_DEBUG -DPDC_WIDE -I/usr/lib/llvm-12/lib/clang/12.0.1/include -I/usr/include/x86_64-linux-gnu/c++/10 -I/usr/include/c++/10 -I/usr/local/include -I/usr/include -I../contrib/googletest/googletest -I../contrib/googletest/googletest/include -I../contrib/Tensorflow/bazel-bin/tensorflow/include -I../contrib/Lyra/include -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/test/eval_test.o test/eval_test.cpp

${OBJECTDIR}/test/fraction_test.o: test/fraction_test.cpp
	${MKDIR} -p ${OBJECTDIR}/test
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -DLOG_LEVEL_NORMAL=LOG_LEVEL_DEBUG -DPDC_WIDE -I/usr/lib/llvm-12/lib/clang/12.0.1/include -I/usr/include/x86_64-linux-gnu/c++/10 -I/usr/include/c++/10 -I/usr/local/include -I/usr/include -I../contrib/googletest/googletest -I../contrib/googletest/googletest/include -I../contrib/Tensorflow/bazel-bin/tensorflow/include -I../contrib/Lyra/include -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/test/fraction_test.o test/fraction_test.cpp

${OBJECTDIR}/test/lexer_test.o: test/lexer_test.cpp
	${MKDIR} -p ${OBJECTDIR}/test
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -DLOG_LEVEL_NORMAL=LOG_LEVEL_DEBUG -DPDC_WIDE -I/usr/lib/llvm-12/lib/clang/12.0.1/include -I/usr/include/x86_64-linux-gnu/c++/10 -I/usr/include/c++/10 -I/usr/local/include -I/usr/include -I../contrib/googletest/googletest -I../contrib/googletest/googletest/include -I../contrib/Tensorflow/bazel-bin/tensorflow/include -I../contrib/Lyra/include -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/test/lexer_test.o test/lexer_test.cpp

${OBJECTDIR}/test/newlang_test.o: test/newlang_test.cpp
	${MKDIR} -p ${OBJECTDIR}/test
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -DLOG_LEVEL_NORMAL=LOG_LEVEL_DEBUG -DPDC_WIDE -I/usr/lib/llvm-12/lib/clang/12.0.1/include -I/usr/include/x86_64-linux-gnu/c++/10 -I/usr/include/c++/10 -I/usr/local/include -I/usr/include -I../contrib/googletest/googletest -I../contrib/googletest/googletest/include -I../contrib/Tensorflow/bazel-bin/tensorflow/include -I../contrib/Lyra/include -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/test/newlang_test.o test/newlang_test.cpp

${OBJECTDIR}/test/nlc_test.o: test/nlc_test.cpp
	${MKDIR} -p ${OBJECTDIR}/test
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -DLOG_LEVEL_NORMAL=LOG_LEVEL_DEBUG -DPDC_WIDE -I/usr/lib/llvm-12/lib/clang/12.0.1/include -I/usr/include/x86_64-linux-gnu/c++/10 -I/usr/include/c++/10 -I/usr/local/include -I/usr/include -I../contrib/googletest/googletest -I../contrib/googletest/googletest/include -I../contrib/Tensorflow/bazel-bin/tensorflow/include -I../contrib/Lyra/include -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/test/nlc_test.o test/nlc_test.cpp

${OBJECTDIR}/test/object_test.o: test/object_test.cpp
	${MKDIR} -p ${OBJECTDIR}/test
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -DLOG_LEVEL_NORMAL=LOG_LEVEL_DEBUG -DPDC_WIDE -I/usr/lib/llvm-12/lib/clang/12.0.1/include -I/usr/include/x86_64-linux-gnu/c++/10 -I/usr/include/c++/10 -I/usr/local/include -I/usr/include -I../contrib/googletest/googletest -I../contrib/googletest/googletest/include -I../contrib/Tensorflow/bazel-bin/tensorflow/include -I../contrib/Lyra/include -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/test/object_test.o test/object_test.cpp

${OBJECTDIR}/test/parser_test.o: test/parser_test.cpp
	${MKDIR} -p ${OBJECTDIR}/test
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -DLOG_LEVEL_NORMAL=LOG_LEVEL_DEBUG -DPDC_WIDE -I/usr/lib/llvm-12/lib/clang/12.0.1/include -I/usr/include/x86_64-linux-gnu/c++/10 -I/usr/include/c++/10 -I/usr/local/include -I/usr/include -I../contrib/googletest/googletest -I../contrib/googletest/googletest/include -I../contrib/Tensorflow/bazel-bin/tensorflow/include -I../contrib/Lyra/include -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/test/parser_test.o test/parser_test.cpp

${OBJECTDIR}/variable.o: variable.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -DDEBUG -DLOG_LEVEL_NORMAL=LOG_LEVEL_DEBUG -DPDC_WIDE -I/usr/lib/llvm-12/lib/clang/12.0.1/include -I/usr/include/x86_64-linux-gnu/c++/10 -I/usr/include/c++/10 -I/usr/local/include -I/usr/include -I../contrib/googletest/googletest -I../contrib/googletest/googletest/include -I../contrib/Tensorflow/bazel-bin/tensorflow/include -I../contrib/Lyra/include -I.. -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/variable.o variable.cpp

: warning_pop.h parser.yy.cpp location.hh
	@echo Выполнение шага пользовательского сборки
	

: warning_push.h parser.yy.cpp location.hh
	@echo Выполнение шага пользовательского сборки
	

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} 
	${RM} lexer.yy.cpp lexer.yy.h
	${RM} parser.yy.h parser.yy.cpp location.hh
	${RM} 
	${RM} 
	${RM} 
	${RM} 

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc