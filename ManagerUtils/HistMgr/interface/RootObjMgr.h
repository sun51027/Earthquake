#ifndef MANAGERUTILS_ROOTMGR_ROOTOBJMGR_H
#define MANAGERUTILS_ROOTMGR_ROOTOBJMGR_H

#include <map>
#include <memory>
#include <regex>
#include <string>
#include <vector>

namespace mgr{
    template<typename T>
    class RootObjMgr {
        public:

            RootObjMgr ();
            virtual ~RootObjMgr ();

            RootObjMgr( const RootObjMgr& )            = delete;
            RootObjMgr& operator=( const RootObjMgr& ) = delete;


            // Common objects for single object accessing
            void                     AddObj( T* );
            void                     RemoveObj( const std::string& );
            T*                       GetObj( const std::string& );
            const T*                 GetObj( const std::string& ) const;
            std::vector<std::string> ObjNameList() const;
            std::vector<T*>          GetObjContains( const std::string& );


            // C++ style iterators for looping objects (independent of the leading string)easiler
            // Notice that the dereferencing of the iterator will return an instance of  T*
            // so use loops only like : "for( const auto obj : objmgr )" (not the lack of referencing)
            typedef std::map<std::string, std::unique_ptr<T> > ObjContainer;
            class iterator;
            class const_iterator;

            inline iterator       begin()       { return _objmap.begin(); }
            inline iterator       end()         { return _objmap.end(); }
            inline const_iterator begin() const { return _objmap.begin(); }
            inline const_iterator end()   const { return _objmap.end(); }

        protected:

            std::string MakeStoreName( const std::string&, const std::string& ) const;

        private:

            ObjContainer _objmap;
    };
}/* mgr */

/*******************************************************************************
*   Including template class implementation
*******************************************************************************/
#include "ManagerUtils/HistMgr/src/RootObjMgr.ipp"

/*******************************************************************************
*   Including template interator classes
*******************************************************************************/
#include "ManagerUtils/HistMgr/src/RootObjIter.ipp"

#endif
