// -*- C++ -*-
//
// ======================================================================
//
// Brad T. Aagaard, U.S. Geological Survey
// Charles A. Williams, GNS Science
// Matthew G. Knepley, Rice University
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
 * @file libsrc/feassemble/IntegratorPointwise.hh
 *
 * @brief Object containing operations for implicit and explicit
 * time integration of the equations defined by pointwise functions.
 */

#if !defined(pylith_feassemble_integratorpointwise_hh)
#define pylith_feassemble_integratorpointwise_hh

// Include directives ---------------------------------------------------
#include "feassemblefwd.hh" // forward declarations

#include "Integrator.hh" // ISA Integrator

#include "pylith/topology/FieldBase.hh" // USES FieldBase

#include "pylith/topology/topologyfwd.hh" // HOLDSA Field

#include <map> // HOLDSA std::map

// IntegratorPointwise -------------------------------------------------
/** @brief General operations for implicit and explicit
 * time integration of equations defined by pointwise functions.
 */
class pylith::feassemble::IntegratorPointwise
{ // IntegratorPointwise
  friend class TestIntegratorPointwise; // unit testing

// PUBLIC TYPEDEFS //////////////////////////////////////////////////////
public :

// PUBLIC MEMBERS ///////////////////////////////////////////////////////
public :

  /// Constructor
  IntegratorPointwise(void);

  /// Destructor
  virtual
  ~IntegratorPointwise(void);

  /// Deallocate PETSc and local data structures.
  virtual
  void deallocate(void);

  /** Get auxiliary fields.
   *
   * @return field Field over material.
   */
  const topology::Field& auxFields(void) const;

  /** Check whether material has a given auxiliary field.
   *
   * @param[in] name Name of field.
   *
   * @returns True if material has auxiliary field, false otherwise.
   */
  bool hasAuxField(const char* name);

  /** Get auxiliary field.
   *
   * @param[in] field Field over material.
   * @param[in] name Name of field to retrieve.
   */
  void getAuxField(topology::Field *field,
		   const char* name) const;

  /** Set spatial database for auxiliary fields.
   *
   * @param[in] value Pointer to database.
   */
  void auxFieldsDB(spatialdata::spatialdb::SpatialDB* value);

  /** Set discretization information for auxiliary subfield.
   *
   * @param[in] name Name of auxiliary subfield.
   * @feInfo Discretization information for subfield.
   */
  void auxFieldDiscretization(const char* name,
			      const pylith::topology::FieldBase::DiscretizeInfo& feInfo);

  /** Get discretization information for auxiliary subfield.
   *
   * @param[in] name Name of subfield.
   * @return Discretization information for auxiliary subfield. If
   * discretization information was not set, then use "default".
   */
  const pylith::topology::FieldBase::DiscretizeInfo& auxFieldDiscretization(const char* name) const;

  /** Check whether Jacobian needs to be recomputed.
   *
   * @returns True if Jacobian needs to be recomputed, false otherwise.
   */
  virtual
  bool needNewJacobian(void) const;

  /** Check whether integrator generates a symmetric Jacobian.
   *
   * @returns True if integrator generates symmetric Jacobian.
   */
  virtual
  bool isJacobianSymmetric(void) const;

  /** Set manager of scales used to nondimensionalize problem.
   *
   * @param dim Nondimensionalizer.
   */
  void normalizer(const spatialdata::units::Nondimensional& dim);

  /** Verify configuration is acceptable.
   *
   * @param[in] mesh Finite-element mesh
   */
  virtual
  void verifyConfiguration(const topology::Mesh& mesh) const;
  
  /** Verify constraints are acceptable.
   *
   * @param field Solution field.
   */
  virtual
  void checkConstraints(const topology::Field& solution) const;

  /** Initialize integrator.
   *
   * @param[in] solution Solution field (layout).
   */
  virtual
  void initialize(const pylith::topology::Field& solution) = 0;

  /** Compute RHS residual for G(t,u).
   *
   * @param[out] residual Residual field.
   * @param[in] t Current time.
   * @param[in] dt Current time step.
   * @param[in] solution Current trial solution.
   */
  virtual
  void computeRHSResidual(pylith::topology::Field* residual,
			  const PylithReal t,
			  const PylithReal dt,
			  const pylith::topology::Field& solution) = 0;

