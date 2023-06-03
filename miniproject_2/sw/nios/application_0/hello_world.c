/* Audio system main file
 * To monitor terminal in powershell:
 * nios2-terminal --device 2 --instance 0

 * To download code to board
 * nios2-download -g SysAudio.elf --device 2 --instance 0
 */

#include <stdio.h>
#include "system.h"
#include "io.h"
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/alt_irq.h>
#include "altera_avalon_mailbox_simple.h"
#include <altera_avalon_performance_counter.h>
#include "altera_up_avalon_audio.h"
#include "altera_up_avalon_audio_and_video_config.h"

// REMEMBER TO CHANGE "LEGACY" TO "ENHANCED" for #define ALT_<X>_INTERRUPT_API_PRESENT IN SYSTEM.H AFTER GENERATING BSP

// To monitor terminal in powershell:
// nios2-terminal --device 2 --instance 0

// To download code to board, change to .elf file directory and run:
// nios2-download -g SysAudio.elf --device 2 --instance 0

// PIO definitions for the interrupt handling on the switches
#define PIO_IntrSwitch_Data	0
#define PIO_IntrSwitch_IRQEN	4*2
#define PIO_IntrSwitch_IRQFLAG	4*3

// Audio Core registers
#define AudioControlReg	0
#define AudioFifoSpaceReg	4
#define AudioLData	8
#define AudioRData	12

// Initiate audio_ready_flag
volatile int audio_ready_flag = 0;
// Playback memory location storage array
volatile alt_u32 ready_audio[2];

// Message storage arrays
volatile alt_u32 mail_send[2];
volatile alt_u32 mail_receive[2];

// Volatile variables to prevent overzealous optimization at compilation
volatile int choice = 0;
volatile alt_u32 dataword = 0;
volatile float error_rate;
volatile float success_rate;

// Setup audio core
alt_up_audio_dev * audio_dev;

// ----------------- Function declarations ------------------------------------
// Mailbox functions
void send_callback(void* report, int status);
void send_mail(alt_u32 start_addr, alt_u32 storage_length);
void receive_callback(void* message);
void receive_mail();
// Audio setup functions
void setup_audio(alt_up_audio_dev * audio_dev);
void check_config_status();
void reset_config_device();
void check_core_status();
void reset_core();
// Audio usage functions
void record_process(int seconds, int test, int run_profiling);
void record_audio_snippet(int start_addr_ptr, int record_words);
void play_audio(int start_addr_ptr, int memory_size);
int test_audio_processing(alt_u32 start_recorded, alt_u32 start_processed, alt_u32 memspace);
// Interrupt and task choosing functions
void choose_task(int task);
void isr_switches(void* context);
void setup_switch_interrupts(uint8_t chosen_switches);
// Misc functions
int min(int a, int b);
// ------------------- End of function declarations ---------------------------

// -------------------------- Function definitions ----------------------------

// Mailbox functions

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
	// Load mailbox
	altera_avalon_mailbox_dev* mailbox_AudioToSigProc = altera_avalon_mailbox_open("/dev/mailbox_simple_0", NULL, NULL);
	// Create mail "envelope"
	alt_u32 mail_send[2] = {start_addr, storage_length};
	// Send message
	altera_avalon_mailbox_send(mailbox_AudioToSigProc, mail_send, 0, POLL);
	// Close mailbox
	altera_avalon_mailbox_close(mailbox_AudioToSigProc);
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
	// Open mailbox
	altera_avalon_mailbox_dev* mailbox_SigProcToAudio = altera_avalon_mailbox_open("/dev/mailbox_simple_1", NULL, NULL);
	// Wait for mail confirming the processing is complete
	altera_avalon_mailbox_retrieve_poll(mailbox_SigProcToAudio, mail_receive, 0);
	alt_dcache_flush_all();
}

// Audio setup functions

