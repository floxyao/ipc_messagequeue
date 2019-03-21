#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include "get_info.h"
#include "const.h"
#include <ctime>
using namespace std;

//Sender 997 sends messages to Receiver 1 and 2.
//Acknowledges messages sent to Receiver 1 and 2.
//Terminates when it observes a random number smaller than 100.
int main() {
  srand(time(NULL));
	int qid = msgget(ftok(".",'u'), 0);
	struct buf {
		long mtype; // required
		int num; // mesg content
    int SID = SENDER_ID_997;
	};
	buf msg; //init
	int size = sizeof(msg)-sizeof(long);
  int random_num = 0;
  int count = 0;
  msg.mtype = 0;
  msg.num = 0;
  bool send997 = true;
  bool rcv2 = true;
  bool rcv1 = true;

  while(send997){
    random_num = rand() % MAX_NUMBER;
    msg.num = random_num;

    if(random_num < 100){
      cout << "******************* Random Number Smaller Than 100 ******************* " << endl;
      send997 = false;
    }
    else{
      if(rcv2){ //Receiver 2 is active, keep receiving messages
        msg.mtype = RECEIVER_TWO;
        msgsnd(qid, (struct msgbuf *)&msg, size, 0);
        cout << getpid() << " Send to R2 (mtype=" << msg.mtype << ") num: " << msg.num << endl << endl;

        msgrcv(qid, (struct msgbuf *)&msg, size, R2_TO_997, 0); // reading
        cout << getpid() << " Received R2 Message: " << msg.num << endl;

        if(msg.num == STOP_R2){ //Receiver 2 has terminated, stop receiving messages
          cout << getpid() << " ********************* R2 HAS STOPPED ********************* " <<endl;
          rcv2 = false;
        }
      }
      if(rcv1){
        //this loop will always be true because this sender only stops when rcvr1 ends
        //else this part of the statement will not be executed once 997 is terminated
        msg.mtype = RECEIVER_ONE;
        msgsnd(qid, (struct msgbuf *)&msg, size, 0);
        cout << getpid() << " Send to R1 (mtype=" << msg.mtype << ") num: " << msg.num << endl;

        msgrcv(qid, (struct msgbuf *)&msg, size, R1_TO_997, 0); // reading
        cout << getpid() << " Received R1 Message: " << msg.num << endl;

        // if(msg.num == STOP_R1){
        //   cout << getpid() << " ********************* R1 HAS STOPPED ********************* " <<endl;
        //   msgrcv(qid, (struct msgbuf *)&msg, size, R1_TO_997, 0); // reading
        //   cout << getpid() << " Received R1 Message: " << msg.num << endl;
        // }
      }
    }
  }
  msg.num = STOP_997; //signal termination to receivers
  msg.mtype = RECEIVER_ONE; //send final message to receiver 1
  msgsnd(qid, (struct msgbuf *)&msg, size, 0);
  cout << "sent final message to r2 msg num = "<<msg.num<<endl;

  msg.mtype = RECEIVER_TWO; //send final message to receiver 2
  msgsnd(qid, (struct msgbuf *)&msg, size, 0);
  cout << "sent final message to r2 msg num = "<<msg.num<<endl;
  cout << getpid() << " Sender 997 done" << endl << endl;

	exit(0);
}
