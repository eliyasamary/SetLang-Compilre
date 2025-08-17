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
```

### Example 1: Grading Students
```setlang
collection highTech, gaming;
collection software, hardware, industrial;
highTech = {"Apple", "Google", "Microsoft", "Nvidia", "Adobe", "Oracle", "Sap"};
highTech = highTech + {"PayPal", "Nice", "Amdocs", "OpenAI", "Ford", "Mercedes"};
gaming = {"Sony", "Apple", "Microsoft", "Google", "Nintendo", "Playtika"};
software = {"Apple", "Microsoft", "Oracle", "Google", "Sap", "PayPal", "Playtika", "Amdocs", "OpenAI"};
hardware = {"Apple", "Nice", "Sony", "Google", "Cummins", "Nucor", "Microsoft", "Nvidia"};
industrial = {"Caterpillar", "Cummins", "Nucor"};
output "Companies that sell hardware & software:" software & hardware;
collection highSW;
highSW = software & highTech;
if (highSW == software)
    output "All software companies are high-tech companies:" highSW;
else
    output "Not all software companies are high-tech companies:" highSW;
highSW = highSW + "Playtika";
if (highSW == software)
    output "Now all software companies are high-tech companies:" highSW;
else
    output "Not all software companies are high-tech companies:" highSW;
output "Companies that do software or hardware:" software + hardware;
if (industrial & software == {})
    output "No industrial companies sell software" ;
output "Companies that sell Hardware but not Gaming Software:" hardware - (software & gaming);
```

---

## 🚀 Steps
### 1: Clone repository
```
git clone https://github.com/<your-username>/set-lang-compiler.git
cd set-lang-compiler
```
### 2: Clean
```
make clean
```
### 3: Make
```
make
```
### 4: Run First Program
```
./parse.exe < prog1.c > output1.c
```

### 5: Run Seconde Program
```
./parse.exe < prog2.c > output2.c
```

---

This compiler was developed as part of a *Compilation course* in the *Bachelor's degree in Software Engineering*.
