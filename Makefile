all:
	gcc -o dns src/*.c

clean:
	rm -f dns
	rm -rf build

test:
	cmake -S . -B build
	cmake --build build
	./build/test --gtest_filter=* --gtest_color=yes
