//
// auto-generated by op2.py
//

//global constants
#ifndef MAX_CONST_SIZE
#define MAX_CONST_SIZE 128
#endif

__constant__ double smoothing_coefficient_cuda;
__constant__ double ff_variable_cuda[5];
__constant__ double ff_flux_contribution_momentum_x_cuda[3];
__constant__ double ff_flux_contribution_momentum_y_cuda[3];
__constant__ double ff_flux_contribution_momentum_z_cuda[3];
__constant__ double ff_flux_contribution_density_energy_cuda[3];
__constant__ int mesh_name_cuda;

//header
#include "op_lib_cpp.h"
#include "op_cuda_rt_support.h"
#include "op_cuda_reduction.h"

void op_decl_const_char(int dim, char const *type,
int size, char *dat, char const *name){
  if (!OP_hybrid_gpu) return;
  if (!strcmp(name,"smoothing_coefficient")) {
    cutilSafeCall(cudaMemcpyToSymbol(smoothing_coefficient_cuda, dat, dim*size));
  }
  else
  if (!strcmp(name,"ff_variable")) {
    cutilSafeCall(cudaMemcpyToSymbol(ff_variable_cuda, dat, dim*size));
  }
  else
  if (!strcmp(name,"ff_flux_contribution_momentum_x")) {
    cutilSafeCall(cudaMemcpyToSymbol(ff_flux_contribution_momentum_x_cuda, dat, dim*size));
  }
  else
  if (!strcmp(name,"ff_flux_contribution_momentum_y")) {
    cutilSafeCall(cudaMemcpyToSymbol(ff_flux_contribution_momentum_y_cuda, dat, dim*size));
  }
  else
  if (!strcmp(name,"ff_flux_contribution_momentum_z")) {
    cutilSafeCall(cudaMemcpyToSymbol(ff_flux_contribution_momentum_z_cuda, dat, dim*size));
  }
  else
  if (!strcmp(name,"ff_flux_contribution_density_energy")) {
    cutilSafeCall(cudaMemcpyToSymbol(ff_flux_contribution_density_energy_cuda, dat, dim*size));
  }
  else
  if (!strcmp(name,"mesh_name")) {
    cutilSafeCall(cudaMemcpyToSymbol(mesh_name_cuda, dat, dim*size));
  }
  else
  {
    printf("error: unknown const name\n"); exit(1);
  }
}

//user kernel files
#include "initialize_variables_kernel_kernel.cu"
#include "zero_5d_array_kernel_kernel.cu"
#include "zero_1d_array_kernel_kernel.cu"
#include "calculate_cell_volumes_kernel.cu"
#include "dampen_ewt_kernel.cu"
#include "copy_double_kernel_kernel.cu"
#include "calculate_dt_kernel_kernel.cu"
#include "get_min_dt_kernel_kernel.cu"
#include "compute_step_factor_kernel_kernel.cu"
#include "compute_flux_edge_kernel_kernel.cu"
#include "compute_bnd_node_flux_kernel_kernel.cu"
#include "time_step_kernel_kernel.cu"
#include "unstructured_stream_kernel_kernel.cu"
#include "residual_kernel_kernel.cu"
#include "calc_rms_kernel_kernel.cu"
#include "count_bad_vals_kernel.cu"
#include "up_pre_kernel_kernel.cu"
#include "up_kernel_kernel.cu"
#include "up_post_kernel_kernel.cu"
#include "down_v2_kernel_pre_kernel.cu"
#include "down_v2_kernel_kernel.cu"
#include "down_v2_kernel_post_kernel.cu"
#include "down_kernel_kernel.cu"
#include "identify_differences_kernel.cu"
#include "count_non_zeros_kernel.cu"
