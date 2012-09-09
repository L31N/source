#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
    //Copy cmd parameters to a stdstring array
    string argvstr[argc];
    for (int i = 0; i < argc; i++)
    {
        argvstr[i] = string(argv[i]);
    }

    //Test weather the Programm runs as root
    int uid = getuid();

    if(uid != 0)
    {
        printf("This Programm requires to be root. Please log in as root or use sudo\n\n");
        return -1;
    }

	if (argc < 2)
	{
	    cout << "Invalid count of arguments ...\nuse --help for more information...\n\n";
	    return -1;
    }


	else if (argvstr[1] == "--help")
	{
		cout << "usage: ledSwitch lednr status (eg. ledSwitch 0 on)\n\n";
		return 0;
	}

    fstream file;


    //open the correct file
    if(argvstr[1]=="0")
    {
        file.open("/sys/class/leds/beagleboard::usr0/brightness", ios_base::out);
    }
    else if(argvstr[1]=="1")
    {
        file.open("/sys/class/leds/beagleboard::usr1/brightness", ios_base::out);
    }
    else if(argvstr[1]=="2")
    {
        file.open("/sys/class/leds/beagleboard::pmu_stat/brightness", ios_base::out);
    }

    //Check for errors
    if(!file.is_open())
    {
        cout << "An error occured" << endl << endl;
        return -1;
    }

    //Write Number to file
    if(argvstr[2]=="on")
    {
        file << "1";
    }
    else if(argvstr[2]=="off")
    {
        file << "0";
    }

    //close file
    file.close();


    return 0;
}
