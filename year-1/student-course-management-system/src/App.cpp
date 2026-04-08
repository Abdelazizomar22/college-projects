#include "App.hpp"

#include "Utils.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>

StudentCourseApp::StudentCourseApp(CsvStorage storage) : storage_(std::move(storage)) {
  snapshot_ = storage_.Load();
}

void StudentCourseApp::PrintMenu() const {
  std::cout << "\n=== Student Course Management System ===\n"
            << "1) Add student\n"
            << "2) Add course\n"
            << "3) Enroll student in course\n"
            << "4) Assign grade\n"
            << "5) Student report\n"
            << "6) List students\n"
            << "7) List courses\n"
            << "8) Save\n"
            << "9) Save & exit\n"
            << "0) Exit (no save)\n";
}

void StudentCourseApp::Run() {
  while (true) {
    PrintMenu();
    int choice = 0;
    if (!input::ReadInt("Choose: ", &choice)) continue;

    switch (choice) {
      case 1:
        AddStudent();
        break;
      case 2:
        AddCourse();
        break;
      case 3:
        EnrollStudent();
        break;
      case 4:
        AssignGrade();
        break;
      case 5:
        PrintStudentReport();
        break;
      case 6:
        ListStudents();
        break;
      case 7:
        ListCourses();
        break;
      case 8:
        Save();
        break;
      case 9:
        Save();
        return;
      case 0:
        return;
      default:
        std::cout << "Unknown choice.\n";
        break;
    }
  }
}

void StudentCourseApp::AddStudent() {
  int id = 0;
  if (!input::ReadInt("Student ID (number): ", &id)) return;
  if (FindStudentById(snapshot_.students, id)) {
    std::cout << "Student with this ID already exists.\n";
    return;
  }

  auto name = input::ReadLine("Student name: ");
  if (name.empty()) return;

  auto student = std::make_unique<Student>();
  student->id = id;
  student->name = name;
  snapshot_.students.push_back(std::move(student));

  std::cout << "Student added.\n";
}

void StudentCourseApp::AddCourse() {
  auto code = input::ReadLine("Course code (e.g., CS101): ");
  if (code.empty()) return;
  if (FindCourseByCode(snapshot_.courses, code)) {
    std::cout << "Course with this code already exists.\n";
    return;
  }

  auto title = input::ReadLine("Course title: ");
  if (title.empty()) return;

  int credit_hours = 0;
  if (!input::ReadInt("Credit hours: ", &credit_hours) || credit_hours <= 0) {
    std::cout << "Invalid credit hours.\n";
    return;
  }

  snapshot_.courses.push_back(Course{code, title, credit_hours});
  std::cout << "Course added.\n";
}

void StudentCourseApp::EnrollStudent() {
  int student_id = 0;
  if (!input::ReadInt("Student ID: ", &student_id)) return;
  if (!FindStudentById(snapshot_.students, student_id)) {
    std::cout << "Student not found.\n";
    return;
  }

  auto course_code = input::ReadLine("Course code: ");
  if (course_code.empty()) return;
  if (!FindCourseByCode(snapshot_.courses, course_code)) {
    std::cout << "Course not found.\n";
    return;
  }

  auto it = std::find_if(snapshot_.enrollments.begin(), snapshot_.enrollments.end(),
                         [&](const Enrollment& e) {
                           return e.student_id == student_id && e.course_code == course_code;
                         });
  if (it != snapshot_.enrollments.end()) {
    std::cout << "Student is already enrolled in this course.\n";
    return;
  }

  Enrollment e;
  e.student_id = student_id;
  e.course_code = course_code;
  e.grade = 0.0;
  e.has_grade = false;
  snapshot_.enrollments.push_back(e);
  std::cout << "Enrollment added.\n";
}

void StudentCourseApp::AssignGrade() {
  int student_id = 0;
  if (!input::ReadInt("Student ID: ", &student_id)) return;
  auto course_code = input::ReadLine("Course code: ");
  if (course_code.empty()) return;

  double grade = 0.0;
  if (!input::ReadDouble("Grade (0..100): ", &grade) || grade < 0.0 || grade > 100.0) {
    std::cout << "Invalid grade.\n";
    return;
  }

  auto it = std::find_if(snapshot_.enrollments.begin(), snapshot_.enrollments.end(),
                         [&](const Enrollment& e) {
                           return e.student_id == student_id && e.course_code == course_code;
                         });
  if (it == snapshot_.enrollments.end()) {
    std::cout << "Enrollment not found.\n";
    return;
  }

  it->grade = grade;
  it->has_grade = true;
  std::cout << "Grade saved.\n";
}

void StudentCourseApp::ListStudents() const {
  std::cout << "\nStudents:\n";
  for (const auto& s : snapshot_.students) {
    std::cout << "- " << s->id << " | " << s->name << "\n";
  }
}

void StudentCourseApp::ListCourses() const {
  std::cout << "\nCourses:\n";
  for (const auto& c : snapshot_.courses) {
    std::cout << "- " << c.code << " | " << c.title << " (" << c.credit_hours << " CH)\n";
  }
}

void StudentCourseApp::PrintStudentReport() const {
  int student_id = 0;
  if (!input::ReadInt("Student ID: ", &student_id)) return;

  const Student* student = FindStudentById(snapshot_.students, student_id);
  if (!student) {
    std::cout << "Student not found.\n";
    return;
  }

  std::cout << "\n=== Report: " << student->name << " (" << student->id << ") ===\n";

  double total_points = 0.0;
  int total_credits = 0;

  for (const auto& e : snapshot_.enrollments) {
    if (e.student_id != student->id) continue;

    const Course* course = FindCourseByCode(snapshot_.courses, e.course_code);
    if (!course) continue;

    std::cout << "- " << course->code << " | " << course->title << " | " << course->credit_hours
              << " CH | ";

    if (e.has_grade) {
      double gpa_points = grading::NumericToGpa(e.grade);
      std::cout << std::fixed << std::setprecision(1) << e.grade << " (GPA " << gpa_points << ")\n";
      total_points += gpa_points * course->credit_hours;
      total_credits += course->credit_hours;
    } else {
      std::cout << "No grade\n";
    }
  }

  double gpa = (total_credits == 0) ? 0.0 : (total_points / static_cast<double>(total_credits));
  std::cout << "---------------------------\n";
  std::cout << "GPA: " << std::fixed << std::setprecision(2) << gpa << "\n";
}

void StudentCourseApp::Save() {
  storage_.Save(snapshot_);
  std::cout << "Saved to CSV.\n";
}

