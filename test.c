
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct stat_inf {
	long user;
	long nice;
	long sys;
	long idle;
	long iowait;
	long irq;
	long softirq;
	struct stat_inf * next;
	//long 
};

int set_inf(struct stat_inf *inf, const char *data, size_t data_len);
void free_single_link(void *link,size_t datasize);

/*
int main(int argc, char *argv[])
{
printf("long size:%d\npoint size:%d\n", sizeof(long),sizeof(void *));
Sleep(5000);

}
*/


int main(int argc,char *argv[])
{
	//char *c1 = "cpu  3394073 0 1454221 121745945 183524 38 27288 0 0 0";
	//char *c2 = "cpu0 3394073 0 1454221 121745945 183524 38 27288 0 0 0";
	char *ss[2] = {
				"cpu  3394073 0 1454221 121745945 183524 38 27288 0 0 0",
				"cpu0 3394074 0 1454221 121745945 183524 38 27288 0 0 0"
	};
	/*
	printf("%s\n%s\n", ss[0], ss[1]);
	Sleep(5000);
	*/
	struct stat_inf *sf = NULL;
	struct stat_inf *start;
	for (int i = 0; i < 2; i++) {
		if (sf == NULL) {
			sf = malloc(sizeof(struct stat_inf));
			start = sf;
		}
		else {
			sf->next = malloc(sizeof(struct stat_inf));
			sf = sf->next;
		}
		memset(sf, 0, sizeof(struct stat_inf));
		set_inf(sf, ss[i], strlen(ss[i]));
	}
	
	sf = start;
	while (sf) {
		for (int i = 0; i < 7; i++) {
			printf("%ld\t", *(long *)((char *)sf + i*sizeof(long)));
		}
		printf("\n");
		sf = sf->next;
	}
	sf = start;
	//free_single_link(sf,sizeof(struct stat_inf));
	while(sf){
		//if (sf){
			start = sf->next;
			free(sf);
			sf=start;
		//}
		
			
	}
	//printf("hello");
	//sleep(5);
    return 0;
}

int set_inf(struct stat_inf *inf, const char *data, size_t data_len)
{
	if (inf == NULL || data == NULL || data_len < 3) {
		return -1;
	}
	long t[7];
	int index = 0;
	int i = 0;
	for (; index < data_len;) {
		if ((*(data + index) == ' ' && *(data + index + 1) == ' ')) {
			index++;
			continue;
		}
		if ((*(data + index) == ' ' && *(data + index + 1) != ' ')) {
			t[i++] = atol(data + index);
			if (i>=7){
				break;
			}
			//printf("%ld\n", t);
			//*(long *)((char *)inf + leek) = t;
			//leek = leek + sizeof(long);
			index++;
			continue;
		}
		index++;
	}
	inf->user = t[0];
	inf->nice = t[1];
	inf->sys = t[2];
	inf->idle = t[3];
	inf->iowait = t[4];
	inf->irq = t[5];
	inf->softirq = t[6];
	return 0;
}


void free_single_link(void *link,size_t datasize)
{
	if (link == NULL && datasize <= 0){
		return;
	}
	if (((char *)link + datasize - sizeof(char *)) == NULL) {
		free(link);
		link = NULL;
		return;
	}
	else {
		free_single_link(((char *)link + datasize - sizeof(char *)), datasize);
	}
	return;
}
