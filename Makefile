SUBDIRS = ./NesCore ./Gui ./NesEmulator

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	rm */out -rf
	rm out -rf
run:
	./out/nesemulator

.PHONY: all $(SUBDIRS)
