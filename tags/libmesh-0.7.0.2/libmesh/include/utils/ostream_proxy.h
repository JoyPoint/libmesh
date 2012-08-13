// $Id$

// The libMesh Finite Element Library.
// Copyright (C) 2002-2008 Benjamin S. Kirk, John W. Peterson, Roy H. Stogner
  
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
  
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
  
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



#ifndef __ostream_proxy_h__
#define __ostream_proxy_h__



// C++ includes
#include <iostream>

// Local Includes
// We don't use anything libMesh-specific here (yet)
// #include "libmesh_common.h"


namespace libMesh
{

// Forward Declarations



 // ------------------------------------------------------------
 // OStreamProxy class definition
 //
 // This class is intended to be reseatable like a pointer-to-ostream
 // for flexibility, but to look like a reference when used to produce
 // less awkward user code.
 //
 // It is up to the user to ensure that the target ostream 

 template <typename charT=char, typename traits=std::char_traits<charT> >
 class BasicOStreamProxy
 {
 public:
  /**
   * This class is going to be used to proxy for ostream, but other
   * character and traits types are possible
   */
  typedef std::basic_ostream<charT,traits> streamT;

  /**
   * This class is going to be used to proxy for ostream, but other
   * character and traits types are possible
   */
  typedef std::basic_streambuf<charT,traits> streambufT;

  /**
   * Default constructor.  Takes a reference to the \p target ostream
   * to which we pass output.  The user is responsible for ensuring
   * that this target exists for as long as the proxy does.
   */
  BasicOStreamProxy (streamT& target) : _target(&target) {};

  /**
   * Default destructor.
   */
  ~BasicOStreamProxy () {};

  // 
  // Functions that get passed to the proxied target:
  //

  /**
   * Conversion to ostream&, for when we get passed to a function
   * requesting one.
   */
  operator streamT&() { return *_target; }

  /**
   * Conversion to const ostream&, for when we get passed to a
   * function requesting one.
   */
  operator const streamT&() const { return *_target; }

  /**
   * Redirect any output to the target.
   */
  template<typename T>
  streamT& operator<< (const T& in) {
    return (*_target) << in;
  }

  /**
   * Redirect any ostream manipulators to the target.
   */
  streamT& operator<< (streamT& (*in)(streamT&)) {
    return (*_target) << in;
  }

  /**
   * Redirect any ios manipulators to the target.
   */
  streamT& operator<< (std::basic_ios<charT,traits>& (*in)(std::basic_ios<charT,traits>&)) {
    return (*_target) << in;
  }

  /**
   * Redirect any ios_base manipulators to the target.
   */
  streamT& operator<< (std::ios_base& (*in)(std::ios_base&)) {
    return (*_target) << in;
  }

  /**
   * Get the associated stream buffer
   */
  streambufT* rdbuf () const { return _target->rdbuf(); }

  /**
   * Set the associated stream buffer
   */
  streambufT* rdbuf ( streambufT* sb ) { return _target->rdbuf(sb); }

  /**
   * Flush the associated stream buffer
   */
  streamT& flush () { return _target->flush(); }

  /**
   * Get the associated format flags
   */
  std::ios_base::fmtflags flags ( ) const
    { return _target->flags(); }

  /**
   * Set/get the associated format flags
   */
  std::ios_base::fmtflags flags ( std::ios_base::fmtflags fmtfl ) 
    { return _target->flags(fmtfl); }

  /**
   * Set the associated flags
   */
  std::ios_base::fmtflags setf ( std::ios_base::fmtflags fmtfl ) 
    { return _target->setf(fmtfl); }

  /**
   * Set the associated flags
   */
  std::ios_base::fmtflags setf ( std::ios_base::fmtflags fmtfl,
                                 std::ios_base::fmtflags mask )
    { return _target->setf(fmtfl, mask); }

  /**
   * Clear the associated flags
   */
  void unsetf ( std::ios_base::fmtflags mask ) 
    { _target->unsetf(mask); }

  /**
   * Get the associated write precision
   */
  std::streamsize precision () const
    { return _target->precision(); }

  /**
   * Set the associated write precision
   */
  std::streamsize precision ( std::streamsize prec )
    { return _target->precision(prec); }

  //
  // Functions that affect the Proxy class:
  //

  /**
   * Reset the proxy to point to a different \p target.  Note that this
   * does not delete the previous target.
   */
  void reset (streamT& target) { _target = &target; }

  /**
   * Rather than implement every ostream/ios/ios_base function, we'll
   * be lazy and make esoteric uses go through a \p get() function.
   */
  streamT* get() {
    return _target;
  }

  /**
   * Rather than implement every ostream/ios/ios_base function, we'll
   * be lazy and make esoteric uses go through a \p get() function.
   */
  const streamT* get() const {
    return _target;
  }

 private:
  /**
   * The pointer to the "real" ostream we send everything to.
   */
   streamT* _target;
 };

typedef BasicOStreamProxy<> OStreamProxy;

} // namespace libMesh

#endif // ifndef __ostream_proxy_h__
