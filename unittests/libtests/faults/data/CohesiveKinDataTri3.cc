// -*- C++ -*-
//
// ======================================================================
//
//                           Brad T. Aagaard
//                        U.S. Geological Survey
//
// {LicenseText}
//
// ======================================================================
//

/* Original mesh
 *
 * Cells are 0-1, vertices are 2-5.
 *
 *              3
 *             /|\
 *            / | \
 *           /  |  \
 *          /   |   \
 *         2    |    5
 *          \   |   /
 *           \  |  /
 *            \ | /
 *             \|/
 *              4
 *
 *
 * After adding cohesive elements
 *
 * Cells are 0-1, 8, vertices are 2-7.
 *
 *              6 -8- 3
 *             /|     |\
 *            / |     | \
 *           /  |     |  \
 *          /   |     |   \
 *         2    |     |    5
 *          \   |     |   /
 *           \  |     |  /
 *            \ |     | /
 *             \|     |/
 *              7 -9- 4
 */

#include "CohesiveKinDataTri3.hh"

const char* pylith::faults::CohesiveKinDataTri3::_meshFilename =
  "data/tri3.mesh";

const int pylith::faults::CohesiveKinDataTri3::_spaceDim = 2;

const int pylith::faults::CohesiveKinDataTri3::_cellDim = 1;

const int pylith::faults::CohesiveKinDataTri3::_numBasis = 2;

const int pylith::faults::CohesiveKinDataTri3::_numQuadPts = 1;

const double pylith::faults::CohesiveKinDataTri3::_quadPts[] = {
  0.0,
};

const double pylith::faults::CohesiveKinDataTri3::_quadWts[] = {
  2.0,
};

const double pylith::faults::CohesiveKinDataTri3::_basis[] = {
  0.5,
  0.5
};

const double pylith::faults::CohesiveKinDataTri3::_basisDeriv[] = {
  -0.5,
   0.5
};

const double pylith::faults::CohesiveKinDataTri3::_verticesRef[] = {
  -1.0, 1.0
};

const int pylith::faults::CohesiveKinDataTri3::_id = 10;

const char* pylith::faults::CohesiveKinDataTri3::_label = "fault";

const char* pylith::faults::CohesiveKinDataTri3::_finalSlipFilename = 
  "data/tri3_finalslip.spatialdb";

const char* pylith::faults::CohesiveKinDataTri3::_slipTimeFilename = 
  "data/tri3_sliptime.spatialdb";

const char* pylith::faults::CohesiveKinDataTri3::_riseTimeFilename = 
  "data/tri3_risetime.spatialdb";

const char* pylith::faults::CohesiveKinDataTri3::_matPropsFilename = 
  "data/bulkprops_2d.spatialdb";

const double pylith::faults::CohesiveKinDataTri3::_fieldT[] = {
  8.1, 9.1,
  8.2, 9.2, // 3
  8.3, 9.3, // 4
  8.4, 9.4,
  8.5, 9.5, // 6
  8.7, 9.7, // 7
  8.6, 9.6, // 8
  8.8, 9.8, // 9
};

const double pylith::faults::CohesiveKinDataTri3::_fieldIncr[] = {
  3.1, 4.1,
  3.2, 4.2, // 3
  3.3, 4.3, // 4
  3.4, 4.4,
  3.5, 4.5, // 6
  3.7, 4.7, // 7
  3.6, 4.6, // 8
  3.8, 4.8, // 9
};

const double pylith::faults::CohesiveKinDataTri3::_jacobianLumped[] = {
  1.1, 1.1,
  1.2, 1.2, // 3
  1.3, 1.3, // 4
  1.4, 1.4,
  1.5, 1.5, // 6
  1.7, 1.7, // 7
  1.6, 1.6, // 8
  1.8, 1.8, // 9
};

const int pylith::faults::CohesiveKinDataTri3::_numConstraintVert = 2;
const int pylith::faults::CohesiveKinDataTri3::_constraintVertices[] = {
  8, 9
};


const double pylith::faults::CohesiveKinDataTri3::_orientation[] = {
  0.0, -1.0,  -1.0, 0.0,
  0.0, -1.0,  -1.0, 0.0
};

const double pylith::faults::CohesiveKinDataTri3::_area[] = {
  1.0,
  1.0,
};

