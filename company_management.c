/* ============================================================
   Company Management System
   Language : C
   Description:
   A file-based employee management system supporting full CRUD
   operations (Create, Read, Update, Delete).
   Features:
     - Add employee
     - Display all employees (sorted by ID)
     - Search employees by partial name match
     - Update employee data
     - Delete employee
     - Persistent storage using a comma-delimited (CSV) file
   Fields tracked:
     ID, full name, salary, date of birth, address,
     mobile, enrollment date, email
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_EMPLOYEES 1000
#define FILE_NAME     "employees.csv"
#define LINE_LEN      512

typedef struct {
    int    id;
    char   name[100];
    double salary;
    char   dob[20];          /* date of birth      : DD-MM-YYYY */
    char   address[150];
    char   mobile[20];
    char   enrollDate[20];   /* enrollment date     : DD-MM-YYYY */
    char   email[100];
} Employee;

Employee employees[MAX_EMPLOYEES];
int employeeCount = 0;

/* ---------- utility helpers ---------- */

/* Remove a trailing newline (\n or \r\n) left by fgets */
void trimNewline(char *str) {
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
        len--;
    }
}

/* Replace commas inside free-text fields so the CSV format
   never breaks (commas are the column separator) */
void stripCommas(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ',') str[i] = ';';
    }
}

/* Safe string input with a fixed buffer size */
void readLine(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    trimNewline(buffer);
    stripCommas(buffer);
}

/* Case-insensitive substring search: returns 1 if `needle`
   appears anywhere inside `haystack` */
int containsIgnoreCase(const char *haystack, const char *needle) {
    char h[200], n[200];
    int i;
    for (i = 0; haystack[i] && i < 199; i++) h[i] = (char)tolower((unsigned char)haystack[i]);
    h[i] = '\0';
    for (i = 0; needle[i] && i < 199; i++) n[i] = (char)tolower((unsigned char)needle[i]);
    n[i] = '\0';
    return strstr(h, n) != NULL;
}

int findIndexById(int id) {
    for (int i = 0; i < employeeCount; i++)
        if (employees[i].id == id) return i;
    return -1;
}

int getNextId(void) {
    int maxId = 0;
    for (int i = 0; i < employeeCount; i++)
        if (employees[i].id > maxId) maxId = employees[i].id;
    return maxId + 1;
}

/* ---------- persistence: comma-delimited file I/O ---------- */

void saveToFile(void) {
    FILE *fp = fopen(FILE_NAME, "w");
    if (!fp) {
        printf("Error: could not open file for writing.\n");
        return;
    }
    for (int i = 0; i < employeeCount; i++) {
        Employee *e = &employees[i];
        fprintf(fp, "%d,%s,%.2f,%s,%s,%s,%s,%s\n",
                e->id, e->name, e->salary, e->dob,
                e->address, e->mobile, e->enrollDate, e->email);
    }
    fclose(fp);
}

void loadFromFile(void) {
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp) {
        /* No existing file yet -> start with an empty database */
        return;
    }

    char line[LINE_LEN];
    employeeCount = 0;

    while (fgets(line, LINE_LEN, fp) && employeeCount < MAX_EMPLOYEES) {
        trimNewline(line);
        if (strlen(line) == 0) continue;

        Employee e;
        char *token = strtok(line, ",");
        if (!token) continue;
        e.id = atoi(token);

        token = strtok(NULL, ","); strncpy(e.name, token ? token : "", sizeof(e.name) - 1); e.name[sizeof(e.name)-1]='\0';
        token = strtok(NULL, ","); e.salary = token ? atof(token) : 0.0;
        token = strtok(NULL, ","); strncpy(e.dob, token ? token : "", sizeof(e.dob) - 1); e.dob[sizeof(e.dob)-1]='\0';
        token = strtok(NULL, ","); strncpy(e.address, token ? token : "", sizeof(e.address) - 1); e.address[sizeof(e.address)-1]='\0';
        token = strtok(NULL, ","); strncpy(e.mobile, token ? token : "", sizeof(e.mobile) - 1); e.mobile[sizeof(e.mobile)-1]='\0';
        token = strtok(NULL, ","); strncpy(e.enrollDate, token ? token : "", sizeof(e.enrollDate) - 1); e.enrollDate[sizeof(e.enrollDate)-1]='\0';
        token = strtok(NULL, ","); strncpy(e.email, token ? token : "", sizeof(e.email) - 1); e.email[sizeof(e.email)-1]='\0';

        employees[employeeCount++] = e;
    }
    fclose(fp);
}

/* ---------- CRUD operations ---------- */

void addEmployee(void) {
    if (employeeCount >= MAX_EMPLOYEES) {
        printf("Database is full, cannot add more employees.\n");
        return;
    }

    Employee e;
    e.id = getNextId();

    readLine("Full name        : ", e.name, sizeof(e.name));

    char buf[64];
    readLine("Salary            : ", buf, sizeof(buf));
    e.salary = atof(buf);

    readLine("Date of birth (DD-MM-YYYY)   : ", e.dob, sizeof(e.dob));
    readLine("Address           : ", e.address, sizeof(e.address));
    readLine("Mobile            : ", e.mobile, sizeof(e.mobile));
    readLine("Enrollment date (DD-MM-YYYY) : ", e.enrollDate, sizeof(e.enrollDate));
    readLine("Email             : ", e.email, sizeof(e.email));

    employees[employeeCount++] = e;
    saveToFile();
    printf("Employee added successfully with ID %d.\n", e.id);
}

