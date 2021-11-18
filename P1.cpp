
#include<iostream>
#include<string>
#include<cmath>
#include<ctype.h>
#include<stdio.h>
#include"assert.h"
using namespace std;




/******************** Define Class *****************/

const int MaxQueue = 200 ;

typedef int queueElementType;

class Queue
{
public:
	Queue();
	void enqueue(queueElementType e,int init);
	void enqueue_Dropped(queueElementType e);
	queueElementType dequeue();
	queueElementType dequeue_Dropped();
	queueElementType front();
	bool isEmpty();
	bool Dropped_isEmpty();
	bool InWinSize(queueElementType e,int E,int MaxSeq,int WinSize);
private:
	int f;
	int r;
	int f_Dropped;
	int r_Dropped;
	queueElementType packets[MaxQueue];
	queueElementType Dropped[MaxQueue];
};

int nextExpected(int E,int MaxSeq);
bool isInteger(char* a);




/***************** Main Code **************/

int main(int argc,char** argv)
{

	/********************* Errors Checking *****************/


	int error_flag=0;
	int i=4;

	if( argc<5) 			{ cout<<"Invalid number of arguments "; error_flag=1; }

	if( atoi(argv[1]) == 0 || !isInteger(argv[1])) 	{ cout<<"Invalid seqNumBits ";          error_flag=1; }

	if(argc>2)
	{
		if( atoi(argv[2]) == 0 || !isInteger(argv[2]) || ( atoi(argv[2]) > (pow(2,atoi(argv[1]))-1)  && isInteger(argv[1]) ) )	{ cout<<"Invalid winSize ";  error_flag=1; }
	}

	if(argc>3)
	{
		if ( !isInteger(argv[3]) || ( atoi(argv[3]) > (pow(2,atoi(argv[1]))-1)  && isInteger(argv[1]) ) )
		{
			cout<<"Invalid initSeq ";    error_flag=1;
		}
	}


	while(i<=argc-1)
	{
		if ( !isInteger(argv[i]) || ( ( atoi(argv[i]) > pow(2,atoi(argv[1]))-1 ) && ( pow(2,atoi(argv[1]))-1 > 2 ) ) )
		{
			cout<<"Invalid packet ID";
			error_flag=1;
			break;
		}
		i++;
	}


	if (error_flag == 0 )
	{
		Queue R;
		int initSeq = atoi(argv[3]);
	    int winSize = atoi(argv[2]);
		int MaxSeq = pow(2,atoi(argv[1]))-1;
		int Expected =initSeq;
		i=4;

		cout<<"R ";
		while(i <= argc-1)
		{

			if(!R.InWinSize(atoi(argv[i]),Expected,MaxSeq,winSize)) R.enqueue_Dropped(atoi(argv[i]));                                     // Drop packet if out of Window range
			else
			{
				if ( (atoi(argv[i]) < initSeq && atoi(argv[i]) < Expected) ||  !(atoi(argv[i]) < initSeq && atoi(argv[i]) < Expected) )   // Check if packet is Duplicated
				{
					if(atoi(argv[i]) == Expected)
					{
						cout<<atoi(argv[i])<<" ";																						  // Deliver if packet Expected
						Expected = nextExpected(Expected,MaxSeq);
						while(!R.isEmpty())																								  // Dequeue if front is Expected
						{
							if(R.front() == Expected)
							{
								cout<<R.dequeue()<<" ";
								Expected = nextExpected(Expected,MaxSeq);
							}
							else break;
						}
					}
					else R.enqueue(atoi(argv[i]),initSeq);                                                                                 // Enqueue if packet waited for Expected
				}
				else  R.enqueue_Dropped(atoi(argv[i]));																				 	   // Drop if packet is Duplicated
			}

			i++;
		}
		cout<<"E "<<Expected<<" ";

		cout<<"W ";

		while(!R.isEmpty()) 	cout<<R.dequeue()<<" ";

		cout<<"D ";

		while(!R.Dropped_isEmpty())	cout<<R.dequeue_Dropped()<<" ";

	}

}






//************** Define Functions of Class *******************/



int nextExpected(int E,int MaxSeq)
{
	return (E+1)%(MaxSeq+1);
}


Queue::Queue()
{
	f=-1;
	r=-1;
	f_Dropped = -1;
	r_Dropped = -1;
}

void Queue::enqueue(queueElementType e,int init)
{
	int i=0;
	if(f==-1)
	{
		r++;
		f++;
		packets[r]=e;
	}
	else
	{
		for(i=r;i>=f;i--)
		{
			if( (e < init && packets[i] < init) || ( e >= init && packets[i] >= init ) )
			{
				if ( e<packets[i] ) packets[i+1]=packets[i];
				else break;
			}
			else if ( (e < init && packets[i] > init) )
			{
				break;
			}
			else if ( (e > init && packets[i] < init) )
			{
				packets[i+1]=packets[i];
			}
		}
		if ( e == packets[i])
		{
			r_Dropped++;
			Dropped[r_Dropped]=e;
			i++;
			while(i<=r)
			{
				packets[i]=packets[i+1];
				i++;
			}

		}
		else
		{
			packets[i+1]=e;
			r++;
		}
	}


}




void Queue::enqueue_Dropped(queueElementType e)
{
	r_Dropped++;
	Dropped[r_Dropped]=e;
}

queueElementType Queue::dequeue()
{
	int output_index=f;
	if( r == f) f=r=-1;
	else f++;
	return packets[output_index];
}

queueElementType Queue::dequeue_Dropped()
{
	f_Dropped++;
	return Dropped[f_Dropped];
}

queueElementType Queue::front()
{
	assert(f != -1);
	return packets[f];
}

bool Queue::isEmpty()
{
	if (f == r && f == -1) return true;
	else return false;
}

bool Queue::Dropped_isEmpty()
{
	return bool(f_Dropped == r_Dropped);
}


bool Queue::InWinSize(queueElementType e,int E,int MaxSeq,int WinSize)
{
	int win_counter=1;
	while(win_counter <= WinSize)
	{
		if (e == E)
		{
			return true;
		}

		if(E == MaxSeq)	E=0;
		else E++;
		win_counter++;
	}
	return false;
}

bool isInteger(char* a)
{
	if ( a == NULL || *a == '\0') return false;
	while(*a)
	{
		char c = *a;
		if ( ( isalnum(c) && isalpha(c) ) ||  ( !isalnum(c) && !isalpha(c) ) ) return false;
		a++;
	}
	return true;
}


