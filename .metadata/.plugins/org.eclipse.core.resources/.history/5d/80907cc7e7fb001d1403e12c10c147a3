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
#include <unistd.h>
#include <string.h>
#include <sys/alt_irq.h>
#include "altera_avalon_mailbox_simple.h"
#include <altera_avalon_performance_counter.h>
#include "altera_up_avalon_audio.h"

// REMEMBER TO CHANGE "LEGACY" TO "ENHANCED" for #define ALT_<X>_INTERRUPT_API_PRESENT IN SYSTEM.H AFTER GENERATING BSP

// To monitor terminal in powershell:
// nios2-terminal --device 2 --instance 0

// To download code to board, change to .elf file directory and run:
// nios2-download -g SysAudio.elf --device 2 --instance 0

// PIO definitions for the interrupt handling on the switches
#define PIO_IntrSwitch_Data	0
#define PIO_IntrSwitch_IRQEN	4*2
#define PIO_IntrSwitch_IRQFLAG	4*3
// Initiate audio_ready_flag
volatile int audio_ready_flag = 0;
// Message storage array
volatile alt_u32 mail[2];
// Task choice variable
volatile int choice = 0;
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
void record_audio_snippet(int *start_addr_ptr, int record_words);
void play_audio(int *start_addr_ptr, int play_words);
void test_audio();
// Interrupt and task choosing functions
void choose_task(int task);
void isr_switches(void* context);
void setup_switch_interrupts(uint8_t chosen_switches);

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
	altera_avalon_mailbox_dev* mailbox_AudioToSigProc = altera_avalon_mailbox_open("/dev/Mailbox_AudioToSigProc", send_callback, NULL);
	// Create mail "envelope"
	alt_u32 mail[2] = {start_addr, storage_length};
	// Send message
	altera_avalon_mailbox_send(mailbox_AudioToSigProc, mail, 0, POLL);
	// Close mailbox
	altera_avalon_mailbox_close(mailbox_AudioToSigProc);
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
	// Open mailbox
	altera_avalon_mailbox_dev* mailbox_SigProcToAudio = altera_avalon_mailbox_open("/dev/Mailbox_SigProcToAudio", NULL, receive_callback);
	// Wait for mail confirming the processing is complete
	altera_avalon_mailbox_retrieve_poll(mailbox_SigProcToAudio, mail, 0);
	alt_dcache_flush_all();
	// Raise flag, enabling playback of processed audio
	audio_ready_flag = 1;
	IOWR_32DIRECT(PIO_LEDS_SHARED_BASE, 0, 0x2);
}

// -------------------- Audio functions ---------------------------------------------------

void setup_audio(alt_up_audio_dev * audio_dev)
{
	// open the Audio port
	audio_dev = alt_up_audio_open_dev ("/dev/SysAudio_Audio_Core");
	if ( audio_dev == NULL)
	alt_printf ("Error: could not open audio device \n");
	else
	alt_printf ("Opened audio device \n");
}

void record_process(int seconds)
{
	// Lower audio_ready_flag
	audio_ready_flag = 0;
	// Divide the desired recording time into quarter-second snippets
	int snippets = seconds*4;
	int snippet_counter = 0;
	// 48 kHz sampling frequency => 12k samples per snippet
	int words_per_snippet = 12000;
	alt_u32 mail[2];
	unsigned int *start_address = SDRAM_CONTROLLER_SHARED_BASE;
	send_mail(start_address, snippets);
	// Turn on LED 0, signifying that recording is in progress
	IOWR_32DIRECT(PIO_LEDS_SHARED_BASE, 0, 0x1);
	while(snippet_counter < snippets)
	{
		// Record
		record_audio_snippet(start_address + snippet_counter*words_per_snippet*4, words_per_snippet);
		// Send mail
		send_mail(start_address + snippet_counter*words_per_snippet*4, words_per_snippet);
		snippet_counter = snippet_counter + 1;
	}
	// Turn off LED 0, signifying that recording is done
	IOWR_32DIRECT(PIO_LEDS_SHARED_BASE, 0, 0x0);
}

void record_audio_snippet(int *start_addr_ptr, int record_words)
{
	int words_recorded = 0;
	while( words_recorded < record_words)
	{
		int fifospace = alt_up_audio_read_fifo_avail (audio_dev, ALT_UP_AUDIO_RIGHT);
		int current_iter_count = 0;
		// Check for available data
		if ( fifospace > 95 )
		{
			words_recorded = words_recorded + alt_up_audio_record_l(audio_dev, (start_addr_ptr + words_recorded*4), fifospace);
		}
	}
}

void play_audio(int *start_addr_ptr, int play_words)
{
	int words_played = 0;
	while( words_played < play_words - 128)
	{
		unsigned int space = alt_up_audio_write_fifo_space (audio_dev, ALT_UP_AUDIO_LEFT);
		if (space > BUF_THRESHOLD)
		{
			words_played = words_played + alt_up_audio_write_fifo(audio_dev, start_addr_ptr + words_played*4, space, ALT_UP_AUDIO_LEFT);
		}
	}

	while(alt_up_audio_write_fifo_space(audio_dev, ALT_UP_AUDIO_LEFT) < (play_words - words_played)){}
	alt_up_audio_write_fifo(audio_dev, start_addr_ptr + words_played*4, (play_words - words_played), ALT_UP_AUDIO_LEFT);
}

void test_audio()
{
	/* used for audio record/playback */
	unsigned int l_buf;
	/* read and echo audio data */
	while(1)
	{
		int fifospace = alt_up_audio_read_fifo_avail (audio_dev, ALT_UP_AUDIO_LEFT);
		if ( fifospace > 0 ) // check if data is available
		{
			// read audio buffer
			alt_up_audio_read_fifo (audio_dev, &(l_buf), 1, ALT_UP_AUDIO_LEFT);
			// write audio buffer
			alt_up_audio_write_fifo (audio_dev, &(l_buf), 1, ALT_UP_AUDIO_LEFT);
		}
	}
}

// ----------------- Interrupt and task choosing functions -------------------------

void choose_task(int task)
{
	if(task == 1)
	{
		// First switch (SW0) Record audio for a second
		//record_process(1);
		printf("Interrupt test on CPU %d\n", NIOS2_CPU_ID_VALUE);
	}
	else if(task == 2)
	{
		// Second switch (SW1) Playback processed audio if ready
		if(audio_ready_flag)
		{
			play_audio((int)mail[0], (int)mail[1]);
		}
	}
	else if(task == 4)
	{
		// Third switch (SW2) Test audio by immediately playing back what is recorded
		test_audio();
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
	// alt_ic_isr_register(PIO_SWITCHES_IRQ_INTERRUPT_CONTROLLER_ID, PIO_SWITCHES_IRQ, isr_switches, NULL, NULL);
	alt_irq_register(PIO_SWITCHES_IRQ, NULL, isr_switches);
	return;
}


// ----------------------- Main function ----------------------------------

int main()
{
	setup_audio(audio_dev);
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
