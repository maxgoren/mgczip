mgczip:
	g++ --std=c++17 -c ./src/mgczip.cpp
	g++ *.o -o mgczip

debug:
	g++ -g --std=c++17 -c ./src/mgczip.cpp
	g++ -g *.o -o mgczip

clean:
	rm *.o
	rm ./mgczip

install:
	cp ./mgczip /usr/local/bin