/*
 * Question-3: Low-Level File Operations
 * Secure Employee Record Management System
 * 
 * Uses Linux system calls instead of stdio library:
 * - open()  : File creation and opening
 * - write() : Writing records
 * - read()  : Reading records
 * - lseek() : Direct positioning (random access)
 * - close() : File cleanup
 * 
 * Author: manjitms173
 * Date: July 20, 2026
 */

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define FILENAME "employee_records.dat"
#define MAX_NAME_LEN 50
#define MAX_DEPT_LEN 30

/*
 * Fixed-size employee record structure
 * This is crucial for lseek() to work properly
 */
typedef struct {
    int id;                          // Employee ID (unique)
    char name[MAX_NAME_LEN];        // Employee name
    char department[MAX_DEPT_LEN];   // Department
    double salary;                   // Salary
    int active;                      // 1 = active, 0 = deleted
} Employee;

/*
 * Calculate file offset for a specific record
 * Formula: offset = record_number * sizeof(Employee)
 */
off_t calculate_offset(int record_number) {
    return (off_t)(record_number * sizeof(Employee));
}

/*
 * 1. CREATE FILE
 * 
 * open() flags explained:
 * - O_CREAT  : Create file if it doesn't exist
 * - O_WRONLY : Open for writing only
 * - O_TRUNC  : Truncate file to 0 length if it exists
 * 
 * Permissions: 0644 = rw-r--r--
 * - Owner: read + write
 * - Group: read
 * - Others: read
 */
