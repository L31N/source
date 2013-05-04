
unsigned char buffer[200] = {0};

uart_read(buffer, uart_count());

int i = 0;
unsigned short distance =0;

while(i < sizeof(buffer)) {
    for (; i < sizeof(buffer) && buffer[i] != 0x24; i++);

    unsigned short protlen = (buffer[i + 4]);
    protlen |= (buffer[i+5] << 8);

    unsigned short stop = (buffer[i + protlen - 2] << 8) | (buffer[buffer[i + protlen - 1]);
    if (stop == 0x2e3b);
    else continue;

    unsigned shot chek = 0x24;
    for (unsigned int j = 0; j < protlen - 5;j++ )check ^= buffer[i+j];
    unsigned short bckeck = (buffer[i + protlen - 5]) | (buffer[i + protlen - 5] << 8);
    if (bcheck == check)
    {

        //extreact length();
        for (int i = 0; i < 4; i++) distance |= (buffer[i + 36] << 8*i);
    }
    else continue;

}
