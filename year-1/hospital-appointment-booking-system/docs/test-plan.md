# Test Plan (Quick)

## Smoke tests
- Create a patient and doctor, book an appointment, save, restart, verify data is loaded.

## Validation tests
- Try booking with non-existent patient -> rejected.
- Try booking with non-existent doctor -> rejected.
- Try booking with invalid datetime format -> rejected.
- Book two appointments for the same doctor at the same datetime -> second is rejected.

## Persistence tests
- Save with multiple patients/doctors/appointments then reload and compare counts.

