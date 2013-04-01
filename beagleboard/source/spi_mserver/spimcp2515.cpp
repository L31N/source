
// cpimcp2515.cpp

#include <string.h>
#include <errno.h>

#include "spimcp2515.h"

/*uint8_t wordlength = 8;
uint32_t maxspeedhz = 100000;
uint16_t delay = 0;
uint8_t mode = 0;*/


Mcp2515::Mcp2515(const std::string spidev) {

    wordlength = 8;
    maxspeedhz = 100000;
    delay = 0;
    mode = 0;

    fd = open(spidev.c_str(), O_RDWR);
    if (fd <= 0) {
        std::cerr << "could not open device: " << spidev << std::endl;
        return;
    }

    retval = ioctl(fd, SPI_IOC_WR_MODE, &mode);
    if (retval == -1) {
        std::cout << "cannot set spi mode wr" << std::endl;
        std::cerr << strerror(errno) << std::endl;
    }

    retval = ioctl(fd, SPI_IOC_RD_MODE, &mode);
    if (retval == -1) {
        std::cout << "cannot set spi mode rd" << std::endl;
        std::cerr << strerror(errno) << std::endl;
    }

    retval = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &wordlength);
    if (retval == -1) {
        std::cout << "cannot set spi mode length wr" << std::endl;
        std::cerr << strerror(errno) << std::endl;
    }

    retval = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &wordlength);
    if (retval == -1) {
        std::cout << "cannot set spi mode length rd" << std::endl;
        std::cerr << strerror(errno) << std::endl;
    }

    retval = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &maxspeedhz);
    if (retval == -1) {
        std::cout << "cannot set spi mode speed wr" << std::endl;
        std::cerr << strerror(errno) << std::endl;
    }

    retval = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &maxspeedhz);
    if (retval == -1) {
        std::cout << "cannot set spi mode speed rd" << std::endl;
        std::cerr << strerror(errno) << std::endl;
    }
}

Mcp2515::~Mcp2515() {
    close(fd);
}

bool Mcp2515::mcp_write(unsigned char* buf, size_t length) {
    uint8_t tx[length];
    for (unsigned int i = 0; i < length; i++) tx[i] = buf[i];

    uint8_t rx[length];
    for (unsigned int i = 0; i < length; i++) rx[i] = 0;

    struct spi_ioc_transfer tr;

    tr.tx_buf = (unsigned long)tx;
    tr.rx_buf = (unsigned long)rx;

    tr.len = length;
    tr.delay_usecs = delay;
    tr.speed_hz = maxspeedhz;
    tr.bits_per_word = wordlength;

    int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1) {
        std::cerr << "cannot send spi message" << std::endl;
        return false;
    }
    else return true;
}

bool Mcp2515::mcp_read(unsigned char* buf, size_t length) {
    uint8_t tx[3];
    for (unsigned int i = 0; i < 3; i++) tx[i] = buf[i];

    uint8_t rx[length];
    for (unsigned int i = 0; i < length; i++) rx[i] = 0;

    struct spi_ioc_transfer tr;

    tr.tx_buf = (unsigned long)tx;
    tr.rx_buf = (unsigned long)rx;

    tr.len = length;
    tr.delay_usecs = delay;
    tr.speed_hz = maxspeedhz;
    tr.bits_per_word = wordlength;

    int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    for (unsigned int i = 0; i < length; i++) buf[i] = rx[i];
    if (ret < 1) {
        std::cerr << "cannot send spi message" << std::endl;
        return false;
    }
    else return true;
}

bool Mcp2515::mcp_reset() {
    unsigned char reset = SPI_RESET;
    return this->mcp_write(&reset, 1);
}

bool Mcp2515::mcp_write_register(unsigned char address, unsigned char data) {
    unsigned char buf [3] = { SPI_WRITE, address, data };
    return this->mcp_write(buf, 3);
}

bool Mcp2515::mcp_read_register(unsigned char address, unsigned char& data) {
    unsigned char buf [3] = { SPI_READ, address, 0xff };
    int retval = this->mcp_read(buf, 3);
    data = buf[2];
    return retval;
}

bool Mcp2515::mcp_bit_modify(unsigned char address, unsigned char mask, unsigned char data) {
    unsigned char buf[4] = { SPI_BIT_MODIFY, address, mask, data };
    return this->mcp_write(buf, 4);
}

unsigned char Mcp2515::mcp_read_status(unsigned char type) {
    unsigned char buf [2] = { type, 0xff };
    this->mcp_read(buf, 2);
    return buf[1];
}

