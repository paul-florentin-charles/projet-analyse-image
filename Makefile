CPPFLAGS = -I. -I/opt/opencv/include
CXXFLAGS = -Wall -Wextra -Werror
LDFLAGS = -Wl,-R/opt/opencv/lib -L/opt/opencv/lib
LDLIBS = \
	-lopencv_core\
	-lopencv_imgproc\
	-lopencv_highgui\
	-lopencv_ml

BIN = \
	labeling-color\
	convert\
	kmeans\
	otsu\
	contours\
	filter\
	classify\
	segment\
	#distance\
	#skeleton\
	#morphology\
	#make-se\
	#components

.PHONY: all
all: $(BIN)

#$(BIN): morphology.o

.PHONY: clean
clean:
	$(RM) *~ *.tif

.PHONY: cleanall
cleanall: clean
	$(RM) $(BIN) *.o