int create_file(void) {
    int fd = open(FILENAME, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    
    if (fd < 0) {
        perror("Error creating file");
        return -1;
    }
    
    printf("✓ File created successfully: %s\n", FILENAME);
    printf("  File descriptor: %d\n", fd);
    printf("  Permissions: 0644 (rw-r--r--)\n\n");
    
    close(fd);
    return 0;
}

/*
 * 2. WRITE EMPLOYEE RECORD
 * 
 * write() system call:
 * - Writes raw bytes to file
 * - Returns number of bytes written
 * - Does NOT add formatting (unlike fprintf)
 * 
 * Process:
 * 1. Open file in append mode (O_APPEND)
 * 2. Write entire struct as binary data
 * 3. Close file
 */
int write_employee(const Employee *emp) {
    int fd = open(FILENAME, O_WRONLY | O_APPEND);
    
    if (fd < 0) {
        perror("Error opening file for writing");
        return -1;
    }
    
    // Write the entire struct as raw bytes
    ssize_t bytes_written = write(fd, emp, sizeof(Employee));
    
    if (bytes_written < 0) {
        perror("Error writing record");
        close(fd);
        return -1;
    }
    
    if (bytes_written != sizeof(Employee)) {
        fprintf(stderr, "Warning: Partial write occurred\n");
        close(fd);
        return -1;
    }
    
    printf("✓ Record written successfully\n");
    printf("  ID: %d\n", emp->id);
    printf("  Name: %s\n", emp->name);
    printf("  Department: %s\n", emp->department);
    printf("  Salary: $%.2f\n", emp->salary);
    printf("  Bytes written: %zd\n\n", bytes_written);
    
    close(fd);
    return 0;
}

/*
 * 3. UPDATE SPECIFIC RECORD
 * 
 * lseek() system call:
 * - Moves file pointer to specific position
 * - Allows random access to records
 * 
 * Whence values:
 * - SEEK_SET : From beginning of file
 * - SEEK_CUR : From current position
 * - SEEK_END : From end of file
 * 
 * Process:
 * 1. Calculate offset: record_number * sizeof(Employee)
 * 2. Use lseek() to position file pointer
 * 3. Write new record at that position
 * 4. Only the specific record is modified!
 */
int update_employee(int record_number, const Employee *emp) {
    int fd = open(FILENAME, O_WRONLY);
    
    if (fd < 0) {
        perror("Error opening file for update");
        return -1;
    }
    
    // Calculate position of the record
    off_t offset = calculate_offset(record_number);
    
    // Move file pointer to the record position
    off_t position = lseek(fd, offset, SEEK_SET);
    
    if (position < 0) {
        perror("Error seeking to record position");
        close(fd);
        return -1;
    }
    
    printf("✓ File pointer positioned at offset: %ld\n", (long)position);
    
    // Write updated record
    ssize_t bytes_written = write(fd, emp, sizeof(Employee));
    
    if (bytes_written != sizeof(Employee)) {
        fprintf(stderr, "Error: Failed to update record\n");
        close(fd);
        return -1;
    }
    
    printf("✓ Record #%d updated successfully\n", record_number);
    printf("  New Name: %s\n", emp->name);
    printf("  New Salary: $%.2f\n\n", emp->salary);
    
    close(fd);
    return 0;
}

/*
 * 4. RETRIEVE SPECIFIC RECORD
 * 
 * read() system call:
 * - Reads raw bytes from file
 * - Returns number of bytes read
 * 
 * Process:
 * 1. Calculate offset for desired record
 * 2. Use lseek() to position at record
 * 3. Read exactly sizeof(Employee) bytes
 * 4. Return the record
 */
int read_employee(int record_number, Employee *emp) {
    int fd = open(FILENAME, O_RDONLY);
    
    if (fd < 0) {
        perror("Error opening file for reading");
        return -1;
    }
    
    // Calculate and seek to record position
    off_t offset = calculate_offset(record_number);
    off_t position = lseek(fd, offset, SEEK_SET);
    
    if (position < 0) {
        perror("Error seeking to record");
        close(fd);
        return -1;
    }
    
    // Read the record
    ssize_t bytes_read = read(fd, emp, sizeof(Employee));
    
    if (bytes_read < 0) {
        perror("Error reading record");
        close(fd);
        return -1;
    }
    
    if (bytes_read == 0) {
        fprintf(stderr, "Error: Record #%d does not exist\n", record_number);
        close(fd);
        return -1;
    }
    
    if (bytes_read != sizeof(Employee)) {
        fprintf(stderr, "Error: Partial read occurred\n");
        close(fd);
        return -1;
    }
    
    close(fd);
    return 0;
}

/*
 * Display all records in the file
 */
void display_all_records(void) {
    int fd = open(FILENAME, O_RDONLY);
    
    if (fd < 0) {
        perror("Error opening file");
        return;
    }
    
    printf("\n========================================\n");
    printf("         ALL EMPLOYEE RECORDS           \n");
    printf("========================================\n\n");
    
    Employee emp;
    int record_num = 0;
    
    while (1) {
        ssize_t bytes_read = read(fd, &emp, sizeof(Employee));
        
        if (bytes_read == 0) {
            break; // End of file
        }
        
        if (bytes_read != sizeof(Employee)) {
            fprintf(stderr, "Warning: Corrupt record detected\n");
            break;
        }
        
        if (emp.active) {
            printf("Record #%d:\n", record_num);
            printf("  ID: %d\n", emp.id);
            printf("  Name: %s\n", emp.name);
            printf("  Department: %s\n", emp.department);
            printf("  Salary: $%.2f\n\n", emp.salary);
        }
        
        record_num++;
    }
    
    close(fd);
}

/*
 * Get file size and calculate number of records
 */
void display_file_info(void) {
    struct stat file_stat;
    
    if (stat(FILENAME, &file_stat) < 0) {
        perror("Error getting file info");
        return;
    }
    
    int total_records = file_stat.st_size / sizeof(Employee);
    
    printf("\n========================================\n");
    printf("           FILE INFORMATION             \n");
    printf("========================================\n\n");
    printf("Filename: %s\n", FILENAME);
    printf("File size: %ld bytes\n", (long)file_stat.st_size);
    printf("Record size: %zu bytes\n", sizeof(Employee));
    printf("Total records: %d\n", total_records);
    printf("Permissions: %o\n\n", file_stat.st_mode & 0777);
}

/*
 * Main demonstration program
 */
int main(void) {
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║   Employee Record Management System   ║\n");
    printf("║   Using Low-Level System Calls        ║\n");
    printf("╚════════════════════════════════════════╝\n\n");
    
    // Step 1: Create file
    printf("STEP 1: Creating file...\n");
    printf("----------------------------------------\n");
    if (create_file() < 0) {
        return EXIT_FAILURE;
    }
    
    // Step 2: Write initial records
    printf("STEP 2: Writing employee records...\n");
    printf("----------------------------------------\n");
    
    Employee employees[] = {
        {101, "Alice Johnson", "Engineering", 75000.00, 1},
        {102, "Bob Smith", "Marketing", 65000.00, 1},
        {103, "Carol White", "HR", 60000.00, 1},
        {104, "David Brown", "Finance", 70000.00, 1},
        {105, "Eve Davis", "Engineering", 80000.00, 1}
    };
    
    for (int i = 0; i < 5; i++) {
        if (write_employee(&employees[i]) < 0) {
            return EXIT_FAILURE;
        }
    }
    
    // Step 3: Display file info
    display_file_info();
    
    // Step 4: Display all records
    display_all_records();
    
    // Step 5: Update a specific record
    printf("STEP 3: Updating record #2 (Carol White)...\n");
    printf("----------------------------------------\n");
    
    Employee updated_emp = {103, "Carol White-Miller", "HR", 65000.00, 1};
    if (update_employee(2, &updated_emp) < 0) {
        return EXIT_FAILURE;
    }
    
    // Step 6: Retrieve specific record
    printf("STEP 4: Retrieving record #2...\n");
    printf("----------------------------------------\n");
    
    Employee retrieved_emp;
    if (read_employee(2, &retrieved_emp) == 0) {
        printf("✓ Record retrieved successfully:\n");
        printf("  ID: %d\n", retrieved_emp.id);
        printf("  Name: %s\n", retrieved_emp.name);
        printf("  Department: %s\n", retrieved_emp.department);
        printf("  Salary: $%.2f\n\n", retrieved_emp.salary);
    }
    
    // Step 7: Display all records again
    printf("STEP 5: Displaying all records after update...\n");
    printf("----------------------------------------\n");
    display_all_records();
    
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║        Program Completed Successfully  ║\n");
    printf("╚════════════════════════════════════════╝\n\n");
    
    return EXIT_SUCCESS;
}
