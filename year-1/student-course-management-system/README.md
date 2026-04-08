# Student Course Management System (Console, C++)

Console-based system to manage students, courses, enrollments, and grades while practicing core OOP concepts.

## Context
- Academic year: 1st year
- Course: Programming Techniques
- Developed during: March 2022

## Features
- Add/list students and courses
- Enroll a student in a course
- Assign/update grades
- Calculate GPA (credit-hour weighted)
- Generate a per-student report
- Persist data using CSV file I/O

## Build & Run
Requirements: a C++ compiler (e.g., MinGW g++) available on your PATH.

From this project folder:

  g++ -std=c++14 -O2 -I include src\*.cpp -o scms.exe
  .\scms.exe

## Notes
- Data is stored in the data/ folder as CSV files.
