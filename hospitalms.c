

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

// ---------------- STRUCTURE ----------------
struct Patient {
    int id;
    char name[30];
    int age;
    int priority;   // 1 = Emergency, 0 = Normal
};

// ---------------- GLOBAL VARIABLES ----------------
struct Patient patients[MAX];
int count = 0;

// Stack for discharged patient IDs
int stack[MAX];
int top = -1;

// ---------------- CHECK UNIQUE ID ----------------
int idExists(int id) {
    int i;
    for (i = 0; i < count; i++) {
        if (patients[i].id == id) {
            return 1;
        }
    }
    return 0;
}

// ---------------- SAVE ACTIVE PATIENTS ----------------
void saveToFile() {
    FILE *fp;
    int i;

    fp = fopen("patients.txt", "w");
    if (fp == NULL) {
        printf("File error!\n");
        return;
    }

    for (i = 0; i < count; i++) {
        fprintf(fp, "%d %s %d %d\n",
                patients[i].id,
                patients[i].name,
                patients[i].age,
                patients[i].priority);
    }
    fclose(fp);
}

// ---------------- SAVE DISCHARGED PATIENT ----------------
void saveDischargedToFile(struct Patient p) {
    FILE *fp;

    fp = fopen("discharged.txt", "a");
    if (fp == NULL) {
        printf("File error!\n");
        return;
    }

    fprintf(fp, "%d %s %d %d\n",
            p.id, p.name, p.age, p.priority);

    fclose(fp);
}

// ---------------- ADD PATIENT (PRIORITY INSERTION) ----------------
void addPatient() {
    struct Patient p;
    int i;

    printf("\nEnter Patient ID: ");
    scanf("%d", &p.id);

    if (idExists(p.id)) {
        printf("Error: Patient ID already exists!\n");
        return;
    }

    printf("Enter Name: ");
    scanf("%s", p.name);
    printf("Enter Age: ");
    scanf("%d", &p.age);
    printf("Emergency? (1 = Yes, 0 = No): ");
    scanf("%d", &p.priority);

    if (p.priority == 1) {
        for (i = count; i > 0; i--) {
            patients[i] = patients[i - 1];
        }
        patients[0] = p;
    } else {
        patients[count] = p;
    }

    count++;
    saveToFile();
    printf("Patient added successfully!\n");
}

// ---------------- DISPLAY ACTIVE PATIENTS ----------------
void displayPatients() {
    int i;

    if (count == 0) {
        printf("No active patients.\n");
        return;
    }

    printf("\nID\tName\tAge\tType\n");
    for (i = 0; i < count; i++) {
        printf("%d\t%s\t%d\t%s\n",
               patients[i].id,
               patients[i].name,
               patients[i].age,
               patients[i].priority ? "Emergency" : "Normal");
    }
}

// ---------------- SEARCH PATIENT ----------------
void searchPatient() {
    int id, i;
    FILE *fp;
    struct Patient p;

    printf("Enter Patient ID to search: ");
    scanf("%d", &id);

    // Search active patients
    for (i = 0; i < count; i++) {
        if (patients[i].id == id) {
            printf("\nPatient Found (ACTIVE):\n");
            printf("ID   : %d\n", patients[i].id);
            printf("Name : %s\n", patients[i].name);
            printf("Age  : %d\n", patients[i].age);
            printf("Type : %s\n",
                   patients[i].priority ? "Emergency" : "Normal");
            return;
        }
    }

    // Search discharged patients file
    fp = fopen("discharged.txt", "r");
    if (fp != NULL) {
        while (fscanf(fp, "%d %s %d %d",
               &p.id, p.name, &p.age, &p.priority) != EOF) {

            if (p.id == id) {
                printf("\nPatient Found (DISCHARGED):\n");
                printf("ID   : %d\n", p.id);
                printf("Name : %s\n", p.name);
                printf("Age  : %d\n", p.age);
                printf("Type : %s\n",
                       p.priority ? "Emergency" : "Normal");
                fclose(fp);
                return;
            }
        }
        fclose(fp);
    }

    printf("Patient not found.\n");
}

// ---------------- DISCHARGE PATIENT ----------------
void dischargePatient() {
    int id, i, pos = -1;

    printf("Enter Patient ID to discharge: ");
    scanf("%d", &id);

    for (i = 0; i < count; i++) {
        if (patients[i].id == id) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("Patient not found.\n");
        return;
    }

    // Save full record to discharged file
    saveDischargedToFile(patients[pos]);

    // Push ID to stack
    top++;
    stack[top] = patients[pos].id;

    // Remove from active array
    for (i = pos; i < count - 1; i++) {
        patients[i] = patients[i + 1];
    }

    count--;
    saveToFile();
    printf("Patient discharged successfully!\n");
}

// ---------------- SHOW DISCHARGED STACK ----------------
void showDischarged() {
    int i;

    if (top == -1) {
        printf("No discharged patients.\n");
        return;
    }

    printf("\nRecently Discharged Patients (Stack):\n");
    for (i = top; i >= 0; i--) {
        printf("Patient ID: %d\n", stack[i]);
    }
}

// ---------------- MAIN ----------------
int main() {
    int choice;

    do {
        printf("\n----- HOSPITAL MANAGEMENT SYSTEM -----\n");
        printf("1. Add Patient\n");
        printf("2. Display Active Patients\n");
        printf("3. Search Patient\n");
        printf("4. Discharge Patient\n");
        printf("5. Show Recently Discharged\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            addPatient();
            break;
        case 2:
            displayPatients();
            break;
        case 3:
            searchPatient();
            break;
        case 4:
            dischargePatient();
            break;
        case 5:
            showDischarged();
            break;
        case 0:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice!\n");
        }
    } while (choice != 0);

    return 0;
}