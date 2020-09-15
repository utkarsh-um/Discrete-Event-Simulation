#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "structures.h"
int nC;
int nT;
float tSim;
float tavgServ;
float clock;
struct EventQueue *EvQ;
struct Tellers *allTellers;
struct Customers *allCustomers;
float totservice=0.0;
int funptr=0;


void addCustomer(struct Customer *t1,struct Customers *q1)
{
	if(q1->data == NULL)
	{	
		q1->data = t1;
		q1->next = NULL;
	}
	else
	{
		while(q1->next != NULL )
		{
			q1=q1->next;
		}
		struct Customers *q2=(struct Customers*)malloc(sizeof(struct Customers)); 
		q2->data=t1;
		q2->next=NULL;
		q1->next=q2;
	}
}

void addTeller(struct Teller *t1,struct Tellers *q1)
{
	if(q1->data == NULL)
	{	
		
		q1->data = t1;
		q1->next = NULL;
	}
	else
	{
		while(q1->next != NULL )
		{
			q1=q1->next;
		}
		struct Tellers *q2=(struct Tellers*)malloc(sizeof(struct Tellers)); 
		q2->data=t1;
		q2->next=NULL;
		q1->next=q2;
	}
}

void addEvent(struct Event *t1,struct EventQueue *q1)
{
	if(q1->head == NULL)
	{	
		struct EventNode *n1=(struct EventNode*)malloc(sizeof(struct EventNode));
		n1->data = t1;
		n1->next = NULL;
		q1->head = n1;
	}
		
	else if(q1->head->data->tevent > t1->tevent)
		{
			
			struct EventNode *n1=(struct EventNode*)malloc(sizeof(struct EventNode)); 
			n1->data = t1;
			n1->next = q1->head;
			q1->head = n1;
		}
	
	else
	{
		struct EventNode *n1 = q1->head;
		while(n1->next != NULL && n1->next->data->tevent <= t1->tevent)
		{
			n1=n1->next;
		}
		struct EventNode *n2=(struct EventNode*)malloc(sizeof(struct EventNode)); 
		n2->data = t1;
		n2->next=n1->next;
		n1->next = n2;
	}
}

struct Event* getHead(struct EventQueue *q1)
{
	if(q1->head == NULL)
	{
		return NULL;
	}
	else if(q1->head->next == NULL)
	{
		struct Event *t1=q1->head->data;
		free(q1->head);
		q1->head=NULL;
		return t1;
	}
	else
	{
		struct Event *t1 = q1->head->data;
		struct EventNode *n2 = q1->head->next;
		free(q1->head);
		q1->head=n2;
		return t1;	
	}
}

//-----------------------------------------------------------------------------------------------------------------------------

void addTellerQueue(struct Customer *t1,struct TellerQueue *q1)
{

	if(q1->head == NULL)
	{	
		struct TellerNode *n1=(struct TellerNode*)malloc(sizeof(struct TellerNode));
		n1->data = t1;
		n1->next = NULL;
		q1->head = n1;

	}
	
	else
	{
		int count=0;
		struct TellerNode *n1 = q1->head;
		while(n1->next != NULL)
		{
			n1=n1->next;
		}
		struct TellerNode *n2=(struct TellerNode*)malloc(sizeof(struct TellerNode)); 
		n2->data = t1;
		n2->next = NULL;
		n1->next = n2;
	}
	q1->count++;
}

struct Customer* getHeadTellerQueue(struct TellerQueue *q1)
{
	if(q1->head == NULL)
	{
		return NULL;
	}
	else if(q1->head->next == NULL)
	{
		struct Customer *t1=q1->head->data;
		free(q1->head);
		q1->head=NULL;
		q1->count--;
		return t1;
	}
	else
	{
		struct Customer *t1 = q1->head->data;
		struct TellerNode *n2 = q1->head->next;
		free(q1->head);
		q1->head=n2;
		q1->count--;
		return t1;	
	}
}

int countCust(struct TellerQueue *q1)
{

	return q1->count;
}

// ----------------------------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------------------------------------


