@echo off

if not exist %1 (
	mkdir %1
	mkdir %1\obj
	mkdir %1\obj\libsurface
	mkdir %1\obj\libdas
	mkdir %1\obj\libcommon
	mkdir %1\obj\dam
	mkdir %1\obj\deng
	mkdir %1\deng
	mkdir %1\deng\lib
) else (
	if not exist %1\obj (mkdir %1\obj)
	if not exist %1\obj\libsurface (mkdir %1\obj\libsurface)
	if not exist %1\obj\libdas (mkdir %1\obj\libdas)
	if not exist %1\obj\libcommon (mkdir %1\obj\libcommon)
	if not exist %1\obj\dam (mkdir %1\obj\dam)
	if not exist %1\obj\deng (mkdir %1\obj\deng)
	if not exist %1\deng\lib (mkdir %1\deng\lib)
	if not exist %1\deng (mkdir %1\deng)
)