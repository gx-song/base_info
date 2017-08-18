#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>


struct cpu_inf {
	int index;
	int speed;
	struct cpu_inf * next;	//或许是结构体对齐，暂时没有使用到，保留啦。
};

struct mem_inf {
	int total_mem;
	int free_mem;
	int total_swap;
	int free_swap;
};

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

long cpu_online(void);
struct cpu_inf *get_cpu_inf(void);
off_t sizeoffile(const char *filename);
char *my_getline(const char *data,ssize_t datasize,size_t *linesize,int flag);
struct mem_inf *get_mem_inf(void);
int get_one_num(char *data,size_t datalen);
double loadavg(void);
struct stat_inf *get_cpu_usage(void);
int set_inf(struct stat_inf *inf,const char *data,size_t data_len);
