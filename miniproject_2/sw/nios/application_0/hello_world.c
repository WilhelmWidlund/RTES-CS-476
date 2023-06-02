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

// Task choice variable
volatile int choice = 0;

volatile alt_u32 dataword = 0;

// Setup audio core
alt_up_audio_dev * audio_dev;

// ----------------- Function declarations ------------------------------------
// Mailbox functions
void send_callback(void* report, int status);
void send_mail(alt_u32 start_addr, alt_u32 storage_length);
void receive_callback(void* message);
void receive_mail();
// Audio functions
void setup_audio(alt_up_audio_dev * audio_dev);
void record_process(int seconds);
void record_audio_snippet(int start_addr_ptr, int record_words);
void play_audio(int start_addr_ptr, int memory_size);
int test_audio(alt_u32 start_recorded, alt_u32 start_processed, alt_u32 memspace);
// Interrupt and task choosing functions
void choose_task(int task);
void isr_switches(void* context);
void setup_switch_interrupts(uint8_t chosen_switches);

void check_config_status();
void reset_config_device();

void reset_core();

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
	//alt_printf("-------------- receive_mail ------------- \n");
	// Open mailbox
	altera_avalon_mailbox_dev* mailbox_SigProcToAudio = altera_avalon_mailbox_open("/dev/mailbox_simple_1", NULL, NULL);
	// Wait for mail confirming the processing is complete
	altera_avalon_mailbox_retrieve_poll(mailbox_SigProcToAudio, mail_receive, 0);
	alt_dcache_flush_all();
	//IOWR_32DIRECT(PIO_LEDS_SHARED_BASE, 0, 0x2);
}

// -------------------- Audio functions ---------------------------------------------------

void setup_audio(alt_up_audio_dev * audio_dev)
{
	// open the Audio port
	audio_dev = alt_up_audio_open_dev ("/dev/audio_0");
	if ( audio_dev == NULL)
		alt_printf ("Error: could not open audio device \n");
	else
		alt_printf ("Opened audio device \n");
}

void record_process(int seconds)
{
	//alt_printf("------------------ record_process ------------------------------ \n");
	// Lower audio_ready_flag
	audio_ready_flag = 0;
	// Divide the desired recording time into quarter-second snippets
	int snippets = seconds*4;
	int snippet_counter = 0;
	// 48 kHz sampling frequency => 12k samples per snippet
	int words_per_snippet = 12000;
	// alt_u32 mail[2];
	unsigned int start_address = SDRAM_CONTROLLER_2_BASE;
	send_mail(start_address, snippets);
	unsigned int current_address;
	//alt_printf("First mail has been sent \n");
	// Turn on LED 0, signifying that recording is in progress
	//IOWR_32DIRECT(PIO_LEDS_SHARED_BASE, 0, 0x1);

	// Clear FIFO
	//IOWR_32DIRECT(AUDIO_0_BASE, AudioControlReg, 4);
	//IOWR_32DIRECT(AUDIO_0_BASE, AudioControlReg, 0);
	while(snippet_counter < snippets)
	{
		//alt_printf("In record_process loop... \n");
		current_address = start_address + snippet_counter*words_per_snippet*4;
		alt_printf("Current address is %x \n", current_address);
		// Record
		record_audio_snippet(current_address, words_per_snippet);
		// Send mail
		alt_printf("Sending snippet info mail with address = %x and words per snippet = %x \n", start_address + snippet_counter*words_per_snippet*4, words_per_snippet);
		send_mail(current_address, words_per_snippet);
		alt_printf("record_process loop mail sent \n");
		snippet_counter = snippet_counter + 1;
	}
	alt_printf("Waiting for mail... \n");
	mail_receive[0] = 0x1;
	mail_receive[1] = 0x2;
	// Wait for the Signal Processing subsystem to finish it's task
	receive_mail();
	// Store address and data size of ready audio
	ready_audio[0] = mail_receive[0];
	ready_audio[1] = mail_receive[1];
	// Raise flag, enabling playback of processed audio
	audio_ready_flag = 1;
	alt_printf("Recording finished. Returned address = %x, memory size = %x \n", ready_audio[0], ready_audio[1]);
	// Turn off LED 0, signifying that recording is done
	//IOWR_32DIRECT(PIO_LEDS_SHARED_BASE, 0, 0x0);
	// Test the work
	int errors_found = test_audio(SDRAM_CONTROLLER_2_BASE, ready_audio[0], ready_audio[1]);
	alt_printf("A total of %x words were wrong \n", errors_found);
}

