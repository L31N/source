
#ifndef _CNC_CONV_H_
#define _CNC_CONV_H_

#include <string>
#include <iostream>
#include <fstream>

#include <vector>

using namespace std;

/** ++++++++++++++++++++++++++++++++++++++++++++++++++ **/

class ConfigLine {
    public:
        ConfigLine(string conf_line, int _line_number_in_config_file);
        ConfigLine();
        ~ConfigLine();

    private:
        string text;
        string command;
        string argument;

        int line_number_in_config_file;

        // -------------------- //

        string getCommandFromString(string textline);
        string getTextFromString(string textline);
        string getArgumentFromString(string textlien);

    public:

        string getText(void);
        string getCommand(void);
        string getArgument(void);

        int getLineNumberInConfigFile(void);
};

class Config {
    public:
        Config(string _CONFIG_FILE_PATH);
        ~Config();

        bool getNextFCommand(string& text, string& command, string& argument, int& line_in_cfg);
        bool getNextXCommand(string& text, string& command, string& argument, int& line_in_cfg);
        bool getNextYCommand(string& text, string& command, string& argument, int& line_in_cfg);
        bool getNextZCommand(string& text, string& command, string& argument, int& line_in_cfg);

        bool getFCommand(int number, string& text, string& command, string& argument, int& line_in_cfg);
        bool getXCommand(int number, string& text, string& command, string& argument, int& line_in_cfg);
        bool getYCommand(int number, string& text, string& command, string& argument, int& line_in_cfg);
        bool getZCommand(int number, string& text, string& command, string& argument, int& line_in_cfg);

        int getFElements(void);
        int getXElements(void);
        int getYElements(void);
        int getZElements(void);

        int getNumOfWarnings(void);

    private:
        string CONFIG_FILE_PATH;

        ConfigLine* fcoms;      /// [?]
        ConfigLine* xcoms;      /// [&]
        ConfigLine* ycoms;      /// [%]
        ConfigLine* zcoms;      /// [*]

        size_t file_line;
        size_t file_elements;

        size_t f_line;
        size_t f_elements;

        size_t x_line;
        size_t x_elements;

        size_t y_line;
        size_t y_elements;

        size_t z_line;
        size_t z_elements;

        size_t num_of_warnings;

        // ---------------- //

        void init_file_elements(void);

        void init_fcoms(void);
        void init_xcoms(void);
        void init_ycoms(void);
        void init_zcoms(void);

        bool checkFileForNoneCommands();
        bool checkCommandListForErrors(ConfigLine* coms, size_t elements);
};

/** ++++++++++++++++++++++++++++++++++++++++++++++++++ **/
class IOLine {
    public:
        IOLine(string line, int linenumber);
        IOLine();
        ~IOLine();

    private:
        string ioline;
        int linenum;

    public:
        string getLine();
        string* getLineReference();
        int getLineNumber();
};

/** ++++++++++++++++++++++++++++++++++++++++++++++++++ **/
class Input {
    public:
        Input(string _INPUT_FILE_PATH);
        ~Input();

        bool getNextLine(string& iline, int& line_number);
        bool getLine(int number, string& iline, int& line_number);

        int getElements();

    private:
        string INPUT_FILE_PATH;

        IOLine* inlines;

        size_t elements;

        size_t line;

        // ------------ //

        void init_inlines(void);
};

/** ++++++++++++++++++++++++++++++++++++++++++++++++++ **/

class Output {
    public:
        Output(string _OUTPUT_FILE_PATH);
        ~Output();

        void addNewOutline(IOLine outline);

        bool getNextLine(string& oline, int& line_number);
        bool getLine(int number, string& oline, int& line_number);

        void attachToOutline(int number, string attachment);

        void createOutputFile(void);

        int getElements(void);

    private:
        string OUTPUT_FILE_PATH;

        std::vector<IOLine> vector;

        int line;

};

/** ++++++++++++++++++++++++++++++++++++++++++++++++++ **/

class Converter {
    public:
        Converter(string _CONFIG_FILE_PATH, string _INPUT_FILE_PATH, string _OUTPUT_FILE_PATH);
        ~Converter();

    private:
        string CONFIG_FILE_PATH;
        string INPUT_FILE_PATH;
        string OUTPUT_FILE_PATH;

        Config* cfg;
        Input* ip;
        Output* op;

        int z_line_num;

        size_t num_of_warnings;

        // ---------------------- //

        bool processFComs();
        bool processXComs();
        bool processYComs();
        bool processZComs();

        bool checkLineNotInConfig(string line);
        bool checkForInputNotInConfig();

        bool getFComToLine(string line, string& text, string& command, string& argument);
        bool getXComToLine(string line, string& text, string& command, string& argument);
        bool getYComToLine(string line, string& text, string& command, string& argument);
        bool getZComToLine(string line, string& text, string& command, string& argument);

        //void splitZArgument(string zarg, string*, int& elements);
};

#endif // _CNC_CONV_H_
