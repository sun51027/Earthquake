/*******************************************************************************
*
*  Filename    : Named.hpp
*  Description : Base Class with std::string as ID and latex title
*  Author      : Yi-Mu "Enoch" Chen [ ensc@hep1.phys.ntu.edu.tw ]
*
*******************************************************************************/
#ifndef MANAGERUTILS_COMMON_NAMED_HPP
#define MANAGERUTILS_COMMON_NAMED_HPP

#include <string>

namespace mgr{
    class Named {// Only virtual for the sake of Inheritance Do not overload
        public:

            Named( const std::string& );
            virtual ~Named ();

            // Basic access functions
            virtual const std::string& Name() const final;

            virtual const std::string& LatexName() const final;
            virtual const std::string& RootName()  const final;

            // Setting functions
            virtual void SetLatexName( const std::string& x ) final;
            virtual void SetRootName( const std::string& x ) final;

        private:

            const std::string _name;
            std::string _latexname;
            std::string _rootname;
    };
}

#endif/* end of include guard: MANAGERUTILS_COMMON_NAMED_HPP */