void setup_audio(alt_up_audio_dev * audio_dev)
{
	// open the Audio port
	audio_dev = alt_up_audio_open_dev ("/dev/audio_0");
	if ( audio_dev == NULL)
		alt_printf("Error: could not open audio device \n");
	else
		alt_printf("Opened audio device \n");
}

void check_config_status()
{
	alt_printf("Checking the settings of the Audio and Video Config device... \n");
	alt_up_av_config_dev* config_device = alt_up_av_config_open_dev(AUDIO_AND_VIDEO_CONFIG_0_NAME);
	alt_u32 status = IORD_32DIRECT(AUDIO_AND_VIDEO_CONFIG_0_BASE, 4);
	alt_printf("The status register is %x \n", status);
	int ready = alt_up_av_config_read_ready(config_device);
	alt_printf("The Ready bit is %x \n", ready);
	int ackbit = alt_up_av_config_read_acknowledge(config_device);
	alt_printf("The Acknowledge bit is %x \n", ackbit);
}

void reset_config_device()
{
	alt_up_av_config_dev* config_device = alt_up_av_config_open_dev(AUDIO_AND_VIDEO_CONFIG_0_NAME);
	alt_printf("Resetting Audio and Video Config device... \n");
	int reset = alt_up_av_config_reset(config_device);
	if(reset == 0)
	{
		alt_printf("Successfully reset! \n");
	}
	else
	{
		alt_printf("Reset failed, returned %x \n", reset);
	}
}

void check_core_status()
{
	alt_printf("Checking the settings of the Audio Core device... \n");
	alt_u32 statusreg = IORD_32DIRECT(AUDIO_0_BASE, 0);
	alt_printf("The status register is %x \n", statusreg);
	alt_u32 fiforeg = IORD_32DIRECT(AUDIO_0_BASE, 4);
	alt_u32 fiforeg2 = IORD_32DIRECT(AUDIO_0_BASE, 8);
	alt_printf("the FIFOspace registers are %x and %x \n", fiforeg, fiforeg2);
	alt_u32 ldata = IORD_32DIRECT(AUDIO_0_BASE, 8);
	alt_printf("The ldada register is %x \n", ldata);
	alt_u32 rdata = IORD_32DIRECT(AUDIO_0_BASE, 12);
	alt_printf("The rdata register is = %x \n", rdata);
}

void reset_core()
{
	alt_printf("Resetting Audio core... \n");
	alt_up_audio_reset_audio_core(audio_dev);
}

// Audio usage functions

void record_process(int seconds, int test, int run_profiling)
{
	if(run_profiling == 1)
	{
		// Start profiling for the time spent recording
		PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 1);
	}
	// Lower audio_ready_flag
	audio_ready_flag = 0;
	// Divide the desired recording time into quarter-second snippets
	int snippets = seconds*4;
	int snippet_counter = 0;
	// 48 kHz sampling frequency => 12k samples per snippet
	int words_per_snippet = 12000;
	unsigned int start_address = SDRAM_CONTROLLER_2_BASE;
	// Send mail to Signal Processing subsystem, letting it know a recording process has started, where the data will be
	// stored and how many snippets it will contain
	if(run_profiling != 1)
	{
		alt_printf("Sending initial mail, for a recording of %x snippets... \n", snippets);
	}
	send_mail(start_address, snippets);
	unsigned int current_address;
	while(snippet_counter < snippets)
	{
		current_address = start_address + snippet_counter*words_per_snippet*4;
		// Record
		record_audio_snippet(current_address, words_per_snippet);
		if(run_profiling != 1)
		{
			alt_printf("Snippet recorded, sending mail... \n");
		}
		// Send mail with snippet info
		send_mail(current_address, words_per_snippet);

		snippet_counter = snippet_counter + 1;
	}
	if(run_profiling == 1)
	{
		// Stop profiling for the time spent recording
		PERF_END(PERFORMANCE_COUNTER_0_BASE, 1);
		// Start profiling for the time spent waiting for the other subsystem
		PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 3);
	}
	else
	{
		alt_printf("Waiting for the Signal Processing subsystem to finish... \n");
	}
	receive_mail();
	if(run_profiling == 1)
	{
		// Stop profiling for the time spent waiting for the other subsystem
		PERF_END(PERFORMANCE_COUNTER_0_BASE, 3);
	}
	// Store address and data size of the processed audio
	ready_audio[0] = mail_receive[0];
	ready_audio[1] = mail_receive[1];
	// Raise flag, enabling playback of the processed audio
	audio_ready_flag = 1;
	if(run_profiling != 1)
	{
		alt_printf("Confirmation mail received.\nProcessed audio is stored at address = %x, and of memory size = %x \n", ready_audio[0], ready_audio[1]);
	}
	// Test the work
	if(test == 1)
	{
		if(run_profiling == 1)
		{
			// Start profiling for the time spent verifying the results
			PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 2);
		}
		test_audio_processing(SDRAM_CONTROLLER_2_BASE, ready_audio[0], ready_audio[1]);
		if(run_profiling == 1)
		{
			// Stop profiling for the time spent verifying the results
			PERF_END(PERFORMANCE_COUNTER_0_BASE, 2);
		}
	}
}

