all:
	mkdir -p bin
	cd build && make
	./bin/main
