#include "server.h"
#include "scheduler.h"
#include "event.h"

#include <malloc.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <time.h>
#include <string.h>

using namespace std;


Server :: Server () : a_(this), d_(this)
{
	queue_ = new Queue ();
}

double Server :: exponential(double mean) {

	srand(time(0));
    double u = (double) rand()/(RAND_MAX);
    return -(mean * log(u));
}

void Server :: initialize () {

	status_ = 0;
	itemArrived_ = 0;
	timeLastEvent = 0.0;
	areaQueue = 0.0;
	areaServer = 0.0;
	areaSystem = 0.0;
	totalCustomerServed_ = 0;
	queueDelay_ = 0.0;
	serviceTime_ = 0.0;
	totalQueueDelay_ = 0.0;
	totalServiceTime_ = 0.0;
	memset(time_of_arrival, 0.0, sizeof(time_of_arrival));
		
	double t = exponential (arrivalMean_);
	a_.activate (t);
}

void Server :: createTraceFile () {

	trace_.open ("trace.out", ios::out);
	if (!trace_) {
		cout << "cannot open the trace file.\n";
	}
	trace_<< "trace file for the simulation" << endl;
	trace_ << "format of the file" << endl;
	trace_ << "<event> <time> <item id> <server status> <queue size>" << endl << endl;
}


void Server :: createReport () {

	report_.open ("report.out", ios::out);
	if (!report_) {
		cout << "cannot open the report file.\n";
	}
	report_<< "Report file for the simulation" << endl;

}

void Server :: arrivalHandler () {

	Item* temp;
	itemArrived_++;
	temp = (Item*) malloc (sizeof(Item));
	temp->id_ = itemArrived_;

	time_of_arrival[temp->id_] = Scheduler::now();

	trace_ << "a\t" << Scheduler::now () << "\t" << temp->id_ << "\t" << status_ << "\t" << queue_->length() << endl;

	if (status () == 0) {

		queueDelay_ = 0.0;
		totalQueueDelay_ += queueDelay_;
		
		status() = 1;
		trace_ << "s\t" << Scheduler::now () << "\t" << temp->id_ << "\t" << status_ << "\t" << queue_->length() << endl;
		itemInService_ = temp;

		double t = exponential (departureMean_);
		d_.activate (t);
	} 
	
	else {

		queue_->enque(temp);
	}

	if (temp->id_ < 100 ) {
		double t = exponential (arrivalMean_);
		a_.activate (t);
		
	}
}

void Server :: departureHandler () {

	totalCustomerServed_++;

	if (queue_->length() > 0) {
		trace_ << "d\t" << Scheduler::now () << "\t" << itemInService_->id_ << "\t" << status_ << "\t" << queue_->length() << endl;
	} 
	
	else {
		trace_ << "d\t" << Scheduler::now () << "\t" << itemInService_->id_ << "\t" << 0 << "\t" << queue_->length() << endl;
	}

	if (queue_->length() > 0) {

		itemInService_ = queue_->deque ();

		queueDelay_ = Scheduler::now() - time_of_arrival[itemInService_->id_];
		totalQueueDelay_ += queueDelay_;

		trace_ << "s\t" << Scheduler::now () << "\t" << itemInService_->id_ << "\t" << status_ << "\t" << queue_->length() << endl;

		double t = exponential (departureMean_);
		d_.activate (t);
	
		serviceTime_ = d_.expire() - Scheduler::now();
		totalServiceTime_ += serviceTime_;
	} 
	
	else {

		serviceTime_ = Scheduler::now() - d_.start();
		totalServiceTime_ += serviceTime_;
		
		status () = 0;
		itemInService_ = 0;
	}
}

void Server::updateStat()
{
	double durationSinceLastEvent;

	durationSinceLastEvent = Scheduler::now() - timeLastEvent;
	timeLastEvent = Scheduler::now();

	areaQueue += durationSinceLastEvent*(queue_->length());
	areaServer += durationSinceLastEvent*status_;
	areaSystem += durationSinceLastEvent*(queue_->length()+status_);

}

void Server::generateReport()
{
	double avgQueueDelay = totalQueueDelay_/itemArrived_;
	double avgQueueLength = areaQueue/Scheduler::now();
	double serverUtilization = areaServer /Scheduler::now();
	double avgServiceTime = totalServiceTime_/itemArrived_;
	double avgSystemLength = areaSystem/Scheduler::now();
	double trafficIntensity = arrivalMean_/departureMean_;
	
	report_ <<"Traffic Intensity :\t"<< trafficIntensity<<endl;
	report_<< "Average Queue Delay :\t" << avgQueueDelay << endl;
	report_<< "Average Service Time :\t" << avgServiceTime<<endl;
	report_<< "Average Queue Length :\t" << avgQueueLength << endl;
	report_<< "Average System Length :\t" << avgSystemLength << endl;
	report_<< "Server Utilization :\t" << serverUtilization<< endl;
}

