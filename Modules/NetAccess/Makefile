include ../../Scripts/Makefile


Unit_Depend := NetAccess.c NetAccess.h 					#Local
Unit_Depend += ../cJSON/cJSON.c							#Modules
Unit_Depend += ../Database/Database_v2.c 				#Modules
Unit_Depend += ../Config/GetConfig.c 					#Modules
Unit_Depend += ../../Debug/Debug.c 						#Debug
Unit_Depend += ../../Structure/LinkedList/LinkedList.c	#Structure
Unit_Depend += ../../Structure/PersonInfo/PersonInfo.c 	#Structure

Unit_Lib := -lsqlite3 -lm 

# default rule !!!!!
all:$(Objs) 

.PHONY:clean Client_test Server_test Db_json_test all_test clean_all 
all_test:Client_test Server_test Db_json_test 

#no need to redefined -DNET_CLIENT 
Client_test:$(Unit_Depend) Client.c 
	$(CC) $(CFLAGS) -o $@ $^ $(Unit_Lib) -DNET_CLIENT  

Server_test:$(Unit_Depend) Server.c 
	$(CC) $(CFLAGS) -o $@ $^ $(Unit_Lib) -DNET_SERVER 

Db_json_test:$(Unit_Depend) DbJSON.c 
	$(CC) $(CFLAGS) -o $@ $^ $(Unit_Lib) -DNET_CLIENT -DNET_SERVER 


clean_all: 
	rm -rf *_test *.o 
	@echo clean all done! 

clean:
	rm -rf $(Objs)