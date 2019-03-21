#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <errno.h>
#include "get_info.h"
#include "const.h"
using namespace std;

//Receiver 2 creates the message queue and receives messages from Sender 257 and 997.
//This receiver must be executed first so the queue is created.
//Terminates when message count reaches 5000.
int main() {
  int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600); //create queue
  struct buf {
    long mtype; // required
    int num; // mesg content
    int SID = 0;
  };
  buf msg;
  int size = sizeof(msg)-sizeof(long);
  int msg_count = 0;
  bool sndr997 = true;
  bool sndr257 = true;

  while(msg_count <= MAX_MESSAGES){
    cout<< endl <<"Message Count = "<< msg_count <<endl;
    //msgrcv(qid, (struct msgbuf *)&msg, size, RECEIVER_TWO, 0);
    msgrcv(qid, (struct msgbuf *)&msg, size, RECEIVER_TWO, 0);
    cout << getpid() << " Received Number: " << msg.num << endl;
    msg_count++;

    if(msg.SID == SENDER_ID_997){
      cout << getpid() << " SID = 3 " << endl;
      if(msg.num == STOP_997){
        cout << getpid() << " ********************* 997 HAS STOPPED ********************* " <<endl <<endl;
      }

      msg.mtype = R2_TO_997;
      cout << getpid() << " Acknowledging R2_TO_997 " << endl;
      msgsnd(qid, (struct msgbuf *)&msg, size, 0);
    }else{
      msg.mtype = R2_TO_257; // dummy message
      msgsnd(qid, (struct msgbuf *)&msg, size, 0);
    }
    //edit: no else block line43-48 together
  }//while count <= 100

  cout<< endl <<"MESSAGE COUNT CAPPED = "<< msg_count <<endl;
  msg.num = STOP_R2; //received 5000 messages, stop this receiver

  msg.mtype = R2_TO_257; //send final message to 257
  cout<<"sending message to 257 mtype "<<msg.mtype<<" with number: "<<msg.num<<endl;
  msgsnd(qid, (struct msgbuf *)&msg, size, 0);

  msg.mtype = R2_TO_997; //send final message to 997
  cout<<"sending message to 997 mtype "<<msg.mtype<<" with number: "<<msg.num<<endl;
  msgsnd(qid, (struct msgbuf *)&msg, size, 0);

  msg_count++; //increment one more time to get out of the while loop


   cout << getpid() << " Receiver 2 done. Waiting on last message" << endl ;
   msgrcv(qid, (struct msgbuf *)&msg, size, R1_TO_R2, 0);
   msgctl (qid, IPC_RMID, NULL); //delete queue

  exit(0);
}
