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

// REMEMBER TO CHANGE THE CPU ID VALUE IN SYSTEM.H AFTER GENERATING BSP

// To monitor terminal in powershell:
// nios2-terminal --device 2 --instance 1

// To download code to board, change to .elf file directory and run:
// nios2-download -g SysSigProc.elf --device 2 --instance 1

// PIO definitions for the interrupt handling on the switches
#define PIO_IntrSwitch_Data	0
#define PIO_IntrSwitch_IRQEN	4*2
#define PIO_IntrSwitch_IRQFLAG	4*3

// HW Accelerator address offset definitions for writing
#define HW_ACC_STARTREADADDR	0
#define HW_ACC_STARTWRITEADDR	4
#define HW_ACC_WORDCOUNT		8
#define HW_ACC_VOLUMEMODE		12
#define HW_ACC_START			16
// HW Accelerator address offset definitions for reading
#define HW_ACC_STATUS			12

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
void choose_task(int task, int run_profiling);
void isr_switches(void* context);
void setup_switch_interrupts(uint8_t chosen_switches);
// Main function for running the subsystem task
void overall_function(int choice, int run_profiling);
// Comparison function
void comp_functions(int function_choice, alt_u32 *start_addr, alt_u32 *store_addr, alt_u32 word_count, int op_type);
// HW Accelerator functions
void HW_Accelerator_start(alt_u32 *start_addr, alt_u32 *store_addr, alt_u32 word_count, int op_type);
void HW_Accelerator_wait_for_ready();
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
		overall_function(1, 0);
		//printf("Interrupt test on CPU %d\n", NIOS2_CPU_ID_VALUE);
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
	else if(task == 8)
	{
		// Fourth switch: run with profiling
		// Start overall performance counter
		PERF_RESET(PERFORMANCE_COUNTER_1_BASE);
		PERF_START_MEASURING(PERFORMANCE_COUNTER_1_BASE);

		// Run on HW accelerator with profiling
		overall_function(1, 1);
		// Run on custom instruction with profiling
		overall_function(2, 1);
		// Run on c code implementation with profiling
		overall_function(3, 1);

		// Stop overall performance counter and print results
		PERF_STOP_MEASURING(PERFORMANCE_COUNTER_0_BASE);
		perf_print_formatted_report(PERFORMANCE_COUNTER_0_BASE, alt_get_cpu_freq(), 3, "Accelerator", "Custom instr.", "C code");
	}
}

void isr_switches(void* context)
{
	// Read interrupt source
	uint8_t pinvals = IORD_8DIRECT(PIO_2_BASE, PIO_IntrSwitch_IRQFLAG);
	// Save choice
	choice = (int)pinvals;
	// Clear the interrupt flag
	IOWR_8DIRECT(PIO_2_BASE, PIO_IntrSwitch_IRQFLAG, pinvals);
}

void setup_switch_interrupts(uint8_t chosen_switches)
{
	// Setup interrupts on the chosen switches
	IOWR_8DIRECT(PIO_2_BASE, PIO_IntrSwitch_IRQEN, chosen_switches);
	alt_ic_isr_register(PIO_2_IRQ_INTERRUPT_CONTROLLER_ID, PIO_2_IRQ, isr_switches, NULL, NULL);
	return;
}

// --------------------- Hardware accelerator functions -----------------------

void HW_Accelerator_start(alt_u32 *start_addr, alt_u32 *store_addr, alt_u32 word_count, int op_type)
{
	// Wait for accelerator to be available
	HW_Accelerator_wait_for_ready();
	// Send starting address for reading
	IOWR_32DIRECT(HW_ACCELERATOR_0_BASE, HW_ACC_STARTREADADDR, start_addr);
	// Send starting address for writing
	IOWR_32DIRECT(HW_ACCELERATOR_0_BASE, HW_ACC_STARTWRITEADDR, store_addr);
	// Send word count
	IOWR_32DIRECT(HW_ACCELERATOR_0_BASE, HW_ACC_WORDCOUNT, word_count);
	// Send volume mode
	IOWR_32DIRECT(HW_ACCELERATOR_0_BASE, HW_ACC_VOLUMEMODE, (alt_u32)op_type);
	// Run Hardware accelerator
	IOWR_32DIRECT(HW_ACCELERATOR_0_BASE, HW_ACC_WORDCOUNT, 0x1);
}

