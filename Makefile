# g++ -std=c++17 -I/opt/local/include -L/opt/local/lib port_audio_stream.cc test_port.cc -lfftw3f -lportaudio

CC = g++
CFLAGS = -std=c++17
INCLUDEPATH = /opt/local/include
LINKPATH = /opt/local/lib

test_port: test_port.cc
	$(CC) $(CFLAGS) -I$(INCLUDEPATH) -L$(LINKPATH) port_audio_stream.cc test_port.cc -o test_port -lfftw3f -lportaudio
