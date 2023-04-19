#include <stdio.h>
#include "sys/alt_stdio.h"
#include "sys/alt_cache.h"
#include <altera_avalon_performance_counter.h>
#include "io.h"
#include "system.h"
#include <stdint.h>


// Declare a test number and it's expected result from the custom instruction
uint32_t test = 0x12AAAAEF;
uint32_t truth = 0xEF555512;

// Declare tables for testing 1000 long data
uint32_t in_1000[1000];
uint32_t out_1000[1000];

// Define macros for the accelerator
#define ACC_BUSY			0x00000002
#define ACC_DONE			0x00000001
#define ACC_InputStartAddr	0
#define	ACC_OutputStartAddr	4
#define ACC_Num				8

void init_tables()
{
	int i;
	for(i=0; i<1000; i++)
	{
		in_1000[i] = test;
	}
	alt_dcache_flush_all();
}

uint32_t software_custom_inst(uint32_t input)
{
	uint32_t output;
	output = (input<<24) & 0xff000000;
	output = output + ((input & 0xff000000)>>24);
	uint32_t sought_bit;
	int i;
	for(i=8; i<24; i++)
	{
		sought_bit = (input<<i)&0x80000000;
		output = output + (sought_bit>>(31 - i));
	}
	return output;
}

void software_1000()
{
	// Start performance counter
	PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 1);

	int i;
	for(i=0; i<1000; i++)
	{
		out_1000[i] = software_custom_inst(in_1000[i]);
	}

	// Stop performance counter
	PERF_END(PERFORMANCE_COUNTER_0_BASE, 1);
}

void custom_1000()
{
	// Start performance counter
	PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 2);

	int i;
	for(i=0; i<1000; i++)
	{
		out_1000[i] = ALT_CI_CUSTOM_INSTRUCTION_0(in_1000[i]);
	}

	// Stop performance counter
	PERF_END(PERFORMANCE_COUNTER_0_BASE, 2);
}

void use_accelerator(uint32_t in_addr, uint32_t out_addr, uint32_t length)
{
	// Check if the device is already working
	uint32_t status = IORD_32DIRECT(ACCELERATOR_0_BASE, 0x0);
	if(status == ACC_BUSY)
	{
		return;
	}

	// Start performance counter
	PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 3);

	// Write the three arguments
	IOWR_32DIRECT(ACCELERATOR_0_BASE, ACC_InputStartAddr, in_addr);
	IOWR_32DIRECT(ACCELERATOR_0_BASE, ACC_OutputStartAddr, out_addr);
	IOWR_32DIRECT(ACCELERATOR_0_BASE, ACC_Num, length);

	// Wait for it to be done
	while(IORD_32DIRECT(ACCELERATOR_0_BASE, 0x0) != ACC_DONE);

	// Stop performance counter
	PERF_END(PERFORMANCE_COUNTER_0_BASE, 3);
}

int test_method_once(uint32_t input, uint32_t exp_output, int method)
{
	// Tests any implementation once, returning 1 if it calculates the correct answer
	// and 0 otherwise
	// method = 1 -> C software implementation
	// method = 2 -> Custom instruction implementation
	// method = 3 -> Hardware accelerator implementation
	uint32_t result;
	if(method == 1)
	{
		result = software_custom_inst(input);
	}
	else if(method == 2)
	{
		result = ALT_CI_CUSTOM_INSTRUCTION_0(input);
	}
	else if(method == 3)
	{
		use_accelerator((uint32_t)&input, (uint32_t)&out_1000, 0x1);
		result = out_1000[0];
	}
	return result == exp_output;
}

void test_implementations()
{
	// Test software implementation
	alt_printf("Software implementation test 1: Expected is 1, it gives %x \n", test_method_once(test, truth, 1));
	alt_printf("Software implementation test 2: Expected is 0, it gives %x \n", test_method_once(test, 0xffffffff, 1));

	// Test custom instruction implementation
	alt_printf("Custom instruction implementation test 1: Expected is 1, it gives %x \n", test_method_once(test, truth, 2));
	alt_printf("Custom instruction implementation test 2: Expected is 0, it gives %x \n", test_method_once(test, 0xffffffff, 2));

	// Test accelerator implementation
	alt_printf("Accelerator implementation test 1: Expected is 1, it gives %x \n", test_method_once(test, truth, 3));
	alt_printf("Accelerator implementation test 2: Expected is 0, it gives %x \n", test_method_once(test, 0xffffffff, 3));
}

int main()
{
	// Setup stuff for 1000 words of indata
	init_tables();
	void* in_addr_1000 = (uint32_t)&in_1000[0];
	void* out_addr_1000 = (uint32_t)&out_1000[0];
	uint32_t len_1000 = 0x000003E8;

	// Test the implementations
	test_implementations();

	// Start overall performance counter
	PERF_RESET(PERFORMANCE_COUNTER_0_BASE);
	PERF_START_MEASURING(PERFORMANCE_COUNTER_0_BASE);

	// Run software implementation on 1000 words of indata
	software_1000();

	// Run custom instruction implementation on 1000 words of indata
	custom_1000();

	// Run accelerator implementation on 1000 words of indata
	use_accelerator(in_addr_1000, out_addr_1000, len_1000);

	// Stop overall performance counter and print results
	PERF_STOP_MEASURING(PERFORMANCE_COUNTER_0_BASE);
	perf_print_formatted_report(PERFORMANCE_COUNTER_0_BASE, alt_get_cpu_freq(), 3, "C software", "Custom instruction", "Accelerator");
}