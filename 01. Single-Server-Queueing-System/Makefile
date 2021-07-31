main: main.o event.o queue.o scheduler.o server.o
	g++ -o main main.o event.o queue.o scheduler.o server.o

main.o: main.cc
	g++ -c main.cc

event.o: event.cc event.h server.h scheduler.h
	g++ -c event.cc

queue.o: queue.cc queue.h
	g++ -c queue.cc

scheduler.o: scheduler.cc scheduler.h event.h
	g++ -c scheduler.cc

server.o: server.cc server.h  queue.h event.h
	g++ -c server.cc

clean:
	rm main.exe main.o server.o event.o queue.o scheduler.o trace.out