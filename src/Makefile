CC = g++
CFLAGS = -Wall -std=c++11

SRCDIR = .
BINDIR = ../bin
OBJDIR = ../obj
OPENCV_INCLUDE = -I/usr/include/opencv4
OPENCV_LIBS = -L/usr/lib/x86_64-linux-gnu -l:libopencv_core.so -l:libopencv_highgui.so -l:libopencv_imgcodecs.so -l:libopencv_imgproc.so -l:libopencv_videoio.so

all: $(BINDIR)/main 

$(BINDIR)/main: $(OBJDIR)/main.o
	@mkdir -p $(BINDIR)
	$(CC) -o $@ $^ $(OPENCV_LIBS)

$(OBJDIR)/main.o : $(SRCDIR)/main/main.cpp
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ $(OPENCV_INCLUDE)

clean: 
	@rm -rf $(OBJDIR) $(BINDIR)
