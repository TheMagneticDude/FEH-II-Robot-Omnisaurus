TARGET = Proteus

GITBINARY := git
FEHURL := google.com
FIRMWAREREPO := fehproteusfirmware
#Force C++ 17
export CXXFLAGS := -std=c++17



ifeq ($(OS),Windows_NT)	
	SHELL := CMD
endif

all:
ifeq ($(OS),Windows_NT)	
# check for internet connection
# if there's internet, check to see if FEHproteusfirmware folder exists
	@ping -n 1 -w 1000 $(FEHURL) > NUL & \
	if errorlevel 1 \
	( \
		( echo "Warning: No internet connection!" ) \
	) \
	else \
	( \
		( if exist "$(FIRMWAREREPO)" \
		( \
			echo NO OVERRIDING FIRMWARE \
		) \
		else \
		( \
			echo NO OVERRIDING FIRMWARE \
		) \
		) \
	) 
else
# Mac/Linux
	@ping -c 1 -W 1000 $(FEHURL) > NUL ; \
	if [ "$$?" -ne 0 ]; then \
		echo "Warning: No internet connection!"; \
	else \
		echo "NO OVERRIDING FIRMWARE"; \
	fi

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