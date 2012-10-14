
#include "cncconv.h"

/** CLASS CONFIG_LINE **/
ConfigLine::ConfigLine(string conf_line, int _line_number_in_config_file) {
    text = getTextFromString(conf_line);
    command = getCommandFromString(conf_line);
    argument = getArgumentFromString(conf_line);

    line_number_in_config_file = _line_number_in_config_file;
}

ConfigLine::ConfigLine() {
    text = "";
    command = "";
    argument = "";

    line_number_in_config_file = -1;
}

ConfigLine::~ConfigLine() {}

string ConfigLine::getText() { return text; }
string ConfigLine::getCommand() { return command; }
string ConfigLine::getArgument() { return argument; }

int ConfigLine::getLineNumberInConfigFile() { return line_number_in_config_file; }

string ConfigLine::getTextFromString(string textline) {
    size_t pos = textline.find('[');
    return textline.substr(0, pos);
}

string ConfigLine::getCommandFromString(string textline) {
    size_t pos_first = textline.find('[');
    size_t pos_last = textline.find(']');
    return textline.substr(pos_first, pos_last - pos_first + 1);
}

string ConfigLine::getArgumentFromString(string textline) {
    size_t pos = textline.find(']');
    return textline.substr(pos + 1, std::string::npos);
}

/** CLASS CONFIG **/
Config::Config(string _CONFIG_FILE_PATH) {

    CONFIG_FILE_PATH = _CONFIG_FILE_PATH;

    file_line = 0;
    num_of_warnings = 0;

    init_file_elements();

    init_fcoms();
    init_xcoms();
    init_ycoms();
    init_zcoms();

    checkFileForNoneCommands();
    checkCommandListForErrors(fcoms, f_elements);
    checkCommandListForErrors(xcoms, x_elements);
    checkCommandListForErrors(ycoms, y_elements);
    checkCommandListForErrors(zcoms, z_elements);
}

Config::~Config() {}

bool Config::getNextFCommand(string& text, string& command, string& argument, int& line_in_cfg) {
    if (f_line < f_elements) {
        f_line ++;
        text = fcoms[f_line-1].getText();
        command = fcoms[f_line-1].getCommand();
        argument = fcoms[f_line-1].getArgument();
        line_in_cfg = fcoms[f_line-1].getLineNumberInConfigFile();
        return true;
    }
    else return false;

}

bool Config::getNextXCommand(string& text, string& command, string& argument, int& line_in_cfg) {
    if (x_line < x_elements) {
        x_line ++;
        text = xcoms[x_line-1].getText();
        command = xcoms[x_line-1].getCommand();
        argument = xcoms[x_line-1].getArgument();
        line_in_cfg = xcoms[x_line-1].getLineNumberInConfigFile();
        return true;
    }
    else return false;
}

bool Config::getNextYCommand(string& text, string& command, string& argument, int& line_in_cfg) {
    if (y_line < y_elements) {
        y_line ++;
        text = ycoms[y_line-1].getText();
        command = ycoms[y_line-1].getCommand();
        argument = ycoms[y_line-1].getArgument();
        line_in_cfg = ycoms[y_line-1].getLineNumberInConfigFile();
        return true;
    }
    else return false;
}

bool Config::getNextZCommand(string& text, string& command, string& argument, int& line_in_cfg) {
    if (z_line < z_elements) {
        z_line ++;
        text = zcoms[z_line-1].getText();
        command = zcoms[z_line-1].getCommand();
        argument = zcoms[z_line-1].getArgument();
        line_in_cfg = zcoms[z_line-1].getLineNumberInConfigFile();
        return true;
    }
    else return false;
}

bool Config::getFCommand(int number, string& text, string& command, string& argument, int& line_in_cfg) {
    if (number < int(f_elements)) {
        text = fcoms[number].getText();
        command = fcoms[number].getCommand();
        argument = fcoms[number].getArgument();
        line_in_cfg = fcoms[number].getLineNumberInConfigFile();
        return true;
    }
    else return false;
}

