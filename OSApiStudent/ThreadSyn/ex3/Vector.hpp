#ifndef VECTOR_HPP_
#define VECTOR_HPP_

//=======================================================
// Class: Vector
// contains a size_-size vector of integers.
// Use the function setAndTest to set all elements
// of the vector to a certain value and then test that
// the value is indeed correctly set
//=======================================================

#include "ScopedLocker.hpp"


class Vector
{
public:
   Vector(unsigned int size = 10000) : size_(size)
      {
         vector_ = new int[size_];
         set(0);
         pthread_mutex_init(&mutex_,nullptr);
      }

   ~Vector()
      {
         delete[] vector_;
         pthread_mutex_destroy(&mutex_);
      }

   bool setAndTest(int n)
      {
         ScopedLocker lock(mutex_);
         set(n);
         return test(n);
      }

private:
   void set(int n)
      {
         for(unsigned int i=0; i<size_; i++) vector_[i] = n;
      }

   bool test(int n)
      {
         for(unsigned int i=0; i<size_; i++) if(vector_[i] != n) return false;
         return true;
      }

   int*           vector_;
   unsigned int   size_;
   pthread_mutex_t mutex_;

};

#endif
