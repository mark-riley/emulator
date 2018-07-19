#pragma once

#ifndef EMULATOR_REGISTERPAIR_H
#define EMULATOR_REGISTERPAIR_H

#include <cstdint>

struct register_pair {
    union {
        struct {
            uint8_t l;
            uint8_t h;
        };
        uint16_t rp;
    };
};

#endif //EMULATOR_REGISTERPAIR_H