bool Config::getXCommand(int number, string& text, string& command, string& argument, int& line_in_cfg) {
    if (number < int(x_elements)) {
        text = xcoms[number].getText();
        command = xcoms[number].getCommand();
        argument = xcoms[number].getArgument();
        line_in_cfg = xcoms[number].getLineNumberInConfigFile();
        return true;
    }
    else return false;
}

bool Config::getYCommand(int number, string& text, string& command, string& argument, int& line_in_cfg) {
    if (number < int(y_elements)) {
        text = ycoms[number].getText();
        command = ycoms[number].getCommand();
        argument = ycoms[number].getArgument();
        line_in_cfg = ycoms[number].getLineNumberInConfigFile();
        return true;
    }
    else return false;
}

bool Config::getZCommand(int number, string& text, string& command, string& argument, int& line_in_cfg) {
    if (number < int(z_elements)) {
        text = zcoms[number].getText();
        command = zcoms[number].getCommand();
        argument = zcoms[number].getArgument();
        line_in_cfg = zcoms[number].getLineNumberInConfigFile();
        return true;
    }
    else return false;
}

int Config::getFElements() { return f_elements; }
int Config::getXElements() { return x_elements; }
int Config::getYElements() { return y_elements; }
int Config::getZElements() { return z_elements; }

int Config::getNumOfWarnings() {
    return num_of_warnings;
}

void Config::init_file_elements() {
    ifstream ifs(CONFIG_FILE_PATH.c_str(), ios_base::in);
    if (!ifs.is_open()) {
        cout << "error: could not open config-file: " << CONFIG_FILE_PATH << endl;
        return;
    }
    ifs.seekg(0);
    string tmp;
    for(;!ifs.eof(); file_elements ++) getline(ifs, tmp);
    ifs.close();
}

void Config::init_fcoms() {
    f_line = 0;

    ifstream ifs(CONFIG_FILE_PATH.c_str(), ios_base::in);
    if (!ifs.is_open()) {
        cout << "error: could not open config-file: " << CONFIG_FILE_PATH << endl;
        return;
    }
    ifs.seekg(0);

    string tmpline;
    int fcom_num = 0;

    while(!ifs.eof()) {
        getline(ifs, tmpline);
        if (tmpline[0] != '#' && tmpline.find("[?") != string::npos) {
            fcom_num ++;
        }
    }

    f_elements = fcom_num;
    fcoms = new ConfigLine[fcom_num];

    ifs.close();
    ifs.open(CONFIG_FILE_PATH.c_str(), ios_base::in);
    ifs.seekg(0);
    int j = 1;
    for(int i = 0; !ifs.eof();) {
        getline(ifs, tmpline);
        if (tmpline[0] != '#' && tmpline.find("[?") != string::npos) {
            fcoms[i] = ConfigLine(tmpline, j);
            i++;
        }
        j++;
    }
    ifs.close();
}

void Config::init_xcoms() {
    x_line = 0;

    ifstream ifs(CONFIG_FILE_PATH.c_str(), ios_base::in);
    if (!ifs.is_open()) {
        cout << "error: could not open config-file: " << CONFIG_FILE_PATH << endl;
        return;
    }
    ifs.seekg(0);

    string tmpline;
    int xcom_num = 0;

    while(!ifs.eof()) {
        getline(ifs, tmpline);
        if (tmpline[0] != '#' && tmpline.find("[&") != string::npos) {
            xcom_num ++;
        }
    }

    x_elements = xcom_num;
    xcoms = new ConfigLine[xcom_num];

    ifs.close();
    ifs.open(CONFIG_FILE_PATH.c_str(), ios_base::in);
    ifs.seekg(0);
    int j = 1;
    for(int i = 0; !ifs.eof();) {
        getline(ifs, tmpline);
        if (tmpline[0] != '#' && tmpline.find("[&") != string::npos) {
            xcoms[i] = ConfigLine(tmpline, j);
            i++;
        }
        j++;
    }
    ifs.close();
}

