//
// auto-generated by op2.py
//

#include <cmath>
#include "const.h"
#include "structures.h"
#include "global.h"

void calculate_cell_volumes_omp4_kernel(
  int *map0,
  int map0size,
  double *data2,
  int dat2size,
  double *data0,
  int dat0size,
  double *data3,
  int dat3size,
  int *col_reord,
  int set_size1,
  int start,
  int end,
  int num_teams,
  int nthread,
  int opDat0_calculate_cell_volumes_stride_OP2CONSTANT,
  int direct_calculate_cell_volumes_stride_OP2CONSTANT){

  #pragma omp target teams num_teams(num_teams) thread_limit(nthread) map(to:data2[0:dat2size])\
    map(to:col_reord[0:set_size1],map0[0:map0size],data0[0:dat0size],data3[0:dat3size])
  #pragma omp distribute parallel for schedule(static,1)
  for ( int e=start; e<end; e++ ){
    int n_op = col_reord[e];
    int map0idx;
    int map1idx;
    map0idx = map0[n_op + set_size1 * 0];
    map1idx = map0[n_op + set_size1 * 1];

    //variable mapping
    const double* coords1 = &data0[map0idx];
    const double* coords2 = &data0[map1idx];
    double* ewt = &data2[n_op];
    double* vol1 = &data3[1 * map0idx];
    double* vol2 = &data3[1 * map1idx];

    //inline function
    
      double d[NDIM];
      double dist = 0.0;
      for (int i=0; i<NDIM; i++) {
          d[i] = coords2[(i)*opDat0_calculate_cell_volumes_stride_OP2CONSTANT] - coords1[(i)*opDat0_calculate_cell_volumes_stride_OP2CONSTANT];
          dist += d[i]*d[i];
      }
      dist = sqrt(dist);

      double area = 0.0;
      for (int i=0; i<NDIM; i++) {
          area += ewt[(i)*direct_calculate_cell_volumes_stride_OP2CONSTANT]*ewt[(i)*direct_calculate_cell_volumes_stride_OP2CONSTANT];
      }
      area = sqrt(area);

      double tetra_volume = (1.0/3.0)*0.5 *dist *area;
      *vol1 += tetra_volume;
      *vol2 += tetra_volume;

      for (int i=0; i<NDIM; i++) {
          ewt[(i)*direct_calculate_cell_volumes_stride_OP2CONSTANT] = (d[i] / dist) * area;
      }



      for (int i=0; i<NDIM; i++) {
          ewt[(i)*direct_calculate_cell_volumes_stride_OP2CONSTANT] /= dist;
      }
    //end inline func
  }

}
