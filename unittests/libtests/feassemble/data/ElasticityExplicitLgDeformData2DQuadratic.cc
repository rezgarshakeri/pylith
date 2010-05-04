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

// DO NOT EDIT THIS FILE
// This file was generated from python application elasticitylgdeformexplicitapp.

#include "ElasticityExplicitLgDeformData2DQuadratic.hh"

const int pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_spaceDim = 2;

const int pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_cellDim = 2;

const int pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_numVertices = 6;

const int pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_numCells = 1;

const int pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_numBasis = 6;

const int pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_numQuadPts = 6;

const char* pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_matType = "ElasticPlaneStrain";

const char* pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_matDBFilename = "data/elasticplanestrain.spatialdb";

const int pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_matId = 0;

const char* pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_matLabel = "elastic strain 2-D";

const double pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_dt =   1.00000000e-02;

const double pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_gravityVec[] = {
  0.00000000e+00, -1.00000000e+08,
};

const double pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_vertices[] = {
 -1.00000000e+00, -1.00000000e+00,
  1.00000000e+00,  2.00000000e-01,
 -1.50000000e+00,  5.00000000e-01,
 -2.50000000e-01,  3.50000000e-01,
 -1.25000000e+00, -2.50000000e-01,
  0.00000000e+00, -4.00000000e-01,
};

const int pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_cells[] = {
0,1,2,3,4,5,
};

const double pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_verticesRef[] = {
 -1.00000000e+00, -1.00000000e+00,
  1.00000000e+00, -1.00000000e+00,
 -1.00000000e+00,  1.00000000e+00,
  0.00000000e+00,  0.00000000e+00,
 -1.00000000e+00,  0.00000000e+00,
  0.00000000e+00, -1.00000000e+00,
};

const double pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_quadPts[] = {
 -7.50000000e-01, -7.50000000e-01,
  7.50000000e-01, -7.50000000e-01,
 -7.50000000e-01,  7.50000000e-01,
  0.00000000e+00, -7.50000000e-01,
 -7.50000000e-01,  0.00000000e+00,
  2.50000000e-01,  2.50000000e-01,
};

const double pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_quadWts[] = {
  3.33333333e-01,  3.33333333e-01,  3.33333333e-01,  3.33333333e-01,  3.33333333e-01,  3.33333333e-01,
};

const double pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_basis[] = {
  3.75000000e-01, -9.37500000e-02,
 -9.37500000e-02,  6.25000000e-02,
  3.75000000e-01,  3.75000000e-01,
  0.00000000e+00,  6.56250000e-01,
 -9.37500000e-02,  4.37500000e-01,
 -0.00000000e+00, -0.00000000e+00,
  0.00000000e+00, -9.37500000e-02,
  6.56250000e-01,  4.37500000e-01,
 -0.00000000e+00, -0.00000000e+00,
 -9.37500000e-02,  0.00000000e+00,
 -9.37500000e-02,  2.50000000e-01,
  1.87500000e-01,  7.50000000e-01,
 -9.37500000e-02, -9.37500000e-02,
  0.00000000e+00,  2.50000000e-01,
  7.50000000e-01,  1.87500000e-01,
  3.75000000e-01,  1.56250000e-01,
  1.56250000e-01,  1.56250000e+00,
 -6.25000000e-01, -6.25000000e-01,
};

const double pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_basisDerivRef[] = {
 -1.00000000e+00, -1.00000000e+00,
 -2.50000000e-01,  0.00000000e+00,
  0.00000000e+00, -2.50000000e-01,
  2.50000000e-01,  2.50000000e-01,
 -2.50000000e-01,  1.25000000e+00,
  1.25000000e+00, -2.50000000e-01,
  5.00000000e-01,  5.00000000e-01,
  1.25000000e+00,  0.00000000e+00,
  0.00000000e+00, -2.50000000e-01,
  2.50000000e-01,  1.75000000e+00,
 -2.50000000e-01, -2.50000000e-01,
 -1.75000000e+00, -1.75000000e+00,
  5.00000000e-01,  5.00000000e-01,
 -2.50000000e-01,  0.00000000e+00,
  0.00000000e+00,  1.25000000e+00,
  1.75000000e+00,  2.50000000e-01,
 -1.75000000e+00, -1.75000000e+00,
 -2.50000000e-01, -2.50000000e-01,
 -2.50000000e-01, -2.50000000e-01,
  5.00000000e-01,  0.00000000e+00,
  0.00000000e+00, -2.50000000e-01,
  2.50000000e-01,  1.00000000e+00,
 -2.50000000e-01,  5.00000000e-01,
 -2.50000000e-01, -1.00000000e+00,
 -2.50000000e-01, -2.50000000e-01,
 -2.50000000e-01,  0.00000000e+00,
  0.00000000e+00,  5.00000000e-01,
  1.00000000e+00,  2.50000000e-01,
 -1.00000000e+00, -2.50000000e-01,
  5.00000000e-01, -2.50000000e-01,
  1.00000000e+00,  1.00000000e+00,
  7.50000000e-01,  0.00000000e+00,
  0.00000000e+00,  7.50000000e-01,
  1.25000000e+00,  1.25000000e+00,
 -1.25000000e+00, -1.75000000e+00,
 -1.75000000e+00, -1.25000000e+00,
};

