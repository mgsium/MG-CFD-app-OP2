//
// auto-generated by op2.py
//

#include <math.h>
#include <cmath>
#include "const.h"
#include "inlined_funcs.h"

//user function
class calculate_dt_kernel_kernel;

//host stub function
void op_par_loop_calculate_dt_kernel(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2){

  int nargs = 3;
  op_arg args[3];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(6);
  op_timers_core(&cpu_t1, &wall_t1);
  OP_kernels[6].name      = name;
  OP_kernels[6].count    += 1;


  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  calculate_dt_kernel");
  }

  op_mpi_halo_exchanges_cuda(set, nargs, args);
  if (set->size > 0) {

    //set SYCL execution parameters
    #ifdef OP_BLOCK_SIZE_6
      int nthread = OP_BLOCK_SIZE_6;
    #else
      int nthread = OP_block_size;
    #endif

    int nblocks = 200;

    cl::sycl::buffer<double,1> *arg0_buffer = static_cast<cl::sycl::buffer<double,1>*>((void*)arg0.data_d);
    cl::sycl::buffer<double,1> *arg1_buffer = static_cast<cl::sycl::buffer<double,1>*>((void*)arg1.data_d);
    cl::sycl::buffer<double,1> *arg2_buffer = static_cast<cl::sycl::buffer<double,1>*>((void*)arg2.data_d);
    int set_size = set->size+set->exec_size;
    try {
    op2_queue->submit([&](cl::sycl::handler& cgh) {
      auto arg0 = (*arg0_buffer).template get_access<cl::sycl::access::mode::read_write>(cgh);
      auto arg1 = (*arg1_buffer).template get_access<cl::sycl::access::mode::read_write>(cgh);
      auto arg2 = (*arg2_buffer).template get_access<cl::sycl::access::mode::read_write>(cgh);

      //user fun as lambda
      auto calculate_dt_kernel_gpu = [=]( 
            const double* variable,
            const double* volume,
            double* dt) {
            double density = variable[VAR_DENSITY];
        
            double3 momentum;
            momentum.x = variable[VAR_MOMENTUM+0];
            momentum.y = variable[VAR_MOMENTUM+1];
            momentum.z = variable[VAR_MOMENTUM+2];
        
            double density_energy = variable[VAR_DENSITY_ENERGY];
            double3 velocity; compute_velocity(density, momentum, velocity);
            double speed_sqd      = compute_speed_sqd(velocity);
            double pressure       = compute_pressure(density, density_energy, speed_sqd);
            double speed_of_sound = compute_speed_of_sound(density, pressure);
        
            *dt = double(0.5) * (cbrt(*volume) / (cl::sycl::sqrt(speed_sqd) + speed_of_sound));
        
        };
        
      auto kern = [=](cl::sycl::nd_item<1> item) {

        //process set elements
        for ( int n=item.get_global_linear_id(); n<set_size; n+=item.get_global_range()[0] ){

          //user-supplied kernel call
          calculate_dt_kernel_gpu(&arg0[n*5],
                        &arg1[n*1],
                        &arg2[n*1]);
        }

      };
      cgh.parallel_for<class calculate_dt_kernel_kernel>(cl::sycl::nd_range<1>(nthread*nblocks,nthread), kern);
    });
    }catch(cl::sycl::exception const &e) {
    std::cout << e.what() << std::endl;exit(-1);
    }
  }
  op_mpi_set_dirtybit_cuda(nargs, args);
  op2_queue->wait();
  //update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[6].time     += wall_t2 - wall_t1;
  OP_kernels[6].transfer += (float)set->size * arg0.size;
  OP_kernels[6].transfer += (float)set->size * arg1.size;
  OP_kernels[6].transfer += (float)set->size * arg2.size * 2.0f;
}