  /** Compute RHS Jacobian and preconditioner for G(t,u).
   *
   * @param[out] jacobian Jacobian sparse matrix.
   * @param[out] preconditioner Jacobian preconditioning sparse matrix.
   * @param[in] t Current time.
   * @param[in] dt Current time step.
   * @param[in] solution Current trial solution.
   */
  virtual
  void computeRHSJacobian(pylith::topology::Jacobian* jacobian,
			  pylith::topology::Jacobian* preconditioner,
			  const PylithReal t,
			  const PylithReal dt,
			  const pylith::topology::Field& solution) = 0;

  /** Compute LHS residual for F(t,u,\dot{u}).
   *
   * @param[out] residual Residual field.
   * @param[in] t Current time.
   * @param[in] dt Current time step.
   * @param[in] solution Current trial solution.
   * @param[in] solutionDot Time derivative of current trial solution.
   */
  virtual
  void computeLHSResidual(pylith::topology::Field* residual,
			  const PylithReal t,
			  const PylithReal dt,
			  const pylith::topology::Field& solution,
			  const pylith::topology::Field& solutionDot) = 0;

  /** Compute LHS Jacobian and preconditioner for F(t,u,\dot{u}) with implicit time-stepping.
   *
   * @param[out] jacobian Jacobian sparse matrix.
   * @param[out] preconditioner Jacobian preconditioning sparse matrix.
   * @param[in] t Current time.
   * @param[in] dt Current time step.
   * @param[in] solution Current trial solution.
   * @param[in] solutionDot Time derivative of current trial solution.
   */
  virtual
  void computeLHSJacobianImplicit(pylith::topology::Jacobian* jacobian,
				  pylith::topology::Jacobian* preconditioner,
				  const PylithReal t,
				  const PylithReal dt,
				  const pylith::topology::Field& solution,
				  const pylith::topology::Field& solutionDot) = 0;


  /** Compute LHS Jacobian and preconditioner for F(t,u,\dot{u}) with explicit time-stepping.
   *
   * @param[out] jacobian Jacobian sparse matrix.
   * @param[out] preconditioner Jacobian preconditioning sparse matrix.
   * @param[in] t Current time.
   * @param[in] dt Current time step.
   * @param[in] solution Current trial solution.
   * @param[in] solutionDot Time derivative of current trial solution.
   */
  virtual
  void computeLHSJacobianExplicit(pylith::topology::Jacobian* jacobian,
				  pylith::topology::Jacobian* preconditioner,
				  const PylithReal t,
				  const PylithReal dt,
				  const pylith::topology::Field& solution,
				  const pylith::topology::Field& solutionDot) = 0;


  /** Update state variables as needed.
   *
   * @param[in] solution Solution field.
   */
  virtual
  void updateStateVars(const pylith::topology::Field& solution);

  // PROTECTED TYPEDEFS /////////////////////////////////////////////////
protected :

  typedef std::map<std::string, pylith::topology::FieldBase::DiscretizeInfo> discretizations_type;

// PROTECTED MEMBERS ////////////////////////////////////////////////////
protected :

  spatialdata::units::Nondimensional* _normalizer; ///< Nondimensionalizer.
  utils::EventLogger* _logger; ///< Event logger.

  /// Auxiliary fields for this problem
  topology::Field *_auxFields;

  /// Database of values for auxiliary fields.
  spatialdata::spatialdb::SpatialDB* _auxFieldsDB;

  /// Set auxiliary fields via query.
  pylith::topology::FieldQuery* _auxFieldsQuery;

  /// Map from auxiliary field to discretization.
  discretizations_type _auxFieldsFEInfo;

  /// True if we need to recompute Jacobian for operator, false otherwise.
  /// Default is false;
  bool _needNewJacobian;

  /// True if we need to compute velocity field, false otherwise.
  /// Default is false;
  bool _isJacobianSymmetric;

// NOT IMPLEMENTED //////////////////////////////////////////////////////
private :

  /// Not implemented.
  IntegratorPointwise(const IntegratorPointwise&);

  /// Not implemented
  const IntegratorPointwise& operator=(const IntegratorPointwise&);

}; // IntegratorPointwise

#endif // pylith_feassemble_integratorpointwise_hh


// End of file 
