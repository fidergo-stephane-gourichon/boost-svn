///////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005-2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_EXPAND_BWD_TEST_ALLOCATOR_HPP
#define BOOST_INTERPROCESS_EXPAND_BWD_TEST_ALLOCATOR_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/workaround.hpp>

#include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/interprocess/allocators/allocation_type.hpp>
#include <boost/utility/addressof.hpp>
#include <boost/assert.hpp>
#include <boost/interprocess/detail/utilities.hpp>
#include <boost/interprocess/detail/version_type.hpp>
#include <boost/interprocess/exceptions.hpp>
#include <memory>
#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <assert.h>

/*!\file
   Describes an allocator to test expand capabilities
*/

namespace boost {

namespace interprocess {

namespace test {

//This allocator just allows two allocations. The first one will return 
//mp_buffer + m_offset configured in the constructor. The second one
//will return mp_buffer.
template<class T>
class expand_bwd_test_allocator 
{
 private:
   typedef expand_bwd_test_allocator<T> self_t;
   typedef void *                   aux_pointer_t;
   typedef const void *             cvoid_ptr;

   template<class T2>
   expand_bwd_test_allocator& operator=(const expand_bwd_test_allocator<T2>&);

   expand_bwd_test_allocator& operator=(const expand_bwd_test_allocator&);

   public:
   typedef T                                    value_type;
   typedef T *                                  pointer;
   typedef const T *                            const_pointer;
   typedef T &                                  reference;
   typedef const T &                            const_reference;
   typedef std::size_t                          size_type;
   typedef std::ptrdiff_t                       difference_type;

   typedef detail::version_type<expand_bwd_test_allocator, 2>   version;

   template<class T2>
   struct rebind
   {  typedef expand_bwd_test_allocator<T2>   other;   };

   /*!Constructor from the segment manager. Never throws*/
   expand_bwd_test_allocator(T *buffer, size_type size, difference_type offset) 
      : mp_buffer(buffer), m_size(size)
      , m_offset(offset),  m_allocations(0){ }

   /*!Constructor from other expand_bwd_test_allocator. Never throws*/
   expand_bwd_test_allocator(const expand_bwd_test_allocator &other) 
      : mp_buffer(other.mp_buffer), m_size(other.m_size)
      , m_offset(other.m_offset),  m_allocations(0){ }

   /*!Constructor from related expand_bwd_test_allocator. Never throws*/
   template<class T2>
   expand_bwd_test_allocator(const expand_bwd_test_allocator<T2> &other)
      : mp_buffer(other.mp_buffer), m_size(other.m_size)
      , m_offset(other.m_offset),  m_allocations(0){ }

   pointer address(reference value) const
   {  return pointer(boost::addressof(value));  }

   const_pointer address(const_reference value) const
   {  return const_pointer(boost::addressof(value));  }

   pointer allocate(size_type count, cvoid_ptr hint = 0);

   void deallocate(const pointer &ptr, size_type)
   { }

   template<class Convertible>
   void construct(pointer ptr, const Convertible &value)
   {  new((void*)ptr) value_type(value);  }

   void destroy(pointer ptr)
   {  (*ptr).~value_type();  }

   size_type max_size() const
   {  return m_size;   }

   friend void swap(self_t &alloc1, self_t &alloc2)
   {  
      detail::do_swap(alloc1.mp_buffer, alloc2.mp_buffer);
      detail::do_swap(alloc1.m_size,    alloc2.m_size);
      detail::do_swap(alloc1.m_offset,  alloc2.m_offset);
   }

   //Experimental version 2 expand_bwd_test_allocator functions

   std::pair<pointer, bool>
      allocation_command(allocation_type command,
                         size_type limit_size, 
                         size_type preferred_size,
                         size_type &received_size, const pointer &reuse = 0)
   {
      if(!(command & expand_bwd)){
         assert(0);
      }
      
      received_size = limit_size;

      if(m_allocations == 0){
         if((m_offset + limit_size) > m_size){
            assert(0);
         }
         ++m_allocations;
         return std::pair<pointer, bool>(mp_buffer + m_offset, false);
      }
      else if(m_allocations == 1){
         if(limit_size > m_size){
            assert(0);
         }
         ++m_allocations;
         return std::pair<pointer, bool>(mp_buffer, true);
      }
      else{
         assert(0);
      }
      return std::pair<pointer, bool>(0, true);
   }

   /*!Returns maximum the number of objects the previously allocated memory
      pointed by p can hold.*/
   size_type size(const pointer &p) const
   {  return m_size; }

   /*!Allocates just one object. Memory allocated with this function
      must be deallocated only with deallocate_one().
      Throws boost::interprocess::bad_alloc if there is no enough memory*/
   pointer allocate_one()
   {  return this->allocate(1);  }

   /*!Deallocates memory previously allocated with allocate_one().
      You should never use deallocate_one to deallocate memory allocated
      with other functions different from allocate_one(). Never throws*/
   void deallocate_one(const pointer &p)
   {  return this->deallocate(p, 1);  }

   private:
   pointer           mp_buffer;
   size_type         m_size;
   difference_type   m_offset;
   char              m_allocations;
};

/*!Equality test for same type of expand_bwd_test_allocator*/
template<class T> inline
bool operator==(const expand_bwd_test_allocator<T>  &alloc1, 
                const expand_bwd_test_allocator<T>  &alloc2)
{  return false; }

/*!Inequality test for same type of expand_bwd_test_allocator*/
template<class T> inline
bool operator!=(const expand_bwd_test_allocator<T>  &alloc1, 
                const expand_bwd_test_allocator<T>  &alloc2)
{  return true; }

}  //namespace test {
}  //namespace interprocess {
}  //namespace boost {

#include <boost/interprocess/detail/config_end.hpp>

#endif   //BOOST_INTERPROCESS_EXPAND_BWD_TEST_ALLOCATOR_HPP

