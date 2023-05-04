//202135744_±èÀÇÁø
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

struct PCB {
	int pid;
	int begin;
	int burst;
	int end;
	int wait;
	int run;
	int running;
	int response;
	int starting;
};
int process[50];
int method_schedule = 0;
int process_number = 0;
struct PCB* p = NULL;
void read_proc_list(const char* file_name) {
	FILE* fp = fopen(file_name, "r");
	//first line => process number read
	fscanf(fp, "%d", &process_number);
	//data read
	struct PCB* data = (struct PCB*)malloc(process_number*sizeof(struct PCB));
	for (int i = 0; i < process_number; i++) {
		fscanf(fp, "%d %d %d", &data[i].pid, &data[i].begin, &data[i].burst);
		data[i].end = 0;
		data[i].wait = 0;
		data[i].run = 0;
		data[i].running = 0;
		data[i].response = 0;
		data[i].starting = 0;
	}
	//point save
	p = data;
	//ready queue
	struct PCB* ready = (struct PCB*)malloc(process_number * sizeof(struct PCB));
	int ready_queue_start = 0;
	int ready_queue_end = 0;
	int end_process = 0;
	//basis
	int full = 0;
	struct PCB temp;
	for (int i = 0; i < 50; i++) {
		if (method_schedule == 1) {
			//fcfs
			if (data[ready_queue_end].begin == i) {
				ready[ready_queue_end] = data[ready_queue_end];
				ready_queue_end++;
			}//ready input

			if (ready[end_process].run == ready[end_process].burst) {
				ready[end_process].end = i;
				ready[end_process].running = 0;
				end_process++;
				full = 0;
			}//end_process

			if (full == 0 && !(ready_queue_start == ready_queue_end)){
				ready[ready_queue_start].running = 1;
				if (ready[ready_queue_start].run == 0) {
					ready[ready_queue_start].starting = i;
				}
				process[i] = ready[ready_queue_start].pid;
				ready_queue_start++;
				full = 1;
			}//process input
			else if (full == 0 && (ready_queue_start == ready_queue_end)) {
				process[i] = -1;
			}
			else {
				process[i] = process[i - 1];
			}

			for (int k = 0; k < ready_queue_end; k++) {
				if (ready[k].running == 1) {
					ready[k].run += 1;
				}
				else {
					ready[k].wait += 1;
				}
			}//running
		}
		else if (method_schedule == 2) {
			//sjf
			if (data[ready_queue_end].begin == i) {
				ready[ready_queue_end] = data[ready_queue_end];
				ready_queue_end++;
				if (ready_queue_end > 2) {
					if (ready[ready_queue_end - 1].burst < ready[ready_queue_end - 2].burst) {
						temp = ready[ready_queue_end - 1];
						ready[ready_queue_end - 1] = ready[ready_queue_end - 2];
						ready[ready_queue_end - 2] = temp;
					}
				}//sorting because of next running process
			}//ready input

			if (ready[end_process].run == ready[end_process].burst) {
				ready[end_process].end = i;
				ready[end_process].running = 0;
				end_process++;
				full = 0;
			}//end_process

			if (full == 0 && !(ready_queue_start == ready_queue_end)) {
				ready[ready_queue_start].running = 1;
				if (ready[ready_queue_start].run == 0) {
					ready[ready_queue_start].starting = i;
				}
				process[i] = ready[ready_queue_start].pid;
				ready_queue_start++;
				full = 1;
			}//process input
			else if (full == 0 && (ready_queue_start == ready_queue_end)) {
				process[i] = -1;
			}
			else {
				process[i] = process[i - 1];
			}

			for (int k = 0; k < ready_queue_end; k++) {
				if (ready[k].running == 1) {
					ready[k].run += 1;
				}
				else {
					ready[k].wait += 1;
				}
			}//running
		}
		else if (method_schedule == 3) {
			//sptf
			if (data[ready_queue_end].begin == i) {
				ready[ready_queue_end] = data[ready_queue_end];
				ready_queue_end++;
				if (ready_queue_end - ready_queue_start > 1) {
					for (int k = 0; k < ready_queue_end - 1; k++) {
						for (int l = ready_queue_start; l < ready_queue_end - 1 - k; l++) {
							if (ready[l].burst - ready[l].run > ready[l + 1].burst - ready[l+1].run) {
								temp = ready[l];
								ready[l] = ready[l + 1];
								ready[l + 1] = temp;
							}
						}
					}//sorting by burst
					for (int k = ready_queue_start; k < ready_queue_end ; k++) {
						if (k == ready_queue_start) {
							ready[k].running = 1;
						}
						else {
							ready[k].running = 0;
						}
					}//running update
				}
			}//ready input

			if (ready[ready_queue_start].run == ready[ready_queue_start].burst) {
				ready[ready_queue_start].end = i;
				ready[ready_queue_start].running = 0;
				ready_queue_start++;
			}//end_process

			if (!(ready_queue_start == ready_queue_end)) {
				ready[ready_queue_start].running = 1;
				if (ready[ready_queue_start].run == 0) {
					ready[ready_queue_start].starting = i;
				}
				process[i] = ready[ready_queue_start].pid;
			}
			else {
				process[i] = -1;
			}
			//process input

			for (int k = 0; k < ready_queue_end; k++) {
				if (ready[k].running == 1) {
					ready[k].run += 1;
				}
				else {
					ready[k].wait += 1;
				}
			}//running
		}
		else {
			//error
		}
	}
	p = ready;
}
void set_schedule(int method) {
	if (method == 1) {
		//fcfs
		method_schedule = 1;
	}
	else if (method == 2) {
		//sjf
		method_schedule = 2;
	}
	else if (method == 3) {
		//sptf
		method_schedule = 3;
	}
	else {
		//error
		method_schedule = 0;
	}
}
int do_schedule(int tick) {
	return process[tick];
}
void print_performance() {
	struct PCB temp;
	printf("===================================================\n");
	for (int i = 0; i < process_number -1; i++) {
		for (int k = 0; k < process_number -1 - i; k++) {
			if (p[k].pid > p[k + 1].pid) {
				temp = p[k];
				p[k] = p[k + 1];
				p[k + 1] = temp;
			}
		}
	}
	for (int i = 0; i < process_number; i++) {
		int waiting = p[i].end - p[i].begin - p[i].burst;
		int trun_time = p[i].end - p[i].begin;
		int response = p[i].starting - p[i].begin;
		printf("pid = %d begin = %d finish = %d trun_time = %d waiting = %d response= %d\n",
			p[i].pid,p[i].begin,p[i].end ,trun_time, waiting, response);
	}
}