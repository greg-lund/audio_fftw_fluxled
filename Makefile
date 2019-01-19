# g++ -std=c++17 -I/opt/local/include -L/opt/local/lib port_audio_stream.cc test_port.cc -lfftw3f -lportaudio

CC = g++
CFLAGS = -std=c++17
SOURCE = port_audio_stream.cc test_port.cc
INCLUDEPATH = /opt/local/include
LINKPATH = /opt/local/lib
LFLAGS = -lfftw3f -lportaudio

test_port: test_port.cc
	$(CC) $(CFLAGS) -I$(INCLUDEPATH) -L$(LINKPATH) $(SOURCE) -o test_port $(LFLAGS)