void HW_Accelerator_wait_for_ready()
{
	// Check if the accelerator is already working, and wait until it's done
	int working = 0;
	alt_u32 status;
	status = IORD_32DIRECT(HW_ACCELERATOR_0_BASE, HW_ACC_STATUS);
	// Shift out the 'VolumeMode' bit, leaving only zeros and the 'Working' bit
	working = (int)(status >> 1);
	while(working != 0)
	{
		// Wait for the accelerator to be available
		status = IORD_32DIRECT(HW_ACCELERATOR_0_BASE, 12);
		// Shift out the 'VolumeMode' bit, leaving only zeros and the 'Working' bit
		working = (int)(status >> 1);
	}
}

// --------------------- Overall subsystem function ----------------------------------------
void overall_function(int choice, int run_profiling)
{
	int op_type;
	receive_mail();
	// Check if the volume shift switch is set to up or down
	if((IORD_32DIRECT(PIO_2_BASE, 0) & 0x100) == 0)
	{
		op_type = 0;
	}
	else
	{
		op_type = 1;
	}
	int total_snippets = mail[1];
	// 48 kHz sampling frequency => 12k samples per snippet
	int words_per_snippet = 12000;
	int handled_snippets = 0;
	int memory_size = total_snippets*words_per_snippet*4;
	int *start_storage_address = mail[0] + memory_size;
	if(run_profiling == 1)
	{
		// Start profiling for the current choice
		PERF_BEGIN(PERFORMANCE_COUNTER_1_BASE, choice);
	}
	while(handled_snippets < total_snippets)
	{
		receive_mail();
		// Prepare parameters
		int *snippet_starting_address = mail[0];
		int *snippet_storage_address = start_storage_address + handled_snippets*words_per_snippet*4;
		int snippet_word_count = mail[1];
		// Turn on LED 9, signifying that signal processing is in progress
		//IOWR_32DIRECT(PIO_LEDS_SHARED_BASE, 0, 512);
		if(choice == 1)
		{
			printf("At call accelerator: handled_snippets = %i \n", handled_snippets);
			HW_Accelerator_start(snippet_starting_address, snippet_storage_address, snippet_word_count, op_type);
		}
		else if(choice == 2)
		{
			// call comparison function for custom instruction
			comp_functions(1, snippet_starting_address, snippet_storage_address, snippet_word_count, op_type);
		}
		else if(choice == 3)
		{
			// call comparison function for C code implementation
			comp_functions(0, snippet_starting_address, snippet_storage_address, snippet_word_count, op_type);
		}
		// Turn off LED 9, signifying that signal processing is done
		//IOWR_32DIRECT(PIO_LEDS_SHARED_BASE, 0, 0);
	}
	send_mail((alt_u32)start_storage_address, (alt_u32)memory_size);
	if(run_profiling == 1)
	{
		// Stop profiling for the current choice
		PERF_END(PERFORMANCE_COUNTER_1_BASE, choice);
	}
}

// ------------------ Comparison methods for profiling purposes ----------------------------------

void comp_functions(int function_choice, alt_u32 *start_addr, alt_u32 *store_addr, alt_u32 word_count, int op_type)
{
	alt_u32 data;
	while(word_count > 0)
	{
		// Load data
		data = IORD_32DIRECT(start_addr, 0);
		// Perform operation
		data = (signed)data;
		if(function_choice == 0)
		{
			// Use c code implementation
			if(op_type == 0)
			{
				data = (data >> 1);
			}
			else
			{
				data = (data << 1);
			}
		}
		elif(function_choice == 1)
		{
			// Use custom instruction
			data = ALT_CI_SIGPROCOPERATION_0(data, op_type);
		}
		// Store data
		IOWR_32DIRECT(store_addr, 0, (alt_u32)data);
		// Update variables
		start_addr += 4;
		store_addr += 4;
		word_count --;
	}
}

// ----------------------- Main function ----------------------------------

int main()
{
	// Setup interrupts on the first 4 switches
	setup_switch_interrupts(0x7);


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
