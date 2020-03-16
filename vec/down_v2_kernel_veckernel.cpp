//
// auto-generated by op2.py
//

//user function
//************************************************//
// Copyright 2016-2019 University of Warwick

// Permission is hereby granted, free of charge, to any person obtaining 
// a copy of this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
// sell copies of the Software, and to permit persons to whom the Software is furnished 
// to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included 
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR 
// IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//************************************************//

#ifndef MG_H
#define MG_H

#include <math.h>

#include "const.h"

inline void up_pre_kernel(
    double* variable, 
    int* up_scratch)
{
    variable[VAR_DENSITY] = 0.0;
    variable[VAR_MOMENTUM+0] = 0.0;
    variable[VAR_MOMENTUM+1] = 0.0;
    variable[VAR_MOMENTUM+2] = 0.0;
    variable[VAR_DENSITY_ENERGY] = 0.0;
    *up_scratch = 0;
}

inline void up_kernel(
    const double* variable, 
    double* variable_above, 
    int* up_scratch)
{
    variable_above[VAR_DENSITY]        += variable[VAR_DENSITY];
    variable_above[VAR_MOMENTUM+0]     += variable[VAR_MOMENTUM+0];
    variable_above[VAR_MOMENTUM+1]     += variable[VAR_MOMENTUM+1];
    variable_above[VAR_MOMENTUM+2]     += variable[VAR_MOMENTUM+2];
    variable_above[VAR_DENSITY_ENERGY] += variable[VAR_DENSITY_ENERGY];
    *up_scratch += 1;
}

inline void up_post_kernel(
    double* variable, 
    const int* up_scratch)
{
    double avg = (*up_scratch)==0 ? 1.0 : 1.0 / (double)(*up_scratch);
    variable[VAR_DENSITY] *= avg;
    variable[VAR_MOMENTUM+0] *= avg;
    variable[VAR_MOMENTUM+1] *= avg;
    variable[VAR_MOMENTUM+2] *= avg;
    variable[VAR_DENSITY_ENERGY] *= avg;
}

inline void down_kernel(
    double* variable, 
    const double* residual, 
    const double* coord, 
    const double* residual_above, 
    const double* coord_above)
{
    double dx = fabs(coord[0] - coord_above[0]);
    double dy = fabs(coord[1] - coord_above[1]);
    double dz = fabs(coord[2] - coord_above[2]);
    double dm = sqrt(dx*dx + dy*dy + dz*dz);

    variable[VAR_DENSITY]        -= dm* (residual_above[VAR_DENSITY]        - residual[VAR_DENSITY]);
    variable[VAR_MOMENTUM+0]     -= dx* (residual_above[VAR_MOMENTUM+0]     - residual[VAR_MOMENTUM+0]);
    variable[VAR_MOMENTUM+1]     -= dy* (residual_above[VAR_MOMENTUM+1]     - residual[VAR_MOMENTUM+1]);
    variable[VAR_MOMENTUM+2]     -= dz* (residual_above[VAR_MOMENTUM+2]     - residual[VAR_MOMENTUM+2]);
    variable[VAR_DENSITY_ENERGY] -= dm* (residual_above[VAR_DENSITY_ENERGY] - residual[VAR_DENSITY_ENERGY]);
}

