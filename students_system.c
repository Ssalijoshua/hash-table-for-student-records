#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define HASH_TABLE_SIZE 10

// struct for student
typedef struct Student {
    char name[50];
    int studentNumber;
    char course[50];
    char dateOfBirth[11];
    float tuition;
    struct Student *next;
} Student;

// initialising the table to store a head pointer
typedef struct {
    Student *head;
} HashTable;

int hashFunction(int studentNumber) {
    return studentNumber % HASH_TABLE_SIZE;
}

// For inserting a student to the table as the head
void insert(HashTable hashTable[], Student *student) {
    int index = hashFunction(student->studentNumber);
    student->next = hashTable[index].head;
    hashTable[index].head = student;
}

// For printing the hash table
void printHashTable(HashTable hashTable[]) {
    printf("Hash Table Contents:\n");
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        printf("%d\t", i);
        Student *current = hashTable[i].head;
        while (current != NULL) {
            printf(" %s -> ", current->name);
            current = current->next;
        }
        printf("<-->\n");
    }
}

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

void deleteStudentByNumber(HashTable hashTable[], int studentNumber) {
    int index = hashFunction(studentNumber);
    Student *current = hashTable[index].head;
    Student *prev = NULL;

    while (current != NULL) {
        if (current->studentNumber == studentNumber) {
            if (prev == NULL) {
                hashTable[index].head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current); 
            printf("Student with student number %d deleted successfully.\n", studentNumber);
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("Student with student number %d not found.\n", studentNumber);
}

void deleteStudentByName(HashTable hashTable[], const char *name) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Student *current = hashTable[i].head;
        Student *prev = NULL;

        while (current != NULL) {
            if (strcmp(current->name, name) == 0) {
                if (prev == NULL) {
                    hashTable[i].head = current->next;
                } else {
                    prev->next = current->next;
                }
                free(current); 
                printf("Student with name '%s' deleted successfully.\n", name);
                return;
            }
            prev = current;
            current = current->next;
        }
    }

    printf("Student with name '%s' not found.\n", name);
}

void updateStudentDetails(HashTable hashTable[], int studentNumber) {
    Student *student = searchStudent(hashTable, studentNumber, "");
    if (student == NULL) {
        printf("Student with student number %d not found.\n", studentNumber);
        return;
    }

    printf("Enter updated name: ");
    scanf(" %[^\n]", student->name);
    printf("Enter updated course: ");
    scanf(" %[^\n]", student->course);
    printf("Enter updated date of birth (YYYY-MM-DD): ");
    scanf(" %[^\n]", student->dateOfBirth);
    printf("Enter updated tuition amount: ");
    scanf("%f", &student->tuition);

    printf("Student details updated successfully.\n");
}

void sortByName(HashTable hashTable[]) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Student *current = hashTable[i].head;
        int swapped;

        do {
            swapped = 0;
            while (current != NULL && current->next != NULL) {
                if (strcmp(current->name, current->next->name) > 0) {
                    // Swap nodes
                    Student *temp = current;
                    hashTable[i].head = current->next;
                    current->next = current->next->next;
                    hashTable[i].head->next = temp;
                    swapped = 1;
                }
                current = current->next;
            }
            current = hashTable[i].head;
        } while (swapped);
    }
}

void sortByStudentNumber(HashTable hashTable[]) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        Student *current = hashTable[i].head;
        int swapped;

        do {
            swapped = 0;
            while (current != NULL && current->next != NULL) {
                if (current->studentNumber > current->next->studentNumber) {
                    // Swap nodes
                    Student *temp = current;
                    hashTable[i].head = current->next;
                    current->next = current->next->next;
                    hashTable[i].head->next = temp;
                    swapped = 1;
                }
                current = current->next;
            }
            current = hashTable[i].head;
        } while (swapped);
    }
}

void exportToCSV(HashTable hashTable[], const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Customising header row
    fprintf(file, "Name,Student Number,Course,Date of Birth,Tuition\n");

    // Writing student data from hash table to CSV
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
    HashTable hashTable[HASH_TABLE_SIZE] = { { NULL } };

    int choice;
    do {
        printf("\nMain Menu:\n");
        printf("1. Create a new student\n");
        printf("2. Sort\n");
        printf("3. Delete\n");
        printf("4. Update student details\n");
        printf("5. Export data to CSV file\n");
        printf("6. Print the hash table\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                {
                    // Create a new student
                Student *student = (Student*)malloc(sizeof(Student));
                if (student == NULL) {
                    printf("Memory allocation failed.\n");
                    exit(EXIT_FAILURE);
                }

                printf("\nEnter Student Details:\n");
                printf("Name: ");
                scanf(" %[^\n]s", student->name);  
                printf("Student Number: ");
                scanf("%d", &student->studentNumber);
                printf("Course: ");
                scanf(" %[^\n]s", student->course);  // [^\n] reads the course with spaces
                printf("Date of Birth (yyyy/mm/dd): ");
                scanf("%s", student->dateOfBirth);
                printf("Tuition Amount: ");
                scanf("%f", &student->tuition);

                student->next = NULL; // Initialize next pointer to NULL
                insert(hashTable, student);  // Insert the student into the hash table
                printf("Student added successfully.\n");
                }
               
                break;
            case 2:
                {
                    int sortChoice;
                    printf("\nSort Menu:\n");
                    printf("1. Sort by Name\n");
                    printf("2. Sort by Student Number\n");
                    printf("Enter your sort choice: ");
                    scanf("%d", &sortChoice);

                    switch (sortChoice) {
                        case 1:
                            sortByName(hashTable);
                            printf("Hash table sorted by name.\n");
                            break;
                        case 2:
                            sortByStudentNumber(hashTable);
                            printf("Hash table sorted by student number.\n");
                            break;
                        default:
                            printf("Invalid sort choice.\n");
                    }
                    break;
                }
            case 3:
                {
                    int deleteChoice;
                    printf("\nDelete Menu:\n");
                    printf("1. Delete by Name\n");
                    printf("2. Delete by Student Number\n");
                    printf("Enter your delete choice: ");
                    scanf("%d", &deleteChoice);

                    switch (deleteChoice) {
                        case 1:
                            {
                                char deleteName[50];
                                printf("Enter student name to delete: ");
                                scanf(" %[^\n]", deleteName);
                                deleteStudentByName(hashTable, deleteName);
                                break;
                            }
                        case 2:
                            {
                                int deleteNumber;
                                printf("Enter student number to delete: ");
                                scanf("%d", &deleteNumber);
                                deleteStudentByNumber(hashTable, deleteNumber);
                                break;
                            }
                        default:
                            printf("Invalid delete choice.\n");
                    }
                    break;
                }
            case 4:
                {
                    int updateNumber;
                    printf("Enter the student number whose details you want to update: ");
                    scanf("%d", &updateNumber);
                    updateStudentDetails(hashTable, updateNumber);
                    break;
                }
            case 5:
                {
                    char filename[100];
                    printf("Enter CSV filename : ");
                    scanf(" %[^\n]", filename);
                    exportToCSV(hashTable, filename);
                    break;
                }
            case 6:
                    printHashTable(hashTable);
                break;
            case 7:
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
            default:
                printf("############### Invalid choice. Please try again. ####################\n");
        }
    } while (true);

    return 0;
}
