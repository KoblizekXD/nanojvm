JAVAC      := javac
SRC		   := $(wildcard *.java)
JAVAC_OPTS := -source 8 -target 8 -bootclasspath ../std/build/libs/rt.jar
CLASSES    := $(SRC:.java=.class)

all: $(CLASSES)

%.class: %.java
	$(JAVAC) $(JAVAC_OPTS) $<

clean:
	rm -f *.class