inline void down_v2_kernel_pre(
    double* weight_sum, 
    double* residual_sum)
{
    *weight_sum = 0.0;
    residual_sum[VAR_DENSITY] = 0.0;
    residual_sum[VAR_MOMENTUM+0] = 0.0;
    residual_sum[VAR_MOMENTUM+2] = 0.0;
    residual_sum[VAR_MOMENTUM+1] = 0.0;
    residual_sum[VAR_DENSITY_ENERGY] = 0.0;
}
inline void down_v2_kernel(
    const double* coord2a, 
    const double* coord2b, 
    const double* coord1a, 
    const double* coord1b, 
    const double* residuals1a,
    const double* residuals1b,
    double* residuals1a_prolonged, 
    double* residuals1b_prolonged, 
    double* residuals1a_prolonged_wsum,
    double* residuals1b_prolonged_wsum)
{
    // For each node that has the same coordinates as its MG node parent, 
    // the 'prolonged residual' is simply taken directly from the MG node. 
    //
    // For each other node N, the 'prolonged residuals' is the weighted average 
    // across N's MG node and MG nodes of N's neighbours, requiring an 
    // edge-based loop. The weight is 1.0/distance.

    // Process a2:
    double dx_a1a2 = coord2a[0] - coord1a[0];
    double dy_a1a2 = coord2a[1] - coord1a[1];
    double dz_a1a2 = coord2a[2] - coord1a[2];
    if (dx_a1a2 == 0.0 && dy_a1a2 == 0.0 && dz_a1a2 == 0.0) {
        // a2 == a1:
        residuals1a_prolonged[VAR_DENSITY]        = residuals1a[VAR_DENSITY];
        residuals1a_prolonged[VAR_MOMENTUM+0]     = residuals1a[VAR_MOMENTUM+0];
        residuals1a_prolonged[VAR_MOMENTUM+1]     = residuals1a[VAR_MOMENTUM+1];
        residuals1a_prolonged[VAR_MOMENTUM+2]     = residuals1a[VAR_MOMENTUM+2];
        residuals1a_prolonged[VAR_DENSITY_ENERGY] = residuals1a[VAR_DENSITY_ENERGY];
        *residuals1a_prolonged_wsum = 1.0;
    } else {
        // Calculate contribution of a1 -> a2:
        const double idist_a1a2 = 1.0/sqrt(dx_a1a2*dx_a1a2 + dy_a1a2*dy_a1a2 + dz_a1a2*dz_a1a2);
        residuals1a_prolonged[VAR_DENSITY]        += idist_a1a2*residuals1a[VAR_DENSITY];
        residuals1a_prolonged[VAR_MOMENTUM+0]     += idist_a1a2*residuals1a[VAR_MOMENTUM+0];
        residuals1a_prolonged[VAR_MOMENTUM+1]     += idist_a1a2*residuals1a[VAR_MOMENTUM+1];
        residuals1a_prolonged[VAR_MOMENTUM+2]     += idist_a1a2*residuals1a[VAR_MOMENTUM+2];
        residuals1a_prolonged[VAR_DENSITY_ENERGY] += idist_a1a2*residuals1a[VAR_DENSITY_ENERGY];
        *residuals1a_prolonged_wsum += idist_a1a2;

        // Calculate contribution of b1 >- a2:
        double dx_b1a2 = coord1b[0] - coord2a[0];
        double dy_b1a2 = coord1b[1] - coord2a[1];
        double dz_b1a2 = coord1b[2] - coord2a[2];

        const double idist_b1a2 = 1.0/sqrt(dx_b1a2*dx_b1a2 + dy_b1a2*dy_b1a2 + dz_b1a2*dz_b1a2);
        residuals1a_prolonged[VAR_DENSITY]        += idist_b1a2*residuals1b[VAR_DENSITY];
        residuals1a_prolonged[VAR_MOMENTUM+0]     += idist_b1a2*residuals1b[VAR_MOMENTUM+0];
        residuals1a_prolonged[VAR_MOMENTUM+1]     += idist_b1a2*residuals1b[VAR_MOMENTUM+1];
        residuals1a_prolonged[VAR_MOMENTUM+2]     += idist_b1a2*residuals1b[VAR_MOMENTUM+2];
        residuals1a_prolonged[VAR_DENSITY_ENERGY] += idist_b1a2*residuals1b[VAR_DENSITY_ENERGY];
        *residuals1a_prolonged_wsum += idist_b1a2;
    }

    // Process b2:
    double dx_b1b2 = coord2b[0] - coord1b[0];
    double dy_b1b2 = coord2b[1] - coord1b[1];
    double dz_b1b2 = coord2b[2] - coord1b[2];
    if (dx_b1b2 == 0.0 && dy_b1b2 == 0.0 && dz_b1b2 == 0.0) {
        // b2 == b1:
        residuals1b_prolonged[VAR_DENSITY]        = residuals1b[VAR_DENSITY];
        residuals1b_prolonged[VAR_MOMENTUM+0]      = residuals1b[VAR_MOMENTUM+0];
        residuals1b_prolonged[VAR_MOMENTUM+1]      = residuals1b[VAR_MOMENTUM+1];
        residuals1b_prolonged[VAR_MOMENTUM+2]      = residuals1b[VAR_MOMENTUM+2];
        residuals1b_prolonged[VAR_DENSITY_ENERGY] = residuals1b[VAR_DENSITY_ENERGY];
        *residuals1b_prolonged_wsum = 1.0;
    } else {
        // Calculate contribution of b1 -> b2:
        const double idist_b1b2 = 1.0/sqrt(dx_b1b2*dx_b1b2 + dy_b1b2*dy_b1b2 + dz_b1b2*dz_b1b2);
        residuals1b_prolonged[VAR_DENSITY]        += idist_b1b2*residuals1b[VAR_DENSITY];
        residuals1b_prolonged[VAR_MOMENTUM+0]     += idist_b1b2*residuals1b[VAR_MOMENTUM+0];
        residuals1b_prolonged[VAR_MOMENTUM+1]     += idist_b1b2*residuals1b[VAR_MOMENTUM+1];
        residuals1b_prolonged[VAR_MOMENTUM+2]     += idist_b1b2*residuals1b[VAR_MOMENTUM+2];
        residuals1b_prolonged[VAR_DENSITY_ENERGY] += idist_b1b2*residuals1b[VAR_DENSITY_ENERGY];
        *residuals1b_prolonged_wsum += idist_b1b2;

        // Calculate contribution of a1 -> b2:
        double dx_a1b2 = coord1a[0] - coord2b[0];
        double dy_a1b2 = coord1a[1] - coord2b[1];
        double dz_a1b2 = coord1a[2] - coord2b[2];

        const double idist_a1b2 = 1.0/sqrt(dx_a1b2*dx_a1b2 + dy_a1b2*dy_a1b2 + dz_a1b2*dz_a1b2);
        residuals1b_prolonged[VAR_DENSITY]        += idist_a1b2*residuals1b[VAR_DENSITY];
        residuals1b_prolonged[VAR_MOMENTUM+0]     += idist_a1b2*residuals1b[VAR_MOMENTUM+0];
        residuals1b_prolonged[VAR_MOMENTUM+1]     += idist_a1b2*residuals1b[VAR_MOMENTUM+1];
        residuals1b_prolonged[VAR_MOMENTUM+2]     += idist_a1b2*residuals1b[VAR_MOMENTUM+2];
        residuals1b_prolonged[VAR_DENSITY_ENERGY] += idist_a1b2*residuals1b[VAR_DENSITY_ENERGY];
        *residuals1b_prolonged_wsum += idist_a1b2;
    }
}

