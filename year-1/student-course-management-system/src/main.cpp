#include "App.hpp"
#include "Storage.hpp"

#include <exception>
#include <iostream>

int main() {
  try {
    CsvStorage storage(StoragePaths{});
    StudentCourseApp app(std::move(storage));
    app.Run();
    return 0;
  } catch (const std::exception& ex) {
    std::cerr << "Fatal error: " << ex.what() << "\n";
    return 1;
  }
}