int compareById(const void *a, const void *b) {
    return ((Employee *)a)->id - ((Employee *)b)->id;
}

void printEmployee(const Employee *e) {
    printf("----------------------------------------------\n");
    printf("ID             : %d\n", e->id);
    printf("Name           : %s\n", e->name);
    printf("Salary         : %.2f\n", e->salary);
    printf("Date of Birth  : %s\n", e->dob);
    printf("Address        : %s\n", e->address);
    printf("Mobile         : %s\n", e->mobile);
    printf("Enrollment Date: %s\n", e->enrollDate);
    printf("Email          : %s\n", e->email);
}

void displayAllSorted(void) {
    if (employeeCount == 0) {
        printf("No employee records found.\n");
        return;
    }

    /* Sort a copy so the on-disk / in-memory order is untouched */
    Employee sorted[MAX_EMPLOYEES];
    memcpy(sorted, employees, sizeof(Employee) * employeeCount);
    qsort(sorted, employeeCount, sizeof(Employee), compareById);

    printf("\n=============== Employee List (sorted by ID) ===============\n");
    for (int i = 0; i < employeeCount; i++) {
        printEmployee(&sorted[i]);
    }
    printf("----------------------------------------------\n");
    printf("Total employees: %d\n", employeeCount);
}

void searchByPartialName(void) {
    char query[100];
    readLine("Enter name (or part of it) to search: ", query, sizeof(query));

    int found = 0;
    printf("\n================ Search Results ================\n");
    for (int i = 0; i < employeeCount; i++) {
        if (containsIgnoreCase(employees[i].name, query)) {
            printEmployee(&employees[i]);
            found++;
        }
    }
    if (!found) {
        printf("No employee matched \"%s\".\n", query);
    } else {
        printf("----------------------------------------------\n");
        printf("%d match(es) found.\n", found);
    }
}

void updateEmployee(void) {
    char buf[64];
    readLine("Enter employee ID to update: ", buf, sizeof(buf));
    int id = atoi(buf);
    int idx = findIndexById(id);

    if (idx == -1) {
        printf("No employee found with ID %d.\n", id);
        return;
    }

    Employee *e = &employees[idx];
    printf("Leave a field empty and press Enter to keep its current value.\n");

    char temp[150];

    readLine("New name            : ", temp, sizeof(temp));
    if (strlen(temp) > 0) strncpy(e->name, temp, sizeof(e->name) - 1);

    readLine("New salary          : ", temp, sizeof(temp));
    if (strlen(temp) > 0) e->salary = atof(temp);

    readLine("New date of birth   : ", temp, sizeof(temp));
    if (strlen(temp) > 0) strncpy(e->dob, temp, sizeof(e->dob) - 1);

    readLine("New address         : ", temp, sizeof(temp));
    if (strlen(temp) > 0) strncpy(e->address, temp, sizeof(e->address) - 1);

    readLine("New mobile          : ", temp, sizeof(temp));
    if (strlen(temp) > 0) strncpy(e->mobile, temp, sizeof(e->mobile) - 1);

    readLine("New enrollment date : ", temp, sizeof(temp));
    if (strlen(temp) > 0) strncpy(e->enrollDate, temp, sizeof(e->enrollDate) - 1);

    readLine("New email           : ", temp, sizeof(temp));
    if (strlen(temp) > 0) strncpy(e->email, temp, sizeof(e->email) - 1);

    saveToFile();
    printf("Employee %d updated successfully.\n", id);
}

void deleteEmployee(void) {
    char buf[64];
    readLine("Enter employee ID to delete: ", buf, sizeof(buf));
    int id = atoi(buf);
    int idx = findIndexById(id);

    if (idx == -1) {
        printf("No employee found with ID %d.\n", id);
        return;
    }

    for (int i = idx; i < employeeCount - 1; i++) {
        employees[i] = employees[i + 1];
    }
    employeeCount--;
    saveToFile();
    printf("Employee %d deleted successfully.\n", id);
}

/* ---------- menu ---------- */

void printMenu(void) {
    printf("\n================ Company Management System ================\n");
    printf("1. Add new employee\n");
    printf("2. Display all employees (sorted by ID)\n");
    printf("3. Search employee by partial name\n");
    printf("4. Update employee\n");
    printf("5. Delete employee\n");
    printf("6. Save and exit\n");
    printf("==============================================================\n");
    printf("Choose an option (1-6): ");
}

int main(void) {
    loadFromFile();

    char choiceBuf[16];
    int choice;

    do {
        printMenu();
        fgets(choiceBuf, sizeof(choiceBuf), stdin);
        choice = atoi(choiceBuf);

        switch (choice) {
            case 1: addEmployee();          break;
            case 2: displayAllSorted();     break;
            case 3: searchByPartialName();  break;
            case 4: updateEmployee();       break;
            case 5: deleteEmployee();       break;
            case 6:
                saveToFile();
                printf("Data saved to \"%s\". Goodbye!\n", FILE_NAME);
                break;
            default:
                printf("Invalid option, please choose a number from 1 to 6.\n");
        }
    } while (choice != 6);

    return 0;
}