inline void down_v2_kernel_post(
    const double* residuals1_prolonged, 
    const double* residuals1_prolonged_wsum, 
    const double* residuals2, 
    double* variables2)
{
    // Divide through by weight sum to complete the weighted average started by down_v2_kernel(), 
    // then apply the prolonged residual to grid:
    for (int i=0; i<NVAR; i++) {
        variables2[i] += residuals2[i] - (residuals1_prolonged[i] / (*residuals1_prolonged_wsum));
    }
}

#endif
#ifdef VECTORIZE
//user function -- modified for vectorisation
inline void down_v2_kernel_vec( const double coord2a[*][SIMD_VEC], const double coord2b[*][SIMD_VEC], const double coord1a[*][SIMD_VEC], const double coord1b[*][SIMD_VEC], const double residuals1a[*][SIMD_VEC], const double residuals1b[*][SIMD_VEC], double residuals1a_prolonged[*][SIMD_VEC], double residuals1b_prolonged[*][SIMD_VEC], double residuals1a_prolonged_wsum[*][SIMD_VEC], double residuals1b_prolonged_wsum[*][SIMD_VEC], int idx ) {







    double dx_a1a2 = coord2a[0][idx] - coord1a[0][idx];
    double dy_a1a2 = coord2a[1][idx] - coord1a[1][idx];
    double dz_a1a2 = coord2a[2][idx] - coord1a[2][idx];
    if (dx_a1a2 == 0.0 && dy_a1a2 == 0.0 && dz_a1a2 == 0.0) {

        residuals1a_prolonged[VAR_DENSITY][idx]        = residuals1a[VAR_DENSITY][idx];
        residuals1a_prolonged[VAR_MOMENTUM+0][idx]     = residuals1a[VAR_MOMENTUM+0][idx];
        residuals1a_prolonged[VAR_MOMENTUM+1][idx]     = residuals1a[VAR_MOMENTUM+1][idx];
        residuals1a_prolonged[VAR_MOMENTUM+2][idx]     = residuals1a[VAR_MOMENTUM+2][idx];
        residuals1a_prolonged[VAR_DENSITY_ENERGY][idx] = residuals1a[VAR_DENSITY_ENERGY][idx];
        residuals1a_prolonged_wsum[0][idx]= 1.0;
    } else {

        const double idist_a1a2 = 1.0/sqrt(dx_a1a2*dx_a1a2 + dy_a1a2*dy_a1a2 + dz_a1a2*dz_a1a2);
        residuals1a_prolonged[VAR_DENSITY][idx]        += idist_a1a2*residuals1a[VAR_DENSITY][idx];
        residuals1a_prolonged[VAR_MOMENTUM+0][idx]     += idist_a1a2*residuals1a[VAR_MOMENTUM+0][idx];
        residuals1a_prolonged[VAR_MOMENTUM+1][idx]     += idist_a1a2*residuals1a[VAR_MOMENTUM+1][idx];
        residuals1a_prolonged[VAR_MOMENTUM+2][idx]     += idist_a1a2*residuals1a[VAR_MOMENTUM+2][idx];
        residuals1a_prolonged[VAR_DENSITY_ENERGY][idx] += idist_a1a2*residuals1a[VAR_DENSITY_ENERGY][idx];
        residuals1a_prolonged_wsum[0][idx]+= idist_a1a2;

        double dx_b1a2 = coord1b[0][idx] - coord2a[0][idx];
        double dy_b1a2 = coord1b[1][idx] - coord2a[1][idx];
        double dz_b1a2 = coord1b[2][idx] - coord2a[2][idx];

        const double idist_b1a2 = 1.0/sqrt(dx_b1a2*dx_b1a2 + dy_b1a2*dy_b1a2 + dz_b1a2*dz_b1a2);
        residuals1a_prolonged[VAR_DENSITY][idx]        += idist_b1a2*residuals1b[VAR_DENSITY][idx];
        residuals1a_prolonged[VAR_MOMENTUM+0][idx]     += idist_b1a2*residuals1b[VAR_MOMENTUM+0][idx];
        residuals1a_prolonged[VAR_MOMENTUM+1][idx]     += idist_b1a2*residuals1b[VAR_MOMENTUM+1][idx];
        residuals1a_prolonged[VAR_MOMENTUM+2][idx]     += idist_b1a2*residuals1b[VAR_MOMENTUM+2][idx];
        residuals1a_prolonged[VAR_DENSITY_ENERGY][idx] += idist_b1a2*residuals1b[VAR_DENSITY_ENERGY][idx];
        residuals1a_prolonged_wsum[0][idx]+= idist_b1a2;
    }

    double dx_b1b2 = coord2b[0][idx] - coord1b[0][idx];
    double dy_b1b2 = coord2b[1][idx] - coord1b[1][idx];
    double dz_b1b2 = coord2b[2][idx] - coord1b[2][idx];
    if (dx_b1b2 == 0.0 && dy_b1b2 == 0.0 && dz_b1b2 == 0.0) {

        residuals1b_prolonged[VAR_DENSITY][idx]        = residuals1b[VAR_DENSITY][idx];
        residuals1b_prolonged[VAR_MOMENTUM+0][idx]      = residuals1b[VAR_MOMENTUM+0][idx];
        residuals1b_prolonged[VAR_MOMENTUM+1][idx]      = residuals1b[VAR_MOMENTUM+1][idx];
        residuals1b_prolonged[VAR_MOMENTUM+2][idx]      = residuals1b[VAR_MOMENTUM+2][idx];
        residuals1b_prolonged[VAR_DENSITY_ENERGY][idx] = residuals1b[VAR_DENSITY_ENERGY][idx];
        residuals1b_prolonged_wsum[0][idx]= 1.0;
    } else {

        const double idist_b1b2 = 1.0/sqrt(dx_b1b2*dx_b1b2 + dy_b1b2*dy_b1b2 + dz_b1b2*dz_b1b2);
        residuals1b_prolonged[VAR_DENSITY][idx]        += idist_b1b2*residuals1b[VAR_DENSITY][idx];
        residuals1b_prolonged[VAR_MOMENTUM+0][idx]     += idist_b1b2*residuals1b[VAR_MOMENTUM+0][idx];
        residuals1b_prolonged[VAR_MOMENTUM+1][idx]     += idist_b1b2*residuals1b[VAR_MOMENTUM+1][idx];
        residuals1b_prolonged[VAR_MOMENTUM+2][idx]     += idist_b1b2*residuals1b[VAR_MOMENTUM+2][idx];
        residuals1b_prolonged[VAR_DENSITY_ENERGY][idx] += idist_b1b2*residuals1b[VAR_DENSITY_ENERGY][idx];
        residuals1b_prolonged_wsum[0][idx]+= idist_b1b2;

        double dx_a1b2 = coord1a[0][idx] - coord2b[0][idx];
        double dy_a1b2 = coord1a[1][idx] - coord2b[1][idx];
        double dz_a1b2 = coord1a[2][idx] - coord2b[2][idx];

        const double idist_a1b2 = 1.0/sqrt(dx_a1b2*dx_a1b2 + dy_a1b2*dy_a1b2 + dz_a1b2*dz_a1b2);
        residuals1b_prolonged[VAR_DENSITY][idx]        += idist_a1b2*residuals1b[VAR_DENSITY][idx];
        residuals1b_prolonged[VAR_MOMENTUM+0][idx]     += idist_a1b2*residuals1b[VAR_MOMENTUM+0][idx];
        residuals1b_prolonged[VAR_MOMENTUM+1][idx]     += idist_a1b2*residuals1b[VAR_MOMENTUM+1][idx];
        residuals1b_prolonged[VAR_MOMENTUM+2][idx]     += idist_a1b2*residuals1b[VAR_MOMENTUM+2][idx];
        residuals1b_prolonged[VAR_DENSITY_ENERGY][idx] += idist_a1b2*residuals1b[VAR_DENSITY_ENERGY][idx];
        residuals1b_prolonged_wsum[0][idx]+= idist_a1b2;
    }

}
#endif

