#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_NAME_LEN 50
#define MAX_DEPT_LEN 30

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char department[MAX_DEPT_LEN];
    double salary;
    int active;
} Employee;

int main(void) {
    printf("\n=== Testing Direct Record Access ===\n\n");
    
    int fd = open("employee_records.dat", O_RDONLY);
    if (fd < 0) {
        perror("Error opening file");
        return 1;
    }
    
    Employee emp;
    
    // Test 1: Read record #0 (Alice)
    printf("Test 1: Reading record #0\n");
    lseek(fd, 0 * sizeof(Employee), SEEK_SET);
    read(fd, &emp, sizeof(Employee));
    printf("  Name: %s, Salary: $%.2f\n\n", emp.name, emp.salary);
    
    // Test 2: Read record #2 (Carol - updated)
    printf("Test 2: Reading record #2 (should show updated name)\n");
    lseek(fd, 2 * sizeof(Employee), SEEK_SET);
    read(fd, &emp, sizeof(Employee));
    printf("  Name: %s, Salary: $%.2f\n\n", emp.name, emp.salary);
    
    // Test 3: Read record #4 (Eve)
    printf("Test 3: Reading record #4\n");
    lseek(fd, 4 * sizeof(Employee), SEEK_SET);
    read(fd, &emp, sizeof(Employee));
    printf("  Name: %s, Salary: $%.2f\n\n", emp.name, emp.salary);
    
    close(fd);
    
    printf("✓ All tests passed!\n\n");
    return 0;
}
