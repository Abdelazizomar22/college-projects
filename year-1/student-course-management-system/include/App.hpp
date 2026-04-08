#pragma once

#include "Storage.hpp"

class StudentCourseApp {
 public:
  explicit StudentCourseApp(CsvStorage storage);
  void Run();

 private:
  CsvStorage storage_;
  StorageSnapshot snapshot_;

  void PrintMenu() const;
  void AddStudent();
  void AddCourse();
  void EnrollStudent();
  void AssignGrade();
  void ListStudents() const;
  void ListCourses() const;
  void PrintStudentReport() const;
  void Save();
};
