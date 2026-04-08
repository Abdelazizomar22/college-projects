#pragma once

#include "Models.hpp"

#include <memory>
#include <string>
#include <vector>

namespace input {
std::string ReadLine(const std::string& prompt);
bool ReadInt(const std::string& prompt, int* out);
bool ReadDouble(const std::string& prompt, double* out);
}  // namespace input

namespace grading {
double NumericToGpa(double grade_0_100);
}  // namespace grading

const Student* FindStudentById(const std::vector<std::unique_ptr<Student>>& students, int id);
const Course* FindCourseByCode(const std::vector<Course>& courses, const std::string& code);

