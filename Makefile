include ./Scripts/Makefile

modules_make = $(MAKE) -C $(1);
modules_clean = $(MAKE) clean -C $(1);

all:$(Targets)

.PHONY:Client Server LOCAL mm mc clean

mm:
	@ $(foreach n, $(Modules), $(call modules_make, $(n)))

mc:
	@ $(foreach n, $(Modules), $(call modules_clean, $(n)))

Client:
	cd ./Controller && $(MAKE) Net_test
	mv ./Controller/Net_test .
	@ echo make done!

Server:mm Modules/NetAccess/Server.c 
	cd ./Modules/NetAccess && $(MAKE) Server_test
	mv ./Modules/NetAccess/Server_test .
	@ echo make done! 

LOCAL:mm
	cd ./Controller && $(MAKE) Local_test
	mv ./Controller/Local_test .
	@ echo make done!

clean:mc
	rm -f $(Targets)
	rm -f *_test
	@ echo clean done!
