INCLUDEFLAGS += -I dependencies/src/

LIBFLAGS += -L dependencies/lib
LIBS = -lpthread -lX11 -ldl -lglfw3 -lvulkan
LINKOBJS = main.o renderer.o queuemanager.o window.o swapchaindetails.o filemanager.o objectshandler.o objloader.o textureloader.o timer.o

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

objectshandler.o: src/objectshandler.cpp
	g++ -c src/objectshandler.cpp $(INCLUDEFLAGS)

objloader.o: src/objloader.cpp
	g++ -c src/objloader.cpp $(INCLUDEFLAGS)

textureloader.o: src/textureloader.cpp
	g++ -c src/textureloader.cpp $(INCLUDEFLAGS)

timer.o: src/timer.cpp
	g++ -c src/timer.cpp $(INCLUDEFLAGS)

precompile:	src/headers/renderer.h
	g++ src/headers/renderer.h -o src/headers/renderer.h.gch $(INCLUDEFLAGS)

clean:	
	rm *.o
	rm deng