int min(int a, int b)
{
	return (a > b) ? b : a;
}

void record_audio_snippet(int start_addr_ptr, int record_words)
{
	alt_printf("---------------- record_audio_snippet --------------------- \n");
	alt_printf("At the beginning, start_addr_ptr = %x and record_words = %x \n", start_addr_ptr, record_words);
	int words_recorded = 0;
	int words_in_fifo = 0;
	int current_words = 0;
	int i;

	// With dumbword = 0x2, shift up should produce 0x4, and down should give 0x1...
	// alt_u32 dumbword = 0x2;
	// alt_u32 current_address;

	while( words_recorded < record_words)
	{
		/*
		dataword = IORD_32DIRECT(AUDIO_0_BASE, 8);
		if(dataword != 0)
		{
			IOWR_32DIRECT(start_addr_ptr, words_recorded*4, dataword);
			words_recorded += 1;
		}
		*/

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
		if(words_recorded % 120 == 0)
		{
			alt_printf("Words_recorded = %x, with the last one being %x \n", words_recorded, dataword);
		}
	}
	/*
	// TODO: Remove this shit when verified...
	// Also remove the dumbword init above...
	int dumbtest = 1;
	// Dumb test of other functionality for when no actual microphone is available...
	if (dumbtest == 1)
	{
		current_address = start_addr_ptr + words_recorded*4;
		IOWR_32DIRECT(SDRAM_CONTROLLER_2_BASE, current_address, dumbword);
		words_recorded = words_recorded + 1;
	}
	*/
	alt_printf("Snippet recorded. \n");
}

void play_audio(int start_addr_ptr, int memory_size)
{
	//alt_printf("-------------------- play_audio --------------------- \n");
	//alt_printf("Memory size is %x \n", memory_size);
	int play_words = memory_size/4;
	//alt_printf("amount of words to play is %x \n", play_words);
	int words_played = 0;
	alt_32 current_address = start_addr_ptr;
	int free_space = 0;
	int words_to_buffer;
	int i;

	// TODO: Remove this shit when verified... ------------------------------------------------------
	// Also uncomment the actual play stuff below, both in and out of loop...

	// Dumb test of other functionality for when no actual microphone is available...
	// With dumbword = 0x2, shift up should produce 0x4, and down should give 0x1...
	alt_u32 resultword;
	int dumbtest = 1;

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
		alt_printf("Current address = %x has the value dumbwordresult = %x \n", current_address, resultword);
	}
	alt_printf("End of playback, dumbwordresult = %x \n", resultword);
}

int test_audio(alt_u32 start_recorded, alt_u32 start_processed, alt_u32 memspace)
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
		if (words_tested % 120 == 0)
		{
			alt_printf("The current words are %x and %x \n", test1, test2);
		}
		if((int)test1 != (int)test2)
		{
			faults_found += 1;
		}
		// Increment
		words_tested ++;
	}
	return faults_found;
}

void check_config_status()
{
	alt_up_av_config_dev* config_device = alt_up_av_config_open_dev(AUDIO_AND_VIDEO_CONFIG_0_NAME);
	alt_u32 status = IORD_32DIRECT(AUDIO_AND_VIDEO_CONFIG_0_BASE, 4);
	alt_printf("The status register is %x \n", status);
	int ready = alt_up_av_config_read_ready(config_device);
	alt_printf("The Ready bit is %x \n", ready);
	int ackbit = alt_up_av_config_read_acknowledge(config_device);
	alt_printf("The acknowledge bit is %x \n", ackbit);
}

void reset_config_device()
{
	alt_up_av_config_dev* config_device = alt_up_av_config_open_dev(AUDIO_AND_VIDEO_CONFIG_0_NAME);
	// Test resetting
	alt_printf("Resetting... \n");
	int reset = alt_up_av_config_reset(config_device);
	alt_printf("It returned %x \n", reset);
}

