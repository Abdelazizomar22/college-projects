#pragma once

#include "Models.hpp"

#include <memory>
#include <string>
#include <vector>

struct StoragePaths {
  std::string students_csv = "data/students.csv";
  std::string courses_csv = "data/courses.csv";
  std::string enrollments_csv = "data/enrollments.csv";
};

struct StorageSnapshot {
  std::vector<std::unique_ptr<Student>> students;
  std::vector<Course> courses;
  std::vector<Enrollment> enrollments;
};

class CsvStorage {
 public:
  explicit CsvStorage(StoragePaths paths);

  StorageSnapshot Load() const;
  void Save(const StorageSnapshot& snapshot) const;

 private:
  StoragePaths paths_;

  static std::vector<std::string> SplitCsvLine(const std::string& line);
  static std::string JoinCsvLine(const std::vector<std::string>& fields);
  static std::string Escape(const std::string& field);

  static bool ToInt(const std::string& s, int* out);
  static bool ToDouble(const std::string& s, double* out);
};

