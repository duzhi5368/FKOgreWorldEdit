INCLUDES=`pkg-config OGRE --cflags` -Iinclude
LIBS=`pkg-config OGRE --libs`
CFLAGS=-O2 $(INCLUDES) -fPIC -c

all: src/ETBrush.cpp src/ETIndexHandler.cpp src/ETLightmap.cpp src/ETLoadSaveHeightmap.cpp src/ETSplattingManager.cpp src/ETTerrainInfo.cpp src/ETTerrainManager.cpp src/ETTile.cpp
	g++ $(CFLAGS) -o obj/ETBrush.o src/ETBrush.cpp
	g++ $(CFLAGS) -o obj/ETIndexHandler.o src/ETIndexHandler.cpp
	g++ $(CFLAGS) -o obj/ETLightmap.o src/ETLightmap.cpp
	g++ $(CFLAGS) -o obj/ETLoadSaveHeightmap.o src/ETLoadSaveHeightmap.cpp
	g++ $(CFLAGS) -o obj/ETSplattingManager.o src/ETSplattingManager.cpp
	g++ $(CFLAGS) -o obj/ETTerrainInfo.o src/ETTerrainInfo.cpp
	g++ $(CFLAGS) -o obj/ETTerrainManager.o src/ETTerrainManager.cpp
	g++ $(CFLAGS) -o obj/ETTile.o src/ETTile.cpp
	
	gcc -shared -o bin/libEditableTerrain.so -fPIC obj/ETBrush.o obj/ETIndexHandler.o obj/ETLightmap.o obj/ETLoadSaveHeightmap.o obj/ETSplattingManager.o obj/ETTerrainInfo.o obj/ETTerrainManager.o obj/ETTile.o

clean:
	rm bin/libEditableTerrain.so obj/*.o
