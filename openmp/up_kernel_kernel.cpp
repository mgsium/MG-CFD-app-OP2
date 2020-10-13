//
// auto-generated by op2.py
//

//user function
#include ".././src/Kernels/mg.h"

// host stub function
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
  op_timing_realloc_manytime(16, omp_get_max_threads());
  op_timers_core(&cpu_t1, &wall_t1);
  double non_thread_walltime = 0.0;

  int  ninds   = 2;
  int  inds[3] = {-1,0,1};

  if (OP_diags>2) {
    printf(" kernel routine with indirection: up_kernel\n");
  }

  // get plan
  #ifdef OP_PART_SIZE_16
    int part_size = OP_PART_SIZE_16;
  #else
    int part_size = OP_part_size;
  #endif

  int set_size = op_mpi_halo_exchanges(set, nargs, args);

  if (set_size >0) {

    op_plan *Plan = op_plan_get_stage_upload(name,set,part_size,nargs,args,ninds,inds,OP_STAGE_ALL,0);

    // execute plan
    int block_offset = 0;
    for ( int col=0; col<Plan->ncolors; col++ ){
      if (col==Plan->ncolors_core) {
        op_mpi_wait_all(nargs, args);
      }
      int nblocks = Plan->ncolblk[col];

      // Pause process timing and switch to per-thread timing:
      op_timers_core(&cpu_t2, &wall_t2);
      non_thread_walltime += wall_t2 - wall_t1;
      #pragma omp parallel
      {
        double thr_wall_t1, thr_wall_t2, thr_cpu_t1, thr_cpu_t2;
        op_timers_core(&thr_cpu_t1, &thr_wall_t1);

        int nthreads = omp_get_num_threads();
        int thr = omp_get_thread_num();
        int thr_start = (nblocks * thr) / nthreads;
        int thr_end = (nblocks * (thr+1)) / nthreads;
        if (thr_end > nblocks) thr_end = nblocks;
        for ( int blockIdx=thr_start; blockIdx<thr_end; blockIdx++ ){
          int blockId  = Plan->blkmap[blockIdx + block_offset];
          int nelem    = Plan->nelems[blockId];
          int offset_b = Plan->offset[blockId];
          for ( int n=offset_b; n<offset_b+nelem; n++ ){
            int map1idx = arg1.map_data[n * arg1.map->dim + 0];


            up_kernel(
              &((double*)arg0.data)[5 * n],
              &((double*)arg1.data)[5 * map1idx],
              &((int*)arg2.data)[1 * map1idx]);
          }
        }

        op_timers_core(&thr_cpu_t2, &thr_wall_t2);
        OP_kernels[16].times[thr]  += thr_wall_t2 - thr_wall_t1;
      }

      // Revert to process-level timing:
      op_timers_core(&cpu_t1, &wall_t1);

      block_offset += nblocks;
    }
    OP_kernels[16].transfer  += Plan->transfer;
    OP_kernels[16].transfer2 += Plan->transfer2;
  }

  if (set_size == 0 || set_size == set->core_size) {
    op_mpi_wait_all(nargs, args);
  }
  // combine reduction data
  op_mpi_set_dirtybit(nargs, args);

  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  non_thread_walltime += wall_t2 - wall_t1;
  OP_kernels[16].name      = name;
  OP_kernels[16].count    += 1;
  OP_kernels[16].times[0] += non_thread_walltime;
}