void record_audio_snippet(int start_addr_ptr, int record_words)
{
	int words_recorded = 0;
	int words_in_fifo = 0;
	int current_words = 0;
	int i;
	while( words_recorded < record_words)
	{
		words_in_fifo = (IORD_32DIRECT(AUDIO_0_BASE, AudioFifoSpaceReg) & 0xff00) >> 8;
		current_words = min(words_in_fifo, record_words - words_recorded);
		i = 0;
		while(i < current_words)
		{
			dataword = IORD_32DIRECT(AUDIO_0_BASE, AudioLData);
			IOWR_32DIRECT(start_addr_ptr, (words_recorded + i)*4, dataword);
			i ++;
		}
		words_recorded += current_words;
	}
}

void play_audio(int start_addr_ptr, int memory_size)
{
	int play_words = memory_size/4;
	int words_played = 0;
	alt_32 current_address = start_addr_ptr;
	int free_space = 0;
	int words_to_buffer;
	int i;
	alt_u32 resultword;
	while( words_played < play_words)
	{
		// Check how much space is available
		free_space = (IORD_32DIRECT(AUDIO_0_BASE, AudioFifoSpaceReg) & 0xff000000) >> 24;
		words_to_buffer = min(free_space, play_words - words_played);
		i = 0;
		while(i < words_to_buffer)
		{
			current_address += 4;
			resultword = IORD_32DIRECT(SDRAM_CONTROLLER_2_BASE, current_address);
			IOWR_32DIRECT(AUDIO_0_BASE, AudioLData, resultword);
			IOWR_32DIRECT(AUDIO_0_BASE, AudioRData, resultword);
			i ++;
		}
		words_played += free_space;
	}
}

int test_audio_processing(alt_u32 start_recorded, alt_u32 start_processed, alt_u32 memspace)
{
	int wordcount = memspace/4;
	int words_tested = 0;
	int faults_found = 0;
	alt_u32 test1;
	alt_u32 test2;
	int op_type;
	if((IORD_32DIRECT(PIO_2_BASE, 0) & 0x80) == 0)
	{
		op_type = 0;
	}
	else
	{
		op_type = 1;
	}
	while(words_tested < wordcount)
	{
		// Load both
		test1 = IORD_32DIRECT(start_recorded, words_tested*4);
		test1 = (signed)test1;
		test2 = IORD_32DIRECT(start_processed, words_tested*4);
		// Test
		if(op_type == 0)
		{
			test1 = (test1 & 0x80000000)+((test1 & 0xfffffffe)>>1);
		}
		else if (op_type == 1)
		{
			test1 = (test1 << 1);
		}
		if((int)test1 != (int)test2)
		{
			faults_found += 1;
		}
		// Increment
		words_tested ++;
	}
	error_rate = (float)faults_found/(float)words_tested;
	success_rate = 100*(1 - error_rate);
	printf("Test finished. Success rate %f %% \n", success_rate);
	return faults_found;
}

