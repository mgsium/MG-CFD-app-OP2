//
// auto-generated by op2.py
//

//user function
#include <cmath>
#include "const.h"
#include "structures.h"
#include "global.h"

//user function
//#pragma acc routine
inline void initialize_variables_kernel_openacc( 
    double* variables) {
    for(int j = 0; j < NVAR; j++) {
        variables[j] = ff_variable[j];
    }
}

// host stub function
void op_par_loop_initialize_variables_kernel(char const *name, op_set set,
  op_arg arg0){

  int nargs = 1;
  op_arg args[1];

  args[0] = arg0;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(0);
  op_timers_core(&cpu_t1, &wall_t1);
  OP_kernels[0].name      = name;
  OP_kernels[0].count    += 1;


  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  initialize_variables_kernel");
  }

  int set_size = op_mpi_halo_exchanges_cuda(set, nargs, args);


  if (set_size >0) {


    //Set up typed device pointers for OpenACC

    double* data0 = (double*)arg0.data_d;
    #pragma acc parallel loop independent deviceptr(data0)
    for ( int n=0; n<set->size; n++ ){
      initialize_variables_kernel_openacc(
        &data0[5*n]);
    }
  }

  // combine reduction data
  op_mpi_set_dirtybit_cuda(nargs, args);

  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[0].time     += wall_t2 - wall_t1;
  OP_kernels[0].transfer += (float)set->size * arg0.size * 2.0f;
}
