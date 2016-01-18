#include "apue.h"
#include <sys/msg.h>
#include <pthread.h>

#define NQ 3
#define MAXMSZ 512
#define KEY 0x123
struct mymesg {
	long mtype;
	char mtext[MAXMSZ];
};
int NUM = 0;
void* sender(void *arg){
	key_t  * pkey = arg;
	long qid;
	struct mymesg m;
	char  str[MAXMSZ];
	int i, n;
	if((qid = msgget(*pkey, 0)) < 0)
		printf("can't open queue key %x\n",*pkey);
	for(i = 0; i < NUM; ++i)
	{
		fprintf(stdout, "in thread,sending msg to key:%x, i: %d\n", *pkey, i);
		memset(&m, 0, sizeof(m));
		n = sprintf(str, "i love potato~ %d\n", i);
		strncpy(m.mtext, str, MAXMSZ-1);
		int nb = strlen(m.mtext);
		m.mtype = 1;
		if(msgsnd(qid, &m, nb, 0) < 0)
			printf("snd error\n");
	}
	 fprintf(stdout, "exit thread\n");
}

int
main(int argc, char *argv[])
{
	 key_t key[NQ];
	pthread_t tid[NQ];

	if (argc != 3) {
		fprintf(stderr, "usage: sendmsg [-n count]\n");
		exit(1);
	}
	int c;
	while((c = getopt(argc, argv,"nt:")) != -1)
	{
		if(c == 'n'){
			NUM = atoi(argv[optind]);
			printf("getopt~~~:%d\n",NUM);
		}
	}
	int i;
	for(i = 0; i < NQ; ++i)
	{
		key[i] = KEY + i;
		fprintf(stdout, "in thread,sending msg to key:%x\n", key[i]);
		if(pthread_create(&tid[i], NULL, sender, &key[i]) != 0)
			fprintf(stdout,"create thread error\n");
		
		
	}
	sleep(2);
	exit(0);
}
	