void Config::init_ycoms() {
    y_line = 0;

    ifstream ifs(CONFIG_FILE_PATH.c_str(), ios_base::in);
    if (!ifs.is_open()) {
        cout << "error: could not open config-file: " << CONFIG_FILE_PATH << endl;
        return;
    }
    ifs.seekg(0);

    string tmpline;
    int ycom_num = 0;

    while(!ifs.eof()) {
        getline(ifs, tmpline);
        if (tmpline[0] != '#' && tmpline.find("[%") != string::npos) {
            ycom_num ++;
        }
    }

    y_elements =  ycom_num;
    ycoms = new ConfigLine[ycom_num];

    ifs.close();
    ifs.open(CONFIG_FILE_PATH.c_str(), ios_base::in);
    ifs.seekg(0);
    int j = 1;
    for(int i = 0; !ifs.eof();) {
        getline(ifs, tmpline);
        if (tmpline[0] != '#' && tmpline.find("[%") != string::npos) {
            ycoms[i] = ConfigLine(tmpline, j);
            i++;
        }
        j++;
    }
    ifs.close();
}

void Config::init_zcoms() {
    z_line = 0;

    ifstream ifs(CONFIG_FILE_PATH.c_str(), ios_base::in);
    if (!ifs.is_open()) {
        cout << "error: could not open config-file: " << CONFIG_FILE_PATH << endl;
        return;
    }
    ifs.seekg(0);

    string tmpline;
    int zcom_num = 0;

    while(!ifs.eof()) {
        getline(ifs, tmpline);
        if (tmpline[0] != '#' && tmpline.find("[*") != string::npos) {
            zcom_num ++;
        }
    }

    z_elements = zcom_num;
    zcoms = new ConfigLine[zcom_num];

    ifs.close();
    ifs.open(CONFIG_FILE_PATH.c_str(), ios_base::in);
    ifs.seekg(0);
    int j = 1;
    for(int i = 0; !ifs.eof();) {
        getline(ifs, tmpline);
        if (tmpline[0] != '#' && tmpline.find("[*") != string::npos) {
            zcoms[i] = ConfigLine(tmpline, j);
            i++;
        }
        j++;
    }
    ifs.close();
}

bool Config::checkFileForNoneCommands() {
    ifstream ifs(CONFIG_FILE_PATH.c_str(), ios_base::in);
    if (!ifs.is_open()) {
        cout << "error: could not open config-file: " << CONFIG_FILE_PATH << endl;
        return false;
    }

    string buffer;
    bool retval = false;
    int line = 0;

    while(!ifs.eof()) {
        line ++;
        getline(ifs, buffer);
        if (!buffer.empty() &&
            buffer.find("[?") == string::npos &&
            buffer.find("[&") == string::npos &&
            buffer.find("[%") == string::npos &&
            buffer.find("[*") == string::npos) {
                cout << "WARNING: line " << line << ": does not contain any regular command: \"" << buffer << "\"" << endl;
                num_of_warnings++;
                retval = true;
        }
    }
    ifs.close();
    return retval;
}

bool Config::checkCommandListForErrors(ConfigLine* coms, size_t elements) {
    bool retval = false;
    for (int i = 0; i < int(elements); i++) {
        if (coms[i].getCommand().find("replace]") == string::npos &&
            coms[i].getCommand().find("delete]") == string::npos &&
            coms[i].getCommand().find("attach]") == string::npos) {
                cout << "WARNING: line " << coms[i].getLineNumberInConfigFile() << ": line contains an unknown command: \"" << coms[i].getCommand() << "\"" << endl;
                num_of_warnings++;
                retval = true;
        }
    }
    return retval;
}

