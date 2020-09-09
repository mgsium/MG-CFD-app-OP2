//
// auto-generated by op2.py
//

//user function
#include <math.h>
#include "const.h"

//user function
//#pragma acc routine
inline void down_v2_kernel_openacc( 
    const double* coord2a,
    const double* coord2b,
    const double* coord1a,
    const double* coord1b,
    const double* residuals1a,
    const double* residuals1b,
    double* residuals1a_prolonged,
    double* residuals1b_prolonged,
    double* residuals1a_prolonged_wsum,
    double* residuals1b_prolonged_wsum) {







    double dx_a1a2 = coord2a[0] - coord1a[0];
    double dy_a1a2 = coord2a[1] - coord1a[1];
    double dz_a1a2 = coord2a[2] - coord1a[2];
    if (dx_a1a2 == 0.0 && dy_a1a2 == 0.0 && dz_a1a2 == 0.0) {

        residuals1a_prolonged[VAR_DENSITY]        = residuals1a[VAR_DENSITY];
        residuals1a_prolonged[VAR_MOMENTUM+0]     = residuals1a[VAR_MOMENTUM+0];
        residuals1a_prolonged[VAR_MOMENTUM+1]     = residuals1a[VAR_MOMENTUM+1];
        residuals1a_prolonged[VAR_MOMENTUM+2]     = residuals1a[VAR_MOMENTUM+2];
        residuals1a_prolonged[VAR_DENSITY_ENERGY] = residuals1a[VAR_DENSITY_ENERGY];
        *residuals1a_prolonged_wsum = 1.0;
    } else {

        const double idist_a1a2 = 1.0/sqrt(dx_a1a2*dx_a1a2 + dy_a1a2*dy_a1a2 + dz_a1a2*dz_a1a2);
        residuals1a_prolonged[VAR_DENSITY]        += idist_a1a2*residuals1a[VAR_DENSITY];
        residuals1a_prolonged[VAR_MOMENTUM+0]     += idist_a1a2*residuals1a[VAR_MOMENTUM+0];
        residuals1a_prolonged[VAR_MOMENTUM+1]     += idist_a1a2*residuals1a[VAR_MOMENTUM+1];
        residuals1a_prolonged[VAR_MOMENTUM+2]     += idist_a1a2*residuals1a[VAR_MOMENTUM+2];
        residuals1a_prolonged[VAR_DENSITY_ENERGY] += idist_a1a2*residuals1a[VAR_DENSITY_ENERGY];
        *residuals1a_prolonged_wsum += idist_a1a2;

        double dx_b1a2 = coord1b[0] - coord2a[0];
        double dy_b1a2 = coord1b[1] - coord2a[1];
        double dz_b1a2 = coord1b[2] - coord2a[2];

        const double idist_b1a2 = 1.0/sqrt(dx_b1a2*dx_b1a2 + dy_b1a2*dy_b1a2 + dz_b1a2*dz_b1a2);
        residuals1a_prolonged[VAR_DENSITY]        += idist_b1a2*residuals1b[VAR_DENSITY];
        residuals1a_prolonged[VAR_MOMENTUM+0]     += idist_b1a2*residuals1b[VAR_MOMENTUM+0];
        residuals1a_prolonged[VAR_MOMENTUM+1]     += idist_b1a2*residuals1b[VAR_MOMENTUM+1];
        residuals1a_prolonged[VAR_MOMENTUM+2]     += idist_b1a2*residuals1b[VAR_MOMENTUM+2];
        residuals1a_prolonged[VAR_DENSITY_ENERGY] += idist_b1a2*residuals1b[VAR_DENSITY_ENERGY];
        *residuals1a_prolonged_wsum += idist_b1a2;
    }

    double dx_b1b2 = coord2b[0] - coord1b[0];
    double dy_b1b2 = coord2b[1] - coord1b[1];
    double dz_b1b2 = coord2b[2] - coord1b[2];
    if (dx_b1b2 == 0.0 && dy_b1b2 == 0.0 && dz_b1b2 == 0.0) {

        residuals1b_prolonged[VAR_DENSITY]        = residuals1b[VAR_DENSITY];
        residuals1b_prolonged[VAR_MOMENTUM+0]      = residuals1b[VAR_MOMENTUM+0];
        residuals1b_prolonged[VAR_MOMENTUM+1]      = residuals1b[VAR_MOMENTUM+1];
        residuals1b_prolonged[VAR_MOMENTUM+2]      = residuals1b[VAR_MOMENTUM+2];
        residuals1b_prolonged[VAR_DENSITY_ENERGY] = residuals1b[VAR_DENSITY_ENERGY];
        *residuals1b_prolonged_wsum = 1.0;
    } else {

        const double idist_b1b2 = 1.0/sqrt(dx_b1b2*dx_b1b2 + dy_b1b2*dy_b1b2 + dz_b1b2*dz_b1b2);
        residuals1b_prolonged[VAR_DENSITY]        += idist_b1b2*residuals1b[VAR_DENSITY];
        residuals1b_prolonged[VAR_MOMENTUM+0]     += idist_b1b2*residuals1b[VAR_MOMENTUM+0];
        residuals1b_prolonged[VAR_MOMENTUM+1]     += idist_b1b2*residuals1b[VAR_MOMENTUM+1];
        residuals1b_prolonged[VAR_MOMENTUM+2]     += idist_b1b2*residuals1b[VAR_MOMENTUM+2];
        residuals1b_prolonged[VAR_DENSITY_ENERGY] += idist_b1b2*residuals1b[VAR_DENSITY_ENERGY];
        *residuals1b_prolonged_wsum += idist_b1b2;

        double dx_a1b2 = coord1a[0] - coord2b[0];
        double dy_a1b2 = coord1a[1] - coord2b[1];
        double dz_a1b2 = coord1a[2] - coord2b[2];

        const double idist_a1b2 = 1.0/sqrt(dx_a1b2*dx_a1b2 + dy_a1b2*dy_a1b2 + dz_a1b2*dz_a1b2);
        residuals1b_prolonged[VAR_DENSITY]        += idist_a1b2*residuals1b[VAR_DENSITY];
        residuals1b_prolonged[VAR_MOMENTUM+0]     += idist_a1b2*residuals1b[VAR_MOMENTUM+0];
        residuals1b_prolonged[VAR_MOMENTUM+1]     += idist_a1b2*residuals1b[VAR_MOMENTUM+1];
        residuals1b_prolonged[VAR_MOMENTUM+2]     += idist_a1b2*residuals1b[VAR_MOMENTUM+2];
        residuals1b_prolonged[VAR_DENSITY_ENERGY] += idist_a1b2*residuals1b[VAR_DENSITY_ENERGY];
        *residuals1b_prolonged_wsum += idist_a1b2;
    }
}

