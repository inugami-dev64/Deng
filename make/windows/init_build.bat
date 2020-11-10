@echo off

if not exist %1 (
	mkdir %1
	mkdir %1\obj
	mkdir %1\libsurface
	mkdir %1\obj\deng
	mkdir %1\deng
	mkdir %1\deng\lib
) else (
	if not exist %1\obj (mkdir ..\..\build\obj)
	if not exist %1\obj\libsurface (mkdir ..\..\build\obj\libsurface)
	if not exist %1\obj\deng (mkdir ..\..\build\obj\deng)
	if not exist %1\deng\lib (mkdir ..\..\build\deng\lib)
	if not exist %1\deng (mkdir ..\..\build\deng)
)