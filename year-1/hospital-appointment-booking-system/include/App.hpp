#pragma once

#include "Storage.hpp"

class HospitalApp {
 public:
  explicit HospitalApp(CsvStorageHabs storage);
  void Run();

 private:
  CsvStorageHabs storage_;
  SnapshotHabs snapshot_;

  void PrintMenu() const;

  void AddPatient();
  void AddDoctor();
  void BookAppointment();
  void CancelAppointment();
  void ListDoctorSchedule() const;
  void ListPatientAppointments() const;
  void Save();

  int NextAppointmentId() const;
};

