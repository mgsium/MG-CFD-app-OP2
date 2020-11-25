//
// auto-generated by op2.py
//

#include <math.h>
#include "const.h"

//user function
class down_v2_kernel_pre_kernel;

//host stub function
void op_par_loop_down_v2_kernel_pre(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1){

  int nargs = 2;
  op_arg args[2];

  args[0] = arg0;
  args[1] = arg1;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(19);
  op_timers_core(&cpu_t1, &wall_t1);
  OP_kernels[19].name      = name;
  OP_kernels[19].count    += 1;


  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  down_v2_kernel_pre\n");
  }

  op_mpi_halo_exchanges_cuda(set, nargs, args);
  if (set->size > 0) {

    //set SYCL execution parameters
    #ifdef OP_BLOCK_SIZE_19
      int nthread = OP_BLOCK_SIZE_19;
    #else
      int nthread = OP_block_size;
    #endif

    int nblocks = 200;

    cl::sycl::buffer<double,1> *arg0_buffer = static_cast<cl::sycl::buffer<double,1>*>((void*)arg0.data_d);
    cl::sycl::buffer<double,1> *arg1_buffer = static_cast<cl::sycl::buffer<double,1>*>((void*)arg1.data_d);
    int set_size = set->size+set->exec_size;
    try {
    op2_queue->submit([&](cl::sycl::handler& cgh) {
      auto arg0 = (*arg0_buffer).template get_access<cl::sycl::access::mode::read_write>(cgh);
      auto arg1 = (*arg1_buffer).template get_access<cl::sycl::access::mode::read_write>(cgh);

      //user fun as lambda
      auto down_v2_kernel_pre_gpu = [=]( 
            double* weight_sum,
            double* residual_sum) {
            *weight_sum = 0.0;
            residual_sum[VAR_DENSITY] = 0.0;
            residual_sum[VAR_MOMENTUM+0] = 0.0;
            residual_sum[VAR_MOMENTUM+2] = 0.0;
            residual_sum[VAR_MOMENTUM+1] = 0.0;
            residual_sum[VAR_DENSITY_ENERGY] = 0.0;
        
        };
        
      auto kern = [=](cl::sycl::item<1> item) [[intel::reqd_sub_group_size(SIMD_VEC)]] {

        //process set elements
        int n = item.get_id(0);
        if (n < set_size) {

          //user-supplied kernel call
          down_v2_kernel_pre_gpu(&arg0[n*5],
                                 &arg1[n*1]);
        }

      };
      cgh.parallel_for<class down_v2_kernel_pre_kernel>(cl::sycl::range<1>(set_size), kern);
    });
    }catch(cl::sycl::exception const &e) {
    std::cout << e.what() << std::endl;exit(-1);
    }
  }
  op_mpi_set_dirtybit_cuda(nargs, args);
  op2_queue->wait();
  //update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[19].time     += wall_t2 - wall_t1;
  OP_kernels[19].transfer += (float)set->size * arg0.size * 2.0f;
  OP_kernels[19].transfer += (float)set->size * arg1.size * 2.0f;
}
