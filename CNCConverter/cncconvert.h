
#ifndef _CNC_CONVERT_H_
#define _CNC_CONVERT_H_

#include <iostream>
#include <fstream>
#include <string>

const std::string CONFIG_FILE_PATH = "cnc.conf";
int eekg = 0;
size_t input_line = 0;
size_t config_line = 0;

int warnings_count = 0;

std::string getLineBeforeCommand(std::string _line);
std::string getCommand(std::string _line);
std::string getArgument(std::string _line);

std::string getLineFromInput(std::string _INPUT_FILE_PATH);
bool lookupInConfig(std::string inputLine, std::string& text, std::string& command, std::string& argument);

void insert(std::string _insertion);
void clearOutputFile();

bool input_eof();

#endif // _CNC_CONVERT_H_
