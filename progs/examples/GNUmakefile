#
# $Header: //sw/apps/gpu/drivers/opengl/glut/progs/examples/GNUmakefile#1 $
#

include $(ROOT)/usr/include/make/commondefs

TARGETS = \
	sphere \
	scene \
	movelight \
	glutplane \
	stroke \
	olympic \
	glutdino \
	dinospin \
	mjkwarp \
	highlight \
	subwin \
	origami \
	bitfont \
	lightlab \
	sphere2 \
	dinoball \
	stereo \
	glpuzzle \
	dials \
	blender \
	oclip \
	ohidden \
	olight \
	scube \
	spots \
	stars \
	stenciltst \
	triselect \
	abgr \
	texenv \
	fogtst \
	dials2 \
	surfgrid \
	molehill \
	zoomdino \
	fontdemo \
	splatlogo \
	oversphere \
	fontdemo \
	evaltest \
	sb2db \
	screendoor \
	simple \
	cube \
	reflectdino \
	rendereps \
	dinoshade \
	halomagic \
	trippy \
	mjksift \
	circlefit \
	editgrid \
	dinodraw \
	resolution \
	$(NULL)

LDIRT = mjkimage.c logo.c render.eps

LLDLIBS = -lglut -lGLU -lGL -lXmu -lXi

default all: $(TARGETS)

install: $(TARGETS)
	$(INSTALL) -F /usr/bin $(TARGETS)

mjksift: mjksift.o mjkimage.o
	$(RM) $@
	$(CC) -o $@ mjksift.o mjkimage.o $(LDFLAGS)

mjkwarp: mjkwarp.o mjkimage.o
	$(RM) $@
	$(CC) -o $@ mjkwarp.o mjkimage.o $(LDFLAGS)

mjkimage.o: mjkimage.c.Z
	$(RM) $@
	zcat mjkimage.c.Z > mjkimage.c
	$(CC) -c mjkimage.c
	$(RM) mjkimage.c

splatlogo: splatlogo.o logo.o
	$(RM) $@
	$(CC) -o $@ splatlogo.o logo.o $(LDFLAGS)

logo.o: logo.c.Z
	$(RM) $@
	zcat logo.c.Z > logo.c
	$(CC) -c logo.c
	$(RM) logo.c

dinodraw: dinodraw.o trackball.o
	$(RM) $@
	$(CC) -o $@ dinodraw.o trackball.o $(LDFLAGS)

dinospin: dinospin.o trackball.o
	$(RM) $@
	$(CC) -o $@ dinospin.o trackball.o $(LDFLAGS)

glpuzzle: glpuzzle.o trackball.o
	$(RM) $@
	$(CC) -o $@ glpuzzle.o trackball.o $(LDFLAGS)

include $(COMMONRULES)
