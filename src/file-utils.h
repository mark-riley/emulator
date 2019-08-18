#pragma once

#ifndef EMULATOR_FILE_UTILS_H
#define EMULATOR_FILE_UTILS_H

#include <cstdint>
#include <string>
#include <vector>

std::vector<uint8_t> readFileToBuffer(const std::string&);

#endif