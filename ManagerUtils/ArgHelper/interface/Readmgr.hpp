#ifndef READMGR_HPP
#define READMGR_HPP


#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <vector>

namespace mgr{
    class Readmgr {
        public:

            Readmgr( const std::string& );

            virtual ~Readmgr(){}

            void                        ChangeJSON( const std::string& );
            boost::property_tree::ptree GetSubTree( const std::string& );

            template<typename T>
            T GetSingleData( const std::string& );
            template<typename T>
            T GetSubSingleData( const std::string&, const std::string& );

            template<typename T>
            std::vector<T> GetListData( const std::string& );
            template<typename T>
            std::vector<T> GetSubListData( const std::string&, const std::string& );

        private:

            boost::property_tree::ptree root;
    };
}

#include "ManagerUtils/ArgHelper/src/Readmgr.ipp"

#endif
