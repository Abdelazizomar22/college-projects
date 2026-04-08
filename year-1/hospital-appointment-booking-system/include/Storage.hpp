#pragma once

#include "Models.hpp"

#include <memory>
#include <string>
#include <vector>

struct StoragePathsHabs {
  std::string patients_csv = "data/patients.csv";
  std::string doctors_csv = "data/doctors.csv";
  std::string appointments_csv = "data/appointments.csv";
};

struct SnapshotHabs {
  std::vector<std::unique_ptr<Patient>> patients;
  std::vector<std::unique_ptr<Doctor>> doctors;
  std::vector<Appointment> appointments;
};

class CsvStorageHabs {
 public:
  explicit CsvStorageHabs(StoragePathsHabs paths);

  SnapshotHabs Load() const;
  void Save(const SnapshotHabs& snapshot) const;

  static std::string StatusToString(AppointmentStatus s);
  static AppointmentStatus StatusFromString(const std::string& s);

 private:
  StoragePathsHabs paths_;

  static std::vector<std::string> SplitCsvLine(const std::string& line);
  static std::string JoinCsvLine(const std::vector<std::string>& fields);
  static std::string Escape(const std::string& field);
};

