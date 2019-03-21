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
using namespace std;
// what other design choices could i have made
// in the sense that i have 3 senders sending info and 2 receivers receviing can i call it a client server
// or does it have to involve sockets
// how it would work in client/server



//Receiver 1 receives messages from Sender 251 and 997.
//Terminates when Sender 251 and 997 terminates.
int main() {
  int qid = msgget(ftok(".",'u'), 0);
  struct buf {
    long mtype; // required
    int num; // mesg content
    int SID = 0;
  };
  buf msg;
  int size = sizeof(msg)-sizeof(long);
  bool send997 = true;
  bool send251 = true;

  //r1 needs to send sdummy messages to 997 cause it stops earli
  while(send251 || send997){ //while 251 or 997 are active, keep receiving
    msgrcv(qid, (struct msgbuf *)&msg, size, RECEIVER_ONE, 0);
    cout << getpid() << " Received Number: " << msg.num << " msg.mtype = " << msg.mtype <<endl;
    cout << getpid() << " Received msg.SID: " << msg.SID <<endl<<endl;

    if(msg.SID == SENDER_ID_997){ //received 997, send ack
      cout << getpid() << " SID = 3 " << endl;
      if(msg.num == STOP_997){
        cout << getpid() << " ********************* 997 HAS STOPPED ********************* " <<endl <<endl;
        send997 = false;
      }
      cout << getpid() << " Acknowledging R1_TO_997 " << endl;
      msg.mtype = R1_TO_997;
      msgsnd(qid, (struct msgbuf *)&msg, size, 0);
      cout << getpid() << " Sent 997 message " << endl;
    }
    if(msg.SID == 0 && msg.num == 0){
      //***CHECK FOR LAST MESSAGE***
      //hard code because get_info
      send251 = false;
    }
  }
  cout<< getpid() << " Sending message to R2 to delete the queue"<<endl;
  msg.mtype = R1_TO_R2;
  msgsnd(qid, (struct msgbuf *)&msg, size, 0);
  cout << getpid() << " Receiver 1 done" << endl << endl;
  exit(0);
}
