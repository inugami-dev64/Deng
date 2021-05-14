CC = glslc
BIN = bin/2d/TMF.spv \
	  bin/2d/TMV.spv \
	  bin/2d/UF.spv \
	  bin/2d/UV.spv \
	  bin/3d/TMF.spv \
	  bin/3d/TMV.spv \
	  bin/3d/UF.spv \
	  bin/3d/UV.spv

# Primary target to make
all: .bincheck $(BIN)

.bincheck:
	@echo "Checking for destination directories"
	@if [ ! -d bin/2d ]; then \
		mkdir bin/2d; \
	fi
	@if [ ! -d bin/3d ]; then \
		mkdir bin/3d; \
	fi

# 2D asset shaders
bin/2d/TMF.spv: src/2d/tex_mapped.frag
	@echo "Compiling: bin/2d/TMF.spv"
	@$(CC) src/2d/tex_mapped.frag -o bin/2d/TMF.spv

bin/2d/TMV.spv: src/2d/tex_mapped.vert
	@echo "Compiling: bin/2d/TMV.spv"
	@$(CC) src/2d/tex_mapped.vert -o bin/2d/TMV.spv

bin/2d/UF.spv: src/2d/unmapped.frag
	@echo "Compiling: bin/2d/UF.spv"
	@$(CC) src/2d/unmapped.frag -o bin/2d/UF.spv

bin/2d/UV.spv: src/2d/unmapped.vert
	@echo "Compiling: bin/2d/UV.spv"
	@$(CC) src/2d/unmapped.vert -o bin/2d/UV.spv


# 3D asset shaders
bin/3d/TMF.spv: src/3d/tex_mapped.frag
	@echo "Compiling: bin/3d/TMF.spv"
	@$(CC) src/3d/tex_mapped.frag -o bin/3d/TMF.spv

bin/3d/TMV.spv: src/3d/tex_mapped.vert
	@echo "Compiling: bin/3d/TMV.spv"
	@$(CC) src/3d/tex_mapped.vert -o bin/3d/TMV.spv

bin/3d/UF.spv: src/3d/unmapped.frag
	@echo "Compiling: bin/3d/UF.spv"
	@$(CC) src/3d/unmapped.frag -o bin/3d/UF.spv

bin/3d/UV.spv: src/3d/unmapped.vert
	@echo "Compiling: bin/3d/UV.spv"
	@$(CC) src/3d/unmapped.vert -o bin/3d/UV.spv


# Clean operation
.PHONY: clean
clean: 
	@rm -rf bin/*
