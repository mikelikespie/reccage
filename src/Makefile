CC=g++
CFLAGS=-Wall -ggdb3
LDFLAGS=-lpthread
SOURCES=ConcurrentDataSet.cpp  DataSet.cpp  reccagemingw.cpp DistanceFunctions.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=reccage

all: $(SOURCES) $(EXECUTABLE)
		
$(EXECUTABLE): $(OBJECTS) 
		$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
		$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(EXECUTABLE) *.o

python:
	python setup.py build_ext --inplace
