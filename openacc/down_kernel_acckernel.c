//
// auto-generated by op2.py
//

//user function
#include <math.h>
#include "const.h"

//user function
//#pragma acc routine
inline void down_kernel_openacc( 
    double* variable,
    const double* residual,
    const double* coord,
    const double* residual_above,
    const double* coord_above) {
    double dx = fabs(coord[0] - coord_above[0]);
    double dy = fabs(coord[1] - coord_above[1]);
    double dz = fabs(coord[2] - coord_above[2]);
    double dm = sqrt(dx*dx + dy*dy + dz*dz);

    variable[VAR_DENSITY]        -= dm* (residual_above[VAR_DENSITY]        - residual[VAR_DENSITY]);
    variable[VAR_MOMENTUM+0]     -= dx* (residual_above[VAR_MOMENTUM+0]     - residual[VAR_MOMENTUM+0]);
    variable[VAR_MOMENTUM+1]     -= dy* (residual_above[VAR_MOMENTUM+1]     - residual[VAR_MOMENTUM+1]);
    variable[VAR_MOMENTUM+2]     -= dz* (residual_above[VAR_MOMENTUM+2]     - residual[VAR_MOMENTUM+2]);
    variable[VAR_DENSITY_ENERGY] -= dm* (residual_above[VAR_DENSITY_ENERGY] - residual[VAR_DENSITY_ENERGY]);
}

// host stub function
void op_par_loop_down_kernel(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2,
  op_arg arg3,
  op_arg arg4){

  int nargs = 5;
  op_arg args[5];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  args[3] = arg3;
  args[4] = arg4;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(21);
  op_timers_core(&cpu_t1, &wall_t1);
  OP_kernels[21].name      = name;
  OP_kernels[21].count    += 1;

  int  ninds   = 2;
  int  inds[5] = {-1,-1,-1,0,1};

  if (OP_diags>2) {
    printf(" kernel routine with indirection: down_kernel\n");
  }

  // get plan
  #ifdef OP_PART_SIZE_21
    int part_size = OP_PART_SIZE_21;
  #else
    int part_size = OP_part_size;
  #endif

  int set_size = op_mpi_halo_exchanges_cuda(set, nargs, args);


  int ncolors = 0;

  if (set_size >0) {


    //Set up typed device pointers for OpenACC
    int *map3 = arg3.map_data_d;

    double* data0 = (double*)arg0.data_d;
    double* data1 = (double*)arg1.data_d;
    double* data2 = (double*)arg2.data_d;
    double *data3 = (double *)arg3.data_d;
    double *data4 = (double *)arg4.data_d;

    op_plan *Plan = op_plan_get_stage(name,set,part_size,nargs,args,ninds,inds,OP_COLOR2);
    ncolors = Plan->ncolors;
    int *col_reord = Plan->col_reord;
    int set_size1 = set->size + set->exec_size;

    // execute plan
    for ( int col=0; col<Plan->ncolors; col++ ){
      if (col==1) {
        op_mpi_wait_all_cuda(nargs, args);
      }
      int start = Plan->col_offsets[0][col];
      int end = Plan->col_offsets[0][col+1];

      #pragma acc parallel loop independent deviceptr(col_reord,map3,data0,data1,data2,data3,data4)
      for ( int e=start; e<end; e++ ){
        int n = col_reord[e];
        int map3idx = map3[n + set_size1 * 0];


        down_kernel_openacc(
          &data0[5 * n],
          &data1[5 * n],
          &data2[3 * n],
          &data3[5 * map3idx],
          &data4[3 * map3idx]);
      }

    }
    OP_kernels[21].transfer  += Plan->transfer;
    OP_kernels[21].transfer2 += Plan->transfer2;
  }

  if (set_size == 0 || set_size == set->core_size || ncolors == 1) {
    op_mpi_wait_all_cuda(nargs, args);
  }
  // combine reduction data
  op_mpi_set_dirtybit_cuda(nargs, args);

  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[21].time     += wall_t2 - wall_t1;
}
