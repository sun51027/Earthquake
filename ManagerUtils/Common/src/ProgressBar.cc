/*******************************************************************************
*
*  Filename    : Named.cc
*  Description : Implmentation to convertion functions in Named class
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/

#include "ManagerUtils/Common/interface/ProgressBar.hpp"

using namespace std::chrono;
using namespace std;

namespace mgr{
    tuple<int, int, int> format_duration( milliseconds& ms ) {
        auto secs = duration_cast<seconds>(ms);
        ms -= duration_cast<milliseconds>(secs);
        auto mins = duration_cast<minutes>(secs);
        secs -= duration_cast<seconds>(mins);
        auto hour = duration_cast<hours>(mins);
        mins -= duration_cast<minutes>(hour);
        
        return make_tuple( hour.count(), mins.count(), secs.count() );
    }
   
    void ProgressBar(const int& i, const int& entry)
    {
        static high_resolution_clock::time_point t1 = high_resolution_clock::now();
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        milliseconds duration = duration_cast<milliseconds>( t2 - t1 );
        milliseconds eta_duration;
    
        int ratio = i / (entry / 100 );
        if( ratio ){
            eta_duration = duration * ( 100 - ratio ) / ratio;
        }
        else
            eta_duration = std::chrono::milliseconds::zero();
    
        auto elapse = format_duration( duration );
        auto expect = format_duration( eta_duration );
        
        int lpad = (int) (ratio * PBWIDTH / 100. );
        int rpad = PBWIDTH - lpad;
       
        printf(
                "\033[0;36m[\033[0m Total events: %d \033[0;36m|\033[0m Elapsed Time: %02d:%02d:%02d \033[0;36m|\033[0m \033[0;32mETA:\033[0m \033[0;31m%02d:%02d:%02d\033[0m \033[0;36m]\033[0m \033[0;31m%3d%%\033[0m \033[0;36m[\033[0m\033[0;32m%.*s%*s\033[0m\033[0;36m]\033[0m\r", \
                entry, \
                get<0>(elapse), get<1>(elapse), get<2>(elapse), \
                get<0>(expect), get<1>(expect), get<2>(expect), \
                ratio, \
                lpad, PBSTR, rpad, "" );
        fflush (stdout);
    }
}/* mgr */
