CFLAGS  = -O0 -g -std=c99 -lm \
	  -pipe \
	  -ggdb3 -Wstrict-overflow=5 -fstack-protector-all \
      -W -Wall -Wextra \
	  -Wbad-function-cast \
	  -Wcast-align \
	  -Wcast-qual \
	  -Wconversion \
	  -Wfloat-equal \
	  -Wformat-y2k \
	  -Winit-self \
	  -Winline \
	  -Winvalid-pch \
	  -Wmissing-declarations \
	  -Wmissing-field-initializers \
	  -Wmissing-format-attribute \
	  -Wmissing-include-dirs \
	  -Wmissing-noreturn \
	  -Wmissing-prototypes \
	  -Wnested-externs \
	  -Wnormalized=nfc \
	  -Wold-style-definition \
	  -Woverlength-strings \
	  -Wpacked \
	  -Wpadded \
	  -Wpointer-arith \
	  -Wredundant-decls \
	  -Wshadow \
	  -Wsign-compare \
	  -Wstack-protector \
	  -Wstrict-aliasing=2 \
	  -Wstrict-prototypes \
	  -Wundef \
	  -Wunsafe-loop-optimizations \
	  -Wvolatile-register-var \
	  -Wwrite-strings

#------------------------------------------------------------------------------
.PHONY : testa clean

#------------------------------------------------------------------------------
testa : exemplo
	./$< < emacs24-dep.dot
	./$< < paises.dot
	./$< < cidades.dot

exemplo : exemplo.c
	$(CC) $(CFLAGS) \
              -I /usr/include/graphviz \
              -o $@ \
              $< \
	      -l cgraph

teste : grafo.c 
	$(CC) $(CFLAGS) -o $@ $^ -I /usr/include/graphviz -l cgraph 

struct : struct.c
	$(CC) $(CFLAGS) -o $@ $^

emacs24-dep.png : emacs24-dep.dot
	dot -v -Tpng $< -o $@

cidades.png paises.png : %.png : %.dot
	neato -v -Tpng $< -o $@

#------------------------------------------------------------------------------
clean :
	$(RM) exemplo exemplo.o teste teste.o grafo.o *.png