//Interrupt and task choosing functions

void choose_task(int task)
{
	if(task == 1)
	{
		alt_printf("Task 1: Recording... \n");
		// First switch (SW0) Record audio for a second, without testing, without profiling, using the HW accelerator
		record_process(1, 0, 0);
	}
	else if(task == 2)
	{
		alt_printf("Task 2: Playing... \n");
		// Second switch (SW1): Playback processed audio if ready
		if(audio_ready_flag)
		{
			play_audio((int)ready_audio[0], (int)ready_audio[1]);
		}
		else
		{
			alt_printf("Error: No audio is ready! Please record first. \n");
		}
	}
	else if(task == 4)
	{
		alt_printf("Task 3: Profiling... \n");
		// Third switch (SW2): record thrice, once for each method on the Signal Processing side, with testing and profiling
		// Start overall performance counter
		PERF_RESET(PERFORMANCE_COUNTER_0_BASE);
		PERF_START_MEASURING(PERFORMANCE_COUNTER_0_BASE);
		// Record for HW accelerator
		alt_printf("Recording for HW accelerator... \n");
		record_process(1, 1, 1);
		// Record for custom instruction
		alt_printf("Recording for custom instruction... \n");
		record_process(1, 1, 1);
		// Record for c code implementation
		alt_printf("Recording for C code implementation... \n");
		record_process(1, 1, 1);
		// Stop overall performance counter and print results
		PERF_STOP_MEASURING(PERFORMANCE_COUNTER_0_BASE);
		perf_print_formatted_report(PERFORMANCE_COUNTER_0_BASE, alt_get_cpu_freq(), 3, "Recording", "Verifying", "Waiting for #2");
	}
	else if(task == 8)
	{
		alt_printf("Task 4: Verifying processed audio... \n");
		// Fourth switch (SW3): Test audio by immediately playing back what is recorded
		if(audio_ready_flag)
		{
			test_audio_processing(SDRAM_CONTROLLER_2_BASE, ready_audio[0], ready_audio[1]);
		}
		else
		{
			alt_printf("Error: No audio is ready! Please record first. \n");
		}
	}
	else if(task == 16)
	{
		alt_printf("Task 5: Checking audio device status... \n");
		check_config_status();
		check_core_status();
	}
	else if(task == 32)
	{
		alt_printf("Task 6: Reset Audio Core... \n");
		reset_core();
	}
	else if(task == 64)
	{
		alt_printf("Task 7: Reset Audio and Video Config device... \n");
		reset_config_device();
	}
	alt_printf("Task complete. Ready for new task. \n\n", task);
}

void isr_switches(void* context)
{
	// Read interrupt source
	uint8_t pinvals = (IORD_8DIRECT(PIO_2_BASE, PIO_IntrSwitch_IRQFLAG) & 0x7f);
	// Save choice
	choice = (int)pinvals;
	// Send mail to the other CPU, letting it know that the flag can be lowered
	send_mail(choice, 0);
	// Clear the interrupt flag
	IOWR_8DIRECT(PIO_2_BASE, PIO_IntrSwitch_IRQFLAG, pinvals);
}

void setup_switch_interrupts(uint8_t chosen_switches)
{
	// Setup interrupts on the chosen switches
	IOWR_8DIRECT(PIO_2_BASE, PIO_IntrSwitch_IRQEN, chosen_switches);
	alt_irq_register(PIO_2_IRQ, NULL, isr_switches);
	return;
}

// Misc functions
int min(int a, int b)
{
	return (a > b) ? b : a;
}

// ----------------------- Main function ----------------------------------

int main()
{
	setup_audio(audio_dev);
	// Setup interrupts on the first 6 switches
	setup_switch_interrupts(0x7f);
	alt_printf("\n \n \n \n ---------- main --------------- \n");

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
