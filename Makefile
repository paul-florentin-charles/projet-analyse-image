CPPFLAGS = -I. -I/opt/opencv/include
CXXFLAGS = -Wall -Wextra -Werror
LDFLAGS = -Wl,-R/opt/opencv/lib -L/opt/opencv/lib
LDLIBS = \
	-lopencv_core\
	-lopencv_imgproc\
	-lopencv_highgui

BIN = \
	labeling-color\
	convert\
	kmeans\
	otsu\
	contours\
	filter\
	#components

.PHONY: all
all: $(BIN)

.PHONY: clean
clean:
	$(RM) *~ *.tif

.PHONY: cleanall
cleanall: clean
	$(RM) $(BIN) *.o
