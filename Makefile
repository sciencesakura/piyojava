CFLAGS := -std=c11 -g -I src/
# CFLAGS := -std=c11 -g -I src/ -DDEBUG
SRCS := $(shell find src/ -type f -name '*.c')
HEAD := $(shell find src/ -type f -name '*.h')
OBJS := $(SRCS:.c=.o)

JC := javac
JFLAGS := -g:none
JSRCS := $(shell find ./ -type f -name '*.java')
JCLAS := $(JSRCS:.java=.class)

piyojava: $(OBJS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

check: piyojava expected
	./check.sh

expected: $(JCLAS)
	java Test > expected

clean:
	$(RM) piyojava $(OBJS) $(JCLAS) actual expected *.log

$(OBJS): $(HEAD)

%.class: %.java
	$(JC) $(JFLAGS) $<

.PHONY: check clean
