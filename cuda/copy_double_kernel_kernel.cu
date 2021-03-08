//
// auto-generated by op2.py
//

__constant__ int direct_copy_double_kernel_stride_OP2CONSTANT;
int direct_copy_double_kernel_stride_OP2HOST=-1;
#include "const.h"

//user function
__device__ void copy_double_kernel_gpu( 
	const double* variables,
	double* old_variables) {
	for (int i=0; i<NVAR; i++) {
		old_variables[(i)*direct_copy_double_kernel_stride_OP2CONSTANT] = variables[(i)*direct_copy_double_kernel_stride_OP2CONSTANT];
	}

}

// CUDA kernel function
__global__ void op_cuda_copy_double_kernel(
  const double *__restrict arg0,
  double *arg1,
  int   set_size ) {


  //process set elements
  for ( int n=threadIdx.x+blockIdx.x*blockDim.x; n<set_size; n+=blockDim.x*gridDim.x ){

    //user-supplied kernel call
    copy_double_kernel_gpu(arg0+n,
                       arg1+n);
  }
}


//host stub function
void op_par_loop_copy_double_kernel(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1){

  int nargs = 2;
  op_arg args[2];

  args[0] = arg0;
  args[1] = arg1;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(5);
  op_timers_core(&cpu_t1, &wall_t1);
  OP_kernels[5].name      = name;
  OP_kernels[5].count    += 1;


  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  copy_double_kernel");
  }

  int set_size = op_mpi_halo_exchanges_cuda(set, nargs, args);
  if (set_size > 0) {

    if ((OP_kernels[5].count==1) || (direct_copy_double_kernel_stride_OP2HOST != getSetSizeFromOpArg(&arg0))) {
      direct_copy_double_kernel_stride_OP2HOST = getSetSizeFromOpArg(&arg0);
      cudaMemcpyToSymbol(direct_copy_double_kernel_stride_OP2CONSTANT,&direct_copy_double_kernel_stride_OP2HOST,sizeof(int));
    }
    //set CUDA execution parameters
    #ifdef OP_BLOCK_SIZE_5
      int nthread = OP_BLOCK_SIZE_5;
    #else
      int nthread = OP_block_size;
    #endif

    int nblocks = 200;

    op_cuda_copy_double_kernel<<<nblocks,nthread>>>(
      (double *) arg0.data_d,
      (double *) arg1.data_d,
      set->size );
  }
  op_mpi_set_dirtybit_cuda(nargs, args);
  cutilSafeCall(cudaDeviceSynchronize());
  //update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[5].time     += wall_t2 - wall_t1;
  OP_kernels[5].transfer += (float)set->size * arg0.size;
  OP_kernels[5].transfer += (float)set->size * arg1.size * 2.0f;
}
