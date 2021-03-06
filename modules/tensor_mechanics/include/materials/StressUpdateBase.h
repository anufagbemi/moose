/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef STRESSUPDATEBASE_H
#define STRESSUPDATEBASE_H

#include "Conversion.h"
#include "InputParameters.h"
#include "Material.h"
#include "RankFourTensor.h"
#include "RankTwoTensor.h"

// Forward declaration
class StressUpdateBase;

template <>
InputParameters validParams<StressUpdateBase>();

/**
 * StressUpdateBase is a material that is not called by MOOSE because
 * of the compute=false flag set in the parameter list.  This class is a base class
 * for materials that perform some internal computational
 * procedure (such as an iterative return-mapping procedure) to compute an
 * admissible state (which is usually an admissible stress that lies on or
 * within the yield surface, as well as a set of internal parameters such as
 * plastic strains).  The computational procedure must return the admissible stress
 * and a decomposition of the applied strain into elastic and inelastic components.
 * All materials inheriting from this class must be called by a separate material,
 * such as ComputeMultipleInelasticStress
 */
class StressUpdateBase : public Material
{
public:
  StressUpdateBase(const InputParameters & parameters);

  /**
   * Given a strain increment that results in a trial stress, perform some
   * procedure (such as an iterative return-mapping process) to produce
   * an admissible stress, an elastic strain increment and an inelastic
   * strain increment, as well as d(stress)/d(strain) (or some approximation
   * to it).
   *
   * This method is called by ComputeMultipleInelasticStress.
   * This method is pure virutal: all inheriting classes must overwrite this method.
   *
   * @param strain_increment Upon input: the strain increment.  Upon output: the elastic strain
   * increment
   * @param inelastic_strain_increment The inelastic_strain resulting from the interative procedure
   * @param rotation_increment The finite-strain rotation increment
   * @param stress_new Upon input: the trial stress that results from applying strain_increment as
   * an elastic strain.  Upon output: the admissible stress
   * @param stress_old The old value of stress
   * @param elasticity_tensor The elasticity tensor
   * @param compute_full_tangent_operator The calling routine would like the full consistent tangent
   * operator to be placed in tangent_operator, if possible.
   * @param tangent_operator d(stress)/d(strain), or some approximation to it  If
   * compute_full_tangent_operator=false, then tangent_operator=elasticity_tensor is an appropriate
   * choice
   */
  virtual void updateState(RankTwoTensor & strain_increment,
                           RankTwoTensor & inelastic_strain_increment,
                           const RankTwoTensor & rotation_increment,
                           RankTwoTensor & stress_new,
                           const RankTwoTensor & stress_old,
                           const RankFourTensor & elasticity_tensor,
                           const RankTwoTensor & elastic_strain_old,
                           bool compute_full_tangent_operator,
                           RankFourTensor & tangent_operator) = 0;

  /// Sets the value of the global variable _qp for inheriting classes
  void setQp(unsigned int qp);

  ///@{ Retained as empty methods to avoid a warning from Material.C in framework. These methods are unused in all inheriting classes and should not be overwritten.
  void resetQpProperties() final {}
  void resetProperties() final {}
  ///@}
};

#endif // STRESSUPDATEBASE_H
