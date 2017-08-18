#include "info.h"

static long loop;

long cpu_online(void)
{
	long cpus;
	cpus = sysconf(_SC_NPROCESSORS_ONLN);
	return cpus;
}

struct cpu_inf *get_cpu_inf(void)
{
	const char *filename = "/proc/cpuinfo";
	int fd;
	off_t filesize;
	long online_cpu = cpu_online();
	char *buf = NULL;
	struct cpu_inf *cpu_info;
	size_t bufsize = sizeof(struct cpu_inf);//*online_cpu;
	char *line_buf;
	size_t line_size;	
	int cpu_index = 0;
	char *cpu_speed_char;
	int cpu_speed_int;
	cpu_info = malloc(bufsize);
	memset(cpu_info,0,bufsize);
	if ((fd = open(filename,O_RDONLY)) == -1) {
		return (void *)0x0;
	}
	filesize = sizeoffile(filename);
	if (filesize == -1) {}
	if (online_cpu == -1) {}
	if (filesize == 0){
		filesize = 4096;
	}
	buf = malloc(filesize);
	memset(buf,0,filesize);
	if (read(fd,buf,filesize) == -1) {
		free(buf);
		free(cpu_info);
		close(fd);
		return (void *)0x0;
	}
	
	loop = 0;
	while(line_buf=my_getline(buf,filesize,&line_size,1)){
		if (strncmp(line_buf,"cpu MHz",strlen("cpu MHz")) == 0){
			int index = 0;
			while(!(line_buf[strlen("cpu MHz")+index] == '0' || 
				line_buf[strlen("cpu MHz")+index] == '1' ||
				line_buf[strlen("cpu MHz")+index] == '2' ||
				line_buf[strlen("cpu MHz")+index] == '3' ||
				line_buf[strlen("cpu MHz")+index] == '4' ||
				line_buf[strlen("cpu MHz")+index] == '5' ||
				line_buf[strlen("cpu MHz")+index] == '6' ||
				line_buf[strlen("cpu MHz")+index] == '7' ||
				line_buf[strlen("cpu MHz")+index] == '8' ||
				line_buf[strlen("cpu MHz")+index] == '9'
				//line_buf[strlen("cpu MHz")+index] != '0'
				)){
				index++;
			}
			//index --;
			cpu_speed_char = malloc(line_size-strlen("cpu MHz")-index+1);
			memset(cpu_speed_char,0,line_size-strlen("cpu MHz")-index+1);
			memcpy((void *)cpu_speed_char,(const void *)line_buf+strlen("cpu MHz")+index,line_size-strlen("cpu MHz")-index);
			cpu_speed_int = atoi(cpu_speed_char);
			free(cpu_speed_char);
			((struct cpu_inf *)(cpu_info+(cpu_index*sizeof(struct cpu_inf))))->index = cpu_index;
			((struct cpu_inf *)(cpu_info+(cpu_index*sizeof(struct cpu_inf))))->speed = cpu_speed_int;
			cpu_index ++;
			free(line_buf);
			break;
		}
		free(line_buf);
		if (!(cpu_index<online_cpu)){
			break;
		}
	}
	close(fd);
	free(buf);
	
	//free(cpu_info);
	//return (void *)0x0;
	if (cpu_info->speed == 0){
		return NULL;
	}
	return cpu_info;
}

off_t sizeoffile(const char *filename)
{
	if (filename == NULL){
		return -1;
	}
	int fd;
	struct stat buf;
	memset(&buf,0,sizeof(struct stat));
	if ((fd = open(filename,O_RDONLY)) == -1){
		return -1;
	}
	if (fstat(fd,&buf) == -1){
		close(fd);
		return -1;
	}
	close(fd);
	return buf.st_size;
}

char *my_getline(const char *data,ssize_t datasize,size_t *linesize,int flag)
{
	if(data == NULL){
		return NULL;
	}
	//static long loop;
	long loop_1;
	size_t linesize_1 = 0;
	char *linedata = NULL;
	if (flag == 0){
		loop = 0;
	}
	loop_1 = loop;
	if (loop == datasize){
		*linesize =0;
		return NULL;
	}
	for(;loop < datasize; loop++){
		if (data[loop] == '\n'){
			loop++;
			break;
		}
		linesize_1 ++;
	}
	linedata = malloc(linesize_1);
	memset(linedata,0,linesize_1);
	memcpy((void *)linedata,(const void *)&data[loop_1],linesize_1);
	*linesize = linesize_1;
	return linedata;
}

struct mem_inf *get_mem_inf(void)
{
	static int total_mem;
	int free_mem;
	static int total_swap;
	int free_swap;
	const char *filename = "/proc/meminfo";
	struct mem_inf *mem_info;
	char *filebuf;
	int fd;
	char *line_buf;
	size_t line_size;
	
	mem_info = malloc(sizeof(struct mem_inf));
	memset(mem_info,0,sizeof(struct mem_inf));
	filebuf = malloc(4096);
	memset(filebuf,0,4096);
	