//bool Mcp2515::mcp_init(can_bitrate_t bitrate) {
//    if (bitrate >= 8) {
//        std::cerr << "invalid bitrate ..." << std::endl;
//        return false;
//    }
//
//    this->mcp_reset();
//    usleep(10000);
//
//    unsigned char bcnf[3];
//    for (unsigned int i = 0; i < 3; i++) bcnf[i] = _mcp2515_cnf [bitrate][i];
//    this->mcp_write_register(CNF3, bcnf[0]);
//    this->mcp_write_register(CNF2, bcnf[1]);
//    this->mcp_write_register(CNF1, bcnf[2]);
//
//    this->mcp_write_register(CANINTE, (1<<RX1IE)|(1<<RX0IE));
//
//    // Buffer 0 : Empfangen aller Nachrichten
//    this->mcp_write_register( RXB0CTRL, (1<<RXM1)|(1<<RXM0) );
//
//    // Buffer 1 : Empfangen aller Nachrichten
//    this->mcp_write_register( RXB1CTRL, (1<<RXM1)|(1<<RXM0) );
//
//    // Alle Bits der Empfangsmaske loeschen,
//    // damit werden alle Nachrichten empfangen
//    this->mcp_write_register( RXM0SIDH, 0 );
//    this->mcp_write_register( RXM0SIDL, 0 );
//    this->mcp_write_register( RXM0EID8, 0 );
//    this->mcp_write_register( RXM0EID0, 0 );
//
//    this->mcp_write_register( RXM1SIDH, 0 );
//    this->mcp_write_register( RXM1SIDL, 0 );
//    this->mcp_write_register( RXM1EID8, 0 );
//    this->mcp_write_register( RXM1EID0, 0 );
//
//    this->mcp_write_register(BFPCTRL, 0);
//
//    this->mcp_write_register(TXRTSCTRL, 0);
//
//    this->mcp_bit_modify(CANCTRL, 0xe0, 0);
//
//    return true;
//}

bool Mcp2515::mcp_init(can_bitrate_t bitrate) {
    if (bitrate >= 8) {
        std::cerr << "invalid bitrate ..." << std::endl;
        return false;
    }

    // reset chip
    this->mcp_reset();
    usleep(10000);

    // set baudrate (CNF1 .. 3 (Bittiming) / activate interrupts
    unsigned char cnf [6] = { SPI_WRITE, CNF3 };
    for (unsigned int i = 2; i < 3+2; i++) {
        cnf[i] = _mcp2515_cnf [bitrate][i-2];
    }
    cnf [5] = (1<<RX1IE)|(1<<RX0IE);
    this->mcp_write(cnf, 6);

//    /** set the filters to accept all messages **/
//    // Buffer 0 : Empfangen aller Nachrichten
//    this->mcp_write_register( RXB0CTRL, (1<<RXM1)|(1<<RXM0) );
//
//    // Buffer 1 : Empfangen aller Nachrichten
//    this->mcp_write_register( RXB1CTRL, (1<<RXM1)|(1<<RXM0) );
//
//    // Alle Bits der Empfangsmaske loeschen,
//    // damit werden alle Nachrichten empfangen
//    this->mcp_write_register( RXM0SIDH, 0 );
//    this->mcp_write_register( RXM0SIDL, 0 );
//    this->mcp_write_register( RXM0EID8, 0 );
//    this->mcp_write_register( RXM0EID0, 0 );
//
//    this->mcp_write_register( RXM1SIDH, 0 );
//    this->mcp_write_register( RXM1SIDL, 0 );
//    this->mcp_write_register( RXM1EID8, 0 );
//    this->mcp_write_register( RXM1EID0, 0 );

    // TXnRTS bits as interrupts
    this->mcp_write_register(TXRTSCTRL, 0);

    // activate pin-functions for RX0BF and RX1BF
    this->mcp_write_register(BFPCTRL, (1<<B0BFE)|(1<<B1BFE)|(1<<B0BFM)|(1<<B1BFM));

    // check the chips respondance
    bool error = false;
    unsigned char tmpbuf = 0;
    this->mcp_read_register(CNF2, tmpbuf);
    if (tmpbuf != _mcp2515_cnf[bitrate][1]) {
        error = true;
        std::cerr << "could not initiate mcp2515" << std::endl;
    }

    // put device back into normal mode
    this->mcp_write_register(CANCTRL, 0);

    if (error) return false;
    else {
        unsigned char tmp = 0;
        do {
            this->mcp_read_register(CANSTAT, tmp);
        } while((tmp & 0xe0) != 0);
    }

    return true;
}

unsigned char Mcp2515::mcp_read_rx_status() {
    return this->mcp_read_status(SPI_RX_STATUS);
}

