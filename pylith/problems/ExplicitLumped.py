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

## @file pylith/problems/ExplicitLumped.py
##
## @brief Python ExplicitLumped object for solving equations using an
## explicit formulation with a lumped Jacobian matrix that is stored
## as a Field.
##
## Factory: pde_formulation

from Formulation import Formulation
from pylith.utils.profiling import resourceUsageString

# ExplicitLumped class
class ExplicitLumped(Formulation):
  """
  Python ExplicitLumped object for solving equations using an explicit
  formulation.

  The formulation has the general form, [A(t)] {u(t+dt)} = {b(t)},
  where we want to solve for {u(t+dt)}, A(t) is usually constant
  (i.e., independent of time), and {b(t)} usually depends on {u(t)}
  and {u(t-dt)}.

  Jacobian: A(t)
  solution: u(t+dt)
  residual: b(t) - A(t) \hat u(t+dt)
  constant: b(t)

  Factory: pde_formulation.
  """

  # INVENTORY //////////////////////////////////////////////////////////

  class Inventory(Formulation.Inventory):
    """
    Python object for managing ExplicitLumped facilities and properties.

    Provide appropriate solver for lumped Jacobian as the default.
    """

    ## @class Inventory
    ## Python object for managing ExplicitLumped facilities and properties.
    ##
    ## \b Properties
    ## @li None
    ##
    ## \b Facilities
    ## @li \b solver Algebraic solver.

    import pyre.inventory

    from SolverLumped import SolverLumped
    solver = pyre.inventory.facility("solver", family="solver",
                                     factory=SolverLumped)
    solver.meta['tip'] = "Algebraic solver."

  # PUBLIC METHODS /////////////////////////////////////////////////////

  def __init__(self, name="explicit"):
    """
    Constructor.
    """
    Formulation.__init__(self, name)
    self._loggingPrefix = "TSEx "
    return


  def elasticityIntegrator(self):
    """
    Get integrator for elastic material.
    """
    from pylith.feassemble.ElasticityExplicit import ElasticityExplicit
    return ElasticityExplicit()


  def initialize(self, dimension, normalizer):
    """
    Initialize problem for explicit time integration.
    """
    logEvent = "%sinit" % self._loggingPrefix
    self._eventLogger.eventBegin(logEvent)
    
    Formulation.initialize(self, dimension, normalizer)

    from pylith.utils.petsc import MemoryLogger
    logger = MemoryLogger.singleton()
    logger.setDebug(0)
    logger.stagePush("Problem")

    # Allocate other fields, reusing layout from dispIncr
    self._info.log("Creating other fields.")
    self.fields.add("disp(t-dt)", "displacement")
    self.fields.copyLayout("dispIncr(t->t+dt)")
    self._debug.log(resourceUsageString())

    # Setup fields and set to zero
    dispTmdt = self.fields.get("disp(t-dt)")
    dispTmdt.zero()
    dispT = self.fields.get("disp(t)")
    dispT.zero()
    residual = self.fields.get("residual")
    residual.zero()
    residual.createVector()
    self._debug.log(resourceUsageString())
    logger.stagePop()

    self._info.log("Creating lumped Jacobian matrix.")
    from pylith.topology.topology import MeshField
    jacobian = MeshField(self.mesh)
    jacobian.label("jacobian")
    jacobian.vectorFieldType(jacobian.VECTOR)
    jacobian.newSection(jacobian.VERTICES_FIELD, dimension)
    jacobian.allocate()
    self.jacobian = jacobian
    self._debug.log(resourceUsageString())

    logger.stagePush("Problem")
    self._info.log("Initializing solver.")
    self.solver.initialize(self.fields, self.jacobian, self)
    self._debug.log(resourceUsageString())

    # Solve for increment in displacement field.
    for constraint in self.constraints:
      constraint.useSolnIncr(True)
    for integrator in self.integratorsMesh + self.integratorsSubMesh:
      integrator.useSolnIncr(True)

    logger.stagePop()
    logger.setDebug(0)
    self._eventLogger.eventEnd(logEvent)
    return


  def prestep(self, t, dt):
    """
    Hook for doing stuff before advancing time step.
    """
    logEvent = "%sprestep" % self._loggingPrefix
    self._eventLogger.eventBegin(logEvent)
    
    dispIncr = self.fields.get("dispIncr(t->t+dt)")
    for constraint in self.constraints:
      constraint.setFieldIncr(t, t+dt, dispIncr)

    needNewJacobian = False
    for integrator in self.integratorsMesh + self.integratorsSubMesh:
      integrator.timeStep(dt)
      if integrator.needNewJacobian():
        needNewJacobian = True
    if needNewJacobian:
      self._reformJacobian(t, dt)

    self._eventLogger.eventEnd(logEvent)
    return


  def step(self, t, dt):
    """
    Advance to next time step.
    """
    logEvent = "%sstep" % self._loggingPrefix
    self._eventLogger.eventBegin(logEvent)

    self._reformResidual(t, dt)
    
    self._info.log("Solving equations.")
    residual = self.fields.get("residual")
    dispIncr = self.fields.get("dispIncr(t->t+dt)")
    self.solver.solve(dispIncr, self.jacobian, residual)

    self._eventLogger.eventEnd(logEvent)
    return


  def poststep(self, t, dt):
    """
    Hook for doing stuff after advancing time step.
    """
    logEvent = "%spoststep" % self._loggingPrefix
    self._eventLogger.eventBegin(logEvent)
    
    dispIncr = self.fields.get("dispIncr(t->t+dt)")
    dispT = self.fields.get("disp(t)")
    dispTmdt = self.fields.get("disp(t-dt)")

    dispTmdt.copy(dispT)
    dispT += dispIncr
    dispIncr.zero()

    Formulation.poststep(self, t, dt)

    self._eventLogger.eventEnd(logEvent)    
    return


  # PRIVATE METHODS ////////////////////////////////////////////////////

  def _configure(self):
    """
    Set members based using inventory.
    """
    Formulation._configure(self)
    self.solver = self.inventory.solver
    return


  def _reformJacobian(self, t, dt):
    """
    Reform Jacobian matrix for operator.
    """
    self._debug.log(resourceUsageString())
    self._info.log("Integrating Jacobian operator.")
    self._eventLogger.stagePush("Reform Jacobian")

    self.updateSettings(self.jacobian, self.fields, t, dt)
    self.reformJacobianLumped()

    self._eventLogger.stagePop()

    if self.viewJacobian:
      self.jacobian.view("Lumped Jacobian")

    self._debug.log(resourceUsageString())
    return


  def _cleanup(self):
    """
    Deallocate PETSc and local data structures.
    """
    if not self.fields is None:
      self.fields.cleanup()
    return


# FACTORIES ////////////////////////////////////////////////////////////

def pde_formulation():
  """
  Factory associated with ExplicitLumped.
  """
  return ExplicitLumped()


# End of file 