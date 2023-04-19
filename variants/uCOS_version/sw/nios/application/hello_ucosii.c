#include <stdio.h>
#include "includes.h"
#include "system.h"
#include "io.h"
#include "sys/alt_irq.h"
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_pio_regs.h"

/* Definition of Task Stacks */
#define TASK_STACKSIZE 2048
OS_STK task1_stk[TASK_STACKSIZE];
OS_STK task2_stk[TASK_STACKSIZE];
OS_STK task3_stk[TASK_STACKSIZE];
OS_STK task4_stk[TASK_STACKSIZE];

/* Definition of Task Priorities */
#define TASK1_PRIORITY 1
#define TASK2_PRIORITY 2
#define TASK3_PRIORITY 3
#define TASK4_PRIORITY 4
#define QUEUE_SIZE 10

// Timer 0 macros
#define arbval			0x0000ffff

// Parallel port macros
#define PARPORT_Data	0
#define PARPORT_SetBits	4
#define PARPORT_ClrBits	8
#define PARPORT_IRQEN	12
#define PARPORT_IRQCLR	16

// PIO macros
#define PIO_CLR_EDGES_ADDR	12
#define PIO_ALL_CLR			0xf

OS_EVENT *sem_res;
OS_FLAG_GRP *flag_res;
OS_FLAGS flags;
OS_EVENT *mail_res;
OS_EVENT *queue_res;
//unsigned char c = 0x0f;


typedef struct
{
	unsigned char button_number;
	unsigned char edge;
} msg;

msg* msg_queue[10];
msg some_msg;

unsigned int start,stop;

void task_sem(void* pdata)
{
	INT8U err;
	while (1)
	{
		OSSemPend(sem_res,0,&err);
		IOWR_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE, arbval);
		stop = IORD_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE);
		if(err == OS_NO_ERR)
			printf("Getting semaphore in : %u cycle\n", start - stop);
		else
			printf("Some error has occurred: Not getting semaphore\n");
		// Set channel 0 low
		IOWR_32DIRECT(PARALLELPORT_0_BASE, PARPORT_Data, 0x0);
	 }
}

void sem_isr(void* context)
{
	// Set channel 0 high
	//IOWR_32DIRECT(PARALLELPORT_0_BASE, PARPORT_Data, 0x1);
	// Clear interrupt bit
	IOWR_32DIRECT(PIO_0_BASE, PIO_CLR_EDGES_ADDR, PIO_ALL_CLR);
	INT8U err;
	//unsigned char v = IORD(PIO_0_BASE, 3);
	OSSemPost(sem_res);
	IOWR_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE, arbval);
	start = IORD_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE);
}

void task_flag_or(void* pdata)
{

	INT8U err;
	while (1)
	{
		// OR version
		printf("Testing flag OR version\n");
		OSFlagPend(flag_res, 0x0f, OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME, 0, &err);

		// Set channel 0 low
		IOWR_32DIRECT(PARALLELPORT_0_BASE, PARPORT_Data, 0x0);

		IOWR_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE, arbval);
		stop = IORD_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE);
		if(err == OS_NO_ERR)
			printf("Getting flag in : %u cycle\n", start - stop);
		else
			printf("Not getting flag\n");
	}
}

void flag_isr_or(void* context)
{
	// Set channel 0 high
	IOWR_32DIRECT(PARALLELPORT_0_BASE, PARPORT_Data, 0x1);

	INT8U err;
	unsigned char v = IORD_32DIRECT(PIO_0_BASE, 12);
	v = (v & 0x0f);

	// Clear interrupt
	IOWR_32DIRECT(PIO_0_BASE, 12, 0x0f);


	OSFlagPost(flag_res, 0xff, OS_FLAG_SET, &err);
	IOWR_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE, arbval);
	start = IORD_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE);
}

void task_flag_and(void* pdata)
{

	INT8U err;
	while (1)
	{
		// AND version
		printf("Testing flag AND version\n");
		OSFlagPend(flag_res, 0xf, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);

		// Set channel 0 low
		IOWR_32DIRECT(PARALLELPORT_0_BASE, PARPORT_Data, 0x0);

		IOWR_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE, arbval);
		stop = IORD_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE);
		if(err == OS_NO_ERR)
			printf("Getting flag in : %u cycle\n", start - stop);
		else
			printf("Not getting flag\n");
		OSFlagPost(flag_res, 0xf, OS_FLAG_CLR, &err);
	}
}

void flag_isr_and(void* context)
{
	// Set channel 0 high
	IOWR_32DIRECT(PARALLELPORT_0_BASE, PARPORT_Data, 0x1);

	INT8U err;
	unsigned char v = IORD_32DIRECT(PIO_0_BASE, 12);
	v = (v & 0x0f);
	OSFlagPost(flag_res, v, OS_FLAG_SET, &err);
	IOWR_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE, arbval);
	start = IORD_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE);
	IOWR_32DIRECT(PIO_0_BASE, 12, 0x0f);
}