bool Mcp2515::mcp_write_can(can_t* message) {

     /*uint8_t length = message->length;

    // ID einstellen
    this->mcp_write_register(TXB0SIDH, (uint8_t) (message->id>>3));
    this->mcp_write_register(TXB0SIDL, (uint8_t) (message->id<<5));

    // Ist die Nachricht ein "Remote Transmit Request"
    if (message->rtr)
    {
        // Eine RTR Nachricht hat zwar eine Laenge,
        // aber keine Daten

        // Nachrichten Laenge + RTR einstellen
        this->mcp_write_register(TXB0DLC, (1<<RTR) | length);
    }
    else
    {
        // Nachrichten Laenge einstellen
        this->mcp_write_register(TXB0DLC, length);

        // Daten
        for (uint8_t i = 0; i < length; i++) {
            this->mcp_write_register(TXB0D0 + i, message->data[i]);
        }
    }

    // CAN Nachricht verschicken
    unsigned char buf = (SPI_RTS | 0x01);
    this->mcp_write(&buf, 1);

    return true;*/

    //uint8_t status, address;

    // Status des MCP2515 auslesen
    unsigned char status, address;
    this->mcp_read_register(SPI_READ_STATUS, status);

    // Statusbyte:
    //
    // Bit  Funktion
    //  2   TXB0CNTRL.TXREQ
    //  4   TXB1CNTRL.TXREQ
    //  6   TXB2CNTRL.TXREQ
    //

    if (!(status & (1 << 2))) {
        address = 0x00;
    }
    else if (!(status & (1 << 4))) {
        address = 0x02;
    }
    else if (!(status & (1 << 6))) {
        address = 0x04;
    }
    else {
        // Alle Puffer sind belegt,
        // Nachricht kann nicht verschickt werden
        std::cerr << "cannot send message -> all sending buffers are full" << std::endl;
        return false;
    }

    unsigned char sbuf_len = 6;
    unsigned char sbuf[14] = { SPI_WRITE | address, message->id >> 3, message->id << 5, 0x00, 0x00 };

    unsigned char length = message->length;
    if (length > 8) length = 8;

    if (message->rtr) sbuf[5] = (1 << RTR) | length;
    else {
        sbuf[5] = length;

        for (unsigned int i = 0; i < length; i++) {
            sbuf[6+i] = message->data[i];
            sbuf_len ++;
        }
    }

    this->mcp_write(sbuf, sbuf_len);


    // send CAN message -----------------
    if (address == 0x00) {
        unsigned char buf = SPI_RTS | 0x01;
        this->mcp_write(&buf, 1);
    }
    else {
        unsigned char buf = SPI_RTS | address;
        this->mcp_write(&buf, 1);
    }

    return true;
}

unsigned char Mcp2515::mcp_read_can(Mcp2515::can_t* msg) {

    unsigned char status;
    this->mcp_read_register(CANINTF, status);

    unsigned char rbuf_len = 14;
    unsigned char rbuf [rbuf_len];

    std::cout << std::hex;
    std::cout << "status: " << int(status) << std::endl;

    if (status & (1 << 0)) rbuf[0] = SPI_READ_RX;                   // message in buffer 0
    else if (status & (1 << 1)) rbuf[0] = SPI_READ_RX | 0x04;      // message in buffer 1
    else {                                                          // fault, no new messages ...
        std::cerr << "error: no new can messages in buffer" << std::endl;
        return 0xff;
    }

    rbuf[1] = 0xff;     // dummy byte can id
    rbuf[2] = 0xff;     // dummy byte can id

    rbuf[3] = 0xff;
    rbuf[4] = 0xff;

    rbuf[5] = 0xff;     // dummy byte length

    for (unsigned int i = 0; i < 8; i++) rbuf[6+i] = 0xff;      // dummy bytes data

    this->mcp_read(rbuf, rbuf_len);

    //std::cerr << std::hex;
    /*for (unsigned int i = 0; i < 14; i++) {
        std::cout << "rbuf" << i << ": " << rbuf[i] << std::endl;
    }*/

    // extract data from buffer and fill data into the new can message

    msg->id = (uint16_t) rbuf[1] << 3;
    msg->id |= (uint16_t) rbuf[2] >> 5;

    uint8_t length = rbuf[5] & 0x0f;
    msg->length = length;

    for (unsigned char i = 0; i < length; i++) {
        msg->data[i] = rbuf[6+i];
    }


    if (status & (1 << 6)) msg->rtr = 1;
    else msg->rtr = 0;

    // reset interrupt flag
    if (status & (1 << 6)) this->mcp_bit_modify(CANINTF, (1 << RX0IF), 0);
    else this->mcp_bit_modify(CANINTF, (1 << RX1IF), 0);

    return (status & 0x07);

}

const uint8_t Mcp2515::_mcp2515_cnf[8][3] = {
    // 10 kbps
    {	0x04,
        0xb6,
        0xe7
    },
    // 20 kbps
    {	0x04,
        0xb6,
        0xd3
    },
    // 50 kbps
    {	0x04,
        0xb6,
        0xc7
    },
    // 100 kbps
    {	0x04,
        0xb6,
        0xc3
    },
    // 125 kbps
    {	(1<<PHSEG21),					// CNF3
        (1<<BTLMODE)|(1<<PHSEG11),		// CNF2
        (1<<BRP2)|(1<<BRP1)|(1<<BRP0)	// CNF1
    },
    // 250 kbps
    {	0x03,
        0xac,
        0x81
    },
    // 500 kbps
    {	0x03,
        0xac,
        0x80
    },
    // 1 Mbps
    {	(1<<PHSEG21),
        (1<<BTLMODE)|(1<<PHSEG11),
        0
    }
};

