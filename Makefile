CFLAGS := -std=c11 -g -I src/
SRCS := $(shell find src/ -type f -name '*.c')
HEAD := $(shell find src/ -type f -name '*.h')
OBJS := $(SRCS:.c=.o)

JC := javac
JFLAGS := -g:none
JSRCS := $(shell find ./ -type f -name '*.java')
JCLAS := $(JSRCS:.java=.class)

piyojava: $(OBJS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

check: piyojava Test.class
	./piyojava Test

clean:
	$(RM) piyojava $(OBJS)

$(OBJS): $(HEAD)

%.class: %.java
	$(JC) $(JFLAGS) $<

.PHONY: check clean
