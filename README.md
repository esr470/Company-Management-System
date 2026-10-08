# 🏢 Company Management System

A simple **Company Management System** developed in **C**.

The project is a file-based employee management system that supports complete **CRUD operations** and stores employee data in a CSV file.

## ✨ Features

* ➕ Add new employees
* 📋 Display all employees
* 🔍 Search employees by partial name
* ✏️ Update employee information
* 🗑️ Delete employees
* 🔢 Sort employees by ID
* 💾 Save data permanently using a CSV file
* 📂 Load saved employee data when the program starts

## 👤 Employee Information

The system stores:

* ID
* Full Name
* Salary
* Date of Birth
* Address
* Mobile Number
* Enrollment Date
* Email

## 🛠️ Technologies

* C
* Standard C Library
* File Handling
* Structures
* Arrays
* Functions
* Sorting
* String Handling

## 📁 Project Structure

```text
Company-Management-System/
│
├── company_management.c
├── employees.csv
└── README.md
```

## 💾 Data Storage

Employee information is stored in:

```text
employees.csv
```

The program automatically saves employee data after adding, updating, or deleting an employee.

## ▶️ How to Run

### 1. Clone the repository

```bash
git clone https://github.com/YOUR_USERNAME/Company-Management-System.git
```

### 2. Open the project folder

```bash
cd Company-Management-System
```

### 3. Compile the program

Using GCC:

```bash
gcc company_management.c -o company_management
```

### 4. Run the program

On Windows:

```bash
company_management.exe
```

On Linux/macOS:

```bash
./company_management
```

## 📋 Main Menu

The program provides the following options:

```text
1. Add new employee
2. Display all employees
3. Search employee by partial name
4. Update employee
5. Delete employee
6. Save and exit
```

## 🧠 Concepts Used

This project demonstrates several important C programming concepts:

* `struct`
* Functions
* Arrays
* Pointers
* File handling
* String manipulation
* Searching
* Sorting
* CRUD operations

## 🚀 Future Improvements

Some possible improvements:

* Add stronger input validation
* Add a graphical user interface
* Add login and authentication
* Add more advanced employee searching
* Improve CSV parsing
* Add department and job title fields

---

## 👩‍💻 Project

**Company Management System**

Built with ❤️ using C.
