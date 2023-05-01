Multiprocessor Tutorial version v1 readme.txt

This file contains the following sections:

o  Package Contents
o  Tool Requirements
o  Project Directory Names
o  Usage Instructions
o  Release History


Package Contents
================

Examples, software designs, hardware designs and other information to accompany 
Creating Multiprocessor Nios II Systems Tutorial.

Tool Requirements
=================

The examples in this archive require the following software tools:

- Quartus II software version 11.0  or later
- Nios II Software Build Tools for Eclipse version 11.0 or later


This design also requires the following development board:

  - Altera Cyclone III 3C120 Development Board

Please contact your local sales representative if you do not have the 
necessary software tools.


Project Directory Names
=======================

When you extract the archive the following folder structure will be created:

<my_design>
|--- multiprocessor_tutorial_start_v1
    |--- software
        |--- dining_philosophers_src
        |--- hello_world_src
        |--- philosophers_monitor_src

|--- multiprocessor_tutorial_final_v1
    |--- software
        |--- dining_philosophers_src
        |--- hello_world_src
        |--- philosophers_monitor_src
        |--- app0
        |--- app1
        |--- app2
        |--- app3
        |--- app4
        |--- app_hello_world
        |--- app_monitor
        |--- bsp0
        |--- bsp1
        |--- bsp2
        |--- bsp3
        |--- bsp4
        |--- bsp_hello_world
        |--- bsp_monitor



Usage Instructions
==================

To use the hardware and software examples in this archive, refer to 
"Creating Multiprocessor Nios II Systems Tutorial", available at:
http://www.altera.com/literature/tt/tt_nios2_multiprocessor_tutorial.pdf. 

You can download the "Creating Multiprocessor Nios II Systems Tutorial" 
accompanying hardware and software examples from altera.com, at:
http://www.altera.com/literature/tt/tt_nios2_multiprocessor_design.zip 

multiprocessor_tutorial_start_v1 directory contains the starting point hardware 
design with a single Nios II microprocessor

multiprocessor_tutorial_final_v1 directory contains the end point hardware 
design with six Nios II microprocessors, after the following the Qsys
modification steps outlined in the Multiprocessor Tutorial.

Following the steps in the Multiprocessor Tutorial, execute the 
multiprocessor_tutorial_hw.sh, philosopher.sh, and philosophers_monitor.sh scripts in 
multiprocessor_tutorial_final_v1/source directories.  Execute the philosopher.sh
script 5 times, passing in numbers 0 - 4 as the first parameter.  Execute each
script in its own shell, so that you can see all activity simultaneously.

Release History
===============

Version 1.0
-------------

Initial release


Version 2.0
-------------

New HW and SW designs were created.  The hardware design now supports the 
NEEK (Nios II Embedded Evaluation Kit development board), as well as 
a TSE ethernet for running Nichestack on one of the Nios II microprocessors.  
The hardware design integrates an external vectored interrupt controller. 

The software design now runs a complete TCP/IP network stack on one Nios II 
microprocessor, while handing off commands collected from a network socket to 
a second Nios II microprocessor via a hardware mailbox SOPC Builder component.  
The second microprocessor's application source code represents the real-time 
portion of the application, which in this case simply varies the frequency of 
a blinking LED.  In a real world application, this real-time functionality on
the second microprocessor could be sampling a speedometer, for example.  This 
simple blinking led function, in addition to providing visual feedback verifying 
communication between the multiple Nios II microprocessors, provides a clear 
place for the application developer to replace and invoke their deterministic 
application specific needs.  

Updated the system and components using Quartus II software and Nios II EDS 
version 9.1 SP1.

Version v1
-------------

New HW and SW designs.  Demonstrates constructions of a hierarchical system 
with 6 Nios II CPUs.  The classic Dining Philosophers synchronization problem
is demonstrated, with each CPU acting as 1 of the philosophers, and each 
hardware mutex acting as the chopsticks.

Last updated May, 2011
Copyright © 2011 Altera Corporation. All rights reserved.
