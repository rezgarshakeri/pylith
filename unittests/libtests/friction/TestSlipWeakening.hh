// -*- C++ -*-
//
// ----------------------------------------------------------------------
//
//                           Brad T. Aagaard
//                        U.S. Geological Survey
//
// {LicenseText}
//
// ----------------------------------------------------------------------
//

/**
 * @file unittests/libtests/friction/TestSlipWeakening.hh
 *
 * @brief C++ TestSlipWeakening object
 *
 * C++ unit testing for SlipWeakening.
 */

#if !defined(pylith_friction_testelasticisotropic3d_hh)
#define pylith_friction_testelasticisotropic3d_hh

#include "TestFrictionModel.hh"

/// Namespace for pylith package
namespace pylith {
  namespace friction {
    class TestSlipWeakening;
  } // friction
} // pylith

/// C++ unit testing for SlipWeakening
class pylith::friction::TestSlipWeakening : public TestFrictionModel
{ // class TestSlipWeakening

  // CPPUNIT TEST SUITE /////////////////////////////////////////////////
  CPPUNIT_TEST_SUITE( TestSlipWeakening );

  CPPUNIT_TEST( testDBToProperties );
  CPPUNIT_TEST( testNonDimProperties );
  CPPUNIT_TEST( testDimProperties );
  CPPUNIT_TEST( testDBToStateVars );
  CPPUNIT_TEST( testNonDimStateVars );
  CPPUNIT_TEST( testDimStateVars );
  CPPUNIT_TEST( testHasProperty );
  CPPUNIT_TEST( testHasStateVar );
  CPPUNIT_TEST( test_calcFriction );
  CPPUNIT_TEST( test_updateStateVars );

  CPPUNIT_TEST_SUITE_END();

  // PUBLIC METHODS /////////////////////////////////////////////////////
public :

  /// Setup testing data.
  void setUp(void);

  /// Test hasProperty().
  void testHasProperty(void);

  /// Test hasStateVar().
  void testHasStateVar(void);

}; // class TestSlipWeakening

#endif // pylith_friction_testelasticisotropic3d_hh


// End of file 