include ../Scripts/Makefile

Unit_Depend := *.c *.h 									#Local
Unit_Depend += ../Debug/Debug.c 						#Debug
Unit_Depend += ../Modules/cJSON/cJSON.c 				#Modules
Unit_Depend += ../Modules/Database/Database_v2.c 		#Modules
Unit_Depend += ../Modules/NetAccess/NetAccess.c   		#Modules
Unit_Depend += ../Modules/Config/GetConfig.c 			#Modules
Unit_Depend += ../Modules/RegexHelper/RegexSearch.c		#Modules
Unit_Depend += ../UserInterface/UserInterface.c 		#Modules

Unit_Depend += ../Structure/LinkedList/LinkedList.c		#Structure
Unit_Depend += ../Structure/PersonInfo/PersonInfo.c 	#Structure

Unit_Lib := -lsqlite3 -lm 
# default rule !!!!!
all:$(Objs)

.PHONY:clean Net_test Local_test all_test clean_all
all_test:Net_test Local_test

Net_test:$(Unit_Depend)
	$(CC) $(CFLAGS) -o $@ $^ $(Unit_Lib) -DNET_ENABLED -DNET_CLIENT 

Local_test:$(Unit_Depend)
	$(CC) $(CFLAGS) -o $@ $^ $(Unit_Lib) -DLOCAL_ENABLED 

clean_all:
	rm -rf *_test *.o 
	@echo clean all done! 

clean:
	rm -rf $(Objs)