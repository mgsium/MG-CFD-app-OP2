//
// auto-generated by op2.py
//

#include <cmath>
#include "const.h"
#include "structures.h"
#include "global.h"

//user function
class calculate_cell_volumes_kernel;

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

  //get plan
  #ifdef OP_PART_SIZE_3
    int part_size = OP_PART_SIZE_3;
  #else
    int part_size = OP_part_size;
  #endif

  op_mpi_halo_exchanges_cuda(set, nargs, args);
  if (set->size > 0) {

    op_plan *Plan = op_plan_get_stage(name,set,part_size,nargs,args,ninds,inds,OP_STAGE_INC);

    cl::sycl::buffer<double,1> *arg0_buffer = static_cast<cl::sycl::buffer<double,1>*>((void*)arg0.data_d);
    cl::sycl::buffer<double,1> *arg3_buffer = static_cast<cl::sycl::buffer<double,1>*>((void*)arg3.data_d);
    cl::sycl::buffer<int,1> *map0_buffer = static_cast<cl::sycl::buffer<int,1>*>((void*)arg0.map_data_d);
    cl::sycl::buffer<double,1> *arg2_buffer = static_cast<cl::sycl::buffer<double,1>*>((void*)arg2.data_d);
    cl::sycl::buffer<int,1> *ind_map_buffer = static_cast<cl::sycl::buffer<int,1>*>((void*)Plan->ind_map);
    cl::sycl::buffer<short,1> *arg_map_buffer = static_cast<cl::sycl::buffer<short,1>*>((void*)Plan->loc_map);
    cl::sycl::buffer<int,1> *ind_arg_sizes_buffer = static_cast<cl::sycl::buffer<int,1>*>((void*)Plan->ind_sizes);
    cl::sycl::buffer<int,1> *ind_arg_offs_buffer = static_cast<cl::sycl::buffer<int,1>*>((void*)Plan->ind_offs);
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
      #ifdef OP_BLOCK_SIZE_3
      int nthread = OP_BLOCK_SIZE_3;
      #else
      int nthread = OP_block_size;
      #endif

      int nblocks = Plan->ncolblk[col];
      if (Plan->ncolblk[col] > 0) {

        int ind_arg1_shmem = Plan->nsharedColInd[col+Plan->ncolors*0];
        try {
        op2_queue->submit([&](cl::sycl::handler& cgh) {
          auto ind_arg0 = (*arg0_buffer).template get_access<cl::sycl::access::mode::read_write>(cgh);
          auto ind_arg1 = (*arg3_buffer).template get_access<cl::sycl::access::mode::read_write>(cgh);
          auto opDat0Map =  (*map0_buffer).template get_access<cl::sycl::access::mode::read>(cgh);
          auto ind_map = (*ind_map_buffer).template get_access<cl::sycl::access::mode::read>(cgh);
          auto arg_map = (*arg_map_buffer).template get_access<cl::sycl::access::mode::read>(cgh);
          auto ind_arg_sizes = (*ind_arg_sizes_buffer).template get_access<cl::sycl::access::mode::read>(cgh);
          auto ind_arg_offs = (*ind_arg_offs_buffer).template get_access<cl::sycl::access::mode::read>(cgh);
          auto blkmap    = (*blkmap_buffer).template get_access<cl::sycl::access::mode::read>(cgh);
          auto offset    = (*offset_buffer).template get_access<cl::sycl::access::mode::read>(cgh);
          auto nelems    = (*nelems_buffer).template get_access<cl::sycl::access::mode::read>(cgh);
          auto ncolors   = (*ncolors_buffer).template get_access<cl::sycl::access::mode::read>(cgh);
          auto colors    = (*colors_buffer).template get_access<cl::sycl::access::mode::read>(cgh);

          auto arg2 = (*arg2_buffer).template get_access<cl::sycl::access::mode::read_write>(cgh);

          cl::sycl::accessor<double, 1, cl::sycl::access::mode::read_write,
             cl::sycl::access::target::local> ind_arg1_s(ind_arg1_shmem, cgh);


          //user fun as lambda
          auto calculate_cell_volumes_gpu = [=]( 
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
                dist = cl::sycl::sqrt(dist);
            
                double area = 0.0;
                for (int i=0; i<NDIM; i++) {
                    area += ewt[i]*ewt[i];
                }
                area = cl::sycl::sqrt(area);
            
                double tetra_volume = (1.0/3.0)*0.5 *dist *area;
                *vol1 += tetra_volume;
                *vol2 += tetra_volume;
            
                for (int i=0; i<NDIM; i++) {
                    ewt[i] = (d[i] / dist) * area;
                }
            
            
            
                for (int i=0; i<NDIM; i++) {
                    ewt[i] /= dist;
                }
            
            };
            
          auto kern = [=](cl::sycl::nd_item<1> item) {
            double arg3_l[1];
            double arg4_l[1];


            //get sizes and shift pointers and direct-mapped data

            int blockId = blkmap[item.get_group_linear_id()  + block_offset];

            int nelem    = nelems[blockId];
            int offset_b = offset[blockId];

            int nelems2  = item.get_local_range()[0]*(1+(nelem-1)/item.get_local_range()[0]);
            int ncolor   = ncolors[blockId];

            int ind_arg1_size = ind_arg_sizes[0+blockId*1];

            int ind_arg1_map = 0*set_size + ind_arg_offs[0+blockId*1];


            for ( int n=item.get_local_id(0); n<ind_arg1_size*1; n+=item.get_local_range()[0] ){
              ind_arg1_s[n] = ZERO_double;
            }

            item.barrier(cl::sycl::access::fence_space::local_space);

            for ( int n=item.get_local_id(0); n<nelems2; n+=item.get_local_range()[0] ){
              int col2 = -1;
              int map0idx;
              int map1idx;
              if (n<nelem) {
                //initialise local variables
                for ( int d=0; d<1; d++ ){
                  arg3_l[d] = ZERO_double;
                }
                for ( int d=0; d<1; d++ ){
                  arg4_l[d] = ZERO_double;
                }
                map0idx = opDat0Map[n + offset_b + set_size * 0];
                map1idx = opDat0Map[n + offset_b + set_size * 1];


                //user-supplied kernel call
                calculate_cell_volumes_gpu(&ind_arg0[map0idx*3],
                           &ind_arg0[map1idx*3],
                           &arg2[(n+offset_b)*3],
                           arg3_l,
                           arg4_l);
                col2 = colors[n+offset_b];
              }

              //store local variables

              int arg3_map;
              int arg4_map;
              if (col2>=0) {
                arg3_map = arg_map[0*set_size+n+offset_b];
                arg4_map = arg_map[1*set_size+n+offset_b];
              }

              for ( int col=0; col<ncolor; col++ ){
                if (col2==col) {
                  arg3_l[0] += ind_arg1_s[0+arg3_map*1];
                  ind_arg1_s[0+arg3_map*1] = arg3_l[0];
                  arg4_l[0] += ind_arg1_s[0+arg4_map*1];
                  ind_arg1_s[0+arg4_map*1] = arg4_l[0];
                }
                item.barrier(cl::sycl::access::fence_space::local_space);
              }
            }
            for ( int n=item.get_local_id(0); n<ind_arg1_size*1; n+=item.get_local_range()[0] ){
              ind_arg1[n%1+ind_map[ind_arg1_map+n/1]*1] += ind_arg1_s[n];
            }

          };
          cgh.parallel_for<class calculate_cell_volumes_kernel>(cl::sycl::nd_range<1>(nthread*nblocks,nthread), kern);
        });
        }catch(cl::sycl::exception const &e) {
        std::cout << e.what() << std::endl;exit(-1);
        }

      }
      block_offset += Plan->ncolblk[col];
    }
    OP_kernels[3].transfer  += Plan->transfer;
    OP_kernels[3].transfer2 += Plan->transfer2;
  }
  op_mpi_set_dirtybit_cuda(nargs, args);
  op2_queue->wait();
  //update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[3].time     += wall_t2 - wall_t1;
}
