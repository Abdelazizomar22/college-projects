#include "App.hpp"

#include "Utils.hpp"

#include <algorithm>
#include <iostream>

HospitalApp::HospitalApp(CsvStorageHabs storage) : storage_(std::move(storage)) {
  snapshot_ = storage_.Load();
}

void HospitalApp::PrintMenu() const {
  std::cout << "\n=== Hospital Appointment Booking System ===\n"
            << "1) Register patient\n"
            << "2) Add doctor\n"
            << "3) Book appointment\n"
            << "4) Cancel appointment\n"
            << "5) View doctor schedule\n"
            << "6) View patient appointments\n"
            << "7) Save\n"
            << "8) Save & exit\n"
            << "0) Exit (no save)\n";
}

void HospitalApp::Run() {
  while (true) {
    PrintMenu();
    int choice = 0;
    if (!input::ReadInt("Choose: ", &choice)) continue;

    switch (choice) {
      case 1:
        AddPatient();
        break;
      case 2:
        AddDoctor();
        break;
      case 3:
        BookAppointment();
        break;
      case 4:
        CancelAppointment();
        break;
      case 5:
        ListDoctorSchedule();
        break;
      case 6:
        ListPatientAppointments();
        break;
      case 7:
        Save();
        break;
      case 8:
        Save();
        return;
      case 0:
        return;
      default:
        std::cout << "Unknown choice.\n";
        break;
    }
  }
}

void HospitalApp::AddPatient() {
  int id = 0;
  if (!input::ReadInt("Patient ID: ", &id)) return;
  if (FindPatient(snapshot_.patients, id)) {
    std::cout << "Patient with this ID already exists.\n";
    return;
  }
  auto name = input::ReadLine("Patient name: ");
  if (name.empty()) return;
  auto phone = input::ReadLine("Phone: ");
  if (phone.empty()) return;

  auto p = std::make_unique<Patient>();
  p->id = id;
  p->name = name;
  p->phone = phone;
  snapshot_.patients.push_back(std::move(p));

  std::cout << "Patient registered.\n";
}

void HospitalApp::AddDoctor() {
  int id = 0;
  if (!input::ReadInt("Doctor ID: ", &id)) return;
  if (FindDoctor(snapshot_.doctors, id)) {
    std::cout << "Doctor with this ID already exists.\n";
    return;
  }
  auto name = input::ReadLine("Doctor name: ");
  if (name.empty()) return;
  auto specialty = input::ReadLine("Specialty: ");
  if (specialty.empty()) return;

  auto d = std::make_unique<Doctor>();
  d->id = id;
  d->name = name;
  d->specialty = specialty;
  snapshot_.doctors.push_back(std::move(d));

  std::cout << "Doctor added.\n";
}

int HospitalApp::NextAppointmentId() const {
  int max_id = 0;
  for (const auto& a : snapshot_.appointments) max_id = std::max(max_id, a.id);
  return max_id + 1;
}

void HospitalApp::BookAppointment() {
  int patient_id = 0;
  if (!input::ReadInt("Patient ID: ", &patient_id)) return;
  if (!FindPatient(snapshot_.patients, patient_id)) {
    std::cout << "Patient not found.\n";
    return;
  }

  int doctor_id = 0;
  if (!input::ReadInt("Doctor ID: ", &doctor_id)) return;
  if (!FindDoctor(snapshot_.doctors, doctor_id)) {
    std::cout << "Doctor not found.\n";
    return;
  }

  auto datetime = input::ReadLine("DateTime (YYYY-MM-DD HH:MM): ");
  if (!LooksLikeDateTime(datetime)) {
    std::cout << "Invalid datetime format.\n";
    return;
  }

  // Prevent doctor double-booking for the same datetime (scheduled only).
  bool conflict = std::any_of(snapshot_.appointments.begin(), snapshot_.appointments.end(),
                              [&](const Appointment& a) {
                                return a.doctor_id == doctor_id && a.datetime == datetime &&
                                       a.status == AppointmentStatus::Scheduled;
                              });
  if (conflict) {
    std::cout << "Doctor is already booked at that time.\n";
    return;
  }

  Appointment appt;
  appt.id = NextAppointmentId();
  appt.patient_id = patient_id;
  appt.doctor_id = doctor_id;
  appt.datetime = datetime;
  appt.status = AppointmentStatus::Scheduled;

  snapshot_.appointments.push_back(appt);
  std::cout << "Appointment booked. ID = " << snapshot_.appointments.back().id << "\n";
}

void HospitalApp::CancelAppointment() {
  int id = 0;
  if (!input::ReadInt("Appointment ID: ", &id)) return;

  auto it = std::find_if(snapshot_.appointments.begin(), snapshot_.appointments.end(),
                         [&](const Appointment& a) { return a.id == id; });
  if (it == snapshot_.appointments.end()) {
    std::cout << "Appointment not found.\n";
    return;
  }

  it->status = AppointmentStatus::Cancelled;
  std::cout << "Appointment cancelled.\n";
}

void HospitalApp::ListDoctorSchedule() const {
  int doctor_id = 0;
  if (!input::ReadInt("Doctor ID: ", &doctor_id)) return;
  const Doctor* doctor = FindDoctor(snapshot_.doctors, doctor_id);
  if (!doctor) {
    std::cout << "Doctor not found.\n";
    return;
  }

  std::cout << "\nSchedule for Dr. " << doctor->name << " (" << doctor->specialty << ")\n";
  for (const auto& a : snapshot_.appointments) {
    if (a.doctor_id != doctor->id) continue;
    std::cout << "- [" << a.id << "] " << a.datetime << " | " << CsvStorageHabs::StatusToString(a.status) << "\n";
  }
}

void HospitalApp::ListPatientAppointments() const {
  int patient_id = 0;
  if (!input::ReadInt("Patient ID: ", &patient_id)) return;
  const Patient* patient = FindPatient(snapshot_.patients, patient_id);
  if (!patient) {
    std::cout << "Patient not found.\n";
    return;
  }

  std::cout << "\nAppointments for " << patient->name << "\n";
  for (const auto& a : snapshot_.appointments) {
    if (a.patient_id != patient->id) continue;
    std::cout << "- [" << a.id << "] Doctor " << a.doctor_id << " | " << a.datetime << " | "
              << CsvStorageHabs::StatusToString(a.status) << "\n";
  }
}

void HospitalApp::Save() {
  storage_.Save(snapshot_);
  std::cout << "Saved to CSV.\n";
}

