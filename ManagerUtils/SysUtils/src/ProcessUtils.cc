/*******************************************************************************
*
*  Filename    : SystemUtils.cc
*  Description : System interaction utility functions
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#include "ManagerUtils/SysUtils/interface/ProcessUtils.hpp"
#include "ManagerUtils/SysUtils/interface/TimeUtils.hpp"

#include <boost/format.hpp>

#include <cstdlib>
#include <cstring>// for strerror()
#include <errno.h>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <time.h>

using namespace std;

#define MAX_BUFFER 65536

namespace mgr{
    /******************************************************************************/

    unsigned
    NumOfThreads()
    {
        return std::thread::hardware_concurrency();
    }

    /*******************************************************************************
    *   Getting command line ouputs
    *******************************************************************************/
    string
    GetCMDOutput( const std::string& cmd )
    {
        static unsigned callcount = 0;
        // Main reference
        // http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix
        char buffer[ MAX_BUFFER ];
        std::string result = "";
        FILE* pipe         = popen( cmd.c_str(), "r" );
        ++callcount;

        if( !pipe ){
            // Error message generation
            // http://stackoverflow.com/questions/25188891/why-is-my-popen-failing
            cerr << "PIPE error: " << strerror( errno ) << endl;
            cerr << "Function called: " << callcount << " times" << endl;
            cerr << "Input command: " << cmd << endl;
            system( "cat /proc/meminfo" );
            throw std::runtime_error( "popen() failed!" );
        }

        while( !feof( pipe ) ){
            if( fgets( buffer, MAX_BUFFER, pipe ) != NULL ){
                result += buffer;
            }
        }

        pclose( pipe );
        return result;
    }

    /******************************************************************************/

    string
    GetCMDSTDOutput( const string& cmd )
    {
        return GetCMDOutput( cmd + " 2> /dev/null" );
    }

    /******************************************************************************/

    string
    GetCMDERROutput( const string& cmd )
    {
        return GetCMDOutput( cmd + " > /dev/null  2>&1 " );
    }

    /******************************************************************************/

    int
    HasProcess( const string& x, const string& exclude )
    {
        string cmd = "ps -U ${USER} ";
        cmd += " | awk '{print $4}' ";
        cmd += " | grep " + x;

        if( exclude != "" ){
            cmd += "| grep --invert-match  " + exclude;
        }

        cmd += " | wc --lines";
        string ans = GetCMDOutput( cmd );
        return stoi( ans );
    }

    /******************************************************************************/

    void
    WaitProcess( const string& x, const string& exclude, const unsigned sleeptime )
    {
        while( 1 ){
            const int proc_count     = HasProcess( x, exclude );
            const string time_string = CurrentDateTime();

            if( proc_count == 0 ){
                break;
            }

            boost::format pausemsg( "\r[%s|%s] %d instance(s) of [%s]..." );
            cout << pausemsg % getenv( "HOSTNAME" ) % time_string % proc_count % x << flush;
            SleepMillSec( sleeptime );
        }

        cout << "All Done!" << endl;
    }
}/* mgr */
