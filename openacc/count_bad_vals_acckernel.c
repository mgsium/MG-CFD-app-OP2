//
// auto-generated by op2.py
//

//user function
#include "utils.h"

int direct_count_bad_vals_stride_OP2CONSTANT;
int direct_count_bad_vals_stride_OP2HOST=-1;
//user function
//#pragma acc routine
inline void count_bad_vals_openacc( 
    const double* value,
    int* count) {
    #if defined(OPENACC) || defined(__HIPSYCL__) || defined(TRISYCL_CL_LANGUAGE_VERSION)

    #else
        for (int v=0; v<NVAR; v++) {
            if (isnan(value[(v)*direct_count_bad_vals_stride_OP2CONSTANT]) || isinf(value[(v)*direct_count_bad_vals_stride_OP2CONSTANT])) {
                *count += 1;
            }
        }
    #endif
}

// host stub function
void op_par_loop_count_bad_vals(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1){

  int*arg1h = (int *)arg1.data;
  int nargs = 2;
  op_arg args[2];

  args[0] = arg0;
  args[1] = arg1;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(15);
  op_timers_core(&cpu_t1, &wall_t1);
  OP_kernels[15].name      = name;
  OP_kernels[15].count    += 1;


  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  count_bad_vals");
  }

  int set_size = op_mpi_halo_exchanges_cuda(set, nargs, args);

  int arg1_l = arg1h[0];

  if (set_size >0) {

    if ((OP_kernels[15].count==1) || (direct_count_bad_vals_stride_OP2HOST != getSetSizeFromOpArg(&arg0))) {
      direct_count_bad_vals_stride_OP2HOST = getSetSizeFromOpArg(&arg0);
      direct_count_bad_vals_stride_OP2CONSTANT = direct_count_bad_vals_stride_OP2HOST;
    }

    //Set up typed device pointers for OpenACC

    double* data0 = (double*)arg0.data_d;
    #pragma acc parallel loop independent deviceptr(data0) reduction(+:arg1_l)
    for ( int n=0; n<set->size; n++ ){
      count_bad_vals_openacc(
        &data0[n],
        &arg1_l);
    }
  }

  // combine reduction data
  arg1h[0] = arg1_l;
  op_mpi_reduce_int(&arg1,arg1h);
  op_mpi_set_dirtybit_cuda(nargs, args);

  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[15].time     += wall_t2 - wall_t1;
  OP_kernels[15].transfer += (float)set->size * arg0.size;
}
