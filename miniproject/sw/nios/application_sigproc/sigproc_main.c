/* Signal processing system main file
 * To monitor terminal in powershell:
 * nios2-terminal --device 2 --instance 1

 * To download code to board
 * nios2-download -g SysSigProc.elf --device 2 --instance 1
 */

#include <stdio.h>
#include "system.h"
#include "io.h"
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/alt_irq.h>
#include "altera_avalon_mailbox_simple.h"
#include <altera_avalon_performance_counter.h>

// PIO definitions for the interrupt handling on the switches
#define PIO_IntrSwitch_Data	0
#define PIO_IntrSwitch_IRQEN	4*2
#define PIO_IntrSwitch_IRQFLAG	4*3

// Message storage array
volatile alt_u32 mail[2];
// Task choice variable
volatile int choice = 0;

// ----------------- Function declarations ------------------------------------
// Mailbox functions
void send_callback(void* report, int status);
void send_mail(alt_u32 start_addr, alt_u32 storage_length);
void receive_callback(void* message);
void receive_mail();
// Interrupt and task choice functions
void choose_task(int task);
void isr_switches(void* context);
void setup_switch_interrupts(uint8_t chosen_switches);
// DMA/Custom instruction function
void overall_function(int choice);


// ----------------- Function definitions -------------------------------------

// ----------------- Mailbox functions ----------------------------------------

void send_callback(void* report, int status)
{
	if(!status)
	{
		printf("Sending completed.\n");
	}
	else
	{
		printf("Sending error.\n");
	}
}

void send_mail(alt_u32 start_addr, alt_u32 storage_length)
{
	// Load mailbox
	altera_avalon_mailbox_dev* mailbox_SigProcToAudio = altera_avalon_mailbox_open("/dev/Mailbox_SigProcToAudio", send_callback, NULL);
	// Create mail "envelope"
	alt_u32 mail[2] = {start_addr, storage_length};
	// Send message
	altera_avalon_mailbox_send(mailbox_SigProcToAudio, mail, 0, POLL);
	// Close mailbox
	altera_avalon_mailbox_close(mailbox_SigProcToAudio);
}

void receive_callback(void* message)
{
	if(message != NULL)
	{
		printf("Receiving completed.\n");
	}
	else
	{
		printf("Receiving error.\n");
	}
}

void receive_mail()
{
	// Load mailbox
	altera_avalon_mailbox_dev* mailbox_AudioToSigProc = altera_avalon_mailbox_open("/dev/Mailbox_AudioToSigProc", NULL, receive_callback);

	altera_avalon_mailbox_retrieve_poll(mailbox_AudioToSigProc, mail, 0);
	alt_dcache_flush_all();
}



// ----------------- Interrupt and task choosing functions -------------------------


void choose_task(int task)
{
	if(task == 1)
	{
		// First switch (SW0)
		overall_function(1);
	}
	else if(task == 2)
	{
		// Second switch (SW1)
	}
	else if(task == 4)
	{
		// Third switch (SW2)
		// Etc. add as needed...
	}
}

void isr_switches(void* context)
{
	// Read interrupt source
	uint8_t pinvals = IORD_8DIRECT(PIO_SWITCHES_BASE, PIO_IntrSwitch_IRQFLAG);
	// Save choice
	choice = (int)pinvals;
	// Clear the interrupt flag
	IOWR_8DIRECT(PIO_SWITCHES_BASE, PIO_IntrSwitch_IRQFLAG, pinvals);
}

void setup_switch_interrupts(uint8_t chosen_switches)
{
	// Setup interrupts on the chosen switches
	IOWR_8DIRECT(PIO_SWITCHES_BASE, PIO_IntrSwitch_IRQEN, chosen_switches);
	alt_ic_isr_register(PIO_SWITCHES_IRQ_INTERRUPT_CONTROLLER_ID, PIO_SWITCHES_IRQ, isr_switches, NULL, NULL);
	return;
}

// --------------------- DMA/Custom Instruction function -----------------------

void overall_function(int choice)
{
	receive_mail();
	int total_snippets = mail[1];
	// 48 kHz sampling frequency => 12k samples per snippet
	int words_per_snippet = 12000;
	int handled_snippets = 0;
	int memory_size = total_snippets*words_per_snippet*4;
	int *start_storage_address = mail[0] + memory_size;
	while(handled_snippets < total_snippets)
	{
		receive_mail();
		// Turn on LED 9, signifying that signal processing is in progress
		IOWR_32DIRECT(PIO_LEDS_SHARED_BASE, 0, 512);
		if(choice == 1)
		{
			// TODO: call accelerator
			printf("At call accelerator: handled_snippets = %i\n", handled_snippets);
		}
		else
		{
			// TODO: call C comparison function
		}
		// Turn off LED 9, signifying that signal processing is done
		IOWR_32DIRECT(PIO_LEDS_SHARED_BASE, 0, 0);
	}
	send_mail((alt_u32)start_storage_address, (alt_u32)memory_size);
}

// ----------------------- Main function ----------------------------------

int main()
{
	// Setup interrupts on the first 2 switches
	setup_switch_interrupts(0x3);

	// Wait for switches
	while(1)
	{
		// Poll choice once every millisecond
		usleep(1000);
		if(choice != 0)
		{
			choose_task(choice);
			choice = 0x0;
		}
	};


  	return 0;
}
