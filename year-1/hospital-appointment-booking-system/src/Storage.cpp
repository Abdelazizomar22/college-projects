#include "Storage.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

CsvStorageHabs::CsvStorageHabs(StoragePathsHabs paths) : paths_(std::move(paths)) {}

std::string CsvStorageHabs::StatusToString(AppointmentStatus s) {
  switch (s) {
    case AppointmentStatus::Scheduled:
      return "scheduled";
    case AppointmentStatus::Cancelled:
      return "cancelled";
    case AppointmentStatus::Completed:
      return "completed";
    default:
      return "scheduled";
  }
}

AppointmentStatus CsvStorageHabs::StatusFromString(const std::string& s) {
  if (s == "cancelled") return AppointmentStatus::Cancelled;
  if (s == "completed") return AppointmentStatus::Completed;
  return AppointmentStatus::Scheduled;
}

SnapshotHabs CsvStorageHabs::Load() const {
  SnapshotHabs snapshot;

  // Patients
  {
    std::ifstream in(paths_.patients_csv);
    std::string line;
    while (std::getline(in, line)) {
      if (line.empty()) continue;
      auto fields = SplitCsvLine(line);
      if (fields.size() < 3) continue;
      auto p = std::make_unique<Patient>();
      p->id = std::stoi(fields[0]);
      p->name = fields[1];
      p->phone = fields[2];
      snapshot.patients.push_back(std::move(p));
    }
  }

  // Doctors
  {
    std::ifstream in(paths_.doctors_csv);
    std::string line;
    while (std::getline(in, line)) {
      if (line.empty()) continue;
      auto fields = SplitCsvLine(line);
      if (fields.size() < 3) continue;
      auto d = std::make_unique<Doctor>();
      d->id = std::stoi(fields[0]);
      d->name = fields[1];
      d->specialty = fields[2];
      snapshot.doctors.push_back(std::move(d));
    }
  }

  // Appointments
  {
    std::ifstream in(paths_.appointments_csv);
    std::string line;
    while (std::getline(in, line)) {
      if (line.empty()) continue;
      auto fields = SplitCsvLine(line);
      if (fields.size() < 5) continue;
      Appointment a;
      a.id = std::stoi(fields[0]);
      a.patient_id = std::stoi(fields[1]);
      a.doctor_id = std::stoi(fields[2]);
      a.datetime = fields[3];
      a.status = StatusFromString(fields[4]);
      snapshot.appointments.push_back(a);
    }
  }

  return snapshot;
}

void CsvStorageHabs::Save(const SnapshotHabs& snapshot) const {
  // Patients
  {
    std::ofstream out(paths_.patients_csv, std::ios::trunc);
    if (!out) throw std::runtime_error("Failed to write patients CSV");
    for (const auto& p : snapshot.patients) {
      out << JoinCsvLine({std::to_string(p->id), p->name, p->phone}) << "\n";
    }
  }

  // Doctors
  {
    std::ofstream out(paths_.doctors_csv, std::ios::trunc);
    if (!out) throw std::runtime_error("Failed to write doctors CSV");
    for (const auto& d : snapshot.doctors) {
      out << JoinCsvLine({std::to_string(d->id), d->name, d->specialty}) << "\n";
    }
  }

  // Appointments
  {
    std::ofstream out(paths_.appointments_csv, std::ios::trunc);
    if (!out) throw std::runtime_error("Failed to write appointments CSV");
    for (const auto& a : snapshot.appointments) {
      out << JoinCsvLine({std::to_string(a.id), std::to_string(a.patient_id), std::to_string(a.doctor_id), a.datetime,
                          StatusToString(a.status)})
          << "\n";
    }
  }
}

std::vector<std::string> CsvStorageHabs::SplitCsvLine(const std::string& line) {
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

std::string CsvStorageHabs::JoinCsvLine(const std::vector<std::string>& fields) {
  std::ostringstream out;
  for (size_t i = 0; i < fields.size(); i++) {
    if (i) out << ",";
    out << Escape(fields[i]);
  }
  return out.str();
}

std::string CsvStorageHabs::Escape(const std::string& field) {
  bool needs_quotes = field.find_first_of(",\"\n\r") != std::string::npos;
  if (!needs_quotes) return field;

  std::string escaped;
  escaped.reserve(field.size() + 2);
  escaped.push_back('"');
  for (char c : field) {
    if (c == '"') escaped += "\"\"";
    else escaped.push_back(c);
  }
  escaped.push_back('"');
  return escaped;
}