const double pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_fieldTIncr[] = {
 -4.00000000e-01, -6.00000000e-01,
  7.00000000e-01,  8.00000000e-01,
  0.00000000e+00,  2.00000000e-01,
 -5.00000000e-01, -4.00000000e-01,
  3.00000000e-01,  9.00000000e-01,
 -3.00000000e-01, -9.00000000e-01,
};

const double pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_fieldT[] = {
 -3.00000000e-01, -4.00000000e-01,
  5.00000000e-01,  6.00000000e-01,
  0.00000000e+00,  1.00000000e-01,
 -2.00000000e-01, -3.00000000e-01,
  2.00000000e-01,  3.00000000e-01,
 -1.00000000e-01, -2.00000000e-01,
};

const double pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_fieldTmdt[] = {
 -2.00000000e-01, -3.00000000e-01,
  3.00000000e-01,  4.00000000e-01,
  0.00000000e+00, -1.00000000e-01,
 -3.00000000e-01, -2.00000000e-01,
  1.00000000e-01,  4.00000000e-01,
 -2.00000000e-01, -6.00000000e-01,
};

const double pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_valsResidual[] = {
  3.35186619e+10,  8.10189832e+10,
 -7.94930135e+10, -4.37826989e+10,
  2.10035850e+10,  2.15156871e+10,
  7.31131484e+10, -6.34970387e+09,
 -5.42588065e+10, -9.94923096e+10,
  6.13022939e+09,  4.70980577e+10,
};

