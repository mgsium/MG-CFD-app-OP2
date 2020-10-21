//
// auto-generated by op2.py
//

#include <cmath>
#include "const.h"
#include "structures.h"
#include "global.h"

//user function
__device__ void calculate_cell_volumes_gpu( 
    const double* coords1,
    const double* coords2,
    double* ewt,
    double* vol1,
    double* vol2) {
    double d[NDIM];
    double dist = 0.0;
    for (int i=0; i<NDIM; i++) {
        d[i] = coords2[i] - coords1[i];
        dist += d[i]*d[i];
    }
    dist = sqrt(dist);

    double area = 0.0;
    for (int i=0; i<NDIM; i++) {
        area += ewt[i]*ewt[i];
    }
    area = sqrt(area);

    double tetra_volume = (1.0/3.0)*0.5 *dist *area;
    *vol1 += tetra_volume;
    *vol2 += tetra_volume;

    for (int i=0; i<NDIM; i++) {
        ewt[i] = (d[i] / dist) * area;
    }



    for (int i=0; i<NDIM; i++) {
        ewt[i] /= dist;
    }

}

// CUDA kernel function
__global__ void op_cuda_calculate_cell_volumes(
  const double *__restrict ind_arg0,
  double *__restrict ind_arg1,
  const int *__restrict opDat0Map,
  double *arg2,
  int start,
  int end,
  int   set_size) {
  double arg3_l[1];
  double arg4_l[1];
  int tid = threadIdx.x + blockIdx.x * blockDim.x;
  if (tid + start < end) {
    int n = tid + start;
    //initialise local variables
    double arg3_l[1];
    for ( int d=0; d<1; d++ ){
      arg3_l[d] = ZERO_double;
    }
    double arg4_l[1];
    for ( int d=0; d<1; d++ ){
      arg4_l[d] = ZERO_double;
    }
    int map0idx;
    int map1idx;
    map0idx = opDat0Map[n + set_size * 0];
    map1idx = opDat0Map[n + set_size * 1];

    //user-supplied kernel call
    calculate_cell_volumes_gpu(ind_arg0+map0idx*3,
                           ind_arg0+map1idx*3,
                           arg2+n*3,
                           arg3_l,
                           arg4_l);
    atomicAdd(&ind_arg1[0+map0idx*1],arg3_l[0]);
    atomicAdd(&ind_arg1[0+map1idx*1],arg4_l[0]);
  }
}


//host stub function
void op_par_loop_calculate_cell_volumes(char const *name, op_set set,
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
  op_timing_realloc(3);
  op_timers_core(&cpu_t1, &wall_t1);
  OP_kernels[3].name      = name;
  OP_kernels[3].count    += 1;


  int    ninds   = 2;
  int    inds[5] = {0,0,-1,1,1};

  if (OP_diags>2) {
    printf(" kernel routine with indirection: calculate_cell_volumes\n");
  }
  int set_size = op_mpi_halo_exchanges_cuda(set, nargs, args);
  if (set_size > 0) {

    //set CUDA execution parameters
    #ifdef OP_BLOCK_SIZE_3
      int nthread = OP_BLOCK_SIZE_3;
    #else
      int nthread = OP_block_size;
    #endif

    for ( int round=0; round<2; round++ ){
      if (round==1) {
        op_mpi_wait_all_cuda(nargs, args);
      }
      int start = round==0 ? 0 : set->core_size;
      int end = round==0 ? set->core_size : set->size + set->exec_size;
      if (end-start>0) {
        int nblocks = (end-start-1)/nthread+1;
        op_cuda_calculate_cell_volumes<<<nblocks,nthread>>>(
        (double *)arg0.data_d,
        (double *)arg3.data_d,
        arg0.map_data_d,
        (double*)arg2.data_d,
        start,end,set->size+set->exec_size);
      }
    }
  }
  op_mpi_set_dirtybit_cuda(nargs, args);
  cutilSafeCall(cudaDeviceSynchronize());
  //update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[3].time     += wall_t2 - wall_t1;
}
