
pathInc = -I/usr/include/directfb/direct 

CC = clang++ $(CXXFLAGS) -c -Wall -std=c++14 -ferror-limit=100 $(pathInc)
LD = clang++ $(LDFLAGS) obj/*.o

.OBJDIR: ./

all : epyx

epyx: main
		mv *.o obj/
		-rm -rf run
		-mkdir run
		$(LD) -o run/epyx_worldrecords

main:	clean
		$(CC) *.cpp 
		-mkdir -p obj

clean:	
		-rm obj/*.o


