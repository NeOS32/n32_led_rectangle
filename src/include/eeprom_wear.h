// SPDX-FileCopyrightText: 2021 Sebastian Serewa <neos32.project@gmail.com>
//
// SPDX-License-Identifier: Apache-2.0

#ifndef eeprom_wear_h_
#define eeprom_wear_h_

#include <Arduino.h>
#include <FastCRC.h>
#include <EEPROM.h>

#define MARKER 0xA5

struct myConfigData_s {
    int r, g, b;
    int main_lum;
    int mode;
};
struct myConfig_s {
    struct myConfigData_s data;
    int marker;
    int checksum;
};

struct eeprom_wear_s {
    int _max_size;
    bool readCfgAbs(myConfig_s& CFG, int read_offset) {
        EEPROM.get(read_offset, CFG);

        FastCRC8 CRC8;
        if (MARKER == CFG.marker)
            if (CFG.checksum == CRC8.smbus((const uint8_t*)&(CFG.data), sizeof(myConfigData_s)))
                return true;

        return false;
    }
    void writeCfgAbs(const myConfigData_s& cfg, int write_offset) {
        FastCRC8 CRC8;
        myConfig_s CFG;

        CFG.data = cfg;
        CFG.checksum = CRC8.smbus((const uint8_t*)&(CFG.data), sizeof(myConfigData_s));
        CFG.marker = MARKER;

        EEPROM.put(write_offset, CFG);
    };

    int readCfg(myConfigData_s& cfg) {
        myConfig_s CFG;

        for (int offset = 0; offset <= (int)(_max_size - sizeof(myConfig_s) - 1); offset++)
            if (true == this->readCfgAbs(CFG, offset)) {
                cfg= CFG.data;
                return offset + 1;
            }

        return -1;
    }
    int writeCfg(const myConfigData_s& cfg, int write_offset) {

        if (write_offset >= (int)(_max_size - sizeof(myConfig_s) - 1))
            write_offset = 0;

        this->writeCfgAbs(cfg, write_offset);

        return write_offset + 1; // next write offset
    };
    eeprom_wear_s(int max_size = 0) {
        if (max_size)
            _max_size = max_size;
        else
            _max_size = EEPROM.length();
    };
};

#endif // eeprom_wear_h_
