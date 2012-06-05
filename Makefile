
all: compile

configure:
	mkdir -p build
	mkdir -p build/bin
	(cd build/bin && ln -s ../../examples/assets)
	(cd build && cmake -DCMAKE_BUILD_TYPE=Release ..)

configure-debug:
	mkdir -p build
	(cd build && cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_DEBUG=Yes ..)

compile: 
	make -C build

shaders:
	ruby make-shaders.rb

run-cube:
	(cd build/bin && ./cube)

clean:
	make -C build clean

distclean:
	(rm -rf build)
