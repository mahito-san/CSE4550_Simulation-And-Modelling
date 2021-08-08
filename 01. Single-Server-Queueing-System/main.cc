#include <iostream>
#include <fstream>
using namespace std;

#include "scheduler.h"
#include "server.h"

int main ()
{
	Scheduler *sch = new Scheduler ();
	sch->initialize ();

	Server* s = new Server ();
	s->createTraceFile ();

	s->arrivalMean () = 1.0;
	s->departureMean () = 0.9;
	s->initialize ();

	sch->run ();
	s->createReport();
	s->generateReport();

	return 0;
}
