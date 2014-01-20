# codeday

NAME = codeday
SRC = src
OBJ = obj
CC = g++
PREFIX = /usr/local
RES = /usr/share/codeday
LDFLAGS =  -lXi -lXrandr -lXxf86vm -lX11 -lpthread -lglfw3 -lGL -lGLEW -lGLU -lfreeimage -lSDL2
CFLAGS = --std=c++11 -Wall -O2
INC = -Iinc

_OBJS = game.o window.o shader.o vertex.o buffer.o vao.o texture.o controller.o physics.o player.o ${NAME}.o
OBJS = $(patsubst %,$(OBJ)/%,$(_OBJS))

$(OBJ)/%.o: $(SRC)/%.cpp
	@$(CC) -c $(INC) -o $@ $< $(CFLAGS)

all: options obj ${NAME}

obj:
	@mkdir -p $(OBJ)

options:
	@echo "${NAME} build options:"
	@echo "CC= ${CC}"
	@echo "LDFLAGS: ${LDFLAGS}"

object: $(OBJS)

${NAME}: $(OBJS) 
	@echo CC -o $@
	@${CC} -o ${NAME} ${OBJS} ${LDFLAGS}

clean:
	@rm -f ${OBJ}/*.o ${NAME}

install: all
	@echo installing to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${RES}
	@cp -f ${NAME} ${DESTDIR}${PREFIX}/bin
	@cp -v res/* ${RES}
	@chmod 755 ${DESTDIR}${PREFIX}/bin/${NAME}

uninstall:
	@echo removing from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/${NAME}
