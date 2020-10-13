//
// auto-generated by op2.py
//

#include <math.h>
#include "const.h"

//user function
__device__ void down_kernel_gpu( 
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

// CUDA kernel function
__global__ void op_cuda_down_kernel(
  const double *__restrict ind_arg0,
  const double *__restrict ind_arg1,
  const int *__restrict opDat3Map,
  double *arg0,
  const double *__restrict arg1,
  const double *__restrict arg2,
  int    block_offset,
  int   *blkmap,
  int   *offset,
  int   *nelems,
  int   *ncolors,
  int   *colors,
  int   nblocks,
  int   set_size) {

  __shared__ int    nelem, offset_b;

  extern __shared__ char shared[];

  if (blockIdx.x+blockIdx.y*gridDim.x >= nblocks) {
    return;
  }
  if (threadIdx.x==0) {

    //get sizes and shift pointers and direct-mapped data

    int blockId = blkmap[blockIdx.x + blockIdx.y*gridDim.x  + block_offset];

    nelem    = nelems[blockId];
    offset_b = offset[blockId];

  }
  __syncthreads(); // make sure all of above completed

  for ( int n=threadIdx.x; n<nelem; n+=blockDim.x ){
    int map3idx;
    map3idx = opDat3Map[n + offset_b + set_size * 0];


    //user-supplied kernel call
    down_kernel_gpu(arg0+(n+offset_b)*5,
                arg1+(n+offset_b)*5,
                arg2+(n+offset_b)*3,
                ind_arg0+map3idx*5,
                ind_arg1+map3idx*3);
  }
}


//host stub function
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


  int    ninds   = 2;
  int    inds[5] = {-1,-1,-1,0,1};

  if (OP_diags>2) {
    printf(" kernel routine with indirection: down_kernel\n");
  }

  //get plan
  #ifdef OP_PART_SIZE_21
    int part_size = OP_PART_SIZE_21;
  #else
    int part_size = OP_part_size;
  #endif

  int set_size = op_mpi_halo_exchanges_cuda(set, nargs, args);
  if (set->size > 0) {

    op_plan *Plan = op_plan_get(name,set,part_size,nargs,args,ninds,inds);

    //execute plan

    int block_offset = 0;
    for ( int col=0; col<Plan->ncolors; col++ ){
      if (col==Plan->ncolors_core) {
        op_mpi_wait_all_cuda(nargs, args);
      }
      #ifdef OP_BLOCK_SIZE_21
      int nthread = OP_BLOCK_SIZE_21;
      #else
      int nthread = OP_block_size;
      #endif

      dim3 nblocks = dim3(Plan->ncolblk[col] >= (1<<16) ? 65535 : Plan->ncolblk[col],
      Plan->ncolblk[col] >= (1<<16) ? (Plan->ncolblk[col]-1)/65535+1: 1, 1);
      if (Plan->ncolblk[col] > 0) {
        op_cuda_down_kernel<<<nblocks,nthread>>>(
        (double *)arg3.data_d,
        (double *)arg4.data_d,
        arg3.map_data_d,
        (double*)arg0.data_d,
        (double*)arg1.data_d,
        (double*)arg2.data_d,
        block_offset,
        Plan->blkmap,
        Plan->offset,
        Plan->nelems,
        Plan->nthrcol,
        Plan->thrcol,
        Plan->ncolblk[col],
        set->size+set->exec_size);

      }
      block_offset += Plan->ncolblk[col];
    }
    OP_kernels[21].transfer  += Plan->transfer;
    OP_kernels[21].transfer2 += Plan->transfer2;
  }
  op_mpi_set_dirtybit_cuda(nargs, args);
  cutilSafeCall(cudaDeviceSynchronize());
  //update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[21].time     += wall_t2 - wall_t1;
}
