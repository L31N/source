
#include "cncconvert.h"

std::string INPUT_FILE_PATH; // = "inputfile.txt";
std::string OUTPUT_FILE_PATH; // = "outputfile.txt";

int main (int argc, char* argv[]) {

    if (argc == 3) {
        INPUT_FILE_PATH = argv[1];
        OUTPUT_FILE_PATH = argv[2];
    }
    else {
        std::cout << "\nUsage: " << argv[0] << " [INPUT_FILE_PATH] [OUTPUT_FILE_PATH]\n" << std::endl;
        return 0;
    }

    /// check for config-file ...
    std::ifstream checkfs(CONFIG_FILE_PATH.c_str(), std::ios_base::in);
    if (!checkfs.is_open()) {
        checkfs.close();
        bool fConfigFileCorrect = false;

        while(!fConfigFileCorrect) {
            std::cout << "\nno cnc.conf found...\nplease insert an availiable filepath to the config-file: ";
            std::cin >> CONFIG_FILE_PATH;
            std::cout << "\n" << std::endl;
            std::ifstream checkfs(CONFIG_FILE_PATH.c_str(), std::ios_base::in);
            if (checkfs.is_open()) fConfigFileCorrect = true;
            checkfs.close();
        }
    }

    clearOutputFile();

    std::string text;
    std::string command;
    std::string argument;

    std::cout << "\n#############################\n" << std::endl;

    while(!input_eof()) {
        std::string inputLine = getLineFromInput(INPUT_FILE_PATH);
        if (lookupInConfig(inputLine, text, command, argument)) {
            if (command == "replace") {
                insert(argument);
                std::cout << "line " << input_line << ": replaced: \"" << text << "\" with \"" << argument << "\"" << std::endl;
            }
            else if (command == "delete") {
                std::cout << "line " << input_line << ": deleted: \"" << text << "\"" << std::endl;
            }
            else if (command == "attach") {
                std::string output_str (text);
                output_str+=argument;
                insert(output_str);
                std::cout << "line " << input_line << ": replaced: \"" << text << "\" with \"" << output_str << "\"" << std::endl;
            }
            else {
                if (inputLine != "") {
                    std::cout << "WARNING: line " << config_line << ": unknown command: [" << command << "]" << std::endl;
                    warnings_count ++;
                }
            }
        }
        else if (inputLine != "") {
            /// check line for x-command
            std::string xtext = "";
            std::string xcommand = "";
            std::string xargument = "";

            bool fXcom = false;
            config_line = 0;
            conf_eekg = 0;


            while(getNextXCommand(xtext, xcommand, xargument)) {

                /// check weather the text of the x-command is contained in the actual input-line ...
                if(inputLine.find(xtext, 0) != std::string::npos) {
                    fXcom = true;
                    /// --> found xtext in the input-line
                    if (xcommand == "&replace") {
                        insert(xargument);
                        std::cout << "line " << input_line << ": replaced \"" << inputLine << "\" with \"" << xargument << "\"" << std::endl;
                    }
                    else if (xcommand == "&delete") {
                        std::cout << "line " << input_line << ": deleted \"" << inputLine << "\"" << std::endl;
                    }
                    else if (xcommand == "&attach") {
                        std::string output_str = inputLine + xargument;
                        insert(output_str);
                        std::cout << "line " << input_line << ": replaced \"" << inputLine << "\" with \"" << output_str << "\"" << std::endl;
                    }

                    else if (xcommand == "%replace") {
                        std::string output_str = inputLine;
                        size_t tmppos = output_str.find(xtext, 0);
                        output_str.replace(tmppos, xtext.length(), xargument);
                        insert(output_str);

                        std::cout << "line " << input_line << ": replaced \"" << inputLine << "\" with \"" << output_str << "\"" << std::endl;
                    }
                    else if (xcommand == "%delete") {
                        std::string output_str = inputLine;
                        size_t tmppos = output_str.find(xtext, 0);
                        output_str.erase(tmppos, xtext.length());
                        insert(output_str);

                        std::cout << "line " << input_line << ": replaced \"" << inputLine << "\" with \"" << output_str << "\"" << std::endl;
                    }
                    else if (xcommand == "%attach") {
                        std::string output_str = inputLine;
                        size_t tmppos = output_str.find(xtext, 0);
                        output_str.insert(tmppos, xargument);
                        insert(output_str);

                        std::cout << "line " << input_line << ": replaced \"" << inputLine << "\" with \"" << output_str << "\"" << std::endl;
                    }


                    else {
                        std::cout << "WARNING: unknown xcommand in line: " << config_line << " [" << xcommand << "]" << std::endl;
                        warnings_count ++;
                    }
                }
            }
            if (!fXcom) {
                std::cout << "WARNING: line " << input_line << ": not found in config-file." << std::endl;
                warnings_count ++;
            }
        }
    }

    std::cout << "\n#############################" << std::endl;
    std::cout << "reached end of inputfile ..." << std::endl;
    std::cout << warnings_count << " Warning(s)" << std::endl;
    std::cout << "new script was created: " << OUTPUT_FILE_PATH << "\n" << std::endl;

    return 0;
}


