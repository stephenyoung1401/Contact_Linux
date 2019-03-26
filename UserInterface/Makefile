include ../Scripts/Makefile

Unit_Depend := *.c *.h 
Unit_Depend += ../Structure/LinkedList/LinkedList.c
Unit_Depend += ../Structure/PersonInfo/PersonInfo.c 
Unit_Lib := #NULL 

# default rule !!!!!
all:$(Objs)

.PHONY:clean test clean_all
test:$(Unit_Depend)
	$(CC) $(CFLAGS) -o $@ $^ $(Unit_Lib) -DDEBUG_USER_INTERFACE

clean_all:
	rm -rf test *.o 
	@echo clean all done! 

clean:
	rm -rf $(Objs)