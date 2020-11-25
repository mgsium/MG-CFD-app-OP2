//
// auto-generated by op2.py
//

#include <math.h>
#include <cmath>
#include "const.h"

#ifndef INLINED_FUNCS_H
#define INLINED_FUNCS_H
#include "const.h"
#include "structures.h"
#define DEBUGGABLE_ABORT fprintf(stderr, "%s:%d\n", __FILE__, __LINE__); fflush(stderr); fflush(stdout); exit(EXIT_FAILURE);
static inline OP_FUN_PREFIX void zero_array(int nelr, double* variables)
{
    #ifdef OMP
    #pragma omp parallel for schedule(static)
    #endif
    for(int i = 0; i < nelr; i++)
    {
        variables[i] = 0.0;
    }
}
static inline OP_FUN_PREFIX void zero_edges(int nelr, edge* mx, edge* my, edge* mz, edge* p, edge* pe)
{
    #ifdef OMP
    #pragma omp parallel
    {
    #endif
        #ifdef VERITAS
            pdump_start_profile();
        #endif
        #ifdef OMP
        #pragma omp for schedule(static)
        #endif
        #ifdef SIMD
        #pragma simd vectorlength(SIMD)
        #else
        #pragma novector
        #endif
        for(int i = 0; i < nelr; i++)
        {
            mx[i].a = 0.0;
            mx[i].b = 0.0;
            my[i].a = 0.0;
            my[i].b = 0.0;
            mz[i].a = 0.0;
            mz[i].b = 0.0;
            p[i].a = 0.0;
            p[i].b = 0.0;
            pe[i].a = 0.0;
            pe[i].b = 0.0;
        }
        #ifdef VERITAS
            pdump_end_profile();
        #endif
    #ifdef OMP
    }
    #endif
}
static inline OP_FUN_PREFIX void compute_flux_contribution(
    double& density, double3& momentum, 
    double& density_energy, 
    double& pressure, 
    double3& velocity, 
    double* fc_momentum_x, 
    double* fc_momentum_y, 
    double* fc_momentum_z, 
    double* fc_density_energy)
{
    fc_momentum_x[0] = velocity.x*momentum.x + pressure;
    fc_momentum_x[1] = velocity.x*momentum.y;
    fc_momentum_x[2] = velocity.x*momentum.z;
    fc_momentum_y[0] = fc_momentum_x[1];
    fc_momentum_y[1] = velocity.y*momentum.y + pressure;
    fc_momentum_y[2] = velocity.y*momentum.z;
    fc_momentum_z[0] = fc_momentum_x[2];
    fc_momentum_z[1] = fc_momentum_y[2];
    fc_momentum_z[2] = velocity.z*momentum.z + pressure;
    double de_p = density_energy+pressure;
    fc_density_energy[0] = velocity.x*de_p;
    fc_density_energy[1] = velocity.y*de_p;
    fc_density_energy[2] = velocity.z*de_p;
}
static inline OP_FUN_PREFIX double compute_speed_sqd(double3& velocity)
{
    return velocity.x*velocity.x + velocity.y*velocity.y + velocity.z*velocity.z;
}
static inline OP_FUN_PREFIX double compute_pressure(double& density, double& density_energy, double& speed_sqd)
{
    return (double(GAMMA)-double(1.0))*(density_energy - double(0.5)*density*speed_sqd);
}
#ifdef IDIVIDE
    static inline OP_FUN_PREFIX void compute_velocity(double& inverse_density, double3& momentum, double3& velocity)
    {
        velocity.x = momentum.x*inverse_density;
        velocity.y = momentum.y*inverse_deinlined_funcs.hnsity;
        velocity.z = momentum.z*inverse_density;
    }
    static inline OP_FUN_PREFIX double compute_speed_of_sound(double& inverse_density, double& pressure)
    {
        return cl::sycl::sqrt((double(GAMMA)*pressure)*inverse_density);
    }
#else
    static OP_FUN_PREFIX inline void compute_velocity(double& density, double3& momentum, double3& velocity)
    {
        velocity.x = momentum.x / density;
        velocity.y = momentum.y / density;
        velocity.z = momentum.z / density;
    }
    static inline OP_FUN_PREFIX double compute_speed_of_sound(double& density, double& pressure)
    {
        return cl::sycl::sqrt(double(GAMMA)*pressure / density);
    }