// host stub function
void op_par_loop_down_v2_kernel(char const *name, op_set set,
  op_arg arg0,
  op_arg arg1,
  op_arg arg2,
  op_arg arg3,
  op_arg arg4,
  op_arg arg5,
  op_arg arg6,
  op_arg arg7,
  op_arg arg8,
  op_arg arg9){

  int nargs = 10;
  op_arg args[10];

  args[0] = arg0;
  args[1] = arg1;
  args[2] = arg2;
  args[3] = arg3;
  args[4] = arg4;
  args[5] = arg5;
  args[6] = arg6;
  args[7] = arg7;
  args[8] = arg8;
  args[9] = arg9;
  //create aligned pointers for dats
  ALIGNED_double const double * __restrict__ ptr0 = (double *) arg0.data;
  __assume_aligned(ptr0,double_ALIGN);
  ALIGNED_double const double * __restrict__ ptr1 = (double *) arg1.data;
  __assume_aligned(ptr1,double_ALIGN);
  ALIGNED_double const double * __restrict__ ptr2 = (double *) arg2.data;
  __assume_aligned(ptr2,double_ALIGN);
  ALIGNED_double const double * __restrict__ ptr3 = (double *) arg3.data;
  __assume_aligned(ptr3,double_ALIGN);
  ALIGNED_double const double * __restrict__ ptr4 = (double *) arg4.data;
  __assume_aligned(ptr4,double_ALIGN);
  ALIGNED_double const double * __restrict__ ptr5 = (double *) arg5.data;
  __assume_aligned(ptr5,double_ALIGN);
  ALIGNED_double       double * __restrict__ ptr6 = (double *) arg6.data;
  __assume_aligned(ptr6,double_ALIGN);
  ALIGNED_double       double * __restrict__ ptr7 = (double *) arg7.data;
  __assume_aligned(ptr7,double_ALIGN);
  ALIGNED_double       double * __restrict__ ptr8 = (double *) arg8.data;
  __assume_aligned(ptr8,double_ALIGN);
  ALIGNED_double       double * __restrict__ ptr9 = (double *) arg9.data;
  __assume_aligned(ptr9,double_ALIGN);

  // initialise timers
  double cpu_t1, cpu_t2, wall_t1, wall_t2;
  op_timing_realloc(20);
  op_timers_core(&cpu_t1, &wall_t1);

  if (OP_diags>2) {
    printf(" kernel routine with indirection: down_v2_kernel\n");
  }

  int exec_size = op_mpi_halo_exchanges(set, nargs, args);

  if (exec_size >0) {

    #ifdef VECTORIZE
    #pragma novector
    for ( int n=0; n<(exec_size/SIMD_VEC)*SIMD_VEC; n+=SIMD_VEC ){
      if (n+SIMD_VEC >= set->core_size) {
        op_mpi_wait_all(nargs, args);
      }
      ALIGNED_double double dat0[3][SIMD_VEC];
      ALIGNED_double double dat1[3][SIMD_VEC];
      ALIGNED_double double dat2[3][SIMD_VEC];
      ALIGNED_double double dat3[3][SIMD_VEC];
      ALIGNED_double double dat4[5][SIMD_VEC];
      ALIGNED_double double dat5[5][SIMD_VEC];
      ALIGNED_double double dat6[5][SIMD_VEC];
      ALIGNED_double double dat7[5][SIMD_VEC];
      ALIGNED_double double dat8[1][SIMD_VEC];
      ALIGNED_double double dat9[1][SIMD_VEC];
      #pragma omp simd simdlen(SIMD_VEC)
      for ( int i=0; i<SIMD_VEC; i++ ){
        int idx0_3 = 3 * arg0.map_data[(n+i) * arg0.map->dim + 0];
        int idx1_3 = 3 * arg0.map_data[(n+i) * arg0.map->dim + 1];
        int idx2_3 = 3 * arg2.map_data[(n+i) * arg2.map->dim + 0];
        int idx3_3 = 3 * arg2.map_data[(n+i) * arg2.map->dim + 1];
        int idx4_5 = 5 * arg2.map_data[(n+i) * arg2.map->dim + 0];
        int idx5_5 = 5 * arg2.map_data[(n+i) * arg2.map->dim + 1];

        dat0[0][i] = (ptr0)[idx0_3 + 0];
        dat0[1][i] = (ptr0)[idx0_3 + 1];
        dat0[2][i] = (ptr0)[idx0_3 + 2];

        dat1[0][i] = (ptr1)[idx1_3 + 0];
        dat1[1][i] = (ptr1)[idx1_3 + 1];
        dat1[2][i] = (ptr1)[idx1_3 + 2];

        dat2[0][i] = (ptr2)[idx2_3 + 0];
        dat2[1][i] = (ptr2)[idx2_3 + 1];
        dat2[2][i] = (ptr2)[idx2_3 + 2];

        dat3[0][i] = (ptr3)[idx3_3 + 0];
        dat3[1][i] = (ptr3)[idx3_3 + 1];
        dat3[2][i] = (ptr3)[idx3_3 + 2];

        dat4[0][i] = (ptr4)[idx4_5 + 0];
        dat4[1][i] = (ptr4)[idx4_5 + 1];
        dat4[2][i] = (ptr4)[idx4_5 + 2];
        dat4[3][i] = (ptr4)[idx4_5 + 3];
        dat4[4][i] = (ptr4)[idx4_5 + 4];

        dat5[0][i] = (ptr5)[idx5_5 + 0];
        dat5[1][i] = (ptr5)[idx5_5 + 1];
        dat5[2][i] = (ptr5)[idx5_5 + 2];
        dat5[3][i] = (ptr5)[idx5_5 + 3];
        dat5[4][i] = (ptr5)[idx5_5 + 4];

        dat6[0][i] = 0.0;
        dat6[1][i] = 0.0;
        dat6[2][i] = 0.0;
        dat6[3][i] = 0.0;
        dat6[4][i] = 0.0;

        dat7[0][i] = 0.0;
        dat7[1][i] = 0.0;
        dat7[2][i] = 0.0;
        dat7[3][i] = 0.0;
        dat7[4][i] = 0.0;

        dat8[0][i] = 0.0;

        dat9[0][i] = 0.0;

      }
      #pragma omp simd simdlen(SIMD_VEC)
      for ( int i=0; i<SIMD_VEC; i++ ){
        down_v2_kernel_vec(
          dat0,
          dat1,
          dat2,
          dat3,
          dat4,
          dat5,
          dat6,
          dat7,
          dat8,
          dat9,
          i);
      }
      for ( int i=0; i<SIMD_VEC; i++ ){
        int idx6_5 = 5 * arg0.map_data[(n+i) * arg0.map->dim + 0];
        int idx7_5 = 5 * arg0.map_data[(n+i) * arg0.map->dim + 1];
        int idx8_1 = 1 * arg0.map_data[(n+i) * arg0.map->dim + 0];
        int idx9_1 = 1 * arg0.map_data[(n+i) * arg0.map->dim + 1];

        (ptr6)[idx6_5 + 0] += dat6[0][i];
        (ptr6)[idx6_5 + 1] += dat6[1][i];
        (ptr6)[idx6_5 + 2] += dat6[2][i];
        (ptr6)[idx6_5 + 3] += dat6[3][i];
        (ptr6)[idx6_5 + 4] += dat6[4][i];

        (ptr7)[idx7_5 + 0] += dat7[0][i];
        (ptr7)[idx7_5 + 1] += dat7[1][i];
        (ptr7)[idx7_5 + 2] += dat7[2][i];
        (ptr7)[idx7_5 + 3] += dat7[3][i];
        (ptr7)[idx7_5 + 4] += dat7[4][i];

        (ptr8)[idx8_1 + 0] += dat8[0][i];

        (ptr9)[idx9_1 + 0] += dat9[0][i];

      }
    }

    //remainder
    for ( int n=(exec_size/SIMD_VEC)*SIMD_VEC; n<exec_size; n++ ){
    #else
    for ( int n=0; n<exec_size; n++ ){
    #endif
      if (n==set->core_size) {
        op_mpi_wait_all(nargs, args);
      }
      int map0idx = arg0.map_data[n * arg0.map->dim + 0];
      int map1idx = arg0.map_data[n * arg0.map->dim + 1];
      int map2idx = arg2.map_data[n * arg2.map->dim + 0];
      int map3idx = arg2.map_data[n * arg2.map->dim + 1];

      down_v2_kernel(
        &(ptr0)[3 * map0idx],
        &(ptr1)[3 * map1idx],
        &(ptr2)[3 * map2idx],
        &(ptr3)[3 * map3idx],
        &(ptr4)[5 * map2idx],
        &(ptr5)[5 * map3idx],
        &(ptr6)[5 * map0idx],
        &(ptr7)[5 * map1idx],
        &(ptr8)[1 * map0idx],
        &(ptr9)[1 * map1idx]);
    }
  }

  if (exec_size == 0 || exec_size == set->core_size) {
    op_mpi_wait_all(nargs, args);
  }
  // combine reduction data
  op_mpi_set_dirtybit(nargs, args);

  // update kernel record
  op_timers_core(&cpu_t2, &wall_t2);
  OP_kernels[20].name      = name;
  OP_kernels[20].count    += 1;
  OP_kernels[20].time     += wall_t2 - wall_t1;
  OP_kernels[20].transfer += (float)set->size * arg0.size;
  OP_kernels[20].transfer += (float)set->size * arg2.size;
  OP_kernels[20].transfer += (float)set->size * arg4.size;
  OP_kernels[20].transfer += (float)set->size * arg6.size * 2.0f;
  OP_kernels[20].transfer += (float)set->size * arg8.size * 2.0f;
  OP_kernels[20].transfer += (float)set->size * arg0.map->dim * 4.0f;
  OP_kernels[20].transfer += (float)set->size * arg2.map->dim * 4.0f;
}
