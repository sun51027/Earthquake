namespace mgr {
    /*******************************************************************************
    *   Implementing directly as all-in-one class
    *******************************************************************************/
    template < typename T >
    class RootObjMgr < T > ::iterator
    {
        private:

            typedef typename RootObjMgr < T > ::ObjContainer::iterator OrigIt;
            OrigIt _it;

        public:

            iterator( const OrigIt &x )
            {
                _it = x;
            }

            ~iterator()
            {
            }

            const T*
            operator*( ) const { return _it->second.get();
            }

            T*
            operator*( ){ return _it->second.get();
            }

            // No jumping operators here
            iterator& operator++ ( ){ ++_it;
                                      return *this;
            }

            iterator& operator-- ( ){ --_it;
                                      return *this;
            }

            iterator
                operator++ (int){iterator tmp( _it );
                                 ++_it;
                                 return tmp;
            }

            iterator
                operator-- (int){iterator tmp( _it );
                                 --_it;
                                 return *this;
            }

            bool
                operator == ( const iterator &x ){ return _it == x._it;
            }

            bool
                operator != ( const iterator &x ){ return _it != x._it;
            }
    };

    /*******************************************************************************
    *   Constant iterator class
    *******************************************************************************/

    template < typename T >
    class RootObjMgr < T > ::const_iterator
    {
        private:

            typedef typename RootObjMgr < T > ::ObjContainer::const_iterator OrigIt;
            OrigIt _it;

        public:

            const_iterator( const OrigIt &x )
            {
                _it = x;
            }

            ~const_iterator()
            {
            }

            const T*
            operator*( ) const { return _it->second.get();
            }

            T*
            operator*( ){ return _it->second.get();
            }

            // No jumping operators here
            const_iterator& operator++ ( ){ ++_it;
                                            return *this;
            }

            const_iterator& operator-- ( ){ --_it;
                                            return *this;
            }

            const_iterator
                operator++ (int){iterator tmp( _it );
                                 ++_it;
                                 return tmp;
            }

            const_iterator
                operator-- (int){iterator tmp( _it );
                                 --_it;
                                 return *this;
            }

            bool
                operator == ( const const_iterator &x ){ return _it == x._it;
            }

            bool
                operator != ( const const_iterator &x ){ return _it != x._it;
            }
    };
}/* mgr */
