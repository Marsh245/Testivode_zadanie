תאריך וזמן התחלה:  11:24🕛/27.03.2024
תאריך וזמן סיום משימה:  00:56🕛/28.03.2024



# ########################################### #
# Doubly Linked List Multi-Threading Program  #
# To Count Zero and one bits in each ELements #
# ########################################### #

## Overview
This program demonstrates multi-threading in C using a doubly linked list data structure. 
Two threads are created to concurrently  for processing  elements, counting   bits 
(zero bits and one bits) of the integer data on each thred.

## Features
- Concurrent processing of a doubly linked list using POSIX threads (`pthread.h`).
- Dynamic creation of the doubly linked list with randomly generated integer values.(rand() %256)
- Multi-threaded processing of list elements with separate threads counting zero and one bits.
- bits shifting 
- to ensure thread safety ya izpolsoval mutex locking.

  
## Requirements
- C compiler (e.g., GCC)
- POSIX threads library (`pthread.h`)
- Standard C libraries: stdio.h, stdlib.h, stdbool.h, unistd.h

  
## **NOTE**:
  Me Marshoud dont use C language on my daily basis, solving this task i've learnt how to work with memory
  how multi threading is implemented in C language, impleneting a data structure like Doubly linked list
  which demonstrates a very importnat skill and its 
  # Ability to LEARN