void reset_core()
{
	alt_up_audio_reset_audio_core(audio_dev);
}

// ----------------- Interrupt and task choosing functions -------------------------

void choose_task(int task)
{
	//alt_printf("----------------- choose_task ------------------------- \n");
	alt_printf("Task is %x \n", task);
	if(task == 1)
	{
		alt_printf("Recording... \n");
		// First switch (SW0) Record audio for a second
		record_process(1);
		//printf("Interrupt test on CPU %d\n", NIOS2_CPU_ID_VALUE);
	}
	else if(task == 2)
	{
		alt_printf("Playing... \n");
		// Second switch (SW1): Playback processed audio if ready
		if(audio_ready_flag)
		{
			//alt_printf("Playing audio with parameters ready_audio[0] = %x and ready_audio[1] = %x \n", ready_audio[0], ready_audio[1]);
			play_audio((int)ready_audio[0], (int)ready_audio[1]);
		}
		else
		{
			alt_printf("No audio is ready! Record first. \n");
		}
	}
	else if(task == 4)
	{
		alt_printf("Running profiling... \n");
		// Third switch (SW2): run with profiling
		// Record for HW accelerator
		record_process(1);
		// Record for custom instruction
		record_process(1);
		// Record for c code implementation
		record_process(1);
	}
	else if(task == 8)
	{
		alt_printf("Testing audio \n");
		// Fourth switch (SW3): Test audio by immediately playing back what is recorded
		int errors = test_audio(0, 48000*4, 48000);
		alt_printf("A total of %x words were wrong \n", errors);
	}
	else if(task == 16)
	{
		check_config_status();
	}
	else if(task == 32)
	{
		reset_config_device();
	}
	else if(task == 64)
	{
		alt_u32 fiforeg = IORD_32DIRECT(AUDIO_0_BASE, 4);
		alt_u32 fiforeg2 = IORD_32DIRECT(AUDIO_0_BASE, 8);
		alt_u32 fiforeg3 = IORD_32DIRECT(AUDIO_0_BASE, 4);
		alt_u32 fiforeg4 = IORD_32DIRECT(AUDIO_0_BASE, 8);
		alt_printf("the registers of space are %x and %x \n", fiforeg, fiforeg3);
		alt_printf("%x and %x", fiforeg2, fiforeg4);


		alt_u32 statusreg = IORD_32DIRECT(AUDIO_0_BASE, 0);
		alt_printf("The core status register is %x \n", statusreg);
		fiforeg = IORD_32DIRECT(AUDIO_0_BASE, 4);
		alt_printf("The core fifo register is %x \n", fiforeg);
		alt_u32 ldata = IORD_32DIRECT(AUDIO_0_BASE, 8);
		alt_printf("ldada = %x \n", ldata);
		alt_u32 rdata = IORD_32DIRECT(AUDIO_0_BASE, 12);
		alt_printf("rdada = %x \n", rdata);
		alt_u32 rfifo = alt_up_audio_read_fifo_avail(audio_dev, ALT_UP_AUDIO_RIGHT);
		alt_u32 lfifo = alt_up_audio_read_fifo_avail(audio_dev, ALT_UP_AUDIO_LEFT);
		alt_printf("With their read_avail functions, we get that right = %x and left = %x \n", rfifo, lfifo);

	}
	else if(task == 128)
	{
		reset_core();
	}
	alt_printf("Task complete. Ready for new task. \n\n", task);
}

void isr_switches(void* context)
{
	//alt_printf("----------------- isr_switches ------------------------- \n");
	// Read interrupt source
	uint8_t pinvals = (IORD_8DIRECT(PIO_2_BASE, PIO_IntrSwitch_IRQFLAG) & 0xff);
	// Save choice
	choice = (int)pinvals;
	//alt_printf("Sending flag mail... \n");
	// Send mail to the other CPU, letting it know that the flag can be lowered
	send_mail(choice, 0);
	//alt_printf("Flag mail sent, with choice = %x \n", choice);
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


// ----------------------- Main function ----------------------------------

int main()
{
	setup_audio(audio_dev);
	// Setup interrupts on the first 6 switches
	setup_switch_interrupts(0xff);
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
