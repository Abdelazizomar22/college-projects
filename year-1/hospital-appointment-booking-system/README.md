# Hospital Appointment Booking System (Console, C++)

Console-based appointment booking system for a hospital/clinic. Includes basic SDLC-style documentation (requirements, simple UML, test plan).

## Context
- Academic year: 1st year
- Course: Software Engineering
- Developed during: April 2022

## Features
- Register patients
- Add doctors
- Book appointments (prevents doctor double-booking)
- Cancel appointments
- View doctor schedule and patient appointments
- Persist data to CSV using file I/O

## Build & Run
Requirements: a C++ compiler (e.g., MinGW g++) available on your PATH.

From this project folder:

  g++ -std=c++14 -O2 -I include src\*.cpp -o habs.exe
  .\habs.exe

## Docs
- docs/requirements.md
- docs/test-plan.md
- docs/uml (PlantUML text diagrams)
