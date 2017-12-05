FLAGS = -g -Wall -Werror
LFLAGS = -lncurses

BIN = rogue
OBJS = main.o dungeon.o core.o character.o point.o room.o turn.o util.o dijkstra.o heap.o move.o path.o ui.o debug.o pc.o npc.o dice.o monster.o

all: $(BIN) none

$(BIN):$(OBJS)
	@echo Linking $@
	@g++ $(FLAGS) $(OBJS) -o $(BIN) $(LFLAGS)

-include $(OBJS:.o=.d)

%.o:%.cpp
	@echo Compiling $<
	@g++ $(FLAGS) -MMD -MF $*.d -c $<

%.o:%.c
	@echo Compiling $<
	@gcc $(FLAGS) -MMD -MF $*.d -c $<

clean:
	@echo Removing generated file
	@rm -f *.o *.d $(BIN) *~ *.log

none:
	@echo Compilation Complete
