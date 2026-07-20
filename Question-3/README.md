# Question-3: Low-Level File Operations

Employee Record Management using Linux System Calls

## Objective

Build a file utility using system calls only:
1. Create file
2. Write employee records
3. Update specific records
4. Retrieve records efficiently

## Key Concept: Fixed-Size Records

Each employee record is exactly 96 bytes:
- id: 4 bytes
- name: 50 bytes
- department: 30 bytes
- salary: 8 bytes
- active: 4 bytes

## System Calls Used

1. open() - Create/Open file
2. write() - Write binary data
3. read() - Read binary data
4. lseek() - Position in file
5. close() - Close file

## How It Works

To update record #2:
1. Calculate offset: 2 x 96 = 192 bytes
2. Open file
3. lseek to byte 192
4. Write new data
5. Close file

## Usage

Compile:
gcc -o employee_manager employee_manager.c

Run:
./employee_manager

Test:
gcc -o test_read test_read.c
./test_read

## Files

- employee_manager.c - Main program
- test_read.c - Test program
- employee_records.dat - Binary data file

## Key Learning

Fixed-size records enable efficient random access and in-place updates.