const double pylith::faults::CohesiveKinDataTri3::_residual[] = {
  0.0,  0.0,
 -9.6, -8.6, // 3
 -9.8, -8.8, // 4
  0.0,  0.0,
 +9.6, +8.6, // 6
 +9.8, +8.8, // 7
  0.3,  0.3, // 8
  0.4,  0.4, // 9
};

const double pylith::faults::CohesiveKinDataTri3::_residualIncr[] = {
  0.0,  0.0,
 -9.6, -8.6, // 3
 -9.8, -8.8, // 4
  0.0,  0.0,
 +9.6, +8.6, // 6
 +9.8, +8.8, // 7
  0.3,  0.3, // 8
  0.4,  0.4, // 9
};

const double pylith::faults::CohesiveKinDataTri3::_jacobian[] = {
  0.0, 0.0, // 2x
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0, // 2y
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0, // 3x
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0,+1.0, // 8
  0.0, 0.0,
  0.0, 0.0, // 3y
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
 +1.0, 0.0, //  8
  0.0, 0.0,
  0.0, 0.0, // 4x
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0,+1.0, //  9
  0.0, 0.0, // 4y
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
 +1.0, 0.0, //  9
  0.0, 0.0, // 5x
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0, // 5y
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0, // 6x
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0,-1.0, //  8
  0.0, 0.0,
  0.0, 0.0, // 6y
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
 -1.0, 0.0, //  8
  0.0, 0.0,
  0.0, 0.0, // 7x
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0,-1.0, //  9
  0.0, 0.0, // 7y
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
 -1.0, 0.0, //  9

  0.0, 0.0, // 8x
  0.0,+1.0, //  3
  0.0, 0.0,
  0.0, 0.0,
  0.0,-1.0, //  6
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0, // 8y
 +1.0, 0.0, //  3
  0.0, 0.0,
  0.0, 0.0,
 -1.0, 0.0, //  6
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0,

  0.0, 0.0, // 9x
  0.0, 0.0,
  0.0,+1.0, //  4
  0.0, 0.0,
  0.0, 0.0,
  0.0,-1.0, //  7
  0.0, 0.0,
  0.0, 0.0,
  0.0, 0.0, // 9y
  0.0, 0.0,
 +1.0, 0.0, //  4
  0.0, 0.0,
  0.0, 0.0,
 -1.0, 0.0, //  7
  0.0, 0.0,
  0.0, 0.0,
};

const double pylith::faults::CohesiveKinDataTri3::_fieldIncrAdjusted[] = {
  3.1, 4.1,
  11.3208825088, 10.4802977015, // 3
  10.9812907996, 10.2898485695, // 4
  3.4, 4.4,
  -2.99670600701, -0.524238161213, // 6
  -2.17392825851, 0.119527564532, // 7
  11.1363572418, 14.3450590105, // 8
  11.5868031403, 14.7856780395, // 9
};

pylith::faults::CohesiveKinDataTri3::CohesiveKinDataTri3(void)
{ // constructor
  meshFilename = const_cast<char*>(_meshFilename);
  spaceDim = _spaceDim;
  cellDim = _cellDim;
  numBasis = _numBasis;
  numQuadPts = _numQuadPts;
  quadPts = const_cast<double*>(_quadPts);
  quadWts = const_cast<double*>(_quadWts);
  basis = const_cast<double*>(_basis);
  basisDeriv = const_cast<double*>(_basisDeriv);
  verticesRef = const_cast<double*>(_verticesRef);
  id = _id;
  label = const_cast<char*>(_label);
  finalSlipFilename = const_cast<char*>(_finalSlipFilename);
  slipTimeFilename = const_cast<char*>(_slipTimeFilename);
  riseTimeFilename = const_cast<char*>(_riseTimeFilename);
  matPropsFilename = const_cast<char*>(_matPropsFilename);
  fieldT = const_cast<double*>(_fieldT);
  fieldIncr = const_cast<double*>(_fieldIncr);
  jacobianLumped = const_cast<double*>(_jacobianLumped);
  orientation = const_cast<double*>(_orientation);
  area = const_cast<double*>(_area);
  residualIncr = const_cast<double*>(_residualIncr);
  residual = const_cast<double*>(_residual);
  jacobian = const_cast<double*>(_jacobian);
  fieldIncrAdjusted = const_cast<double*>(_fieldIncrAdjusted);
  constraintVertices = const_cast<int*>(_constraintVertices);
  numConstraintVert = _numConstraintVert;  
} // constructor

pylith::faults::CohesiveKinDataTri3::~CohesiveKinDataTri3(void)
{}


// End of file
