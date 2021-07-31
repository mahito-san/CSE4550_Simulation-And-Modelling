#ifndef server_h
#define server_h

#include "event.h"
#include "queue.h"
//#include "scheduler.h"

#include <math.h>
#include <iostream>
#include <fstream>
using namespace std;

class Server
{
	public:
		Server ();
		void initialize ();
		void arrivalHandler ();
		void departureHandler ();
		
		void createTraceFile ();
		void updateStat();
		void createReport ();
		void generateReport();
	
		inline int& status () { return (status_); }
		inline int& itemArrived () { return (itemArrived_); }
		inline double& arrivalMean () { return (arrivalMean_); }
		inline double& departureMean () { return (departureMean_); }

	private:
		Queue* queue_;
		ArrivalEvent a_;
		DepartureEvent d_;
		
		int status_;
		int itemArrived_;
		Item* itemInService_;		
		ofstream trace_;
		ofstream report_;

		double arrivalMean_;
		double departureMean_;
		double exponential (double mean);

		double timeLastEvent;
		double areaQueue;
		double areaSystem;
		double areaServer;

		int totalCustomerServed_;
		double queueDelay_ ;
		double serviceTime_ ;
		double totalQueueDelay_ ;
		double totalServiceTime_ ;

		double time_of_arrival[1000];

};
#endif   




   
