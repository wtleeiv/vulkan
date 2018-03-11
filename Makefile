P=vulkan
OBJECTS=
CFLAGS= -g -Wall -std=c11 -I$(VULKAN_SDK)/include
LDLIBS= -L$(VULKAN_SDK)/lib `pkg-config --static --libs glfw3` -lvulkan

$(P): $(OBJECTS)

.PHONY: test clean

test: $(P)
	./$(P)

clean:
	$(RM) $(P)
