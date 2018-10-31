.PHONY: all

all:
	make -C secure
	make -C non-secure
	
	cat secure/secure.bin non-secure/normal.bin > test.bin
	
clean:
	rm test.bin
	make -C secure clean
	make -C non-secure clean

