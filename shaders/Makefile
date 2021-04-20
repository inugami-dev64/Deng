CC = glslc
BIN = bin/2d_unmapped.spv \
	  bin/2d_tex_mapped.spv \
	  bin/3d_tex_mapped.spv \
	  bin/3d_tex_mapped_nor.spv \
	  bin/3d_unmapped.spv \
	  bin/3d_unmapped_nor.spv \
	  bin/unmapped_frag.spv \
	  bin/tex_mapped_frag.spv 

all: $(BIN)


# 2D vertex shaders
bin/2d_unmapped.spv: src/2d_unmapped.vert
	$(CC) src/2d_unmapped.vert -o bin/2d_unmapped.spv

bin/2d_tex_mapped.spv: src/2d_tex_mapped.vert
	$(CC) src/2d_tex_mapped.vert -o bin/2d_tex_mapped.spv

# 3D vertex shaders
bin/3d_tex_mapped.spv: src/3d_tex_mapped.vert
	$(CC) src/3d_tex_mapped.vert -o bin/3d_tex_mapped.spv

bin/3d_tex_mapped_nor.spv: src/3d_tex_mapped_nor.vert
	$(CC) src/3d_tex_mapped_nor.vert -o bin/3d_tex_mapped_nor.spv

bin/3d_unmapped.spv: src/3d_unmapped.vert
	$(CC) src/3d_unmapped.vert -o bin/3d_unmapped.spv

bin/3d_unmapped_nor.spv: src/3d_unmapped_nor.vert
	$(CC) src/3d_unmapped_nor.vert -o bin/3d_unmapped_nor.spv

# Fragment shaders 
bin/unmapped_frag.spv: src/unmapped.frag
	$(CC) src/unmapped.frag -o bin/unmapped_frag.spv

bin/tex_mapped_frag.spv: src/tex_mapped.frag
	$(CC) src/tex_mapped.frag -o bin/tex_mapped_frag.spv


.PHONY: clean
clean:
	rm -rf bin/*
