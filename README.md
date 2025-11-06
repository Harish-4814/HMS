# 🏥 Hospital Management System (Phase 2)

## 📘 Overview
This is a **C-based console application** for managing hospital patients.  
It uses **queues** to manage active patients and **stacks** to handle undoing discharges.  
The program allows adding, displaying, discharging, undoing discharges, and saving/loading patient data from a file.

---

## ⚙️ Features
- ➕ Add new patients  
- 👀 Display all patients in the queue  
- 🏠 Discharge the first patient (FIFO)  
- ↩️ Undo the most recent discharge (LIFO)  
- 💾 Save and load data from a file (`patients.txt`)  
- 🚪 Auto-save data on exit  

---

## 🧱 Data Structures Used
- **Queue** → Manages current patients (First In, First Out)  
- **Stack** → Stores discharged patients for undo (Last In, First Out)

Each patient record is stored as:
```c
struct Patient {
    int id;
    char name[50];
    int age;
    char disease[50];
};
