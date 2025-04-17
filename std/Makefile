SRC_DIR := src
OUT_DIR := build/classes
LIB_DIR := build/libs
OUT_LIB := $(LIB_DIR)/rt.jar
JAR := jar

JAVA_SOURCES := $(shell find $(SRC_DIR) -name "*.java")
JAVAC_OPTS := -target 8 -source 8 -bootclasspath "" -cp ""

all: $(OUT_LIB) 

compile:
	mkdir -p $(OUT_DIR)
	javac -d $(OUT_DIR) $(JAVAC_OPTS) $(JAVA_SOURCES)

$(OUT_LIB): compile 
	mkdir -p $(LIB_DIR)
	cd $(OUT_DIR) && $(JAR) cf ../libs/$(notdir $(OUT_LIB)) .

clean:
	rm -rf build

.PHONY: all compile clean
