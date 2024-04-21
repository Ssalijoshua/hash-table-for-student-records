#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_TABLE_SIZE 10  


// Define structure for a student (which also acts as a node)
typedef struct Student {
    char name[50];
    int studentNumber;
    char course[50];
    char dateOfBirth[11]; // Format: yyyy/mm/dd
    float tuition; // Tuition amount
    struct Student *next; // Pointer to the next student (for linked list)
} Student;

typedef struct {
    Student *head; // Pointer to the head of the linked list
} HashTable;

// Hash function to determine index for a given student number
int hashFunction(int studentNumber) {
    return studentNumber % HASH_TABLE_SIZE;
}


void insert(HashTable hashTable[], Student *student) {
    int index = hashFunction(student->studentNumber);
    
    // Insert the student at the beginning of the linked list at 'index'
    student->next = hashTable[index].head;
    hashTable[index].head = student;
}

// Print the contents of the hash table
void printHashTable(HashTable hashTable[]) {
    printf("Hash Table Contents:\n");
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        printf("%d\t", i);
        Student *current = hashTable[i].head;
        while (current != NULL) {
            printf(" %s -> ", current->name);
            current = current->next;
        }
        printf("NULL\n");
    }
}

// Search for a student by student number or name
Student* searchStudent(HashTable hashTable[], int studentNumber, const char *name) {
    int index = hashFunction(studentNumber);
    Student *current = hashTable[index].head;

    while (current != NULL) {
        if (current->studentNumber == studentNumber || strcmp(current->name, name) == 0) {
            return current; 
        }
        current = current->next;
    }

    return NULL;
}

// Delete a student from the hash table
void deleteStudent(HashTable hashTable[], int studentNumber, const char *name) {
    int index = hashFunction(studentNumber);
    Student *current = hashTable[index].head;
    Student *prev = NULL;

    while (current != NULL) {
        if (current->studentNumber == studentNumber || strcmp(current->name, name) == 0) {
            // Found the student, unlink and delete the node
            if (prev == NULL) {
                // Student is the head of the list
                hashTable[index].head = current->next;
            } else {
                // Student is not the head of the list
                prev->next = current->next;
            }
            free(current); 
            printf("Student deleted successfully.\n");
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("Student not found.\n");
}

// Print the details of a single student
void printStudentDetails(Student *student) {
    printf("Name: %s\n", student->name);
    printf("Student Number: %d\n", student->studentNumber);
    printf("Course: %s\n", student->course);
    printf("Date of Birth: %s\n", student->dateOfBirth);
    printf("Tuition Amount: %.2f\n", student->tuition);
}

// swap two student nodes to sort
void swapStudents(Student *a, Student *b) {
    Student temp = *a;
    *a = *b;
    *b = temp;
}

// Sort the contents of the hash table based on student name
void sortHashTable(HashTable hashTable[]) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Student *current = hashTable[i].head;
        int swapped;

        do {
            swapped = 0;
            while (current != NULL && current->next != NULL) {
                if (strcmp(current->name, current->next->name) > 0) {
                    swapStudents(current, current->next);
                    swapped = 1;
                }
                current = current->next;
            }
            current = hashTable[i].head; 
        } while (swapped);
    }
}

// Export hash table data to a CSV file
void exportToCSV(HashTable hashTable[], const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Write header row
    fprintf(file, "Name,Student Number,Course,Date of Birth,Tuition\n");

    // Write student data from hash table to CSV
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Student *current = hashTable[i].head;
        while (current != NULL) {
            fprintf(file, "%s,%d,%s,%s,%.2f\n",
                    current->name, current->studentNumber,
                    current->course, current->dateOfBirth,
                    current->tuition);
            current = current->next;
        }
    }

    fclose(file);
    printf("Data exported to CSV file successfully.\n");
}


int main() {
    HashTable hashTable[HASH_TABLE_SIZE] = { { NULL } };  // Initialize hash table
    
    int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Create a new student\n");
        printf("2. Sort the hash table by student name\n");
        printf("3. Search for a student\n");
        printf("4. Delete a student\n");
        printf("5. Print the hash table\n");
        printf("6. Export content to CSVfile\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                // Create a new student
                Student *student = (Student*)malloc(sizeof(Student));
                if (student == NULL) {
                    printf("Memory allocation failed.\n");
                    exit(EXIT_FAILURE);
                }

                printf("\nEnter Student Details:\n");
                printf("Name: ");
                scanf(" %[^\n]s", student->name);  // Read the name (with spaces)
                printf("Student Number: ");
                scanf("%d", &student->studentNumber);
                printf("Course: ");
                scanf(" %[^\n]s", student->course);  // Read the course (with spaces)
                printf("Date of Birth (yyyy/mm/dd): ");
                scanf("%s", student->dateOfBirth);
                printf("Tuition Amount: ");
                scanf("%f", &student->tuition);

                student->next = NULL; // Initialize next pointer to NULL
                insert(hashTable, student);  // Insert the student into the hash table
                printf("Student added successfully.\n");
                break;
            }
            case 2: {
                // Sort the hash table by student name
                sortHashTable(hashTable);
                printf("Hash table sorted by student name.\n");
                break;
            }
            case 3: {
                // Search for a student
                int searchNumber;
                char searchName[50];

                printf("\nEnter student number or name to search: ");
                scanf("%d", &searchNumber);  // Assume integer input for student number
                scanf(" %[^\n]s", searchName);  // Read the name (with spaces)

                Student *foundStudent = searchStudent(hashTable, searchNumber, searchName);
                if (foundStudent != NULL) {
                    printf("\nStudent found:\n");
                    printStudentDetails(foundStudent);
                } else {
                    printf("\nStudent not found.\n");
                }
                break;
            }
            case 4: {
                // Delete a student
                int deleteNumber;
                char deleteName[50];

                printf("\nEnter student number or name to delete: ");
                scanf("%d", &deleteNumber);  // Assume integer input for student number
                scanf(" %[^\n]s", deleteName);  // Read the name (with spaces)

                deleteStudent(hashTable, deleteNumber, deleteName);
                break;
            }
            case 5: {
                // Print the hash table
                printHashTable(hashTable);
                break;
            }
            case 6:{
                exportToCSV(hashTable, "Students_details");
            }
            case 7: {
                // Exit the program
                printf("Exiting...\n");

                            // Free allocated memory for students (nodes)
                for (int i = 0; i < HASH_TABLE_SIZE; i++) {
                    Student *current = hashTable[i].head;
                    while (current != NULL) {
                        Student *temp = current;
                        current = current->next;
                        free(temp); // Free each student node
                    }
                }
                break;
            }


            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice < 8);

    return 0;
}
