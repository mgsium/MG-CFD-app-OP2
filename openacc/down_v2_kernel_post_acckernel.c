//
// auto-generated by op2.py
//

//user function
#include ".././src/Kernels/mg.h"
//user function
//#pragma acc routine
inline void down_v2_kernel_post_openacc( 
    const double* residuals1_prolonged,
    const double* residuals1_prolonged_wsum,
    const double* residuals2,
    double* variables2) {


    for (int i=0; i<NVAR; i++) {
        variables2[i] += residuals2[i] - (residuals1_prolonged[i] / (*residuals1_prolonged_wsum));
    }
}

// host stub function
void op_par_loop_down_v2_kernel_post(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2,
  op_arg arg3){

  int nargs = 4;
  op_arg args[4];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  args[3] = arg3;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(20);
  op_timers_core(&cpu_t1, &wall_t1);
  OP_kernels[20].name      = name;
  OP_kernels[20].count    += 1;


  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  down_v2_kernel_post");
  }

  op_mpi_halo_exchanges_cuda(set, nargs, args);


  if (set->size >0) {


    //Set up typed device pointers for OpenACC

    double* data0 = (double*)arg0.data_d;
    double* data1 = (double*)arg1.data_d;
    double* data2 = (double*)arg2.data_d;
    double* data3 = (double*)arg3.data_d;
    #pragma acc parallel loop independent deviceptr(data0,data1,data2,data3)
    for ( int n=0; n<set->size; n++ ){
      down_v2_kernel_post_openacc(
        &data0[5*n],
        &data1[1*n],
        &data2[5*n],
        &data3[5*n]);
    }
  }

  // combine reduction data
  op_mpi_set_dirtybit_cuda(nargs, args);

  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[20].time     += wall_t2 - wall_t1;
  OP_kernels[20].transfer += (float)set->size * arg0.size;
  OP_kernels[20].transfer += (float)set->size * arg1.size;
  OP_kernels[20].transfer += (float)set->size * arg2.size;
  OP_kernels[20].transfer += (float)set->size * arg3.size * 2.0f;
}
