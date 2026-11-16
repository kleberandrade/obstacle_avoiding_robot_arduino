CXX ?= c++
CXXFLAGS := -std=c++11 -Wall -Wextra -Werror -pedantic
INCLUDES := -Ifirmware/line_follower
.PHONY: test example firmware clean
build:
	mkdir -p build
test: | build
	$(CXX) $(CXXFLAGS) $(INCLUDES) tests/controller_test.cpp -o build/controller_test
	./build/controller_test
	$(CXX) $(CXXFLAGS) $(INCLUDES) tests/firmware_test.cpp -o build/firmware_test
	./build/firmware_test
example: | build
	$(CXX) $(CXXFLAGS) $(INCLUDES) examples/track.cpp -o build/track
	./build/track
firmware:
	arduino-cli compile --profile uno firmware/line_follower
clean:
	rm -rf build
