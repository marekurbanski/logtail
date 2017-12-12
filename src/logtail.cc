#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;



/*
 * Function converts integer value to string
*/
string int_to_str(int number)
    {
    std::string s = std::to_string(number);
    return s;
    }


/*
 * Function convert string to integer
*/
int str_to_int(string number)
    {    
    return std::stoi( number );
    }


/*
 * Function check if directory exists or not
*/
bool directory_exists( const char* pzPath )
    {
    if ( pzPath == NULL) return false;

    DIR *pDir;
    bool bExists = false;

    pDir = opendir (pzPath);

    if (pDir != NULL)
	{
        bExists = true;    
        (void) closedir (pDir);
	}
    return bExists;
    }


/*
 * Function check if file exists or not
*/
bool is_file_exist(const char *fileName)
    {
    std::ifstream infile(fileName);
    return infile.good();
    }


/*
 * Function get .logtail directory
 * This directory is each for every user - because every one can see different part of file
*/
string get_logtail_dir()
    {
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;    
    const char *logtail_dir_name = "/.logtail/";
    
    char * dir = new char[strlen(homedir) + strlen(logtail_dir_name) + 1];
    strcpy(dir, homedir);
    strcat(dir, logtail_dir_name);

    if( ! directory_exists( dir ) )
	{
	mkdir(dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}
    return dir;
    }



/*
 * Write to file what was the last position in file
 * this will be taken next time we will display this file
*/
bool write_to_file(char * filename, char * num_lines)
    {
    string dir_str = get_logtail_dir();
    char * dir = new char[dir_str.length() + 1];
    strcpy(dir, dir_str.c_str());
    char * file_path = new char[strlen(dir) + strlen(filename) + 1];
    strcpy(file_path,dir);
    strcat(file_path, filename);

    std::ofstream outfile( file_path );
    cout << file_path;
    outfile << num_lines << std::endl;

    outfile.close();
    
    return true;

    }

/*
 * Function read how many lines we have readed 
 * last time, when function program was executed
*/
int read_number_from_file(string filename)
    {
    char * p_filename = new char[filename.length() + 1];
    strcpy(p_filename, filename.c_str());
    
    string dir_str = get_logtail_dir();
    char * dir = new char[dir_str.length() + 1];
    
    strcpy(dir, dir_str.c_str());
    char * file_path = new char[strlen(dir) + strlen(p_filename) + 1];
    
    strcpy(file_path,dir);
    strcat(file_path, p_filename);

    std::string line = "0";
    
    if( is_file_exist(file_path) )
	{
	std::ifstream input( file_path );
	getline( input, line );
	}
    return str_to_int(line);
    }


/*
 * Main function to display file content
*/
bool parse_file(char * file)
    {
    // our result - default is true, it can be changed only when someone will rewrite file
    bool res = true;
    // starting from the begining of file
    int num = 0;

    // get the last positon displayed in this file
    int last_time = read_number_from_file(file);;

    std::ifstream input( file );
    for( std::string line; getline( input, line ); )
	{
	num++;
        if ( num > last_time )
	    {
	    cout << line << endl;
	    }
	}
    /*
     * This is the case when someone has overwriten our file
     * and now we have to parse him from begining
    */
    if ( last_time > num )
	{
	num = 0;
	res = false;
	}
    char * p_num = new char[int_to_str(num).length() + 1];
    strcpy(p_num, int_to_str(num).c_str());
    write_to_file(file, p_num);

    return res;
    }


/*
 * Help, and some info about
*/
bool help(string main_name)
    {
    cout << endl;
    cout << endl;
    cout << " logtail v.1.0" << endl;
    cout << "----------------------------------------------" << endl;
    cout << "Usage: " << main_name << " {file_name}" << endl;
    cout << endl;
    cout << "suggestions and questions: marek@www2.pl";
    cout << endl;
    cout << endl;

    return true;
    }


/*
 * Default main()
 * It gets command line params, and do the job
 * It will do it twice, when someone will overwrite log file
 */
int main(int argc, char* argv[])
    {
    if (argc < 2)
	{
	// I expect at least one parameter, I will display help
	help( argv[0] );
	return 1;
	}
    
    if (! parse_file(argv[1]))
	{
	/*
	 * Because someone has overwriten our file, 
	 * and number of lines is smaller than last time
	 * we have to parse it from begining
	*/
	parse_file(argv[1]);
	}
    return 0;
    }