void addcustTotellers(struct Customer* c1, struct Tellers* qtt)
{
	struct Tellers* qT = qtt;
	int min = countCust(qT->data->customers);
	int counter=0;
	struct Teller* t1 = qT->data;
	while(qT != NULL)
	{
		int count = countCust(qT->data->customers);
		if(count<min)
		{
			t1 = qT->data;
			min = count;
			counter=0;
		}
		if(count == min)
		{
			counter++;
		}
		qT = qT->next;
	} 
	if(counter > 1)
	{
		int arr[counter];
		qT = qtt;
		int ind=0;
		int jnd=0;
		while(qT != NULL)
		{
			int count = countCust(qT->data->customers);
			if(count == min)
			{
				arr[ind] = jnd;
				ind++;
			}
			jnd++;
			qT=qT->next;
		}
		int nu = arr[rand()%counter] ;
		qT = qtt;
		while(nu--)
			qT = qT->next;
		t1 = qT->data;
	}
	addTellerQueue(c1,t1->customers);


}
//-------------------------------------------------------------------------


 void serviceCompleted(struct Customer* c1)
{
			
			c1->ttotal = c1->twait + c1->tserv;
			totservice+=c1->tserv;
}

//----------------------------------------------------------------------------------------------------------------------------------------

void tellerEvent(struct Teller* t1, struct Tellers* qtt)
{
	struct Customer *c1;
	struct Tellers* qT = qtt;
	if(countCust(t1->customers) > 0)
	{
		c1 = getHeadTellerQueue(t1->customers);
		c1->tserv =  2.0*tavgServ*((float)rand()/(float)(RAND_MAX));
		c1->twait = clock - c1->tarr;
		struct Event* e1 = (struct Event*)malloc(sizeof(struct Event));
		e1->type = 3;
		e1->asC = c1;
		e1->tevent = clock + c1->tserv;
		e1->action3 = serviceCompleted;
		//printf("Function Pointer\n");
		funptr++;
		addEvent(e1,EvQ);
		t1->tserv += c1->tserv;
		struct Event* e2 = (struct Event*)malloc(sizeof(struct Event));
		e2->type = 2;
		e2->asT = t1;
		e2->tevent = clock + c1->tserv;
		e2->action2 = tellerEvent;
		//printf("Function Pointer\n");
		funptr++;
		addEvent(e2,EvQ);

	}
	else
	{
		int counter = 0;
		qT = qtt;
		while(qT != NULL)
		{
			int count = countCust(qT->data->customers);
			if(count > 0)
			{
				counter++;
			}
			qT=qT->next;
		}
		if(counter == 0)
		{
			if(clock < tSim)
			{
				struct Event* e1 = (struct Event*)malloc(sizeof(struct Event));
				e1->type = 2;
				e1->asT = t1;
				e1->action2 = tellerEvent;
				//printf("Function Pointer\n");
				funptr++;
				e1->tevent = clock + (149 * ((float)rand()/(float)(RAND_MAX)) + 1)/60;
				t1->idleTime += e1->tevent - clock;
				addEvent(e1,EvQ);
			}
		}
		else 
		{
			int arr[counter];
			qT = qtt;
			int ind=0;
			int jnd=0;
			while(qT != NULL)
			{
				int count = countCust(qT->data->customers);
				if(count > 0)
				{
					arr[ind] = jnd;
					ind++;
				}
				jnd++;
				qT=qT->next;
			}
			int nu = arr[rand()%counter] ;
			qT = qtt;
			while(nu--)
				qT = qT->next;
			struct Teller* t2 = qT->data;

			c1 = getHeadTellerQueue(t2->customers);

			c1->tserv =  2.0*tavgServ*((float)rand()/(float)(RAND_MAX));
			c1->twait = clock - c1->tarr;
			struct Event* e1 = (struct Event*)malloc(sizeof(struct Event));
			e1->type = 3;
			e1->asC = c1;
			e1->tevent = clock + c1->tserv;
			e1->action3 = serviceCompleted;
			//printf("Function Pointer\n");
			funptr++;
			addEvent(e1,EvQ);
			t1->tserv += c1->tserv;
			struct Event* e2 = (struct Event*)malloc(sizeof(struct Event));
			e2->type = 2;
			e2->asT = t1;
			e2->tevent = clock + c1->tserv;
			e2->action2 = tellerEvent;
			//printf("Function Pointer\n");
			funptr++;
			addEvent(e2,EvQ);
		}

	}

	qT = qtt;
		while(qT != NULL)
		{
			int count = countCust(qT->data->customers);
			qT=qT->next;
		}
}

// ----------------------------------------------------------------------------------------------------------------------------------------





