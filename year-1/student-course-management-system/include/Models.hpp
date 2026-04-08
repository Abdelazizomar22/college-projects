#pragma once

#include <string>

struct Student {
  int id{};
  std::string name;
};

struct Course {
  std::string code;
  std::string title;
  int credit_hours{};
};

struct Enrollment {
  int student_id{};
  std::string course_code;
  double grade{0.0};   // 0..100
  bool has_grade{false};
};