// host stub function
void op_par_loop_down_v2_kernel(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2,
  op_arg arg3,
  op_arg arg4,
  op_arg arg5,
  op_arg arg6,
  op_arg arg7,
  op_arg arg8,
  op_arg arg9){

  int nargs = 10;
  op_arg args[10];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  args[3] = arg3;
  args[4] = arg4;
  args[5] = arg5;
  args[6] = arg6;
  args[7] = arg7;
  args[8] = arg8;
  args[9] = arg9;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(19);
  op_timers_core(&cpu_t1, &wall_t1);
  OP_kernels[19].name      = name;
  OP_kernels[19].count    += 1;

  int  ninds   = 5;
  int  inds[10] = {0,0,1,1,2,2,3,3,4,4};

  if (OP_diags>2) {
    printf(" kernel routine with indirection: down_v2_kernel\n");
  }

  // get plan
  #ifdef OP_PART_SIZE_19
    int part_size = OP_PART_SIZE_19;
  #else
    int part_size = OP_part_size;
  #endif

  int set_size = op_mpi_halo_exchanges_cuda(set, nargs, args);


  int ncolors = 0;

  if (set_size >0) {


    //Set up typed device pointers for OpenACC
    int *map0 = arg0.map_data_d;
    int *map2 = arg2.map_data_d;

    double *data0 = (double *)arg0.data_d;
    double *data2 = (double *)arg2.data_d;
    double *data4 = (double *)arg4.data_d;
    double *data6 = (double *)arg6.data_d;
    double *data8 = (double *)arg8.data_d;

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

      #pragma acc parallel loop independent deviceptr(col_reord,map0,map2,data0,data2,data4,data6,data8)
      for ( int e=start; e<end; e++ ){
        int n = col_reord[e];
        int map0idx = map0[n + set_size1 * 0];
        int map1idx = map0[n + set_size1 * 1];
        int map2idx = map2[n + set_size1 * 0];
        int map3idx = map2[n + set_size1 * 1];


        down_v2_kernel_openacc(
          &data0[3 * map0idx],
          &data0[3 * map1idx],
          &data2[3 * map2idx],
          &data2[3 * map3idx],
          &data4[5 * map2idx],
          &data4[5 * map3idx],
          &data6[5 * map0idx],
          &data6[5 * map1idx],
          &data8[1 * map0idx],
          &data8[1 * map1idx]);
      }

    }
    OP_kernels[19].transfer  += Plan->transfer;
    OP_kernels[19].transfer2 += Plan->transfer2;
  }

  if (set_size == 0 || set_size == set->core_size || ncolors == 1) {
    op_mpi_wait_all_cuda(nargs, args);
  }
  // combine reduction data
  op_mpi_set_dirtybit_cuda(nargs, args);

  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[19].time     += wall_t2 - wall_t1;
}
