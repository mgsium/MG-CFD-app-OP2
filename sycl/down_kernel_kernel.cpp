//
// auto-generated by op2.py
//

#include <math.h>
#include "const.h"

//user function
class down_kernel_kernel;

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

  op_mpi_halo_exchanges_cuda(set, nargs, args);
  if (set->size > 0) {

    //set SYCL execution parameters
    #ifdef OP_BLOCK_SIZE_21
      int nthread = OP_BLOCK_SIZE_21;
    #else
      int nthread = OP_block_size;
    #endif

    cl::sycl::buffer<double,1> *arg3_buffer = static_cast<cl::sycl::buffer<double,1>*>((void*)arg3.data_d);
    cl::sycl::buffer<double,1> *arg4_buffer = static_cast<cl::sycl::buffer<double,1>*>((void*)arg4.data_d);
    cl::sycl::buffer<int,1> *map3_buffer = static_cast<cl::sycl::buffer<int,1>*>((void*)arg3.map_data_d);
    cl::sycl::buffer<double,1> *arg0_buffer = static_cast<cl::sycl::buffer<double,1>*>((void*)arg0.data_d);
    cl::sycl::buffer<double,1> *arg1_buffer = static_cast<cl::sycl::buffer<double,1>*>((void*)arg1.data_d);
    cl::sycl::buffer<double,1> *arg2_buffer = static_cast<cl::sycl::buffer<double,1>*>((void*)arg2.data_d);
    int set_size = set->size+set->exec_size;
    for ( int round=0; round<2; round++ ){
      if (round==1) {
        op_mpi_wait_all_cuda(nargs, args);
      }
      int start = round==0 ? 0 : set->core_size;
      int end = round==0 ? set->core_size : set->size + set->exec_size;
      if (end-start>0) {
        int nblocks = (end-start-1)/nthread+1;
        try {
        op2_queue->submit([&](cl::sycl::handler& cgh) {
          auto ind_arg0 = (*arg3_buffer).template get_access<cl::sycl::access::mode::read_write>(cgh);
          auto ind_arg1 = (*arg4_buffer).template get_access<cl::sycl::access::mode::read_write>(cgh);
          auto opDat3Map =  (*map3_buffer).template get_access<cl::sycl::access::mode::read>(cgh);

          auto arg0 = (*arg0_buffer).template get_access<cl::sycl::access::mode::read_write>(cgh);
          auto arg1 = (*arg1_buffer).template get_access<cl::sycl::access::mode::read_write>(cgh);
          auto arg2 = (*arg2_buffer).template get_access<cl::sycl::access::mode::read_write>(cgh);


          //user fun as lambda
          auto down_kernel_gpu = [=]( 
                double* variable,
                const double* residual,
                const double* coord,
                const double* residual_above,
                const double* coord_above) {
                double dx = fabs(coord[0] - coord_above[0]);
                double dy = fabs(coord[1] - coord_above[1]);
                double dz = fabs(coord[2] - coord_above[2]);
                double dm = cl::sycl::sqrt(dx*dx + dy*dy + dz*dz);
            
                variable[VAR_DENSITY]        -= dm* (residual_above[VAR_DENSITY]        - residual[VAR_DENSITY]);
                variable[VAR_MOMENTUM+0]     -= dx* (residual_above[VAR_MOMENTUM+0]     - residual[VAR_MOMENTUM+0]);
                variable[VAR_MOMENTUM+1]     -= dy* (residual_above[VAR_MOMENTUM+1]     - residual[VAR_MOMENTUM+1]);
                variable[VAR_MOMENTUM+2]     -= dz* (residual_above[VAR_MOMENTUM+2]     - residual[VAR_MOMENTUM+2]);
                variable[VAR_DENSITY_ENERGY] -= dm* (residual_above[VAR_DENSITY_ENERGY] - residual[VAR_DENSITY_ENERGY]);
            
            };
            
          auto kern = [=](cl::sycl::nd_item<1> item) {
            int tid = item.get_global_linear_id();
            if (tid + start < end) {
              int n = tid+start;
              //initialise local variables
              int map3idx;
              map3idx = opDat3Map[n + set_size * 0];

              //user-supplied kernel call
              down_kernel_gpu(&arg0[n*5],
                &arg1[n*5],
                &arg2[n*3],
                &ind_arg0[map3idx*5],
                &ind_arg1[map3idx*3]);
            }

          };
          cgh.parallel_for<class down_kernel_kernel>(cl::sycl::nd_range<1>(nthread*nblocks,nthread), kern);
        });
        }catch(cl::sycl::exception const &e) {
        std::cout << e.what() << std::endl;exit(-1);
        }

      }
    }
  }
  op_mpi_set_dirtybit_cuda(nargs, args);
  op2_queue->wait();
  //update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[21].time     += wall_t2 - wall_t1;
}