void tellerEventCommon(struct Teller* t1, struct TellerQueue* common)
{

	struct Customer *c1;
	if(countCust(common) > 0)
	{
		c1 = getHeadTellerQueue(common);
		// c1->tserv =  2.0*tavgServ*((float)rand()/(float)(RAND_MAX));
		c1->twait = clock - c1->tarr;
		struct Event* e1 = (struct Event*)malloc(sizeof(struct Event));
		e1->type = 3;
		e1->asC = c1;
		e1->tevent = clock + c1->tserv;
		e1->action3c = serviceCompleted;
		//printf("Function Pointer\n");
		funptr++;
		addEvent(e1,EvQ);
		t1->tserv += c1->tserv;
		struct Event* e2 = (struct Event*)malloc(sizeof(struct Event));
		e2->type = 2;
		e2->asT = t1;
		e2->tevent = clock + c1->tserv;
		e2->action2c = tellerEventCommon;
		//printf("Function Pointer\n");
		funptr++;
		addEvent(e2,EvQ);


	}
	else
	{
			if(clock < tSim)
			{
				struct Event* e1 = (struct Event*)malloc(sizeof(struct Event));
				e1->type = 2;
				e1->asT = t1;
				e1->action2c = tellerEventCommon;
				//printf("Function Pointer\n");
				funptr++;
				e1->tevent = clock + (149 * ((float)rand()/(float)(RAND_MAX)) + 1)/60;
				t1->idleTime += e1->tevent - clock;
				addEvent(e1,EvQ);
			}
	}
}

void addcustTotellerscommon(struct Customer* c1, struct TellerQueue* common)
{
	
	addTellerQueue(c1,common);

}







// ----------------------------------------------------------------------------------------------------------------------------------------

void simulate(struct EventQueue *qE,struct Tellers *qT,struct Customers *qC)
{
	struct Event *e = getHead(qE);
	while(e!=NULL)
	{
		clock = e->tevent;
		if(e->type == 1)
		{
			e->action1(e->asC,qT);
		}
		else if(e->type == 2)
		{
			e->action2(e->asT,qT);
			
		}
		else if(e->type == 3)
		{
			e->action3(e->asC);
			
		}
		free(e);
		e=getHead(qE);

	}
}

void simulatecommon(struct EventQueue *qE,struct TellerQueue *qT)
{
	struct Event *e = getHead(qE);
	while(e!=NULL)
	{	
		clock = e->tevent;
		if(e->type == 1)
		{
			e->action1c(e->asC,qT);
		}
		else if(e->type == 2)
		{
			e->action2c(e->asT,qT);
			
		}
		else if(e->type == 3)
		{
			e->action3c(e->asC);
			
		}

		free(e);
		e=getHead(qE);

	}
}




//-----------------------------------------------------------------------
	
	float avgtot(struct Customers* q1)
	{
		float maxwait=0.0;
		float sum = 0.0;
		float sqsum = 0.0; 
		// printf("Customer ServiceTime\tWaitTime\tTotalTime\n");
		
		while(q1!=NULL)
		{
			// printf("%d\t%f\t%f\t%f\n",i,q1->data->tserv,q1->data->twait,q1->data->ttotal);
			if(maxwait < q1->data->twait)
				maxwait = q1->data->twait;
			sum += q1->data->ttotal;
			sqsum += q1->data->ttotal*q1->data->ttotal;
			q1=q1->next;
		}
		float mean = sum/(float)nC;
		float stddev = sqrt((sqsum/(float)nC) - mean*mean);
		return mean;
	}

	void printCustomerstats(struct Customers* q1)
	{	
		printf("-----------------------\n");
		
		float maxwait=0.0;
		float sum = 0.0;
		float sqsum = 0.0; 
		// printf("Customer ServiceTime\tWaitTime\tTotalTime\n");
		printf("Customer Stats\n");
		while(q1!=NULL)
		{
			// printf("%d\t%f\t%f\t%f\n",i,q1->data->tserv,q1->data->twait,q1->data->ttotal);
			if(maxwait < q1->data->twait)
				maxwait = q1->data->twait;
			sum += q1->data->ttotal;
			sqsum += q1->data->ttotal*q1->data->ttotal;
			q1=q1->next;
		}
		float mean = sum/(float)nC;
		float stddev = sqrt((sqsum/(float)nC) - mean*mean);
		printf("\t\tMax Wait Time of Customer: %f\n",maxwait);
		printf("\t\tMean Total Time of Customer: %f\n",mean);
		printf("\t\tStd Deviation of Total Time of Customer: %f\n",stddev);
	}

	void printTellerstats(struct Tellers* q1)
	{	
		int i=0;
		float sersum = 0.0;
		float idlsum = 0.0;

		// printf("Teller ServiceTime\tIdleTime\n");
		printf("Teller Stats\n");
		while(q1!=NULL)
		{
			// printf("%d\t%f\t%f\n",i,q1->data->tserv,q1->data->idleTime);
			sersum+=q1->data->tserv;
			idlsum+=q1->data->idleTime;
			q1->data->tserv=0;
			q1->data->idleTime=0;
			q1=q1->next;
			i++;
		}
		printf("\t\tTotal Service Time of All Tellers: %f\n",sersum);
		printf("\t\tTotal Idle Time of All Tellers: %f\n",idlsum);
		printf("\t\tAverage	 Service Time of All Tellers: %f\n",sersum/nT);
		printf("\t\tAverage of Idle Time of All Tellers: %f\n",idlsum/nT);
		printf("-----------------------\n");
	}


