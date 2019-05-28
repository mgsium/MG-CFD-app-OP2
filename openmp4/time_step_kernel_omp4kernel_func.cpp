//
// auto-generated by op2.py
//

#include ".././src/Kernels/time_stepping_kernels.h"

void time_step_kernel_omp4_kernel(
  int *arg0,
  double *data1,
  int dat1size,
  double *data2,
  int dat2size,
  double *data3,
  int dat3size,
  double *data4,
  int dat4size,
  int count,
  int num_teams,
  int nthread){

  int arg0_l = *arg0;
  #pragma omp target teams num_teams(num_teams) thread_limit(nthread) map(to:data1[0:dat1size],data2[0:dat2size],data3[0:dat3size],data4[0:dat4size])
  #pragma omp distribute parallel for schedule(static,1)
  for ( int n_op=0; n_op<count; n_op++ ){
    //variable mapping
    const int* rkCycle = &arg0_l;
    const double* step_factor = &data1[1*n_op];
    double* flux = &data2[5*n_op];
    const double* old_variable = &data3[5*n_op];
    double* variable = &data4[5*n_op];

    //inline function
    
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
    //end inline func
  }

  *arg0 = arg0_l;
}
