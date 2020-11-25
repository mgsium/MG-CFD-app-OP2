//
// auto-generated by op2.py
//

#include <math.h>
#include "const.h"

//user function
class up_kernel_kernel;

//host stub function
void op_par_loop_up_kernel(char const *name, op_set set,
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
  op_timing_realloc(17);
  op_timers_core(&cpu_t1, &wall_t1);
  OP_kernels[17].name      = name;
  OP_kernels[17].count    += 1;


  int    ninds   = 2;
  int    inds[3] = {-1,0,1};

  if (OP_diags>2) {
    printf(" kernel routine with indirection: up_kernel\n");
  }

  //get plan
  #ifdef OP_PART_SIZE_17
    int part_size = OP_PART_SIZE_17;
  #else
    int part_size = OP_part_size;
  #endif

  op_mpi_halo_exchanges_cuda(set, nargs, args);
  if (set->size > 0) {

    op_plan *Plan = op_plan_get_stage(name,set,part_size,nargs,args,ninds,inds,OP_STAGE_ALL);

    cl::sycl::buffer<double,1> *arg1_buffer = static_cast<cl::sycl::buffer<double,1>*>((void*)arg1.data_d);
    cl::sycl::buffer<int,1> *arg2_buffer = static_cast<cl::sycl::buffer<int,1>*>((void*)arg2.data_d);
    cl::sycl::buffer<int,1> *map1_buffer = static_cast<cl::sycl::buffer<int,1>*>((void*)arg1.map_data_d);
    cl::sycl::buffer<double,1> *arg0_buffer = static_cast<cl::sycl::buffer<double,1>*>((void*)arg0.data_d);
    cl::sycl::buffer<int,1> *blkmap_buffer = static_cast<cl::sycl::buffer<int,1>*>((void*)Plan->blkmap);
    cl::sycl::buffer<int,1> *offset_buffer = static_cast<cl::sycl::buffer<int,1>*>((void*)Plan->offset);
    cl::sycl::buffer<int,1> *nelems_buffer = static_cast<cl::sycl::buffer<int,1>*>((void*)Plan->nelems);
    cl::sycl::buffer<int,1> *ncolors_buffer = static_cast<cl::sycl::buffer<int,1>*>((void*)Plan->nthrcol);
    cl::sycl::buffer<int,1> *colors_buffer = static_cast<cl::sycl::buffer<int,1>*>((void*)Plan->thrcol);
    int set_size = set->size+set->exec_size;
    //execute plan

    int block_offset = 0;
    for ( int col=0; col<Plan->ncolors; col++ ){
      if (col==Plan->ncolors_core) {
        op_mpi_wait_all_cuda(nargs, args);
      }
      int nthread = SIMD_VEC;

      int nblocks = op2_queue->get_device().get_info<cl::sycl::info::device::max_compute_units>();
      int nblocks2 = Plan->ncolblk[col];
      if (Plan->ncolblk[col] > 0) {
        try {
        op2_queue->submit([&](cl::sycl::handler& cgh) {
          auto ind_arg0 = (*arg1_buffer).template get_access<cl::sycl::access::mode::read_write>(cgh);
          auto ind_arg1 = (*arg2_buffer).template get_access<cl::sycl::access::mode::read_write>(cgh);
          auto opDat1Map =  (*map1_buffer).template get_access<cl::sycl::access::mode::read>(cgh);
          auto blkmap    = (*blkmap_buffer).template get_access<cl::sycl::access::mode::read>(cgh);
          auto offset    = (*offset_buffer).template get_access<cl::sycl::access::mode::read>(cgh);
          auto nelems    = (*nelems_buffer).template get_access<cl::sycl::access::mode::read>(cgh);
          auto ncolors   = (*ncolors_buffer).template get_access<cl::sycl::access::mode::read>(cgh);
          auto colors    = (*colors_buffer).template get_access<cl::sycl::access::mode::read>(cgh);

          auto arg0 = (*arg0_buffer).template get_access<cl::sycl::access::mode::read_write>(cgh);


          //user fun as lambda
          auto up_kernel_gpu = [=]( 
                const double* variable,
                double* variable_above,
                int* up_scratch) {
                variable_above[VAR_DENSITY]        += variable[VAR_DENSITY];
                variable_above[VAR_MOMENTUM+0]     += variable[VAR_MOMENTUM+0];
                variable_above[VAR_MOMENTUM+1]     += variable[VAR_MOMENTUM+1];
                variable_above[VAR_MOMENTUM+2]     += variable[VAR_MOMENTUM+2];
                variable_above[VAR_DENSITY_ENERGY] += variable[VAR_DENSITY_ENERGY];
                *up_scratch += 1;
            
            };
            
          auto kern = [=](cl::sycl::nd_item<1> item) [[intel::reqd_sub_group_size(SIMD_VEC)]] {
            double arg1_l[5];
            int arg2_l[1];


            //get sizes and shift pointers and direct-mapped data

            int blocksPerWG = (nblocks2-1)/item.get_group_range(0)+1;
            for ( int idx=item.get_group_linear_id()*blocksPerWG; idx<(item.get_group_linear_id()+1)*blocksPerWG && idx < nblocks2; idx++ ){
              int blockId = blkmap[idx + block_offset];

              int nelem    = nelems[blockId];
              int offset_b = offset[blockId];
              sycl::ONEAPI::sub_group sg = item.get_sub_group();

              int nelems2  = item.get_local_range()[0]*(1+(nelem-1)/item.get_local_range()[0]);
              int ncolor   = ncolors[blockId];


              for ( int n=item.get_local_id(0); n<nelems2; n+=item.get_local_range()[0] ){
                int col2 = -1;
                int map1idx;
                if (n<nelem) {
                  //initialise local variables
                  for ( int d=0; d<5; d++ ){
                    arg1_l[d] = ZERO_double;
                  }
                  for ( int d=0; d<1; d++ ){
                    arg2_l[d] = ZERO_int;
                  }
                  map1idx = opDat1Map[n + offset_b + set_size * 0];


                  //user-supplied kernel call
                  up_kernel_gpu(&arg0[(n+offset_b)*5],
                                arg1_l,
                                arg2_l);
                  col2 = colors[n+offset_b];
                }

                //store local variables

                for ( int col=0; col<ncolor; col++ ){
                  if (col2==col) {
                    arg1_l[0] += ind_arg0[0+map1idx*5];
                    arg1_l[1] += ind_arg0[1+map1idx*5];
                    arg1_l[2] += ind_arg0[2+map1idx*5];
                    arg1_l[3] += ind_arg0[3+map1idx*5];
                    arg1_l[4] += ind_arg0[4+map1idx*5];
                    ind_arg0[0+map1idx*5] = arg1_l[0];
                    ind_arg0[1+map1idx*5] = arg1_l[1];
                    ind_arg0[2+map1idx*5] = arg1_l[2];
                    ind_arg0[3+map1idx*5] = arg1_l[3];
                    ind_arg0[4+map1idx*5] = arg1_l[4];
                    arg2_l[0] += ind_arg1[0+map1idx*1];
                    ind_arg1[0+map1idx*1] = arg2_l[0];
                  }
                  sg.barrier();
                }
              }

            }
          };
          cgh.parallel_for<class up_kernel_kernel>(cl::sycl::nd_range<1>(nthread*nblocks,nthread), kern);
        });
        }catch(cl::sycl::exception const &e) {
        std::cout << e.what() << std::endl;exit(-1);
        }

      }
      block_offset += Plan->ncolblk[col];
    }
    OP_kernels[17].transfer  += Plan->transfer;
    OP_kernels[17].transfer2 += Plan->transfer2;
  }
  op_mpi_set_dirtybit_cuda(nargs, args);
  op2_queue->wait();
  //update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[17].time     += wall_t2 - wall_t1;
}