float callerFormulti(int control)
{	
		EvQ=(struct EventQueue*)malloc(sizeof(struct EventQueue)); 
		EvQ->head=NULL;
		struct Customers* q11 = allCustomers;
		while(q11!=NULL)
		{
			struct Customer* c1 = q11->data;
			struct Event* e1 = (struct Event*)malloc(sizeof(struct Event));
			e1->type = 1;
			e1->tevent = c1->tarr;
			e1->asC = c1;
			e1->action1 = addcustTotellers;
			//printf("Function Pointer\n");
			funptr++;
			addEvent(e1,EvQ);
			q11=q11->next;
			
		}
		struct Tellers* q21 = allTellers;
		while(q21!=NULL)
		{
			struct Teller* t1=q21->data;
			t1->customers->count = 0.0;
			t1->idleTime = 0.0;
			t1->tserv = 0.0;
			struct Event* e1 = (struct Event*)malloc(sizeof(struct Event));
			e1->type = 2;
			e1->tevent = t1->tidle;
			e1->asT = t1;
			e1->action2 = tellerEvent;
			//printf("Function Pointer\n");
			funptr++;
			addEvent(e1,EvQ);
			q21=q21->next;
			
		}
		clock=0.0;

		simulate(EvQ,allTellers,allCustomers);
		free(EvQ->head);
		free(EvQ);

		if(control==0){
			printf("Number of Customers Served: %d\n",nC );
		printf("Total time serve all the customers: %f\n",totservice );
		printf("No. of Tellers: %d\n",nT );
		printf("One Queue per Teller\n");
		printCustomerstats(allCustomers);
		printTellerstats(allTellers);
		return 0.0;
	}
	else
	{
		return avgtot(allCustomers);
	}
		
}

float callerForcommon(int control)
{

		EvQ=(struct EventQueue*)malloc(sizeof(struct EventQueue)); 
		EvQ->head=NULL;

		

		struct Customers* q1 = allCustomers;
		while(q1!=NULL)
		{
			struct Customer* c1 = q1->data;
			c1->tarr=tSim * ((float)rand()/(float)(RAND_MAX));
			struct Event* e1 = (struct Event*)malloc(sizeof(struct Event));
			e1->type = 1;
			e1->tevent = c1->tarr;
			e1->asC = c1;
			e1->action1c = addcustTotellerscommon;
			//printf("Function Pointer\n");
			funptr++;
			addEvent(e1,EvQ);
			q1=q1->next;
			
		}
		struct Tellers* q2 = allTellers;
		while(q2!=NULL)
		{
			struct Teller* t1=q2->data;
			t1->customers->count = 0.0;
			t1->idleTime = 0.0;
			t1->tserv = 0.0;
			struct Event* e1 = (struct Event*)malloc(sizeof(struct Event));
			e1->type = 2;
			e1->tevent = t1->tidle;
			e1->asT = t1;
			e1->action2c = tellerEventCommon;
			//printf("Function Pointer\n");
			funptr++;
			addEvent(e1,EvQ);
			q2=q2->next;
			
		}

		clock = 0.0;
		struct TellerQueue* common = (struct TellerQueue*)malloc(sizeof(struct TellerQueue));
		common->head=NULL;
		common->count=0;
		simulatecommon(EvQ,common);
		free(EvQ->head);
		free(EvQ);
		free(common->head);
		free(common);
		if(control==0)
		{
			printf("Single Queue \n");
			printCustomerstats(allCustomers);
			printTellerstats(allTellers);
			return 0.0;
		}
		else
		{
			return avgtot(allCustomers);
		}

}


