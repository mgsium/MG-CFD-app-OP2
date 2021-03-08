//
// auto-generated by op2.py
//

//user function
#include <math.h>
#include <cmath>
#include "const.h"
#include "inlined_funcs.h"

int direct_time_step_kernel_stride_OP2CONSTANT;
int direct_time_step_kernel_stride_OP2HOST=-1;
//user function
//#pragma acc routine
inline void time_step_kernel_openacc( 
    const int* rkCycle,
    const double* step_factor,
    double* flux,
    const double* old_variable,
    double* variable) {
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

// host stub function
void op_par_loop_time_step_kernel(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2,
  op_arg arg3,
  op_arg arg4){

  int*arg0h = (int *)arg0.data;
  int nargs = 5;
  op_arg args[5];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  args[3] = arg3;
  args[4] = arg4;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(11);
  op_timers_core(&cpu_t1, &wall_t1);
  OP_kernels[11].name      = name;
  OP_kernels[11].count    += 1;


  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  time_step_kernel");
  }

  int set_size = op_mpi_halo_exchanges_cuda(set, nargs, args);

  int arg0_l = arg0h[0];

  if (set_size >0) {

    if ((OP_kernels[11].count==1) || (direct_time_step_kernel_stride_OP2HOST != getSetSizeFromOpArg(&arg2))) {
      direct_time_step_kernel_stride_OP2HOST = getSetSizeFromOpArg(&arg2);
      direct_time_step_kernel_stride_OP2CONSTANT = direct_time_step_kernel_stride_OP2HOST;
    }

    //Set up typed device pointers for OpenACC

    double* data1 = (double*)arg1.data_d;
    double* data2 = (double*)arg2.data_d;
    double* data3 = (double*)arg3.data_d;
    double* data4 = (double*)arg4.data_d;
    #pragma acc parallel loop independent deviceptr(data1,data2,data3,data4)
    for ( int n=0; n<set->size; n++ ){
      time_step_kernel_openacc(
        &arg0_l,
        &data1[1*n],
        &data2[n],
        &data3[n],
        &data4[n]);
    }
  }

  // combine reduction data
  op_mpi_set_dirtybit_cuda(nargs, args);

  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[11].time     += wall_t2 - wall_t1;
  OP_kernels[11].transfer += (float)set->size * arg1.size;
  OP_kernels[11].transfer += (float)set->size * arg2.size * 2.0f;
  OP_kernels[11].transfer += (float)set->size * arg3.size;
  OP_kernels[11].transfer += (float)set->size * arg4.size * 2.0f;
}
