#include "Storage.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

CsvStorage::CsvStorage(StoragePaths paths) : paths_(std::move(paths)) {}

StorageSnapshot CsvStorage::Load() const {
  StorageSnapshot snapshot;

  // Students
  {
    std::ifstream in(paths_.students_csv);
    std::string line;
    while (std::getline(in, line)) {
      if (line.empty()) continue;
      auto fields = SplitCsvLine(line);
      if (fields.size() < 2) continue;
      int id = 0;
      if (!ToInt(fields[0], &id)) continue;
      auto s = std::make_unique<Student>();
      s->id = id;
      s->name = fields[1];
      snapshot.students.push_back(std::move(s));
    }
  }

  // Courses
  {
    std::ifstream in(paths_.courses_csv);
    std::string line;
    while (std::getline(in, line)) {
      if (line.empty()) continue;
      auto fields = SplitCsvLine(line);
      if (fields.size() < 3) continue;
      int ch = 0;
      if (!ToInt(fields[2], &ch)) continue;
      snapshot.courses.push_back(Course{fields[0], fields[1], ch});
    }
  }

  // Enrollments
  {
    std::ifstream in(paths_.enrollments_csv);
    std::string line;
    while (std::getline(in, line)) {
      if (line.empty()) continue;
      auto fields = SplitCsvLine(line);
      if (fields.size() < 3) continue;

      int sid = 0;
      if (!ToInt(fields[0], &sid)) continue;

      Enrollment e;
      e.student_id = sid;
      e.course_code = fields[1];
      e.grade = 0.0;
      e.has_grade = false;

      double g = 0.0;
      if (ToDouble(fields[2], &g)) {
        e.grade = g;
        e.has_grade = true;
      }

      snapshot.enrollments.push_back(e);
    }
  }

  return snapshot;
}

void CsvStorage::Save(const StorageSnapshot& snapshot) const {
  // Students
  {
    std::ofstream out(paths_.students_csv, std::ios::trunc);
    if (!out) throw std::runtime_error("Failed to write students CSV");
    for (const auto& s : snapshot.students) {
      out << JoinCsvLine({std::to_string(s->id), s->name}) << "\n";
    }
  }

  // Courses
  {
    std::ofstream out(paths_.courses_csv, std::ios::trunc);
    if (!out) throw std::runtime_error("Failed to write courses CSV");
    for (const auto& c : snapshot.courses) {
      out << JoinCsvLine({c.code, c.title, std::to_string(c.credit_hours)}) << "\n";
    }
  }

  // Enrollments
  {
    std::ofstream out(paths_.enrollments_csv, std::ios::trunc);
    if (!out) throw std::runtime_error("Failed to write enrollments CSV");
    for (const auto& e : snapshot.enrollments) {
      out << JoinCsvLine(
                 {std::to_string(e.student_id), e.course_code, e.has_grade ? std::to_string(e.grade) : ""})
          << "\n";
    }
  }
}

std::vector<std::string> CsvStorage::SplitCsvLine(const std::string& line) {
  std::vector<std::string> fields;
  std::string current;
  bool in_quotes = false;

  for (size_t i = 0; i < line.size(); i++) {
    char c = line[i];
    if (c == '"') {
      if (in_quotes && i + 1 < line.size() && line[i + 1] == '"') {
        current.push_back('"');
        i++;
      } else {
        in_quotes = !in_quotes;
      }
    } else if (c == ',' && !in_quotes) {
      fields.push_back(current);
      current.clear();
    } else {
      current.push_back(c);
    }
  }

  fields.push_back(current);
  return fields;
}

std::string CsvStorage::JoinCsvLine(const std::vector<std::string>& fields) {
  std::ostringstream out;
  for (size_t i = 0; i < fields.size(); i++) {
    if (i) out << ",";
    out << Escape(fields[i]);
  }
  return out.str();
}

std::string CsvStorage::Escape(const std::string& field) {
  bool needs_quotes = field.find_first_of(",\"\n\r") != std::string::npos;
  if (!needs_quotes) return field;

  std::string escaped;
  escaped.reserve(field.size() + 2);
  escaped.push_back('"');
  for (char c : field) {
    if (c == '"') {
      escaped += "\"\"";
    } else {
      escaped.push_back(c);
    }
  }
  escaped.push_back('"');
  return escaped;
}

bool CsvStorage::ToInt(const std::string& s, int* out) {
  if (!out) return false;
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

bool CsvStorage::ToDouble(const std::string& s, double* out) {
  if (!out) return false;
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

