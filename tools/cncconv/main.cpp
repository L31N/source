
#include "cncconv.h"

int main (int argc, char** argv) {

    if (argc != 4) {
        cout << "Usage: CONFIG_FILE_PATH INPUT_FILE_PATH OUTPUT_FILE_PATH" << endl;
        return -1;
    }
    else {
        Converter conv(argv[1], argv[2], argv[3]);
        return 0;
    }
}
