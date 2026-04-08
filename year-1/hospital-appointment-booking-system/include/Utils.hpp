#pragma once

#include "Models.hpp"

#include <memory>
#include <string>
#include <vector>

namespace input {
std::string ReadLine(const std::string& prompt);
bool ReadInt(const std::string& prompt, int* out);
}  // namespace input

bool LooksLikeDateTime(const std::string& s);

const Patient* FindPatient(const std::vector<std::unique_ptr<Patient>>& patients, int id);
const Doctor* FindDoctor(const std::vector<std::unique_ptr<Doctor>>& doctors, int id);

