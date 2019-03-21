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

//Sender 251 sends messages to Receiver 1
//No Acknowledgement Messages.
//Terminates on kill command.
int main() {
	srand(time(NULL));
	int qid = msgget(ftok(".",'u'), 0);
	struct buf {
		long mtype; // required
		int num; // mesg content
		int SID = SENDER_ID_251;
	};
	buf msg, *msg2; //init
	int size = sizeof(msg)-sizeof(long);
	int random_num = 0;

	msg.num = STOP;
	msg.mtype = RECEIVER_ONE;
	get_info(qid, (struct msgbuf *)&msg, size, RECEIVER_ONE);
	cout << getpid() << " get info" << msg.mtype << ") num: " << msg.num << endl;

  while(true){ //not good practice, but in this program i think it's okay (loop until killed)
		random_num = rand() % MAX_NUMBER;
    msg.num = random_num;
    msg.mtype = RECEIVER_ONE;
    msgsnd(qid, (struct msgbuf *)&msg, size, 0); //keep sending messages
		cout << getpid() << " Send to R1 (mtype=" << msg.mtype << ") num: " << msg.num << endl;
	}
	exit(0);
}