std::string getLineBeforeCommand(std::string _line) {
    size_t pos = _line.find('[');
    return _line.substr(0, pos);
}

std::string getCommand(std::string _line) {
    size_t pos_first = _line.find('[');
    size_t pos_last = _line.find(']');
    return _line.substr(pos_first + 1, pos_last - pos_first -1);
}

std::string getArgument(std::string _line) {
    size_t pos = _line.find(']');
    return _line.substr(pos + 1, std::string::npos);
}

std::string getLineFromInput(std::string _INPUT_FILE_PATH) {
    std::ifstream ifs(_INPUT_FILE_PATH.c_str(), std::ios_base::in);
    ifs.seekg(eekg);
    if (ifs.is_open()) {
        std::string buffer;
        while(!ifs.eof()) {
            input_line++;
            getline(ifs, buffer);
            if (!buffer.empty()) {
                eekg = ifs.tellg();
                ifs.close();
                return std::string(buffer);
            }
        }
        eekg = -1;
        ifs.close();
        return "";
    }
    else {
        ifs.close();
        return "";
    }
}

bool lookupInConfig(std::string inputLine, std::string& text, std::string& command, std::string& argument) {
    config_line = 0;
    std::ifstream conffs(CONFIG_FILE_PATH.c_str(), std::ios_base::in);
    if (conffs.is_open()) {
        /// search for entry in configfile
        std::string iLine = "";
        do {
            config_line++;
            getline(conffs, iLine);
        } while(iLine.find(inputLine, 0) == std::string::npos && !conffs.eof());

        if (iLine != "" && iLine[0] != '#' && !conffs.eof()) {

            text = getLineBeforeCommand(iLine);
            command = getCommand(iLine);
            argument = getArgument(iLine);

            conffs.close();
            return true;
        }
        else {
            //std::cout << "string not found in config ..." << std::endl;
            conffs.close();
            return false;
        }
    }
    else {
        conffs.close();
        return false;
    }
}

void insert(std::string _insertion) {
    std::ofstream ofs(OUTPUT_FILE_PATH.c_str(), std::ios_base::out | std::ios_base::app);
    ofs << _insertion << std::endl;
    ofs.close();
}

void clearOutputFile() {
    std::ofstream ofs(OUTPUT_FILE_PATH.c_str(), std::ios_base::out | std::ios_base::trunc);
    ofs.close();
}

bool input_eof() {
    if (eekg == -1) return true;
    else return false;
}

bool config_eof() {
    if (conf_eekg == -1) return true;
    else return false;
}

std::string getLineFromConfig() {
    std::ifstream ifs(CONFIG_FILE_PATH.c_str(), std::ios_base::in);
    if (ifs.is_open()) {
        ifs.seekg(conf_eekg);
        std::string buffer = "";
        while(!ifs.eof()) {
            config_line ++;
            getline(ifs, buffer);
            if (!buffer.empty() && buffer[0] != '#') {
                conf_eekg = ifs.tellg();
                ifs.close();
                return buffer;
            }
        }
        conf_eekg = -1;
    }
    ifs.close();
    return "";
}


bool getNextXCommand(std::string& xtext, std::string& xcommand, std::string& xargument) {
    while(!config_eof()) {
        std::string buffer = getLineFromConfig();
        if (getCommand(buffer)[0] == '&' || getCommand(buffer)[0] == '%') {

            xtext = getLineBeforeCommand(buffer);
            xcommand = getCommand(buffer);
            xargument = getArgument(buffer);

            return true;
        }
    }
    return false;
}
