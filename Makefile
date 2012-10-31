TARGET=test
CFLAGS=	-g -pg \

OBJS=	\
	./sk.o

DEPS=$(patsubst %.o,%.d,$(OBJS))

getObjOfSuffix= ${patsubst %${2},%.o,${wildcard ${addsuffix ${2},${basename ${1}}}}}

COBJS=${call getObjOfSuffix,${OBJS},.c}
CXXOBJS=${call getObjOfSuffix,${OBJS},.cpp}

LIB= 	\


SO_LIB= \
	-L/usr/local/lib -lboost_system -lboost_program_options \

LDFLAGS= \
	-lpthread \
	-pg \
	#-lev \

.PHONY: all clean

all: ${TARGET}

${TARGET}: ${OBJS} ${LIB}
	${CXX} -o $@ $^ ${SO_LIB} ${LDFLAGS}

${CXXOBJS}:%.o:%.cpp
	${CXX} -c -o $@ $< ${CFLAGS}

${COBJS}:%.o:%.c
	${CC} -c -o $@ $< ${CFLAGS}

%.d : %.c
	${CC} -MM -MD ${CFLAGS} $< -o $@ -MT ${@:.d=.o}

%.d : %.cpp
	${CXX} -MM -MD ${CFLAGS} $< -o $@ -MT ${@:.d=.o}

-include $(DEPS)

clean:
	rm -f ${OBJS} ${TARGET} ${DEPS}

