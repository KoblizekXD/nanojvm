JAVAC=javac
SRC=$(wildcard *.java)
CLASSES=$(SRC:.java=.class)

all: $(CLASSES)

%.class: %.java
	$(JAVAC) $<

clean:
	rm -f *.class
