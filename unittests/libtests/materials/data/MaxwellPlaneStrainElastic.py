#!/usr/bin/env python
#
# ----------------------------------------------------------------------
#
#                           Brad T. Aagaard
#                        U.S. Geological Survey
#
# <LicenseText>
#
# ----------------------------------------------------------------------
#

## @file unittests/libtests/materials/data/MaxwellPlaneStrainElastic.py

## @brief Python application for generating C++ data files for testing
## C++ MaxwellPlaneStrain object with elastic behavior.

from ElasticMaterialApp import ElasticMaterialApp

import numpy

# ----------------------------------------------------------------------
dimension = 2
numElasticConsts = 6
tensorSize = 3

# MaxwellPlaneStrainElastic class
class MaxwellPlaneStrainElastic(ElasticMaterialApp):
  """
  Python application for generating C++ data files for testing C++
  MaxwellPlaneStrain object with elastic behavior.
  """
  
  # PUBLIC METHODS /////////////////////////////////////////////////////
  
  def __init__(self, name="maxwellplanestrainelastic"):
    """
    Constructor.
    """
    ElasticMaterialApp.__init__(self, name)

    # import pdb
    # pdb.set_trace()

    numLocs = 2

    self.dimension = dimension
    self.numLocs = numLocs

    self.dbPropertyValues = ["density", "vs", "vp", "viscosity"]
    self.numPropertyValues = numpy.array([1, 1, 1, 1], dtype=numpy.int32)

    self.dbStateVarValues = ["total-strain-xx",
                             "total-strain-yy",
                             "total-strain-xy",
                             "viscous-strain-xx",
                             "viscous-strain-yy",
                             "viscous-strain-zz",
                             "viscous-strain-xy"
                             ]
    self.numStateVarValues = numpy.array([3, 4], dtype=numpy.int32)
    
    densityA = 2500.0
    vsA = 3000.0
    vpA = vsA*3**0.5
    viscosityA = 1.0e18
    strainA = [1.1e-4, 1.2e-4, 1.4e-4]
    initialStressA = [2.1e4, 2.2e4, 2.4e4]
    initialStrainA = [3.1e-5, 3.2e-5, 3.4e-5]
    muA = vsA*vsA*densityA
    lambdaA = vpA*vpA*densityA - 2.0*muA
    maxwellTimeA = viscosityA / muA
    
    densityB = 2000.0
    vsB = 1200.0
    vpB = vsB*3**0.5
    viscosityB = 1.0e18
    strainB = [4.1e-4, 4.2e-4, 4.4e-4]
    initialStressB = [5.1e4, 5.2e4, 5.4e4]
    initialStrainB = [6.1e-5, 6.2e-5, 6.4e-5]
    muB = vsB*vsB*densityB
    lambdaB = vpB*vpB*densityB - 2.0*muB
    maxwellTimeB = viscosityB / muB

    self.lengthScale = 1.0e+3
    self.pressureScale = muA
    self.timeScale = 1.0
    self.densityScale = 1.0e+3

    self.dbProperties = numpy.array([ [densityA, vsA, vpA, viscosityA],
                                      [densityB, vsB, vpB, viscosityB] ], 
                                    dtype=numpy.float64)
    self.properties = numpy.array([ [densityA, muA, lambdaA, maxwellTimeA],
                                    [densityB, muB, lambdaB, maxwellTimeB] ],
                                     dtype=numpy.float64)

    # TEMPORARY, need to determine how to use initial state variables
    self.dbStateVars = numpy.zeros( (numLocs, tensorSize+4),
                                    dtype=numpy.float64)
    self.stateVars = numpy.zeros( (numLocs, tensorSize+4),
                                  dtype=numpy.float64)

    mu0 = self.pressureScale
    density0 = self.densityScale
    time0 = self.timeScale
    self.propertiesNondim = \
        numpy.array([ [densityA/density0, muA/mu0, lambdaA/mu0, maxwellTimeA/time0],
                      [densityB/density0, muB/mu0, lambdaB/mu0, maxwellTimeB/time0] ],
                    dtype=numpy.float64)

    self.stateVarsNondim = self.stateVars # no scaling

    self.initialStress = numpy.array([initialStressA,
                                      initialStressB],
                                    dtype=numpy.float64)
    self.initialStrain = numpy.array([initialStrainA,
                                      initialStrainB],
                                    dtype=numpy.float64)
    
    self.density = numpy.array([densityA,
                                densityB],
                               dtype=numpy.float64)

    self.strain = numpy.array([strainA,
                               strainB],
                               dtype=numpy.float64)
    
    self.stress = numpy.zeros( (numLocs, tensorSize), dtype=numpy.float64)
    self.elasticConsts = numpy.zeros( (numLocs, numElasticConsts), \
                                      dtype=numpy.float64)
    self.stateVarsUpdated = numpy.zeros( (numLocs, tensorSize + 4), \
                                         dtype=numpy.float64)

    (self.elasticConsts[0,:], self.stress[0,:], self.stateVarsUpdated[0,:]) = \
        self._calcStress(strainA, muA, lambdaA, \
                           initialStressA, initialStrainA)
    (self.elasticConsts[1,:], self.stress[1,:], self.stateVarsUpdated[1,:]) = \
        self._calcStress(strainB, muB, lambdaB, \
                           initialStressB, initialStrainB)
    self.dtStableImplicit = 0.2*min(maxwellTimeA, maxwellTimeB)
    return


  def _calcStress(self, strainV, muV, lambdaV, initialStressV, initialStrainV):
    """
    Compute stress and derivative of elasticity matrix.
    """
    C1111 = lambdaV + 2.0*muV
    C1122 = lambdaV
    C1112 = 0.0
    C2222 = lambdaV + 2.0*muV
    C2212 = 0.0
    C1212 = 2.0*muV
    elasticConsts = numpy.array([C1111, C1122, C1112,
                                 C2222, C2212,
                                 C1212], dtype=numpy.float64)

    strain = numpy.reshape(strainV, (tensorSize,1))
    initialStress = numpy.reshape(initialStressV, (tensorSize,1))
    initialStrain = numpy.reshape(initialStrainV, (tensorSize,1))
    elastic = numpy.array([ [C1111, C1122, C1112],
                            [C1122, C2222, C2212],
                            [C1112, C2212, C1212] ], dtype=numpy.float64)
    stress = numpy.dot(elastic, strain-initialStrain) + initialStress
    meanStrain = (strain[0,0] + strain[1,0])/3.0
    viscousStrain = numpy.array([strain[0,0] - meanStrain,
                                 strain[1,0] - meanStrain,
                                 -meanStrain,
                                 strain[2,0]],
                                dtype=numpy.float64)
    viscousStrainVec = numpy.ravel(viscousStrain)
    strainVec = numpy.ravel(strain)
    
    stateVarsUpdated = numpy.concatenate((strainVec, viscousStrainVec))
    return (elasticConsts, numpy.ravel(stress), numpy.ravel(stateVarsUpdated))
  

# MAIN /////////////////////////////////////////////////////////////////
if __name__ == "__main__":

  app = MaxwellPlaneStrainElastic()
  app.run()


# End of file 