	if ((fd = open(filename,O_RDONLY)) == -1) {
		free(filebuf);
		free(mem_info);
		return (void *)0x0;
	}
	if (read(fd,filebuf,4096) == -1) {
		free(filebuf);
		free(mem_info);
		close(fd);
		return (void *)0x0;
	}
	loop = 0;
	while(line_buf=my_getline(filebuf,4096,&line_size,1)){
		
		
		if (strncmp(line_buf,"MemTotal:",strlen("MemTotal:")) == 0){
			//write(STDOUT_FILENO,line_buf,line_size);
			total_mem = get_one_num(line_buf,line_size);
		}
		if (strncmp(line_buf,"MemFree:",strlen("MemFree:")) == 0){
			//write(STDOUT_FILENO,line_buf,line_size);
			free_mem = get_one_num(line_buf,line_size);
		}
		if (strncmp(line_buf,"SwapTotal:",strlen("SwapTotal:")) == 0){
			//write(STDOUT_FILENO,line_buf,line_size);
			total_swap = get_one_num(line_buf,line_size);
		}
		if (strncmp(line_buf,"SwapFree:",strlen("SwapFree:")) == 0){
			//write(STDOUT_FILENO,line_buf,line_size);
			free_swap = get_one_num(line_buf,line_size);
		}
		
		free(line_buf);
	}
	
	close(fd);
	free(filebuf);
	mem_info->total_mem = total_mem;
	mem_info->free_mem = free_mem;
	mem_info->total_swap = total_swap;
	mem_info->free_swap = free_swap;
	return mem_info;

}

int get_one_num(char *data,size_t datalen)
{
	if (data == NULL || datalen < 1) {
		return -1;
	}
	int i;
	for(i=0;i<datalen;i++){
		if (
				*(data+i) == '0' ||
				*(data+i) == '1' ||
				*(data+i) == '2' ||
				*(data+i) == '3' ||
				*(data+i) == '4' ||
				*(data+i) == '5' ||
				*(data+i) == '6' ||
				*(data+i) == '7' ||
				*(data+i) == '8' ||
				*(data+i) == '9' ) {
			return atoi(data+i);
		}
	}
	return -1;
}

int get_one_num1(char *data,size_t datalen)
{
	if (data == NULL){
		return -1;
	}
	if (datalen < 1){
		return -1;
	}
	int ret;
	int end_num_index = 0;
	char *num_string = NULL;
	int num_len = 0;
	int i = 0;
	for (;i< datalen;i++){
		if (data[i] == '0' ||
		    data[i] == '1' ||
		    data[i] == '2' ||
		    data[i] == '3' ||
		    data[i] == '4' ||
		    data[i] == '5' ||
		    data[i] == '6' ||
		    data[i] == '7' ||
		    data[i] == '8' ||
		    data[i] == '9' ) {
			//num_len ++;
			if (!(data[i+1] == '0' ||
			    data[i+1] == '1' ||
			    data[i+1] == '2' ||
			    data[i+1] == '3' ||
			    data[i+1] == '4' ||
			    data[i+1] == '5' ||
			    data[i+1] == '6' ||
			    data[i+1] == '7' ||
			    data[i+1] == '8' ||
			    data[i+1] == '9'
			      )){
				end_num_index = i;
				break;
			}
			num_len ++;
		}
	}
	if (num_len == 0){
		return -1;
	}
	num_string = malloc(num_len+2);
	memset(num_string,0,num_len+2);
	memcpy(num_string,data + end_num_index - num_len,num_len+1);
	ret = atoi(num_string);
	free(num_string);
	return ret;
	
}

double loadavg(void)
{
	const char *filename = "/proc/loadavg";
	char *filebuf = malloc(10);
	int fd;
	double ret;

	memset(filebuf,0,10);
	if ((fd = open(filename,O_RDONLY)) == -1){
		free(filebuf);
		return -1.0;
	}
	if (read(fd,filebuf,10) == -1){
		free(filebuf);
		close(fd);
		return -1.0;
	}
	ret = atof(filebuf);
	free(filebuf);
	close(fd);
	return ret;
}

struct stat_inf *get_cpu_usage(void)
{
	const char *filename = "/proc/stat";
	int fd;
	char *filebuf = NULL;
	size_t real_size;
	struct stat_inf *inf = NULL;
	struct stat_inf *start_inf = NULL;
	char *line_buf;
	size_t line_size;

	if ((fd = open(filename,O_RDONLY)) == -1){
		return NULL;
	}
	filebuf = malloc(7000);
	memset(filebuf,0,7000);
	real_size = read(fd,filebuf,7000);
	if (real_size == -1){
		close(fd);
		free(filebuf);
		return NULL;
	}
	loop = 0;
	int i = 0;
	for(;i <= cpu_online(); i++){
		line_buf = my_getline(filebuf,7000,&line_size,1);
		if (line_buf == NULL){
			break;
		}
		if (inf == NULL){
			inf = malloc(sizeof(struct stat_inf));
			start_inf = inf;
		}else {
			inf->next = malloc(sizeof(struct stat_inf));
			inf = inf->next;
		}
		memset(inf,0,sizeof(struct stat_inf));
		if (strncmp(line_buf,"cpu",3) == 0) {
			set_inf(inf,line_buf,line_size);
		}
		free(line_buf);
	}
	/*while(line_buf=my_getline(filebuf,4096,&line_size,1)){
		

		free(line_buf);
	}
	*/
	free(filebuf);
	close(fd);
	return start_inf;
}



int set_inf(struct stat_inf *inf, const char *data, size_t data_len)
{
	if (inf == NULL || data == NULL || data_len < 3) {
		return -1;
	}
	long t[7] = {-1,-1,-1,-1,-1,-1,-1};
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
	i = 0;
	for(i;i<7;i++){
		if (t[i] == -1){
			return -1;
		}
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


