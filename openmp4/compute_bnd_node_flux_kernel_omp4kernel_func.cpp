//
// auto-generated by op2.py
//

#include <math.h>
#include "inlined_funcs.h"
#include "global.h"
#include "config.h"

void compute_bnd_node_flux_kernel_omp4_kernel(
  int *data0,
  int dat0size,
  double *data1,
  int dat1size,
  int *map2,
  int map2size,
  double *data2,
  int dat2size,
  double *data3,
  int dat3size,
  int *col_reord,
  int set_size1,
  int start,
  int end,
  int num_teams,
  int nthread){

  #pragma omp target teams num_teams(num_teams) thread_limit(nthread) map(to:data0[0:dat0size],data1[0:dat1size]) \
    map(to: ff_variable_ompkernel[:5], ff_flux_contribution_momentum_x_ompkernel[:3], ff_flux_contribution_momentum_y_ompkernel[:3], ff_flux_contribution_momentum_z_ompkernel[:3], ff_flux_contribution_density_energy_ompkernel[:3])\
    map(to:col_reord[0:set_size1],map2[0:map2size],data2[0:dat2size],data3[0:dat3size])
  #pragma omp distribute parallel for schedule(static,1)
  for ( int e=start; e<end; e++ ){
    int n_op = col_reord[e];
    int map2idx = map2[n_op + set_size1 * 0];

    //variable mapping
    const int *g = &data0[1*n_op];
    const double *edge_weight = &data1[3*n_op];
    const double *variables_b = &data2[5 * map2idx];
    double *fluxes_b = &data3[5 * map2idx];

    //inline function
    








      if ((*g) <= 2) {

        
        
        
        
        
        
        
        
            double p_b = variables_b[VAR_DENSITY];
        
            #ifdef IDIVIDE
            double ip_b = 1.0 / p_b;
            #endif
        
            double pe_b, pressure_b;
            double3 velocity_b, momentum_b;
            double flux_contribution_i_momentum_x_b[NDIM],
                   flux_contribution_i_momentum_y_b[NDIM],
                   flux_contribution_i_momentum_z_b[NDIM],
                   flux_contribution_i_density_energy_b[NDIM];
        
            momentum_b.x = variables_b[VAR_MOMENTUM+0];
            momentum_b.y = variables_b[VAR_MOMENTUM+1];
            momentum_b.z = variables_b[VAR_MOMENTUM+2];
            pe_b = variables_b[VAR_DENSITY_ENERGY];
        
            #ifdef IDIVIDE
            compute_velocity(ip_b, momentum_b, velocity_b);
            #else
            compute_velocity(p_b, momentum_b, velocity_b);
            #endif
        
            double speed_sqd_b = compute_speed_sqd(velocity_b);
            double speed_b = std::sqrt(speed_sqd_b);
            pressure_b = compute_pressure(p_b, pe_b, speed_sqd_b);
        
            #ifdef IDIVIDE
            double speed_of_sound_b = compute_speed_of_sound(ip_b, pressure_b);
            #else
            double speed_of_sound_b = compute_speed_of_sound(p_b, pressure_b);
            #endif
        
            compute_flux_contribution(p_b, momentum_b, pe_b,
                pressure_b, velocity_b,
                flux_contribution_i_momentum_x_b,
                flux_contribution_i_momentum_y_b,
                flux_contribution_i_momentum_z_b,
                flux_contribution_i_density_energy_b);
        
            fluxes_b[VAR_DENSITY]        += 0;
            fluxes_b[VAR_MOMENTUM +0]    += edge_weight[0]*pressure_b;
            fluxes_b[VAR_MOMENTUM +1]    += edge_weight[1]*pressure_b;
            fluxes_b[VAR_MOMENTUM +2]    += edge_weight[2]*pressure_b;
            fluxes_b[VAR_DENSITY_ENERGY] += 0;
        

      } else if ((*g) == 3 || ((*g) >= 4 && (*g) <= 7) ) {


        
        
        
        
        
        
        
            double p_b = variables_b[VAR_DENSITY];
        
            #ifdef IDIVIDE
            double ip_b = 1.0 / p_b;
            #endif
        
            double pe_b, pressure_b;
            double3 velocity_b, momentum_b;
            double flux_contribution_i_momentum_x_b[NDIM],
                   flux_contribution_i_momentum_y_b[NDIM],
                   flux_contribution_i_momentum_z_b[NDIM],
                   flux_contribution_i_density_energy_b[NDIM];
        
            momentum_b.x = variables_b[VAR_MOMENTUM+0];
            momentum_b.y = variables_b[VAR_MOMENTUM+1];
            momentum_b.z = variables_b[VAR_MOMENTUM+2];
            pe_b = variables_b[VAR_DENSITY_ENERGY];
        
            #ifdef IDIVIDE
            compute_velocity(ip_b, momentum_b, velocity_b);
            #else
            compute_velocity(p_b, momentum_b, velocity_b);
            #endif
        
            double speed_sqd_b = compute_speed_sqd(velocity_b);
            double speed_b = std::sqrt(speed_sqd_b);
            pressure_b = compute_pressure(p_b, pe_b, speed_sqd_b);
        
            #ifdef IDIVIDE
            double speed_of_sound_b = compute_speed_of_sound(ip_b, pressure_b);
            #else
            double speed_of_sound_b = compute_speed_of_sound(p_b, pressure_b);
            #endif
        
            compute_flux_contribution(p_b, momentum_b, pe_b,
                                      pressure_b, velocity_b,
                                      flux_contribution_i_momentum_x_b,
                                      flux_contribution_i_momentum_y_b,
                                      flux_contribution_i_momentum_z_b,
                                      flux_contribution_i_density_energy_b);
        
            double factor_x = 0.5 * edge_weight[0],
                   factor_y = 0.5 * edge_weight[1],
                   factor_z = 0.5 * edge_weight[2];
        
            fluxes_b[VAR_DENSITY] +=
                  factor_x*(ff_variable_ompkernel[VAR_MOMENTUM+0] + momentum_b.x)
                + factor_y*(ff_variable_ompkernel[VAR_MOMENTUM+1] + momentum_b.y)
                + factor_z*(ff_variable_ompkernel[VAR_MOMENTUM+2] + momentum_b.z);
        
            fluxes_b[VAR_DENSITY_ENERGY] += 
                  factor_x*(ff_flux_contribution_density_energy_ompkernel[0] + flux_contribution_i_density_energy_b[0])
                + factor_y*(ff_flux_contribution_density_energy_ompkernel[1] + flux_contribution_i_density_energy_b[1])
                + factor_z*(ff_flux_contribution_density_energy_ompkernel[2] + flux_contribution_i_density_energy_b[2]);
        
            fluxes_b[VAR_MOMENTUM + 0] += 
                  factor_x*(ff_flux_contribution_momentum_x_ompkernel[0] + flux_contribution_i_momentum_x_b[0])
                + factor_y*(ff_flux_contribution_momentum_x_ompkernel[1] + flux_contribution_i_momentum_x_b[1])
                + factor_z*(ff_flux_contribution_momentum_x_ompkernel[2] + flux_contribution_i_momentum_x_b[2]);
        
            fluxes_b[VAR_MOMENTUM + 1] += 
                  factor_x*(ff_flux_contribution_momentum_y_ompkernel[0] + flux_contribution_i_momentum_y_b[0])
                + factor_y*(ff_flux_contribution_momentum_y_ompkernel[1] + flux_contribution_i_momentum_y_b[1])
                + factor_z*(ff_flux_contribution_momentum_y_ompkernel[2] + flux_contribution_i_momentum_y_b[2]);
        
            fluxes_b[VAR_MOMENTUM + 2] += 
                  factor_x*(ff_flux_contribution_momentum_z_ompkernel[0] + flux_contribution_i_momentum_z_b[0])
                + factor_y*(ff_flux_contribution_momentum_z_ompkernel[1] + flux_contribution_i_momentum_z_b[1])
                + factor_z*(ff_flux_contribution_momentum_z_ompkernel[2] + flux_contribution_i_momentum_z_b[2]);
        
      }
    //end inline func
  }

}
