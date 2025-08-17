# Set Lang Compiler

Set Lang is a high-level programming language designed with a focus on **sets** and **collections** as first-class data types.  
This project implements a **compiler for Set Lang**, translating its syntax into **C/C++** for execution.

---

## ✨ Features

- **Custom Types**
  - `int` – integers (like in C)
  - `str` – string values
  - `set` – set of unique integers
  - `collection` – set of unique strings

- **Operations**
  - Standard arithmetic (`+`, `-`, `*`, `/`) for integers
  - Set/collection operations:
    - Union (`+`)
    - Difference (`-`)
    - Intersection (`&`)
    - Size (`|set|`, `|collection|`)
  - Insert / extract elements from sets & collections

- **Control Structures**
  - `if ... else`
  - `while`
  - `for (element : set | collection)`

- **Input/Output**
  - `input` with prompt text
  - `output` with strings and expressions

---

## 🔧 Example Programs

### Example 1: Grading Students
```setlang
collection class, highGradeStudents, lowGradeStudents, avgGradeStudents;
set grades, gradesHigh;
int grd;
str student;

class = {"Rafi", "Tamar", "Avi"};
gradesHigh = [];
highGradeStudents = {};

for (student:class) {
  output "Grade for:" student;
  input ">" grd;
  grades = grades + grd;
  if (grd >= 90) {
    gradesHigh = gradesHigh + grd;
    highGradeStudents = highGradeStudents + student;
  }
}

if (gradesHigh) {
  output "Number of top grades:" |gradesHigh|;
  output "High Grade Students are:" highGradeStudents;
}
