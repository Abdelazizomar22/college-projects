#include "Utils.hpp"

#include <iostream>

namespace input {

std::string ReadLine(const std::string& prompt) {
  std::cout << prompt;
  std::string s;
  std::getline(std::cin, s);
  return s;
}

bool ReadInt(const std::string& prompt, int* out) {
  if (!out) return false;
  auto s = ReadLine(prompt);
  if (s.empty()) return false;
  try {
    size_t idx = 0;
    int v = std::stoi(s, &idx);
    if (idx != s.size()) return false;
    *out = v;
    return true;
  } catch (...) {
    return false;
  }
}

bool ReadDouble(const std::string& prompt, double* out) {
  if (!out) return false;
  auto s = ReadLine(prompt);
  if (s.empty()) return false;
  try {
    size_t idx = 0;
    double v = std::stod(s, &idx);
    if (idx != s.size()) return false;
    *out = v;
    return true;
  } catch (...) {
    return false;
  }
}

}  // namespace input

namespace grading {

double NumericToGpa(double grade_0_100) {
  if (grade_0_100 >= 90.0) return 4.0;
  if (grade_0_100 >= 80.0) return 3.0;
  if (grade_0_100 >= 70.0) return 2.0;
  if (grade_0_100 >= 60.0) return 1.0;
  return 0.0;
}

}  // namespace grading

const Student* FindStudentById(const std::vector<std::unique_ptr<Student>>& students, int id) {
  for (const auto& s : students) {
    if (s && s->id == id) return s.get();
  }
  return nullptr;
}

const Course* FindCourseByCode(const std::vector<Course>& courses, const std::string& code) {
  for (const auto& c : courses) {
    if (c.code == code) return &c;
  }
  return nullptr;
}

