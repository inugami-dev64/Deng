INCLUDEFLAGS += -I dependencies/src/

LIBFLAGS += -L dependencies/lib #-L dependencies/lib/debug
LIBS = -lpthread -lX11 -ldl -lglfw3 -lvulkan #-lVkLayer_utils
LINKOBJS = main.o renderer.o queuemanager.o window.o swapchaindetails.o filemanager.o objectshandler.o objloader.o textureloader.o timer.o events.o camera.o dengmath.o
LAYER = VK_LAYER_PATH=dependencies/explicit_layer.d

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

events.o: src/events.cpp
	g++ -c src/events.cpp $(INCLUDEFLAGS)

camera.o: src/camera.cpp
	g++ -c src/camera.cpp $(INCLUDEFLAGS)

dengmath.o: src/dengmath.cpp
	g++ -c src/dengmath.cpp $(INCLUDEFLAGS)

precompile:	src/headers/renderer.h
	g++ src/headers/renderer.h -o src/headers/renderer.h.gch $(INCLUDEFLAGS)

run: deng
	$(LAYER) ./deng

clean:	
	rm *.o
	rm deng