void task_mail(void* pdata)
{
	INT8U err;
	msg* bla;

	while (1)
	{
		bla = (struct msg*)OSMboxPend(mail_res, 0, &err);
		IOWR_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE, 9);
		stop = IORD_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE);
		if(err == OS_NO_ERR)
			printf("Getting message in : %u cycles with button number %u and edge %02X\n",start - stop, bla->button_number, bla->edge);
		else
			printf("Not getting message\n");
	}
}

void mail_isr(void* context)
{
	INT8U err;

	unsigned char v = IORD(PIO_0_BASE, 3);
	some_msg.button_number = v;
	some_msg.edge = ((IORD(PIO_0_BASE,0)&0x1) == 0) ? 0x80 : 0;
	OSMboxPost(mail_res, &some_msg);
	IOWR_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE, 9);
	start = IORD_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE);
	// Clear interrupt bit
	IOWR_32DIRECT(PIO_0_BASE, PIO_CLR_EDGES_ADDR, PIO_ALL_CLR);
}

void task_queue(void* pdata)
{
	INT8U err;

	msg* bla;

	while (1)
	{
		bla = (struct msg*)OSQPend(queue_res, 0, &err);
		IOWR_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE, arbval);
		stop = IORD_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE);
		if(err == OS_NO_ERR)
			printf("Getting message from queue in : %u cycles with button number %u and edge %02X\n", start - stop, bla->button_number, bla->edge);
		else
			printf("Not getting message\n");
	}
}
void queue_isr(void* context)
{
	INT8U err;
	unsigned char v = IORD(PIO_0_BASE, 3);
	some_msg.button_number = v;
	some_msg.edge = ((IORD(PIO_0_BASE,0)&0x1) == 0) ? 0x80 : 0;
	OSQPost(queue_res, &some_msg);
	IOWR_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE, arbval);
	start = IORD_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE);

	// Clear interrupt bit
	IOWR_32DIRECT(PIO_0_BASE, 12, 0xf);
}

int main(void)
{
	IOWR_32DIRECT(TIMER_1_BASE, 4, 6);

	IOWR_32DIRECT(PARALLELPORT_0_BASE, PARPORT_Data, 0x0);
	INT8U err;
	//sem_res = OSSemCreate(1);
	//flag_res = OSFlagCreate(0, &err);
	//mail_res = OSMboxCreate(NULL);
	queue_res = OSQCreate(msg_queue, QUEUE_SIZE);

	// Initialize timer
	IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_0_BASE, 0xffff);
	IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_0_BASE, 0xffff);
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE,2);
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE,6);

	//alt_ic_isr_register(PIO_0_IRQ_INTERRUPT_CONTROLLER_ID, PIO_0_IRQ, sem_isr, NULL, NULL);

	//IOWR_ALTERA_AVALON_PIO_IRQ_MASK(PIO_0_BASE, 0x0f);
	//alt_ic_isr_register(PIO_0_IRQ_INTERRUPT_CONTROLLER_ID, PIO_0_IRQ, flag_isr_or, NULL, NULL);
	//alt_ic_isr_register(PIO_0_IRQ_INTERRUPT_CONTROLLER_ID, PIO_0_IRQ, flag_isr_and, NULL, NULL);

	//alt_ic_isr_register(PIO_0_IRQ_INTERRUPT_CONTROLLER_ID, PIO_0_IRQ, mail_isr, NULL, NULL);

	alt_ic_isr_register(PIO_0_IRQ_INTERRUPT_CONTROLLER_ID, PIO_0_IRQ, queue_isr, NULL, NULL);

	IOWR(PIO_0_BASE,3,0x0f);
	IOWR(PIO_0_BASE,2,0x0f);


	//OSTaskCreateExt(task_sem, NULL, (void *)&task1_stk[TASK_STACKSIZE-1], TASK1_PRIORITY, TASK1_PRIORITY, task1_stk, TASK_STACKSIZE, NULL, 0);

	//OSTaskCreateExt(task_flag_or, NULL, (void *)&task2_stk[TASK_STACKSIZE-1], TASK2_PRIORITY, TASK2_PRIORITY, task2_stk, TASK_STACKSIZE, NULL, 0);
	//OSTaskCreateExt(task_flag_and, NULL, (void *)&task2_stk[TASK_STACKSIZE-1], TASK2_PRIORITY, TASK2_PRIORITY, task2_stk, TASK_STACKSIZE, NULL, 0);

	//OSTaskCreateExt(task_mail, NULL, (void *)&task3_stk[TASK_STACKSIZE-1], TASK3_PRIORITY, TASK3_PRIORITY, task3_stk, TASK_STACKSIZE,  NULL, 0);

	OSTaskCreateExt(task_queue, NULL, (void *)&task4_stk[TASK_STACKSIZE-1], TASK4_PRIORITY, TASK4_PRIORITY, task4_stk, TASK_STACKSIZE, NULL, 0);

	OSStart();
	return 0;
 }
