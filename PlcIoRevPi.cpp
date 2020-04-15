
#include "PlcIoRevPi.h"

#include <fcntl.h>
#include <unistd.h>

#include <iostream>
#include <cstring>

#include "PlcMemory.h"

// globales Handle auf das PiControl Geraet
int PiControlHandle_g = -1;

void PlcIoRevPi::begin() {
    /* open handle if needed */
    if (PiControlHandle_g < 0) {
        PiControlHandle_g = open("/dev/piControl0", O_RDWR);
        if (PiControlHandle_g < 0) std::cerr << "Error: open(/dev/piControl0): " << strerror(errno) << std::endl;
    }
}

void PlcIoRevPi::read() {
    // hier kann der Bereich festgelegt werden, welcher von den IO-Karten gelesen werden soll
    // ggf. auch in mehreren Abschnitten
    read(0, PLC_I_SIZE);
}

void PlcIoRevPi::write() {
    // hier kann der Bereich festgelegt werden, welcher zu den IO-Karten geschrieben werden soll
    // ggf. auch in mehreren Abschnitten
    write(3, 1); // LED
    write(81, 2); // DO
}

void PlcIoRevPi::end() {
    /* open handle if needed */
    if (PiControlHandle_g > 0) {
        close(PiControlHandle_g);
        PiControlHandle_g = -1;
    }
}

void PlcIoRevPi::read(uint32_t offset, uint32_t length) {
    if (length <= 0 || length + offset > PLC_I_SIZE) {
        // kein oder zu kleines Prozessabbild vorhanden
        std::cerr << "Error: Input Size too small PlcIo::read(" << offset << "," << length << ")" << std::endl;
        return;
    }
    bool error = false;
    if (PiControlHandle_g < 0) {
        error = true;
    } else {
        /* seek */
        if (lseek(PiControlHandle_g, offset, SEEK_SET) < 0) {
            error = true;
            std::cerr << "Error: PlcIo::read lseek(" << offset << ")" << strerror(errno) << std::endl;
        } else {
            /* read */
            int BytesRead = ::read(PiControlHandle_g, Inputs + offset, length);
            error = BytesRead < 0;
            if (error)
                std::cerr << "Error: PlcIo::read(" << offset << "," << length << ")" << strerror(errno) << std::endl;
        }
    }

    if (error) {
        // Prozessabbild bei Fehler auf 0 setzen
        for (int i = offset; i < offset + length; i++) Inputs[i] = 0;
    }
}

void PlcIoRevPi::write(uint32_t offset, uint32_t length) {
    if (length <= 0 || length + offset > PLC_Q_SIZE) {
        // kein oder zu kleines Prozessabbild vorhanden
        std::cerr << "Error: Output Size too small PlcIo::write(" << offset << "," << length << ")" << std::endl;
        return;
    }
    bool error = false;
    if (PiControlHandle_g < 0) {
        error = true;
    } else {
        /* seek */
        if (lseek(PiControlHandle_g, offset, SEEK_SET) < 0) {
            error = true;
            std::cerr << "Error: PlcIo::write lseek(" << offset << ")" << strerror(errno) << std::endl;
        } else {
            /* write */
            int BytesWritten = ::write(PiControlHandle_g, Outputs + offset, length);
            error = BytesWritten < 0;
            if (error)
                std::cerr << "Error: PlcIo::write(" << offset << "," << length << ")" << strerror(errno) << std::endl;
        }
    }
}
