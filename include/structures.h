
  #ifndef STRUCTURES_H

  #define STRUCTURES_H

struct Customer
{
	float tarr;
	float tserv;
	float twait;
	float ttotal;	
};

struct TellerNode
{
	struct Customer* data;
	struct TellerNode* next;
};

struct TellerQueue
{
	struct TellerNode* head;
	int count;
};

struct Teller
{
	float tidle;
	float tserv;
	float idleTime;
	struct TellerQueue* customers;	

};

struct Tellers
{
	struct Teller* data;
	struct Tellers* next;
};

struct Customers
{
	struct Customer* data;
	struct Customers* next;
};

struct Event{
	int type;
	float tevent;
	void (*action1)(struct Customer*,struct Tellers*);
	void (*action2)(struct Teller*,struct Tellers*);
	void (*action3)(struct Customer*);
	void (*action1c)(struct Customer*,struct TellerQueue*);
	void (*action2c)(struct Teller*,struct TellerQueue*);
	void (*action3c)(struct Customer*);
	struct Customer* asC;
	struct Teller* asT;
};

struct EventNode{
	struct Event* data;
	struct EventNode* next;
};

struct EventQueue
{
	struct EventNode* head;
};

#endif