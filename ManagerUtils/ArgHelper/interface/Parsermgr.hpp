#ifndef PARSERMGR_HPP
#define PARSERMGR_HPP

#include <boost/program_options.hpp>
#include <initializer_list>
#include <string>
#include <vector>

namespace mgr{
    class Parsermgr {
        public:

            enum PARSING_STATUS
            {
                FAIL_PARSER,
                HELP_PARSER,
                PASS_PARSER
            };

            Parsermgr();
            virtual ~Parsermgr(){}

            Parsermgr& AddOptions( const boost::program_options::options_description& );
            int        ParseOptions( int argc, char* argv[] );
            bool       CheckOption( const std::string& );

            std::string              OptName();
            std::string              CustomName( const std::vector<std::string>& );
            std::vector<std::string> GetNamelist()
            {
                return _namelist;
            }

            void GetOptlist();

            template<typename T>
            void AddCutName( std::initializer_list<T> );

            template<typename T>
            void SetFileName( std::initializer_list<T> );

            template<typename T>
            T GetOption( const std::string& );

        private:

            boost::program_options::variables_map _vm;
            boost::program_options::options_description _desc;
            std::vector<std::string> _namelist;
            std::vector<std::string> _cutlist;
    };
}

#include "ManagerUtils/ArgHelper/src/Parsermgr.ipp"

#endif