const double pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_valsJacobian[] = {
  2.24121094e+06,  0.00000000e+00,
  2.41699219e+05,  0.00000000e+00,
  2.41699219e+05,  0.00000000e+00,
  4.21875000e+06,  0.00000000e+00,
 -1.36230469e+06,  0.00000000e+00,
 -1.36230469e+06,  0.00000000e+00,
  0.00000000e+00,  2.24121094e+06,
  0.00000000e+00,  2.41699219e+05,
  0.00000000e+00,  2.41699219e+05,
  0.00000000e+00,  4.21875000e+06,
  0.00000000e+00, -1.36230469e+06,
  0.00000000e+00, -1.36230469e+06,
  2.41699219e+05,  0.00000000e+00,
  3.61083984e+06,  0.00000000e+00,
 -6.73828125e+05,  0.00000000e+00,
  3.45703125e+06,  0.00000000e+00,
 -1.52343750e+06,  0.00000000e+00,
 -1.12792969e+06,  0.00000000e+00,
  0.00000000e+00,  2.41699219e+05,
  0.00000000e+00,  3.61083984e+06,
  0.00000000e+00, -6.73828125e+05,
  0.00000000e+00,  3.45703125e+06,
  0.00000000e+00, -1.52343750e+06,
  0.00000000e+00, -1.12792969e+06,
  2.41699219e+05,  0.00000000e+00,
 -6.73828125e+05,  0.00000000e+00,
  3.61083984e+06,  0.00000000e+00,
  3.45703125e+06,  0.00000000e+00,
 -1.12792969e+06,  0.00000000e+00,
 -1.52343750e+06,  0.00000000e+00,
  0.00000000e+00,  2.41699219e+05,
  0.00000000e+00, -6.73828125e+05,
  0.00000000e+00,  3.61083984e+06,
  0.00000000e+00,  3.45703125e+06,
  0.00000000e+00, -1.12792969e+06,
  0.00000000e+00, -1.52343750e+06,
  4.21875000e+06,  0.00000000e+00,
  3.45703125e+06,  0.00000000e+00,
  3.45703125e+06,  0.00000000e+00,
  2.21484375e+07,  0.00000000e+00,
 -5.39062500e+06,  0.00000000e+00,
 -5.39062500e+06,  0.00000000e+00,
  0.00000000e+00,  4.21875000e+06,
  0.00000000e+00,  3.45703125e+06,
  0.00000000e+00,  3.45703125e+06,
  0.00000000e+00,  2.21484375e+07,
  0.00000000e+00, -5.39062500e+06,
  0.00000000e+00, -5.39062500e+06,
 -1.36230469e+06,  0.00000000e+00,
 -1.52343750e+06,  0.00000000e+00,
 -1.12792969e+06,  0.00000000e+00,
 -5.39062500e+06,  0.00000000e+00,
  8.46679688e+06,  0.00000000e+00,
  6.09375000e+06,  0.00000000e+00,
  0.00000000e+00, -1.36230469e+06,
  0.00000000e+00, -1.52343750e+06,
  0.00000000e+00, -1.12792969e+06,
  0.00000000e+00, -5.39062500e+06,
  0.00000000e+00,  8.46679688e+06,
  0.00000000e+00,  6.09375000e+06,
 -1.36230469e+06,  0.00000000e+00,
 -1.12792969e+06,  0.00000000e+00,
 -1.52343750e+06,  0.00000000e+00,
 -5.39062500e+06,  0.00000000e+00,
  6.09375000e+06,  0.00000000e+00,
  8.46679688e+06,  0.00000000e+00,
  0.00000000e+00, -1.36230469e+06,
  0.00000000e+00, -1.12792969e+06,
  0.00000000e+00, -1.52343750e+06,
  0.00000000e+00, -5.39062500e+06,
  0.00000000e+00,  6.09375000e+06,
  0.00000000e+00,  8.46679688e+06,
};

const double pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_valsResidualLumped[] = {
  3.35171172e+10,  8.10192601e+10,
 -7.94952005e+10, -4.37841381e+10,
  2.10014852e+10,  2.15149774e+10,
  7.31149003e+10, -6.34801637e+09,
 -5.42577005e+10, -9.94955366e+10,
  6.13320302e+09,  4.71014693e+10,
};

const double pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::_valsJacobianLumped[] = {
  4.21875000e+06,  4.21875000e+06,
  3.98437500e+06,  3.98437500e+06,
  3.98437500e+06,  3.98437500e+06,
  2.25000000e+07,  2.25000000e+07,
  5.15625000e+06,  5.15625000e+06,
  5.15625000e+06,  5.15625000e+06,
};

pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::ElasticityExplicitLgDeformData2DQuadratic(void)
{ // constructor
  spaceDim = _spaceDim;
  cellDim = _cellDim;
  numVertices = _numVertices;
  numCells = _numCells;
  numBasis = _numBasis;
  numQuadPts = _numQuadPts;
  matType = const_cast<char*>(_matType);
  matDBFilename = const_cast<char*>(_matDBFilename);
  matId = _matId;
  matLabel = const_cast<char*>(_matLabel);
  dt = _dt;
  gravityVec = const_cast<double*>(_gravityVec);
  vertices = const_cast<double*>(_vertices);
  cells = const_cast<int*>(_cells);
  verticesRef = const_cast<double*>(_verticesRef);
  quadPts = const_cast<double*>(_quadPts);
  quadWts = const_cast<double*>(_quadWts);
  basis = const_cast<double*>(_basis);
  basisDerivRef = const_cast<double*>(_basisDerivRef);
  fieldTIncr = const_cast<double*>(_fieldTIncr);
  fieldT = const_cast<double*>(_fieldT);
  fieldTmdt = const_cast<double*>(_fieldTmdt);
  valsResidual = const_cast<double*>(_valsResidual);
  valsJacobian = const_cast<double*>(_valsJacobian);
  valsResidualLumped = const_cast<double*>(_valsResidualLumped);
  valsJacobianLumped = const_cast<double*>(_valsJacobianLumped);
} // constructor

pylith::feassemble::ElasticityExplicitLgDeformData2DQuadratic::~ElasticityExplicitLgDeformData2DQuadratic(void)
{}


// End of file
