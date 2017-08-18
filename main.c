#include "info.h"

int main(int argc,char *argv[])
{


/*
*	char *hello = "hello\nboy\ngirl!";
*	size_t linesize;
*	char *line;
*	while(line = getline(hello,strlen(hello),&linesize,1)){
*		write(STDOUT_FILENO,line,linesize);
*		free(line);
*	}
*/

	long cpu_onlines = cpu_online();
	int loop = 0;
	struct cpu_inf *cpuinfo = get_cpu_inf();
	/*while(loop < cpu_onlines){
	*	printf("cpu index:%d\tcpu speed:%d\n",
	*		((struct cpu_inf *)(cpuinfo+(loop*sizeof(struct cpu_inf))))->index,
	*		((struct cpu_inf *)(cpuinfo+(loop*sizeof(struct cpu_inf))))->speed
	*	);
	*	loop ++;
	*}
	*/
	printf("cpu_num:%d\t,cpu_speed:%d\n",cpu_onlines,cpuinfo->speed);
	free(cpuinfo);
	struct mem_inf *mem_info = get_mem_inf();
	printf("total_mem:%d\tfree_mem:%d\ttotal_swap:%d\tfree_swap:%d\n",
		mem_info->total_mem,mem_info->free_mem,mem_info->total_swap,mem_info->free_swap);
	free(mem_info);

	struct stat_inf *inf,*start;
	inf = get_cpu_usage();
	start = inf;
	
	while(inf){
		printf("user:%ld,nice:%ld,sys:%ld,idle:%ld,iowait:%ld,irq:%ld,softirq:%ld\n",
			inf->user,inf->nice,inf->sys,inf->idle,inf->iowait,inf->irq,inf->softirq);
		inf = inf->next;
	}
	inf = start;
	while(inf){
		start = inf->next;
		free(inf);
		inf = start;
	}

/*	do{
		printf("usage:%ld\n",inf->user);
		inf = inf->next;
	}while(inf);
	inf = start;
*/
	return 0;
}
