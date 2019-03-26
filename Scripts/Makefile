CC := gcc
# CFLAGS := -Wall -O3 -g 
# CFLAGS := -O2 -g 
Libs += -lpthread -lsqlite3 -lm
Targets := Client Server LOCAL
Source := $(wildcard *.c) 
Objs := $(patsubst %.c,%.o ,$(Source)) 

Modules += Controller 
Modules += Debug 
Modules += Modules/cJSON
Modules += Modules/Config 
Modules += Modules/Database 
Modules += Modules/NetAccess 
Modules += Modules/RegexHelper 
Modules += Structure/LinkedList 
Modules += Structure/PersonInfo 
Modules += UserInterface 

AllObjs := $(addsuffix /*.o ,$(Modules))

# VPATH += $(patsubst " ",":",$(Modules)) 
