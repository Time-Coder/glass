MSYS_PATH := D:/Git/usr/bin
CC := javac

# EXTERNAL_JARS := \

# EXTERNAL_DYNAMIC_LIBS := \

DIRS := $(patsubst ./%, %, $(shell $(MSYS_PATH)/find . -type d -not -path "*/.*"))

SRCS := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.java))
CLASSES := $(patsubst src/%.java,bin/%.class,$(SRCS))

JARS := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.jar))

DYNAMIC_LIBS := $(foreach dir, $(DIRS), $(wildcard $(dir)/*.dll)) \
                $(foreach dir, $(DIRS), $(wildcard $(dir)/*.so))

space :=
space +=
CLASS_PATH := $(subst $(space),\;,$(strip $(EXTERNAL_JARS) $(JARS)))

MAIN := $(shell python find_main.py)
MAIN_CLASS := bin/$(MAIN).class
MAIN_JAVA := src/$(MAIN).java

.PHONY: all
all: $(MAIN_CLASS)

$(MAIN_CLASS): $(SRCS) $(EXTERNAL_JARS) $(JARS)
	@$(MSYS_PATH)/mkdir -p $(dir $@)
	@echo Compiling $(MAIN_JAVA)
	@$(CC) -Xlint:unchecked -cp $(dir $(MAIN_JAVA))\;$(CLASS_PATH) $(MAIN_JAVA) -d bin

.PHONY: clean
clean:
	$(MSYS_PATH)/rm -rf bin/*

.PHONY: run
run: $(MAIN_CLASS)
	@java -cp bin\;$(CLASS_PATH) $(MAIN)

.PHONY: create
create:
	$(MSYS_PATH)/mkdir -p src bin lib