/** CLASS IO_LINE **/
IOLine::IOLine(string line, int linenumber) {
    ioline = line;
    linenum = linenumber;
}

IOLine::IOLine() {
    ioline = "";
    linenum = -1;
}

IOLine::~IOLine() {}

string IOLine::getLine() { return ioline; }
int IOLine::getLineNumber() { return linenum; }



/** CLASS INPUT **/

Input::Input(string _INPUT_FILE_PATH) {
    INPUT_FILE_PATH = _INPUT_FILE_PATH;
    init_inlines();
}

Input::~Input() {}

bool Input::getNextLine(string& iline, int& line_number) {
    if (line < elements) {
        iline = inlines[line].getLine();
        line_number = inlines[line].getLineNumber();
        line ++;
        return true;
    }
    else return false;
}

bool Input::getLine(int number, string& iline, int& line_number) {
    if (number < int(elements)) {
        iline = inlines[number].getLine();
        line_number = inlines[number].getLineNumber();
        return true;
    }
    else return false;
}

int Input::getElements() { return elements; }

void Input::init_inlines() {

    ifstream ifs(INPUT_FILE_PATH.c_str(), ios_base::in);
    if (!ifs.is_open()) {
        cout << "error: could not open inputfile: " << INPUT_FILE_PATH << endl;
        return;
    }

    ifs.seekg(0);
    string tmpstr;
    int i = 0;

    while(!ifs.eof()) {
        getline(ifs, tmpstr);
        if (!tmpstr.empty()) i++;
    }

    elements = i;
    inlines = new IOLine[i];

    ifs.close();
    ifs.open(INPUT_FILE_PATH.c_str(), ios_base::in);
    ifs.seekg(0);
    i = 0;

    int j = 1;
    while(!ifs.eof()) {
        getline(ifs, tmpstr);
        if (!tmpstr.empty()) {
            inlines[i] = IOLine(tmpstr, j);
            i++;
        }
        j++;
        //cout << "in second while()" << endl;
    }

    ifs.close();
}


/** CLASS OUTPUT **/

Output::Output(string _OUTPUT_FILE_PATH) {
    OUTPUT_FILE_PATH = _OUTPUT_FILE_PATH;
    line = 0;
}

Output::~Output() {}

void Output::addNewOutline(string outline) {
    vector.push_back(outline);
}

string Output::getNextLine() {
    if (line < int(vector.size())) {
        return vector[line];
    }
    else return "";
}

string Output::getLine(int number) {
    if (number < int(vector.size())) {
        return vector[number];
    }
    else return "";
}

void Output::createOutputFile() {
    ofstream ofs(OUTPUT_FILE_PATH.c_str(), ios_base::out | ios_base::trunc);
    if (!ofs.is_open()) {
        cout << "error: cannot create outputfile: " << OUTPUT_FILE_PATH << endl;
        return;
    }

    for (int i = 0; i < int(vector.size()); i++) {
        ofs << vector[i] << endl;
    }

    ofs.close();
}

/** CLASS CONVERTER **/
Converter::Converter(string _CONFIG_FILE_PATH, string _INPUT_FILE_PATH, string _OUTPUT_FILE_PATH) {

    cout << "\n#################################################\n" << endl;

    CONFIG_FILE_PATH = _CONFIG_FILE_PATH;
    INPUT_FILE_PATH = _INPUT_FILE_PATH;
    OUTPUT_FILE_PATH = _OUTPUT_FILE_PATH;

    cfg = new Config(CONFIG_FILE_PATH);
    ip = new Input(INPUT_FILE_PATH);
    op = new Output(OUTPUT_FILE_PATH);

    num_of_warnings = cfg->getNumOfWarnings();
    checkForInputNotInConfig();

    processFComs();
    op->createOutputFile();

    cout << "\n#################################################" << endl;
    cout << num_of_warnings << " warning(s)" << endl;
}

Converter::~Converter() {
    delete cfg;
    delete ip;
    delete op;
}

