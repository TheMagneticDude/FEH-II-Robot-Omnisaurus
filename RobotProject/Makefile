TARGET = Proteus

GITBINARY := git
FEHURL := google.com
FIRMWAREREPO := fehproteusfirmware

INCLUDE_DIRS := ../Dependencies ../Dependencies/Movement ../Dependencies/Auto ../Dependencies/Auto/Paths
CXXFLAGS += $(foreach dir, $(INCLUDE_DIRS), -I$(dir))


ifeq ($(OS),Windows_NT)	
	SHELL := CMD
endif

all:
ifeq ($(OS),Windows_NT)	
# check for internet connection
# if there's internet, check to see if FEHproteusfirmware folder exists
# if it does, remove it before cloning the repo

else
# Mac/Linux


endif

ifeq ($(OS),Windows_NT)	
	@cd $(FIRMWAREREPO) && mingw32-make all TARGET=$(TARGET)
	@cd $(FIRMWAREREPO) && mingw32-make deploy TARGET=$(TARGET)
else
	@cd $(FIRMWAREREPO) && make all TARGET=$(TARGET)
	@cd $(FIRMWAREREPO) && make deploy TARGET=$(TARGET)
endif

deploy:
ifeq ($(OS),Windows_NT)	
	@cd $(FIRMWAREREPO) && mingw32-make deploy TARGET=$(TARGET)
else
	@cd $(FIRMWAREREPO) && make deploy TARGET=$(TARGET)
endif

clean:
ifeq ($(OS),Windows_NT)	
	@cd $(FIRMWAREREPO) && mingw32-make clean TARGET=$(TARGET)
else
	@cd $(FIRMWAREREPO) && make clean TARGET=$(TARGET)
endif

run:
ifeq ($(OS),Windows_NT)	
	@cd $(FIRMWAREREPO) && mingw32-make run TARGET=$(TARGET)
else
	@cd $(FIRMWAREREPO) && make run TARGET=$(TARGET)
endif