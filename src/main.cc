#include <iostream>
#include <spdlog/spdlog.h>

int main(int argc, char** argv) {
    // Prepare SPDLOG
    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("%^[%l] %v%$");

    spdlog::trace("Logging prepared.");
    
}