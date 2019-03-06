// -*- C++ -*-
//
// ----------------------------------------------------------------------
//
// Brad T. Aagaard, U.S. Geological Survey
// Charles A. Williams, GNS Science
// Matthew G. Knepley, University of Chicago
//
// This code was developed as part of the Computational Infrastructure
// for Geodynamics (http://geodynamics.org).
//
// Copyright (c) 2010-2017 University of California, Davis
//
// See COPYING for license information.
//
// ----------------------------------------------------------------------
//

/**
 * @file unittests/libtests/meshio/TestOutputTriggerTime.hh
 *
 * @brief C++ TestOutputTriggerTime object.
 *
 * C++ unit testing for Observers of solution.
 */

#if !defined(pylith_meshio_testoutputtriggertime_hh)
#define pylith_meshio_testoutputtriggertime_hh

#include <cppunit/extensions/HelperMacros.h>

#include "pylith/meshio/meshiofwd.hh" // HOLDSA OutputTriggerTime

/// Namespace for pylith package
namespace pylith {
    namespace meshio {
        class TestOutputTriggerTime;
    } // meshio
} // pylith

class pylith::meshio::TestOutputTriggerTime : public CppUnit::TestFixture {
    // CPPUNIT TEST SUITE //////////////////////////////////////////////////////////////////////////////////////////////
    CPPUNIT_TEST_SUITE(TestOutputTriggerTime);

    CPPUNIT_TEST(testTimeSkip);
    CPPUNIT_TEST(testShouldWrite);

    CPPUNIT_TEST_SUITE_END();

    // PUBLIC METHODS //////////////////////////////////////////////////////////////////////////////////////////////////
public:

    /// Test setTimeSkip() and getTimeSkip().
    void testTimeSkip(void);

    /// Test shouldWrite().
    void testShouldWrite(void);

}; // class TestOutputTriggerTime

#endif // pylith_meshio_testoutputtriggertime_hh

// End of file
