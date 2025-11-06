#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

struct Patient {
    int id;
    char name[50];
    int age;
    char disease[50];
};

struct Patient queue[MAX];
struct Patient stack[MAX];
int front = -1, rear = -1, top = -1;

void addPatient();
void displayPatients();
void dischargePatient();
void undoDischarge();
void saveToFile();
void loadFromFile();

void addPatient() {
    if (rear == MAX - 1) {
        printf("\nQueue is full!\n");
        return;
    }

    struct Patient p;
    printf("\nEnter Patient ID: ");
    scanf("%d", &p.id);
    printf("Enter Name: ");
    scanf("%s", p.name);
    printf("Enter Age: ");
    scanf("%d", &p.age);
    printf("Enter Disease: ");
    scanf("%s", p.disease);

    if (front == -1)
        front = 0;
    queue[++rear] = p;

    printf("\nPatient added successfully!\n");
}

void displayPatients() {
    int i;
    if (front == -1) {
        printf("\nNo patients in queue!\n");
        return;
    }

    printf("\n--- Current Patients in Queue ---\n");
    for (i = front; i <= rear; i++) {
        printf("ID: %d | Name: %s | Age: %d | Disease: %s\n",
               queue[i].id, queue[i].name, queue[i].age, queue[i].disease);
    }
}

void dischargePatient() {
    if (front == -1) {
        printf("\nNo patients to discharge!\n");
        return;
    }

    struct Patient discharged = queue[front];
    if (front == rear)
        front = rear = -1;
    else
        front++;

    stack[++top] = discharged;
    printf("\nPatient %s (ID %d) discharged successfully.\n", discharged.name, discharged.id);
}

void undoDischarge() {
    if (top == -1) {
        printf("\nNo discharged patients to undo.\n");
        return;
    }

    struct Patient p = stack[top--];
    if (front == -1)
        front = 0;
    queue[++rear] = p;

    printf("\nDischarge of %s (ID %d) undone successfully.\n", p.name, p.id);
}

void saveToFile() {
    FILE *fp = fopen("patients.txt", "w");
    if (fp == NULL) {
        printf("\nError opening file!\n");
        return;
    }

    int i;
    for (i = front; i <= rear; i++) {
        fprintf(fp, "%d %s %d %s\n",
                queue[i].id, queue[i].name, queue[i].age, queue[i].disease);
    }

    fclose(fp);
    printf("\nData saved successfully to file!\n");
}

void loadFromFile() {
    FILE *fp = fopen("patients.txt", "r");
    if (fp == NULL) {
        printf("\nNo previous data found.\n");
        return;
    }

    struct Patient p;
    while (fscanf(fp, "%d %s %d %s", &p.id, p.name, &p.age, p.disease) != EOF) {
        if (front == -1)
            front = 0;
        queue[++rear] = p;
    }

    fclose(fp);
    printf("\nData loaded successfully from file!\n");
}

int main() {
    int choice;
    loadFromFile();

    while (1) {
        printf("\n--- Hospital Management System (Phase 2) ---\n");
        printf("1. Add Patient\n");
        printf("2. Display Patients\n");
        printf("3. Discharge Patient\n");
        printf("4. Undo Discharge\n");
        printf("5. Save Data\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1)
            addPatient();
        else if (choice == 2)
            displayPatients();
        else if (choice == 3)
            dischargePatient();
        else if (choice == 4)
            undoDischarge();
        else if (choice == 5)
            saveToFile();
        else if (choice == 6) {
            saveToFile();
            printf("\nExiting... All data saved successfully.\n");
            break;
        } else
            printf("\nInvalid choice! Please try again.\n");
    }

    return 0;
}

