include ../../Scripts/Makefile

Unit_Depend := *.c *.h 
Unit_Lib := #NULL 


.PHONY:clean test clean_all

test:$(Unit_Depend)
	$(CC) $(CFLAGS) -o $@ $^ $(Unit_Lib) -DDEBUG_PERSON_INFO

clean_all:
	rm -rf test *.o 
	@echo clean all done! 

# default rule !!!!!
all:$(Objs)

clean:
	rm -rf $(Objs)