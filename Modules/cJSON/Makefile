include ../../Scripts/Makefile

Unit_Depend := *.c *.h 
Unit_Lib := -lm

# default rule !!!!!
all:$(Objs)

.PHONY:clean test clean_all
test:$(Unit_Depend)
	$(CC) $(CFLAGS) -o $@ $^ $(Unit_Lib) -DDEBUG_cJSON 

clean_all:
	rm -rf test *.o 
	@echo clean all done! 

clean:
	rm -rf $(Objs)