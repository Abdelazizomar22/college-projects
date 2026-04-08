#pragma once

#include <string>

struct Patient {
  int id{};
  std::string name;
  std::string phone;
};

struct Doctor {
  int id{};
  std::string name;
  std::string specialty;
};

enum class AppointmentStatus { Scheduled, Cancelled, Completed };

struct Appointment {
  int id{};
  int patient_id{};
  int doctor_id{};
  std::string datetime;  // YYYY-MM-DD HH:MM
  AppointmentStatus status{AppointmentStatus::Scheduled};
};

