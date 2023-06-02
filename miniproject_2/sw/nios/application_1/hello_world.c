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

// Message storage arrays
volatile alt_u32 mail_send[2];
volatile alt_u32 mail_receive[2];

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
// Main function for running the subsystem task
void overall_function(int choice, int run_profiling);
// Comparison functions
void c_function(alt_u32 start_addr, alt_u32 store_addr, alt_u32 word_count, int op_type);
void custom_function(alt_u32 start_addr, alt_u32 store_addr, alt_u32 word_count, int op_type);
// HW Accelerator functions
void HW_Accelerator_start(alt_u32 start_addr, alt_u32 store_addr, alt_u32 word_count, int op_type);
void HW_Accelerator_wait_for_ready();
// ----------------- Function definitions -------------------------------------

// ----------------- Mailbox functions ----------------------------------------

void send_callback(void* report, int status)
{
	if(!status)
	{
		alt_printf("Sending completed, contents are %x and %x \n", mail_send[0], mail_send[1]);
	}
	else
	{
		alt_printf("Sending error.\n");
	}
}

void send_mail(alt_u32 start_addr, alt_u32 storage_length)
{
	//alt_printf("-------------- send_mail ------------- \n");
	// Load mailbox
	altera_avalon_mailbox_dev* mailbox_SigProcToAudio = altera_avalon_mailbox_open("/dev/mailbox_simple_1", NULL, NULL);
	// Create mail "envelope"
	alt_u32 mail_send[2] = {start_addr, storage_length};
	// Send message
	altera_avalon_mailbox_send(mailbox_SigProcToAudio, mail_send, 0, POLL);
	// Close mailbox
	altera_avalon_mailbox_close(mailbox_SigProcToAudio);
}

void receive_callback(void* message)
{
	if(message != NULL)
	{
		alt_printf("Receiving completed, contents are %x and %x \n", mail_receive[0], mail_receive[1]);
	}
	else
	{
		alt_printf("Receiving error.\n");
	}
}

void receive_mail()
{
	//alt_printf("-------------- receive_mail ------------- \n");
	// Load mailbox
	altera_avalon_mailbox_dev* mailbox_AudioToSigProc = altera_avalon_mailbox_open("/dev/mailbox_simple_0", NULL, NULL);
	altera_avalon_mailbox_retrieve_poll(mailbox_AudioToSigProc, mail_receive, 0);
	alt_dcache_flush_all();
}



// ----------------- Interrupt and task choosing functions -------------------------


void choose_task(int task)
{
	//alt_printf("--------------- choose_task ---------------------- \n");
	if(task == 1)
	{
		alt_printf("Performing signal processing on the recording... \n");
		// First switch (SW0)
		overall_function(1, 0);
		//printf("Interrupt test on CPU %d\n", NIOS2_CPU_ID_VALUE);
	}
	else if(task == 2)
	{
		alt_printf("Playing... \n");
		// Second switch (SW1)
		// Play audio, so do nothing here...
		//alt_printf("This subsystem has nothing to do for this task... \n");
	}
	else if(task == 4)
	{
		alt_printf("Running profiling... \n");
		// Third switch (SW2): run with profiling
		// Start overall performance counter
		PERF_RESET(PERFORMANCE_COUNTER_1_BASE);
		PERF_START_MEASURING(PERFORMANCE_COUNTER_1_BASE);
		alt_printf("Running on HW accelerator \n");
		// Run on HW accelerator with profiling
		overall_function(1, 1);
		alt_printf("Running on custom instruction \n");
		// Run on custom instruction with profiling
		overall_function(2, 1);
		alt_printf("Running on c code implementation \n");
		// Run on c code implementation with profiling
		overall_function(3, 1);
		alt_printf("All profiling runs finished \n");
		// Stop overall performance counter and print results
		PERF_STOP_MEASURING(PERFORMANCE_COUNTER_1_BASE);
		perf_print_formatted_report(PERFORMANCE_COUNTER_1_BASE, alt_get_cpu_freq(), 3, "Accelerator", "Custom instr.", "C code");
	}
	else if(task == 8)
	{
		alt_printf("Testing audio... \n");
		// Fourth switch:
		// Audio test, so do nothing here
		//alt_printf("This subsystem has nothing to do for this task... \n");
	}
	alt_printf("Task complete. Ready for new task. \n\n", task);
}

