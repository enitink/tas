bin_PROGRAMS = server
AM_CXXFLAGS = -g -std=c++11 $(INTI_CFLAGS) -DASIO_STANDALONE

AM_CPPFLAGS =\
	-I../inc \
	-I../../../../asio/asio-1.11.0/include

noinst_LIBRARIES= libServer.a

libServer_a_SOURCES = \ 
	server.cpp \
	request_parser.cpp \
	reply.cpp \
	mime_types.cpp \
	connection_manager.cpp \
	connection.cpp \
	request_handler.cpp

server_SOURCES = main.cpp
server_LDADD = \
	../../client/src/libHttpClient.a \
	../../dsalgo/src/libDsAlgo.a \
	./libServer.a \
	-lcurl -lxml2 -lz -lpthread
