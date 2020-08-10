INCLUDEFLAGS += -I vulkan/1.2.148.0/source/Vulkan-Headers/include/
INCLUDEFLAGS += -I glfw/include

LIBFLAGS += -L vulkan/1.2.148.0/x86_64/lib/
LIBFLAGS += -L glfw/build/src
LIBS = -lpthread -lX11 -ldl -lglfw3 -lvulkan
LINKOBJS = main.o renderer.o queuemanager.o window.o swapchaindetails.o filemanager.o

deng: $(LINKOBJS)
	g++ $(LINKOBJS) -o deng $(LIBFLAGS) $(LIBS) 

main.o: src/main.cpp
	g++ -c src/main.cpp $(INCLUDEFLAGS)

renderer.o: src/renderer.cpp
	g++ -c src/renderer.cpp $(INCLUDEFLAGS)

queuemanager.o: src/queuemanager.cpp
	g++ -c src/queuemanager.cpp $(INCLUDEFLAGS)

window.o: src/window.cpp
	g++ -c src/window.cpp $(INCLUDEFLAGS)

swapchaindetails.o: src/swapchaindetails.cpp
	g++ -c src/swapchaindetails.cpp $(INCLUDEFLAGS)

filemanager.o: src/filemanager.cpp
	g++ -c src/filemanager.cpp $(INCLUDEFLAGS)

clean:	
	rm *.o
	rm deng