int main(int argc, char const *argv[])
{
	nC = atoi(argv[1]);
	nT = atoi(argv[2]);
	tSim = atof(argv[3]);
	tavgServ = atof(argv[4]);
	
	
	allTellers=(struct Tellers*)malloc(sizeof(struct Tellers)); 
	allCustomers=(struct Customers*)malloc(sizeof(struct Customers));
	allCustomers->data = NULL;
	allTellers->data = NULL;
	for(int i=0;i<nC;i++)
	{
		struct Customer* c1=(struct Customer*)malloc(sizeof(struct Customer));
		c1->tarr = tSim * ((float)rand()/(float)(RAND_MAX));
		addCustomer(c1,allCustomers);
	}
	for(int i=0;i<nT;i++)
	{
		struct Teller* t1=(struct Teller*)malloc(sizeof(struct Teller));
		t1->tidle =0;// (599 * ((float)rand()/(float)(RAND_MAX)) + 1)/60;
		t1->customers = (struct TellerQueue*)malloc(sizeof(struct TellerQueue)); 
		t1->customers->head=NULL;
		t1->customers->count = 0;
		t1->idleTime = 0;
		t1->tserv = 0;
		addTeller(t1,allTellers);
	}		
	callerFormulti(0);
	// struct Customers *q112=allCustomers;
	// while(q112!=NULL)
	// {
	// 	// struct Customers *q123=q12;
	// 	q112->data->tarr = tSim * ((float)rand()/(float)(RAND_MAX));
	// 	q112=q112->next;
	// 	// free(q123->data);
	// 	// free(q123);
	// }
	callerForcommon(0);
	
	struct Tellers *tq12=allTellers;
	while(tq12!=NULL)
	{
		struct Tellers *tq123=tq12;
		tq12=tq12->next;
		free(tq123->data->customers);
		free(tq123->data);

		free(tq123);
	}
	allTellers=(struct Tellers*)malloc(sizeof(struct Tellers)); 
	allTellers->data = NULL;
	FILE *data=fopen("data.txt","w");

	for(int i=1;i<=100;i++)
	{
		nT=i;
		struct Teller* t1=(struct Teller*)malloc(sizeof(struct Teller));
		t1->tidle = 0;//(599 * ((float)rand()/(float)(RAND_MAX)) + 1)/60;
		t1->customers = (struct TellerQueue*)malloc(sizeof(struct TellerQueue)); 
		t1->customers->head=NULL;
		t1->customers->count = 0;
		t1->idleTime = 0;
		t1->tserv = 0;
		addTeller(t1,allTellers);
		float ti=callerForcommon(1);
		float ti2=callerFormulti(1);
		// printf("%d\n", i);
		fprintf(data,"%d %f\n",i,ti);

	}
	fclose(data);
	char * commands[] = {"set terminal png\n","set xlabel \"No. Of Tellers\"","set ylabel \"Average of Total Time Spend By a Customer in Bank (min)\"","set output \'output/compare.png\'","plot 'data.txt' using 1:2 w lp title 'Single queue'"};


	FILE * gnuplotPipe = popen ("gnuplot -persistent", "w");
		for (int i=0; i < 5; i++)
	    {
	    fprintf(gnuplotPipe, "%s \n", commands[i]);
	    }

	    fflush(gnuplotPipe);
	    pclose(gnuplotPipe);

	struct Customers *q12=allCustomers;
	while(q12!=NULL)
	{
		struct Customers *q123=q12;
		q12=q12->next;
		free(q123->data);
		free(q123);
	}
	struct Tellers *ttq12=allTellers;
	while(ttq12!=NULL)
	{
		struct Tellers *ttq123=ttq12;
		ttq12=ttq12->next;
		free(ttq123->data->customers);
		free(ttq123->data);

		free(ttq123);
	}

	printf("Total No of Times Function Pointers are used : %d\n", funptr);





	return 0;
}