void isr_switches(void* context)
{
	//alt_printf("----------------- isr_switches ---------------------- \n");
	// Read interrupt source
	uint8_t pinvals = IORD_8DIRECT(PIO_2_BASE, PIO_IntrSwitch_IRQFLAG);
	// Save choice
	choice = (int)pinvals;
	// Wait for mail from audio subsystem
	//alt_printf("Waiting for flag mail... \n");
	receive_mail();
	//alt_printf("Flag mail received, choice in mail = %x, own choice = %x \n", mail_receive[0], choice);
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

void HW_Accelerator_start(alt_u32 start_addr, alt_u32 store_addr, alt_u32 word_count, int op_type)
{
	//alt_printf("--------------- HW_Accelerator_start ----------------------- \n");
	// Wait for accelerator to be available
	HW_Accelerator_wait_for_ready();
	//alt_printf("Sending arguments to accelerator \n");
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
	//alt_printf("Accelerator has been started \n");
}

void HW_Accelerator_wait_for_ready()
{
	//alt_printf("------------------ HW_Accelerator_wait_for_ready -------------- \n");
	// Check if the accelerator is already working, and wait until it's done
	int working = 0;
	alt_u32 status;
	status = IORD_32DIRECT(HW_ACCELERATOR_0_BASE, HW_ACC_STATUS);
	// Shift out the 'VolumeMode' bit, leaving only zeros and the 'Working' bit
	working = (int)(status >> 1);
	//alt_printf("Waiting for accelerator to be ready... \n");
	while(working != 0)
	{
		// Wait for the accelerator to be available
		status = IORD_32DIRECT(HW_ACCELERATOR_0_BASE, 12);
		// Shift out the 'VolumeMode' bit, leaving only zeros and the 'Working' bit
		working = (int)(status >> 1);
	}
	//alt_printf("Accelerator is ready \n");
}

// --------------------- Overall subsystem function ----------------------------------------
void overall_function(int choice, int run_profiling)
{
	alt_printf("------------- overall_function ---------------------------- \n");
	int op_type;
	// Receive initial mail, containing starting address and amount of snippets
	receive_mail();
	int total_snippets = mail_receive[1];
	alt_printf("Total snippets are %x \n", total_snippets);
	// 48 kHz sampling frequency => 12k samples per snippet
	int words_per_snippet = 12000;
	int handled_snippets = 0;
	int memory_size = total_snippets*words_per_snippet*4;
	//alt_printf("memory size is %x \n", memory_size);
	int start_storage_address = mail_receive[0] + memory_size;
	// Check if the volume shift switch is set to up or down
	if((IORD_32DIRECT(PIO_2_BASE, 0) & 0x80) == 0)
	{
		op_type = 0;
		alt_printf("Op type 0 chosen \n");
	}
	else
	{
		alt_printf("Op type 1 chosen \n");
		op_type = 1;
	}
	while(handled_snippets < total_snippets)
	{
		receive_mail();
		alt_printf("Loop mail received \n");
		// Prepare parameters
		int snippet_starting_address = mail_receive[0];
		int snippet_storage_address = start_storage_address + handled_snippets*words_per_snippet*4;
		int snippet_word_count = mail_receive[1];
		//alt_printf("Snippet word count is %x \n", snippet_word_count);
		// Turn on LED 9, signifying that signal processing is in progress
		//IOWR_32DIRECT(PIO_LEDS_SHARED_BASE, 0, 512);
		if(choice == 1)
		{
			//alt_printf("At call accelerator: handled_snippets = %x \n", handled_snippets);
			if(run_profiling == 1)
			{
				// Start profiling for the current choice
				PERF_BEGIN(PERFORMANCE_COUNTER_1_BASE, choice);
			}
			HW_Accelerator_start(snippet_starting_address, snippet_storage_address, snippet_word_count, op_type);
			if(run_profiling == 1)
			{
				// Wait for the accelerator to finish
				HW_Accelerator_wait_for_ready();
				// Stop profiling for the current choice
				PERF_END(PERFORMANCE_COUNTER_1_BASE, choice);
			}
			handled_snippets = handled_snippets + 1;
		}
		else if(choice == 2)
		{
			//alt_printf("At call custom instruction: handled_snippets = %x \n", handled_snippets);
			// call comparison function for custom instruction
			if(run_profiling == 1)
			{
				// Start profiling for the current choice
				PERF_BEGIN(PERFORMANCE_COUNTER_1_BASE, choice);
			}
			custom_function(snippet_starting_address, snippet_storage_address, snippet_word_count, op_type);
			if(run_profiling == 1)
			{
				// Stop profiling for the current choice
				PERF_END(PERFORMANCE_COUNTER_1_BASE, choice);
			}
			handled_snippets = handled_snippets + 1;
		}
		else if(choice == 3)
		{
			//alt_printf("At call c comp function: handled_snippets = %x \n", handled_snippets);
			// call comparison function for C code implementation
			if(run_profiling == 1)
			{
				// Start profiling for the current choice
				PERF_BEGIN(PERFORMANCE_COUNTER_1_BASE, choice);
			}
			c_function(snippet_starting_address, snippet_storage_address, snippet_word_count, op_type);
			if(run_profiling == 1)
			{
				// Stop profiling for the current choice
				PERF_END(PERFORMANCE_COUNTER_1_BASE, choice);
			}
			handled_snippets = handled_snippets + 1;
		}
		// Turn off LED 9, signifying that signal processing is done
		//IOWR_32DIRECT(PIO_LEDS_SHARED_BASE, 0, 0);
	}
	alt_printf("All snippets handled. Sending mail with start_storage_address = %x and memory_size = %x \n", start_storage_address, memory_size);
	send_mail(start_storage_address, memory_size);
	//send_mail((alt_u32)start_storage_address, (alt_u32)memory_size);
	alt_printf("Mail sent, done on this side. \n");
	if(run_profiling == 1)
	{
		// Stop profiling for the current choice
		PERF_END(PERFORMANCE_COUNTER_1_BASE, choice);
	}
}

// ------------------ Comparison methods for profiling purposes ----------------------------------

void c_function(alt_u32 start_addr, alt_u32 store_addr, alt_u32 word_count, int op_type)
{
	//alt_printf("------------- c_function ------------------ \n");
	alt_u32 data = 0;
	if(op_type == 0)
	{
		while(word_count > 0)
		{
			// Load data
			data = IORD_32DIRECT(start_addr, 0);
			// Perform operation
			data = (signed)data;
			// Shift right
			data = (data & 0x80000000)+((data & 0xfffffffe)>>1);
			// Store data
			IOWR_32DIRECT(SDRAM_CONTROLLER_2_BASE, (alt_u32)store_addr, (alt_u32)data);
			// Update variables
			start_addr += 4;
			store_addr += 4;
			word_count --;
		}
	}
	else
	{
		while(word_count > 0)
		{
			// Load data
			data = IORD_32DIRECT(start_addr, 0);
			// Perform operation
			data = (signed)data;
			// Shift left
			data = (data << 1);
			// Store data
			IOWR_32DIRECT(SDRAM_CONTROLLER_2_BASE, (alt_u32)store_addr, (alt_u32)data);
			// Update variables
			start_addr += 4;
			store_addr += 4;
			word_count --;
		}
	}
}

void custom_function(alt_u32 start_addr, alt_u32 store_addr, alt_u32 word_count, int op_type)
{
	//alt_printf("------------- custom_function ------------------ \n");
	alt_u32 data = 0;
	while(word_count > 0)
	{
		// Load data
		data = IORD_32DIRECT(start_addr, 0);
		// Perform operation
		data = (signed)data;
		// Use custom instruction
		data = ALT_CI_SIGPROCOPERATION_0(data, op_type);
		// Store data
		IOWR_32DIRECT(SDRAM_CONTROLLER_2_BASE, (alt_u32)store_addr, (alt_u32)data);
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
	//setup_switch_interrupts(0xf);
	alt_printf("\n \n \n \n ---------- main --------------- \n");

	// Wait for switches
	while(1)
	{
		// Use mailbox to start tasks on this subsystem
		//alt_printf("Waiting for choice mail... \n");
		receive_mail();
		//alt_printf("Choice mail received, choice in mail = %x \n", mail_receive[0], choice);
		choice = (int)mail_receive[0];

		// Poll choice once every millisecond
		//usleep(1000);
		if(choice != 0)
		{
			choose_task(choice);
			choice = 0x0;
		}
	};
  	return 0;
}
