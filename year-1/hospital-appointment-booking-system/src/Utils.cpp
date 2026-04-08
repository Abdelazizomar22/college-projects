#include "Utils.hpp"

#include <iostream>

namespace input {

std::string ReadLine(const std::string& prompt) {
  std::cout << prompt;
  std::string s;
  std::getline(std::cin, s);
  return s;
}

bool ReadInt(const std::string& prompt, int* out) {
  if (!out) return false;
  auto s = ReadLine(prompt);
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

}  // namespace input

bool LooksLikeDateTime(const std::string& s) {
  // Very lightweight format check: YYYY-MM-DD HH:MM (16 chars)
  if (s.size() != 16) return false;
  auto is_digit = [](char c) { return c >= '0' && c <= '9'; };
  for (int i : {0, 1, 2, 3, 5, 6, 8, 9, 11, 12, 14, 15}) {
    if (!is_digit(s[static_cast<size_t>(i)])) return false;
  }
  return s[4] == '-' && s[7] == '-' && s[10] == ' ' && s[13] == ':';
}

const Patient* FindPatient(const std::vector<std::unique_ptr<Patient>>& patients, int id) {
  for (const auto& p : patients) {
    if (p && p->id == id) return p.get();
  }
  return nullptr;
}

const Doctor* FindDoctor(const std::vector<std::unique_ptr<Doctor>>& doctors, int id) {
  for (const auto& d : doctors) {
    if (d && d->id == id) return d.get();
  }
  return nullptr;
}

