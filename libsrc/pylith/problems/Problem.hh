// -*- C++ -*-
//
// ======================================================================
//
// Brad T. Aagaard, U.S. Geological Survey
// Charles A. Williams, GNS Science
// Matthew G. Knepley, University of Chicago
//
// This code was developed as part of the Computational Infrastructure
// for Geodynamics (http://geodynamics.org).
//
// Copyright (c) 2010-2015 University of California, Davis
//
// See COPYING for license information.
//
// ======================================================================
//

/**
 * @file libsrc/problems/Problem.hh
 *
 * @brief C++ object that manages formulating the equations.
 */

#if !defined(pylith_problems_problem_hh)
#define pylith_problems_problem_hh

// Include directives ---------------------------------------------------
#include "problemsfwd.hh" // forward declarations

#include "pylith/feassemble/feassemblefwd.hh" // USES Integrator
#include "pylith/topology/topologyfwd.hh" // USES Mesh, Field

#include "pylith/utils/petscfwd.h" // USES PetscVec, PetscMat

#include "pylith/utils/array.hh" // HASA std::vector


// Problem ----------------------------------------------------------
/** Reform the Jacobian and residual for the problem.
 *
 * We cast the problem in terms of F(t,u,du/dt) = G(t,u), u(t0) = u0.
 *
 * In PETSc time stepping (TS) notation, G is the RHS, and F is the I
 * function.
 *
 */
class pylith::problems::Problem
{ // Problem
  friend class TestProblem; // unit testing

// PUBLIC MEMBERS ///////////////////////////////////////////////////////
public :

  /// Constructor
  Problem(void);

  /// Destructor
  ~Problem(void);

  /// Deallocate PETSc and local data structures.
  void deallocate(void);

  /** Set handles to integrators.
   *
   * @param integratorArray Array of integrators.
   * @param numIntegrators Number of integrators.
   */
  void integrators(feassemble::Integrator* integratorArray[] ,
		   const int numIntegrators);
  
  /** Set handles to constraints.
   *
   * @param constraintArray Array of constraints.
   * @param numContraints Number of constraints.
   */
  void constraints(feassemble::Constraint* constraintArray[] ,
		   const int numConstraints);
  
  /** Set handle to preconditioner.
   *
   * @param pc PETSc preconditioner.
   */
  void customPCMatrix(PetscMat& mat);

  /** Initialize.
   *
   */
  virtual
  void initialize(void);

  /** Reform RHS residual, G(t,u).
   *
   * @param residual Residual field, G(t,u).
   * @param t Current time.
   * @param solution Solution field.
   */
  void reformRHSResidual(pylith::topology::Field* residual,
			 const PetscReal t,
			 const pylith::topology::Field& solution);
  
  /* Reform RHS Jacobian for G(t,u).
   *
   * @param jacobian Jacobian matrix.
   * @param t Current time.
   * @param solution Solution field.
   */
  void reformRHSJacobian(pylith::topology::Jacobian* jacobian,
			 const PetscReal t,
			 const pylith::topology::Field& solution);

  /** Reform LHS residual, F(t,u,\dot{u}).
   *
   * @param residual Residual field, F(t,u,\dot{u}).
   * @param t Current time.
   * @param solution Solution field.
   */
  void reformLHSResidual(pylith::topology::Field* residual,
			 const PetscReal t,
			 const pylith::topology::Field& solution);
  
  /* Reform LHS Jacobian for F(t,u,\dot{u}) for implicit time stepping.
   *
   * @param jacobian Jacobian matrix.
   * @param t Current time.
   * @param solution Solution field.
   */
  void reformLHSJacobianImplicit(pylith::topology::Jacobian* jacobian,
				 const PetscReal t,
				 const pylith::topology::Field& solution);

  /* Reform LHS Jacobian for F(t,u,\dot{u}) for explicit time stepping.
   *
   * @param jacobian Jacobian matrix.
   * @param t Current time.
   * @param solution Solution field.
   */
  void reformLHSJacobianExplicit(pylith::topology::Jacobian* jacobian,
				 const PetscReal t,
				 const pylith::topology::Field& solution);

// PROTECTED MEMBERS ////////////////////////////////////////////////////
protected :

  pylith::topology::Field* _solution; ///< Handle to solution field.
  pylith::topology::Field* _residualRHS; ///< Handle to residual field for RHS, G(t,u).
  pylith::topology::Field* _residualLHS; ///< Handle to residual field for LHS, F(t,u,\dot{u}).
  pylith::topology::Jacobian* _jacobianRHS; ///< Handle to Jacobian for RHS, G(t,u).
  pylith::topology::Jacobian* _jacobianLHS; ///< Handle to Jacobian for LHS, F(t,u,\dot{u}).

  std::vector<pylith::feassemble::Integrator*> _integrators; ///< Array of integrators.
  std::vector<pylith::feassemble::Constraint*> _constraints; ///< Array of constraints.

  PetscMat _customConstraintPCMat; ///< Custom PETSc preconditioning matrix for constraints.
  bool _useCustomConstraintPC; ///< True if using custom preconditioner for Lagrange constraints.

// NOT IMPLEMENTED //////////////////////////////////////////////////////
private :

  Problem(const Problem&); ///< Not implemented
  const Problem& operator=(const Problem&); ///< Not implemented

}; // Problem

#endif // pylith_problems_problem_hh


// End of file 
