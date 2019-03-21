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

//Sender 257 sends messages to Receiver 2
//No Acknowledgement Messages.
//Terminates when 251 terminates.
int main() {
  srand(time(NULL));
	int qid = msgget(ftok(".",'u'), 0);
	struct buf {
		long mtype; // required
		int num; // mesg content
    int SID = SENDER_ID_257;
	};
	buf msg; //init
	int size = sizeof(msg)-sizeof(long);
  int random_num = 0;
  int count = 0;
  bool rcv2 = true;

  while(rcv2){ //While Receiver 2 is active, keep receiving messages
    random_num = rand() % MAX_NUMBER;
    msg.num = random_num;
    msg.mtype = RECEIVER_TWO;

    //send to receiver 2
    cout << getpid() << " Sending number: " << msg.num << endl <<endl;
    msgsnd(qid, (struct msgbuf *)&msg, size, 0);

    msgrcv(qid, (struct msgbuf *)&msg, size, R2_TO_257, 0);
    if (msg.num == STOP_R2){
      cout << getpid() << " ********************* R2 HAS STOPPED ********************* " <<endl;
      rcv2 = false;
    }
    cout << getpid() << " Send to R2 (mtype=" << msg.mtype << ") num: " << msg.num << endl;
    count++;
  }
  cout << getpid() << " Sender 257 done" << endl << endl;
	exit(0);
}
