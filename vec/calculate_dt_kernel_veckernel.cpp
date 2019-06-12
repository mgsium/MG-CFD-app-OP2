//
// auto-generated by op2.py
//

//user function
// Copyright 2009, Andrew Corrigan, acorriga@gmu.edu
// This code is from the AIAA-2009-4001 paper

#ifndef COMPUTE_STEP_FACTOR_H
#define COMPUTE_STEP_FACTOR_H

#include <math.h>
#include <cmath>

#include "const.h"
#include "inlined_funcs.h"

inline void calculate_dt_kernel(
    const double* variable, 
    const double* volume, 
    double* dt)
{
    double density = variable[VAR_DENSITY];

    double3 momentum;
    momentum.x = variable[VAR_MOMENTUM+0];
    momentum.y = variable[VAR_MOMENTUM+1];
    momentum.z = variable[VAR_MOMENTUM+2];

    double density_energy = variable[VAR_DENSITY_ENERGY];
    double3 velocity; compute_velocity(density, momentum, velocity);
    double speed_sqd      = compute_speed_sqd(velocity);
    double pressure       = compute_pressure(density, density_energy, speed_sqd);
    double speed_of_sound = compute_speed_of_sound(density, pressure);

    *dt = double(0.5) * (cbrt(*volume) / (sqrt(speed_sqd) + speed_of_sound));
}

inline void get_min_dt_kernel(
    const double* dt, 
    double* min_dt)
{
    if ((*dt) < (*min_dt)) {
        *min_dt = (*dt);
    }
}

inline void compute_step_factor_kernel(
    const double* variable, 
    const double* volume, 
    const double* min_dt, 
    double* step_factor)
{
    double density = variable[VAR_DENSITY];

    double3 momentum;
    momentum.x = variable[VAR_MOMENTUM+0];
    momentum.y = variable[VAR_MOMENTUM+1];
    momentum.z = variable[VAR_MOMENTUM+2];

    double density_energy = variable[VAR_DENSITY_ENERGY];
    double3 velocity; compute_velocity(density, momentum, velocity);
    double speed_sqd      = compute_speed_sqd(velocity);
    double pressure       = compute_pressure(density, density_energy, speed_sqd);
    double speed_of_sound = compute_speed_of_sound(density, pressure);

    // Bring forward a future division-by-volume:
    *step_factor = (*min_dt) / (*volume);
}

inline void time_step_kernel(
    const int* rkCycle,
    const double* step_factor,
    double* flux,
    const double* old_variable,
    double* variable)
{
    double factor = (*step_factor)/double(RK+1-(*rkCycle));

    variable[VAR_DENSITY]        = old_variable[VAR_DENSITY]        + factor*flux[VAR_DENSITY];
    variable[VAR_MOMENTUM+0]     = old_variable[VAR_MOMENTUM+0]     + factor*flux[VAR_MOMENTUM+0];
    variable[VAR_MOMENTUM+1]     = old_variable[VAR_MOMENTUM+1]     + factor*flux[VAR_MOMENTUM+1];
    variable[VAR_MOMENTUM+2]     = old_variable[VAR_MOMENTUM+2]     + factor*flux[VAR_MOMENTUM+2];
    variable[VAR_DENSITY_ENERGY] = old_variable[VAR_DENSITY_ENERGY] + factor*flux[VAR_DENSITY_ENERGY];

    flux[VAR_DENSITY]        = 0.0;
    flux[VAR_MOMENTUM+0]     = 0.0;
    flux[VAR_MOMENTUM+1]     = 0.0;
    flux[VAR_MOMENTUM+2]     = 0.0;
    flux[VAR_DENSITY_ENERGY] = 0.0;
}

#endif

// host stub function
void op_par_loop_calculate_dt_kernel(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2){

  int nargs = 3;
  op_arg args[3];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  //create aligned pointers for dats
  ALIGNED_double const double * __restrict__ ptr0 = (double *) arg0.data;
  __assume_aligned(ptr0,double_ALIGN);
  ALIGNED_double const double * __restrict__ ptr1 = (double *) arg1.data;
  __assume_aligned(ptr1,double_ALIGN);
  ALIGNED_double       double * __restrict__ ptr2 = (double *) arg2.data;
  __assume_aligned(ptr2,double_ALIGN);

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(6);
  op_timers_core(&cpu_t1, &wall_t1);


  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  calculate_dt_kernel");
  }

  int exec_size = op_mpi_halo_exchanges(set, nargs, args);

  if (exec_size >0) {

    #ifdef VECTORIZE
    #pragma novector
    for ( int n=0; n<(exec_size/SIMD_VEC)*SIMD_VEC; n+=SIMD_VEC ){
      #pragma omp simd simdlen(SIMD_VEC)
      for ( int i=0; i<SIMD_VEC; i++ ){
        calculate_dt_kernel(
          &(ptr0)[5 * (n+i)],
          &(ptr1)[1 * (n+i)],
          &(ptr2)[1 * (n+i)]);
      }
    }
    //remainder
    for ( int n=(exec_size/SIMD_VEC)*SIMD_VEC; n<exec_size; n++ ){
    #else
    for ( int n=0; n<exec_size; n++ ){
    #endif
      calculate_dt_kernel(
        &(ptr0)[5*n],
        &(ptr1)[1*n],
        &(ptr2)[1*n]);
    }
  }

  // combine reduction data
  op_mpi_set_dirtybit(nargs, args);

  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[6].name      = name;
  OP_kernels[6].count    += 1;
  OP_kernels[6].time     += wall_t2 - wall_t1;
  OP_kernels[6].transfer += (float)set->size * arg0.size;
  OP_kernels[6].transfer += (float)set->size * arg1.size;
  OP_kernels[6].transfer += (float)set->size * arg2.size * 2.0f;
}