#endif
#endif

//user function
class compute_step_factor_kernel_kernel;

//host stub function
void op_par_loop_compute_step_factor_kernel(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2,
  op_arg arg3){

  double*arg2h = (double *)arg2.data;
  int nargs = 4;
  op_arg args[4];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  args[3] = arg3;

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(8);
  op_timers_core(&cpu_t1, &wall_t1);
  OP_kernels[8].name      = name;
  OP_kernels[8].count    += 1;


  if (OP_diags>2) {
    printf(" kernel routine w/o indirection:  compute_step_factor_kernel\n");
  }

  op_mpi_halo_exchanges_cuda(set, nargs, args);
  if (set->size > 0) {

    //transfer constants to GPU
    int consts_bytes = 0;
    consts_bytes += ROUND_UP(1*sizeof(double));
    allocConstArrays(consts_bytes, "double");
    consts_bytes = 0;
    arg2.data   = OP_consts_h + consts_bytes;
    int arg2_offset = consts_bytes/sizeof(double);
    for ( int d=0; d<1; d++ ){
      ((double *)arg2.data)[d] = arg2h[d];
    }
    consts_bytes += ROUND_UP(1*sizeof(double));
    mvConstArraysToDevice(consts_bytes, "double");
    cl::sycl::buffer<double,1> *consts = static_cast<cl::sycl::buffer<double,1> *>((void*)OP_consts_d);

    //set SYCL execution parameters
    #ifdef OP_BLOCK_SIZE_8
      int nthread = OP_BLOCK_SIZE_8;
    #else
      int nthread = OP_block_size;
    #endif

    int nblocks = 200;

    cl::sycl::buffer<double,1> *arg0_buffer = static_cast<cl::sycl::buffer<double,1>*>((void*)arg0.data_d);
    cl::sycl::buffer<double,1> *arg1_buffer = static_cast<cl::sycl::buffer<double,1>*>((void*)arg1.data_d);
    cl::sycl::buffer<double,1> *arg3_buffer = static_cast<cl::sycl::buffer<double,1>*>((void*)arg3.data_d);
    int set_size = set->size+set->exec_size;
    try {
    op2_queue->submit([&](cl::sycl::handler& cgh) {
      auto arg0 = (*arg0_buffer).template get_access<cl::sycl::access::mode::read_write>(cgh);
      auto arg1 = (*arg1_buffer).template get_access<cl::sycl::access::mode::read_write>(cgh);
      auto arg3 = (*arg3_buffer).template get_access<cl::sycl::access::mode::read_write>(cgh);
      auto consts_d = (*consts).template get_access<cl::sycl::access::mode::read_write>(cgh);

      //user fun as lambda
      auto compute_step_factor_kernel_gpu = [=]( 
            const double* variable,
            const double* volume,
            const double* min_dt,
            double* step_factor) {
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
        
            *step_factor = (*min_dt) / (*volume);
        
        };
        
      auto kern = [=](cl::sycl::item<1> item) [[intel::reqd_sub_group_size(SIMD_VEC)]] {

        //process set elements
        int n = item.get_id(0);
        if (n < set_size) {

          //user-supplied kernel call
          compute_step_factor_kernel_gpu(&arg0[n*5],
                                         &arg1[n*1],
                                         &consts_d[arg2_offset],
                                         &arg3[n*1]);
        }

      };
      cgh.parallel_for<class compute_step_factor_kernel_kernel>(cl::sycl::range<1>(set_size), kern);
    });
    }catch(cl::sycl::exception const &e) {
    std::cout << e.what() << std::endl;exit(-1);
    }
    freeConstArrays("double");
  }
  op_mpi_set_dirtybit_cuda(nargs, args);
  op2_queue->wait();
  //update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[8].time     += wall_t2 - wall_t1;
  OP_kernels[8].transfer += (float)set->size * arg0.size;
  OP_kernels[8].transfer += (float)set->size * arg1.size;
  OP_kernels[8].transfer += (float)set->size * arg3.size * 2.0f;
}
