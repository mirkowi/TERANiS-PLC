//
// Created by mirko on 19.09.2019.
//

#include "PlcIo.h"

#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>

#include "teranis.h"

// globales Handle auf das PiControl Geraet
int PiControlHandle_g = -1;

void TPlcIo::begin() {
    /* open handle if needed */
    if (PiControlHandle_g < 0) {
        PiControlHandle_g = open("/dev/piControl0", O_RDWR);
    }
}

void TPlcIo::read() {
    // hier kann der Bereich festgelegt werden, welcher von den IO-Karten gelesen werden soll
    // ggf. auch in mehreren Abschnitten
    read(0,PLC_I_SIZE);
}

void TPlcIo::write() {
    // hier kann der Bereich festgelegt werden, welcher zu den IO-Karten geschrieben werden soll
    // ggf. auch in mehreren Abschnitten
    write(3,1); // LED
    write(81,2); // DO
}

void TPlcIo::end() {
    /* open handle if needed */
    if (PiControlHandle_g > 0) {
        close(PiControlHandle_g);
        PiControlHandle_g = -1;
    }
}

void TPlcIo::read(uint32_t offset, uint32_t length) {
    if (length <= 0 || length + offset > PLC_I_SIZE) return; // kein oder zu kleines Prozessabbild vorhanden
    bool error = false;
    if (PiControlHandle_g < 0) {
        error = true;
    } else {
        /* seek */
        if (lseek(PiControlHandle_g, offset, SEEK_SET) < 0) {
            error = true;
        } else {
            /* read */
            int BytesRead = ::read(PiControlHandle_g, Inputs+offset, length);
            error = BytesRead < 0;
        }
    }

    if (error) {
        // Prozessabbild bei Fehler auf 0 setzen
        for (int i = offset; i < offset+length; i++) Inputs[i] = 0;
    }
}

void TPlcIo::write(uint32_t offset, uint32_t length) {
    if (length <= 0 || length + offset > PLC_Q_SIZE) return; // kein oder zu kleines Prozessabbild vorhanden
    bool error = false;
    if (PiControlHandle_g < 0) {
        error = true;
    } else {
        /* seek */
        if (lseek(PiControlHandle_g, offset, SEEK_SET) < 0) {
            error = true;
        } else {
            /* write */
            int BytesRead = ::write(PiControlHandle_g, Outputs+offset, length);
            error = BytesRead < 0;
        }
    }
}