bool Converter::checkLineNotInConfig(string line) {
    /// check for fcom
    for (int i = 0; i < cfg->getFElements(); i++) {
        string text, command, argument;
        int cfg_line;

        cfg->getFCommand(i, text, command, argument, cfg_line);
        if (line == text) {
            /// fcom found
            return false;
        }
    }

    /// check for xcom
    for (int i = 0; i < cfg->getXElements(); i++) {
        string text, command, argument;
        int cfg_line;

        cfg->getXCommand(i, text, command, argument, cfg_line);
        if (line.find(text) != string::npos) {
            /// xcom found
            return false;
        }
    }

    /// check for ycom
    for (int i = 0; i < cfg->getYElements(); i++) {
        string text, command, argument;
        int cfg_line;

        cfg->getYCommand(i, text, command, argument, cfg_line);
        if (line.find(text) != string::npos) {
            /// ycom found
            return false;
        }
    }

    /// check for zcom
    for (int i = 0; i < cfg->getZElements(); i ++) {
        string text, command, argument;
        int cfg_line;

        cfg->getZCommand(i, text, command, argument, cfg_line);
        if (line.find(text) != string::npos) {
            /// zcom found
            return false;
        }
    }

    return true;
}

bool Converter::checkForInputNotInConfig() {
    bool retval = false;
    for (int i = 0; i < ip->getElements(); i++) {
        string line;
        int line_number;
        ip->getLine(i, line, line_number);

        if (checkLineNotInConfig(line)) {
            cout << "WARNING: line " << line_number << ": not found in config-file: \"" << line << "\"" << endl;
            num_of_warnings ++;
            retval = true;
        }
    }
    return retval;
}

bool Converter::getFComToLine(string line, string& text, string& command, string& argument) {
    for (int i = 0; i < cfg->getFElements(); i++) {
        int line_number;

        cfg->getFCommand(i, text, command, argument, line_number);

        if (text == line) {
            /// fcom found
            return true;
        }
    }
    text = "";
    command = "";
    argument = "";
    return false;
}

bool Converter::getXComToLine(string line, string& text, string& command, string& argument) {
    for (int i = 0; i < cfg->getXElements(); i++) {
        int line_number;

        cfg->getXCommand(i, text, command, argument, line_number);

        if (line.find(text) != string::npos) {
            /// xcom found
            return true;
        }
    }
    text = "";
    command = "";
    argument = "";
    return false;
}

bool Converter::getYComToLine(string line, string& text, string& command, string& argument) {
    for (int i = 0; i < cfg->getYElements(); i++) {
        int line_number;

        cfg->getYCommand(i, text, command, argument, line_number);

        if (line.find(text) != string::npos) {
            /// xcom found
            return true;
        }
    }
    text = "";
    command = "";
    argument = "";
    return false;
}

bool Converter::getZComToLine(string line, string& text, string& command, string& argument) {
    for (int i = 0; i < cfg->getZElements(); i++) {
        int line_number;

        cfg->getZCommand(i, text, command, argument, line_number);

        if (line.find(text) != string::npos) {
            /// xcom found
            return true;
        }
    }
    text = "";
    command = "";
    argument = "";
    return false;
}

bool Converter::processFComs() {
    for (int i = 0; i < ip->getElements(); i++) {
        string iline, text, command, argument;
        int iline_num;

        ip->getLine(i, iline, iline_num);

        if (getFComToLine(iline, text, command, argument)) {
            /// found fcom to this line ...
            op->addNewOutline(argument);
        }
    }
    return true;
}

bool Converter::processXComs() {
    for (int i = 0; i < ip->getElements(); i++) {
        string iline, text, command, argument;
        int iline_num;

        ip->getLine(i, iline, iline_num);

        if (getXComToLine(iline, text, command, argument)) {
            /// found xcom to this line ...
            op->addNewOutline(argument);
        }
    }
    return true;
}

