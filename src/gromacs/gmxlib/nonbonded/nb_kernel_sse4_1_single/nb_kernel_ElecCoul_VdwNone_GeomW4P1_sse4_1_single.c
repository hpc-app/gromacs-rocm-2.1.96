/*
 * This file is part of the GROMACS molecular simulation package.
 *
 * Copyright (c) 2012,2013,2014,2015,2017, by the GROMACS development team, led by
 * Mark Abraham, David van der Spoel, Berk Hess, and Erik Lindahl,
 * and including many others, as listed in the AUTHORS file in the
 * top-level source directory and at http://www.gromacs.org.
 *
 * GROMACS is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * GROMACS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with GROMACS; if not, see
 * http://www.gnu.org/licenses, or write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.
 *
 * If you want to redistribute modifications to GROMACS, please
 * consider that scientific software is very special. Version
 * control is crucial - bugs must be traceable. We will be happy to
 * consider code for inclusion in the official distribution, but
 * derived work must not be called official GROMACS. Details are found
 * in the README & COPYING files - if they are missing, get the
 * official version at http://www.gromacs.org.
 *
 * To help us fund GROMACS development, we humbly ask that you cite
 * the research papers on the package. Check out http://www.gromacs.org.
 */
/*
 * Note: this file was generated by the GROMACS sse4_1_single kernel generator.
 */
#include "gmxpre.h"

#include "config.h"

#include <math.h>

#include "../nb_kernel.h"
#include "gromacs/gmxlib/nrnb.h"

#include "kernelutil_x86_sse4_1_single.h"

/*
 * Gromacs nonbonded kernel:   nb_kernel_ElecCoul_VdwNone_GeomW4P1_VF_sse4_1_single
 * Electrostatics interaction: Coulomb
 * VdW interaction:            None
 * Geometry:                   Water4-Particle
 * Calculate force/pot:        PotentialAndForce
 */
void
nb_kernel_ElecCoul_VdwNone_GeomW4P1_VF_sse4_1_single
                    (t_nblist                    * gmx_restrict       nlist,
                     rvec                        * gmx_restrict          xx,
                     rvec                        * gmx_restrict          ff,
                     struct t_forcerec           * gmx_restrict          fr,
                     t_mdatoms                   * gmx_restrict     mdatoms,
                     nb_kernel_data_t gmx_unused * gmx_restrict kernel_data,
                     t_nrnb                      * gmx_restrict        nrnb)
{
    /* Suffixes 0,1,2,3 refer to particle indices for waters in the inner or outer loop, or 
     * just 0 for non-waters.
     * Suffixes A,B,C,D refer to j loop unrolling done with SSE, e.g. for the four different
     * jnr indices corresponding to data put in the four positions in the SIMD register.
     */
    int              i_shift_offset,i_coord_offset,outeriter,inneriter;
    int              j_index_start,j_index_end,jidx,nri,inr,ggid,iidx;
    int              jnrA,jnrB,jnrC,jnrD;
    int              jnrlistA,jnrlistB,jnrlistC,jnrlistD;
    int              j_coord_offsetA,j_coord_offsetB,j_coord_offsetC,j_coord_offsetD;
    int              *iinr,*jindex,*jjnr,*shiftidx,*gid;
    real             rcutoff_scalar;
    real             *shiftvec,*fshift,*x,*f;
    real             *fjptrA,*fjptrB,*fjptrC,*fjptrD;
    real             scratch[4*DIM];
    __m128           tx,ty,tz,fscal,rcutoff,rcutoff2,jidxall;
    int              vdwioffset1;
    __m128           ix1,iy1,iz1,fix1,fiy1,fiz1,iq1,isai1;
    int              vdwioffset2;
    __m128           ix2,iy2,iz2,fix2,fiy2,fiz2,iq2,isai2;
    int              vdwioffset3;
    __m128           ix3,iy3,iz3,fix3,fiy3,fiz3,iq3,isai3;
    int              vdwjidx0A,vdwjidx0B,vdwjidx0C,vdwjidx0D;
    __m128           jx0,jy0,jz0,fjx0,fjy0,fjz0,jq0,isaj0;
    __m128           dx10,dy10,dz10,rsq10,rinv10,rinvsq10,r10,qq10,c6_10,c12_10;
    __m128           dx20,dy20,dz20,rsq20,rinv20,rinvsq20,r20,qq20,c6_20,c12_20;
    __m128           dx30,dy30,dz30,rsq30,rinv30,rinvsq30,r30,qq30,c6_30,c12_30;
    __m128           velec,felec,velecsum,facel,crf,krf,krf2;
    real             *charge;
    __m128           dummy_mask,cutoff_mask;
    __m128           signbit = _mm_castsi128_ps( _mm_set1_epi32(0x80000000) );
    __m128           one     = _mm_set1_ps(1.0);
    __m128           two     = _mm_set1_ps(2.0);
    x                = xx[0];
    f                = ff[0];

    nri              = nlist->nri;
    iinr             = nlist->iinr;
    jindex           = nlist->jindex;
    jjnr             = nlist->jjnr;
    shiftidx         = nlist->shift;
    gid              = nlist->gid;
    shiftvec         = fr->shift_vec[0];
    fshift           = fr->fshift[0];
    facel            = _mm_set1_ps(fr->ic->epsfac);
    charge           = mdatoms->chargeA;

    /* Setup water-specific parameters */
    inr              = nlist->iinr[0];
    iq1              = _mm_mul_ps(facel,_mm_set1_ps(charge[inr+1]));
    iq2              = _mm_mul_ps(facel,_mm_set1_ps(charge[inr+2]));
    iq3              = _mm_mul_ps(facel,_mm_set1_ps(charge[inr+3]));

    /* Avoid stupid compiler warnings */
    jnrA = jnrB = jnrC = jnrD = 0;
    j_coord_offsetA = 0;
    j_coord_offsetB = 0;
    j_coord_offsetC = 0;
    j_coord_offsetD = 0;

    outeriter        = 0;
    inneriter        = 0;

    for(iidx=0;iidx<4*DIM;iidx++)
    {
        scratch[iidx] = 0.0;
    }

    /* Start outer loop over neighborlists */
    for(iidx=0; iidx<nri; iidx++)
    {
        /* Load shift vector for this list */
        i_shift_offset   = DIM*shiftidx[iidx];

        /* Load limits for loop over neighbors */
        j_index_start    = jindex[iidx];
        j_index_end      = jindex[iidx+1];

        /* Get outer coordinate index */
        inr              = iinr[iidx];
        i_coord_offset   = DIM*inr;

        /* Load i particle coords and add shift vector */
        gmx_mm_load_shift_and_3rvec_broadcast_ps(shiftvec+i_shift_offset,x+i_coord_offset+DIM,
                                                 &ix1,&iy1,&iz1,&ix2,&iy2,&iz2,&ix3,&iy3,&iz3);

        fix1             = _mm_setzero_ps();
        fiy1             = _mm_setzero_ps();
        fiz1             = _mm_setzero_ps();
        fix2             = _mm_setzero_ps();
        fiy2             = _mm_setzero_ps();
        fiz2             = _mm_setzero_ps();
        fix3             = _mm_setzero_ps();
        fiy3             = _mm_setzero_ps();
        fiz3             = _mm_setzero_ps();

        /* Reset potential sums */
        velecsum         = _mm_setzero_ps();

        /* Start inner kernel loop */
        for(jidx=j_index_start; jidx<j_index_end && jjnr[jidx+3]>=0; jidx+=4)
        {

            /* Get j neighbor index, and coordinate index */
            jnrA             = jjnr[jidx];
            jnrB             = jjnr[jidx+1];
            jnrC             = jjnr[jidx+2];
            jnrD             = jjnr[jidx+3];
            j_coord_offsetA  = DIM*jnrA;
            j_coord_offsetB  = DIM*jnrB;
            j_coord_offsetC  = DIM*jnrC;
            j_coord_offsetD  = DIM*jnrD;

            /* load j atom coordinates */
            gmx_mm_load_1rvec_4ptr_swizzle_ps(x+j_coord_offsetA,x+j_coord_offsetB,
                                              x+j_coord_offsetC,x+j_coord_offsetD,
                                              &jx0,&jy0,&jz0);

            /* Calculate displacement vector */
            dx10             = _mm_sub_ps(ix1,jx0);
            dy10             = _mm_sub_ps(iy1,jy0);
            dz10             = _mm_sub_ps(iz1,jz0);
            dx20             = _mm_sub_ps(ix2,jx0);
            dy20             = _mm_sub_ps(iy2,jy0);
            dz20             = _mm_sub_ps(iz2,jz0);
            dx30             = _mm_sub_ps(ix3,jx0);
            dy30             = _mm_sub_ps(iy3,jy0);
            dz30             = _mm_sub_ps(iz3,jz0);

            /* Calculate squared distance and things based on it */
            rsq10            = gmx_mm_calc_rsq_ps(dx10,dy10,dz10);
            rsq20            = gmx_mm_calc_rsq_ps(dx20,dy20,dz20);
            rsq30            = gmx_mm_calc_rsq_ps(dx30,dy30,dz30);

            rinv10           = sse41_invsqrt_f(rsq10);
            rinv20           = sse41_invsqrt_f(rsq20);
            rinv30           = sse41_invsqrt_f(rsq30);

            rinvsq10         = _mm_mul_ps(rinv10,rinv10);
            rinvsq20         = _mm_mul_ps(rinv20,rinv20);
            rinvsq30         = _mm_mul_ps(rinv30,rinv30);

            /* Load parameters for j particles */
            jq0              = gmx_mm_load_4real_swizzle_ps(charge+jnrA+0,charge+jnrB+0,
                                                              charge+jnrC+0,charge+jnrD+0);

            fjx0             = _mm_setzero_ps();
            fjy0             = _mm_setzero_ps();
            fjz0             = _mm_setzero_ps();

            /**************************
             * CALCULATE INTERACTIONS *
             **************************/

            /* Compute parameters for interactions between i and j atoms */
            qq10             = _mm_mul_ps(iq1,jq0);

            /* COULOMB ELECTROSTATICS */
            velec            = _mm_mul_ps(qq10,rinv10);
            felec            = _mm_mul_ps(velec,rinvsq10);

            /* Update potential sum for this i atom from the interaction with this j atom. */
            velecsum         = _mm_add_ps(velecsum,velec);

            fscal            = felec;

            /* Calculate temporary vectorial force */
            tx               = _mm_mul_ps(fscal,dx10);
            ty               = _mm_mul_ps(fscal,dy10);
            tz               = _mm_mul_ps(fscal,dz10);

            /* Update vectorial force */
            fix1             = _mm_add_ps(fix1,tx);
            fiy1             = _mm_add_ps(fiy1,ty);
            fiz1             = _mm_add_ps(fiz1,tz);

            fjx0             = _mm_add_ps(fjx0,tx);
            fjy0             = _mm_add_ps(fjy0,ty);
            fjz0             = _mm_add_ps(fjz0,tz);

            /**************************
             * CALCULATE INTERACTIONS *
             **************************/

            /* Compute parameters for interactions between i and j atoms */
            qq20             = _mm_mul_ps(iq2,jq0);

            /* COULOMB ELECTROSTATICS */
            velec            = _mm_mul_ps(qq20,rinv20);
            felec            = _mm_mul_ps(velec,rinvsq20);

            /* Update potential sum for this i atom from the interaction with this j atom. */
            velecsum         = _mm_add_ps(velecsum,velec);

            fscal            = felec;

            /* Calculate temporary vectorial force */
            tx               = _mm_mul_ps(fscal,dx20);
            ty               = _mm_mul_ps(fscal,dy20);
            tz               = _mm_mul_ps(fscal,dz20);

            /* Update vectorial force */
            fix2             = _mm_add_ps(fix2,tx);
            fiy2             = _mm_add_ps(fiy2,ty);
            fiz2             = _mm_add_ps(fiz2,tz);

            fjx0             = _mm_add_ps(fjx0,tx);
            fjy0             = _mm_add_ps(fjy0,ty);
            fjz0             = _mm_add_ps(fjz0,tz);

            /**************************
             * CALCULATE INTERACTIONS *
             **************************/

            /* Compute parameters for interactions between i and j atoms */
            qq30             = _mm_mul_ps(iq3,jq0);

            /* COULOMB ELECTROSTATICS */
            velec            = _mm_mul_ps(qq30,rinv30);
            felec            = _mm_mul_ps(velec,rinvsq30);

            /* Update potential sum for this i atom from the interaction with this j atom. */
            velecsum         = _mm_add_ps(velecsum,velec);

            fscal            = felec;

            /* Calculate temporary vectorial force */
            tx               = _mm_mul_ps(fscal,dx30);
            ty               = _mm_mul_ps(fscal,dy30);
            tz               = _mm_mul_ps(fscal,dz30);

            /* Update vectorial force */
            fix3             = _mm_add_ps(fix3,tx);
            fiy3             = _mm_add_ps(fiy3,ty);
            fiz3             = _mm_add_ps(fiz3,tz);

            fjx0             = _mm_add_ps(fjx0,tx);
            fjy0             = _mm_add_ps(fjy0,ty);
            fjz0             = _mm_add_ps(fjz0,tz);

            fjptrA             = f+j_coord_offsetA;
            fjptrB             = f+j_coord_offsetB;
            fjptrC             = f+j_coord_offsetC;
            fjptrD             = f+j_coord_offsetD;

	    gmx_mm_decrement_1rvec_4ptr_swizzle_ps(fjptrA,fjptrB,fjptrC,fjptrD,fjx0,fjy0,fjz0);

            /* Inner loop uses 84 flops */
        }

        if(jidx<j_index_end)
        {

            /* Get j neighbor index, and coordinate index */
            jnrlistA         = jjnr[jidx];
            jnrlistB         = jjnr[jidx+1];
            jnrlistC         = jjnr[jidx+2];
            jnrlistD         = jjnr[jidx+3];
            /* Sign of each element will be negative for non-real atoms.
             * This mask will be 0xFFFFFFFF for dummy entries and 0x0 for real ones,
             * so use it as val = _mm_andnot_ps(mask,val) to clear dummy entries.
             */
            dummy_mask = gmx_mm_castsi128_ps(_mm_cmplt_epi32(_mm_loadu_si128((const __m128i *)(jjnr+jidx)),_mm_setzero_si128()));
            jnrA       = (jnrlistA>=0) ? jnrlistA : 0;
            jnrB       = (jnrlistB>=0) ? jnrlistB : 0;
            jnrC       = (jnrlistC>=0) ? jnrlistC : 0;
            jnrD       = (jnrlistD>=0) ? jnrlistD : 0;
            j_coord_offsetA  = DIM*jnrA;
            j_coord_offsetB  = DIM*jnrB;
            j_coord_offsetC  = DIM*jnrC;
            j_coord_offsetD  = DIM*jnrD;

            /* load j atom coordinates */
            gmx_mm_load_1rvec_4ptr_swizzle_ps(x+j_coord_offsetA,x+j_coord_offsetB,
                                              x+j_coord_offsetC,x+j_coord_offsetD,
                                              &jx0,&jy0,&jz0);

            /* Calculate displacement vector */
            dx10             = _mm_sub_ps(ix1,jx0);
            dy10             = _mm_sub_ps(iy1,jy0);
            dz10             = _mm_sub_ps(iz1,jz0);
            dx20             = _mm_sub_ps(ix2,jx0);
            dy20             = _mm_sub_ps(iy2,jy0);
            dz20             = _mm_sub_ps(iz2,jz0);
            dx30             = _mm_sub_ps(ix3,jx0);
            dy30             = _mm_sub_ps(iy3,jy0);
            dz30             = _mm_sub_ps(iz3,jz0);

            /* Calculate squared distance and things based on it */
            rsq10            = gmx_mm_calc_rsq_ps(dx10,dy10,dz10);
            rsq20            = gmx_mm_calc_rsq_ps(dx20,dy20,dz20);
            rsq30            = gmx_mm_calc_rsq_ps(dx30,dy30,dz30);

            rinv10           = sse41_invsqrt_f(rsq10);
            rinv20           = sse41_invsqrt_f(rsq20);
            rinv30           = sse41_invsqrt_f(rsq30);

            rinvsq10         = _mm_mul_ps(rinv10,rinv10);
            rinvsq20         = _mm_mul_ps(rinv20,rinv20);
            rinvsq30         = _mm_mul_ps(rinv30,rinv30);

            /* Load parameters for j particles */
            jq0              = gmx_mm_load_4real_swizzle_ps(charge+jnrA+0,charge+jnrB+0,
                                                              charge+jnrC+0,charge+jnrD+0);

            fjx0             = _mm_setzero_ps();
            fjy0             = _mm_setzero_ps();
            fjz0             = _mm_setzero_ps();

            /**************************
             * CALCULATE INTERACTIONS *
             **************************/

            /* Compute parameters for interactions between i and j atoms */
            qq10             = _mm_mul_ps(iq1,jq0);

            /* COULOMB ELECTROSTATICS */
            velec            = _mm_mul_ps(qq10,rinv10);
            felec            = _mm_mul_ps(velec,rinvsq10);

            /* Update potential sum for this i atom from the interaction with this j atom. */
            velec            = _mm_andnot_ps(dummy_mask,velec);
            velecsum         = _mm_add_ps(velecsum,velec);

            fscal            = felec;

            fscal            = _mm_andnot_ps(dummy_mask,fscal);

            /* Calculate temporary vectorial force */
            tx               = _mm_mul_ps(fscal,dx10);
            ty               = _mm_mul_ps(fscal,dy10);
            tz               = _mm_mul_ps(fscal,dz10);

            /* Update vectorial force */
            fix1             = _mm_add_ps(fix1,tx);
            fiy1             = _mm_add_ps(fiy1,ty);
            fiz1             = _mm_add_ps(fiz1,tz);

            fjx0             = _mm_add_ps(fjx0,tx);
            fjy0             = _mm_add_ps(fjy0,ty);
            fjz0             = _mm_add_ps(fjz0,tz);

            /**************************
             * CALCULATE INTERACTIONS *
             **************************/

            /* Compute parameters for interactions between i and j atoms */
            qq20             = _mm_mul_ps(iq2,jq0);

            /* COULOMB ELECTROSTATICS */
            velec            = _mm_mul_ps(qq20,rinv20);
            felec            = _mm_mul_ps(velec,rinvsq20);

            /* Update potential sum for this i atom from the interaction with this j atom. */
            velec            = _mm_andnot_ps(dummy_mask,velec);
            velecsum         = _mm_add_ps(velecsum,velec);

            fscal            = felec;

            fscal            = _mm_andnot_ps(dummy_mask,fscal);

            /* Calculate temporary vectorial force */
            tx               = _mm_mul_ps(fscal,dx20);
            ty               = _mm_mul_ps(fscal,dy20);
            tz               = _mm_mul_ps(fscal,dz20);

            /* Update vectorial force */
            fix2             = _mm_add_ps(fix2,tx);
            fiy2             = _mm_add_ps(fiy2,ty);
            fiz2             = _mm_add_ps(fiz2,tz);

            fjx0             = _mm_add_ps(fjx0,tx);
            fjy0             = _mm_add_ps(fjy0,ty);
            fjz0             = _mm_add_ps(fjz0,tz);

            /**************************
             * CALCULATE INTERACTIONS *
             **************************/

            /* Compute parameters for interactions between i and j atoms */
            qq30             = _mm_mul_ps(iq3,jq0);

            /* COULOMB ELECTROSTATICS */
            velec            = _mm_mul_ps(qq30,rinv30);
            felec            = _mm_mul_ps(velec,rinvsq30);

            /* Update potential sum for this i atom from the interaction with this j atom. */
            velec            = _mm_andnot_ps(dummy_mask,velec);
            velecsum         = _mm_add_ps(velecsum,velec);

            fscal            = felec;

            fscal            = _mm_andnot_ps(dummy_mask,fscal);

            /* Calculate temporary vectorial force */
            tx               = _mm_mul_ps(fscal,dx30);
            ty               = _mm_mul_ps(fscal,dy30);
            tz               = _mm_mul_ps(fscal,dz30);

            /* Update vectorial force */
            fix3             = _mm_add_ps(fix3,tx);
            fiy3             = _mm_add_ps(fiy3,ty);
            fiz3             = _mm_add_ps(fiz3,tz);

            fjx0             = _mm_add_ps(fjx0,tx);
            fjy0             = _mm_add_ps(fjy0,ty);
            fjz0             = _mm_add_ps(fjz0,tz);

            fjptrA             = (jnrlistA>=0) ? f+j_coord_offsetA : scratch;
            fjptrB             = (jnrlistB>=0) ? f+j_coord_offsetB : scratch;
            fjptrC             = (jnrlistC>=0) ? f+j_coord_offsetC : scratch;
            fjptrD             = (jnrlistD>=0) ? f+j_coord_offsetD : scratch;

	    gmx_mm_decrement_1rvec_4ptr_swizzle_ps(fjptrA,fjptrB,fjptrC,fjptrD,fjx0,fjy0,fjz0);

            /* Inner loop uses 84 flops */
        }

        /* End of innermost loop */

        gmx_mm_update_iforce_3atom_swizzle_ps(fix1,fiy1,fiz1,fix2,fiy2,fiz2,fix3,fiy3,fiz3,
                                              f+i_coord_offset+DIM,fshift+i_shift_offset);

        ggid                        = gid[iidx];
        /* Update potential energies */
        gmx_mm_update_1pot_ps(velecsum,kernel_data->energygrp_elec+ggid);

        /* Increment number of inner iterations */
        inneriter                  += j_index_end - j_index_start;

        /* Outer loop uses 19 flops */
    }

    /* Increment number of outer iterations */
    outeriter        += nri;

    /* Update outer/inner flops */

    inc_nrnb(nrnb,eNR_NBKERNEL_ELEC_W4_VF,outeriter*19 + inneriter*84);
}
/*
 * Gromacs nonbonded kernel:   nb_kernel_ElecCoul_VdwNone_GeomW4P1_F_sse4_1_single
 * Electrostatics interaction: Coulomb
 * VdW interaction:            None
 * Geometry:                   Water4-Particle
 * Calculate force/pot:        Force
 */
void
nb_kernel_ElecCoul_VdwNone_GeomW4P1_F_sse4_1_single
                    (t_nblist                    * gmx_restrict       nlist,
                     rvec                        * gmx_restrict          xx,
                     rvec                        * gmx_restrict          ff,
                     struct t_forcerec           * gmx_restrict          fr,
                     t_mdatoms                   * gmx_restrict     mdatoms,
                     nb_kernel_data_t gmx_unused * gmx_restrict kernel_data,
                     t_nrnb                      * gmx_restrict        nrnb)
{
    /* Suffixes 0,1,2,3 refer to particle indices for waters in the inner or outer loop, or 
     * just 0 for non-waters.
     * Suffixes A,B,C,D refer to j loop unrolling done with SSE, e.g. for the four different
     * jnr indices corresponding to data put in the four positions in the SIMD register.
     */
    int              i_shift_offset,i_coord_offset,outeriter,inneriter;
    int              j_index_start,j_index_end,jidx,nri,inr,ggid,iidx;
    int              jnrA,jnrB,jnrC,jnrD;
    int              jnrlistA,jnrlistB,jnrlistC,jnrlistD;
    int              j_coord_offsetA,j_coord_offsetB,j_coord_offsetC,j_coord_offsetD;
    int              *iinr,*jindex,*jjnr,*shiftidx,*gid;
    real             rcutoff_scalar;
    real             *shiftvec,*fshift,*x,*f;
    real             *fjptrA,*fjptrB,*fjptrC,*fjptrD;
    real             scratch[4*DIM];
    __m128           tx,ty,tz,fscal,rcutoff,rcutoff2,jidxall;
    int              vdwioffset1;
    __m128           ix1,iy1,iz1,fix1,fiy1,fiz1,iq1,isai1;
    int              vdwioffset2;
    __m128           ix2,iy2,iz2,fix2,fiy2,fiz2,iq2,isai2;
    int              vdwioffset3;
    __m128           ix3,iy3,iz3,fix3,fiy3,fiz3,iq3,isai3;
    int              vdwjidx0A,vdwjidx0B,vdwjidx0C,vdwjidx0D;
    __m128           jx0,jy0,jz0,fjx0,fjy0,fjz0,jq0,isaj0;
    __m128           dx10,dy10,dz10,rsq10,rinv10,rinvsq10,r10,qq10,c6_10,c12_10;
    __m128           dx20,dy20,dz20,rsq20,rinv20,rinvsq20,r20,qq20,c6_20,c12_20;
    __m128           dx30,dy30,dz30,rsq30,rinv30,rinvsq30,r30,qq30,c6_30,c12_30;
    __m128           velec,felec,velecsum,facel,crf,krf,krf2;
    real             *charge;
    __m128           dummy_mask,cutoff_mask;
    __m128           signbit = _mm_castsi128_ps( _mm_set1_epi32(0x80000000) );
    __m128           one     = _mm_set1_ps(1.0);
    __m128           two     = _mm_set1_ps(2.0);
    x                = xx[0];
    f                = ff[0];

    nri              = nlist->nri;
    iinr             = nlist->iinr;
    jindex           = nlist->jindex;
    jjnr             = nlist->jjnr;
    shiftidx         = nlist->shift;
    gid              = nlist->gid;
    shiftvec         = fr->shift_vec[0];
    fshift           = fr->fshift[0];
    facel            = _mm_set1_ps(fr->ic->epsfac);
    charge           = mdatoms->chargeA;

    /* Setup water-specific parameters */
    inr              = nlist->iinr[0];
    iq1              = _mm_mul_ps(facel,_mm_set1_ps(charge[inr+1]));
    iq2              = _mm_mul_ps(facel,_mm_set1_ps(charge[inr+2]));
    iq3              = _mm_mul_ps(facel,_mm_set1_ps(charge[inr+3]));

    /* Avoid stupid compiler warnings */
    jnrA = jnrB = jnrC = jnrD = 0;
    j_coord_offsetA = 0;
    j_coord_offsetB = 0;
    j_coord_offsetC = 0;
    j_coord_offsetD = 0;

    outeriter        = 0;
    inneriter        = 0;

    for(iidx=0;iidx<4*DIM;iidx++)
    {
        scratch[iidx] = 0.0;
    }

    /* Start outer loop over neighborlists */
    for(iidx=0; iidx<nri; iidx++)
    {
        /* Load shift vector for this list */
        i_shift_offset   = DIM*shiftidx[iidx];

        /* Load limits for loop over neighbors */
        j_index_start    = jindex[iidx];
        j_index_end      = jindex[iidx+1];

        /* Get outer coordinate index */
        inr              = iinr[iidx];
        i_coord_offset   = DIM*inr;

        /* Load i particle coords and add shift vector */
        gmx_mm_load_shift_and_3rvec_broadcast_ps(shiftvec+i_shift_offset,x+i_coord_offset+DIM,
                                                 &ix1,&iy1,&iz1,&ix2,&iy2,&iz2,&ix3,&iy3,&iz3);

        fix1             = _mm_setzero_ps();
        fiy1             = _mm_setzero_ps();
        fiz1             = _mm_setzero_ps();
        fix2             = _mm_setzero_ps();
        fiy2             = _mm_setzero_ps();
        fiz2             = _mm_setzero_ps();
        fix3             = _mm_setzero_ps();
        fiy3             = _mm_setzero_ps();
        fiz3             = _mm_setzero_ps();

        /* Start inner kernel loop */
        for(jidx=j_index_start; jidx<j_index_end && jjnr[jidx+3]>=0; jidx+=4)
        {

            /* Get j neighbor index, and coordinate index */
            jnrA             = jjnr[jidx];
            jnrB             = jjnr[jidx+1];
            jnrC             = jjnr[jidx+2];
            jnrD             = jjnr[jidx+3];
            j_coord_offsetA  = DIM*jnrA;
            j_coord_offsetB  = DIM*jnrB;
            j_coord_offsetC  = DIM*jnrC;
            j_coord_offsetD  = DIM*jnrD;

            /* load j atom coordinates */
            gmx_mm_load_1rvec_4ptr_swizzle_ps(x+j_coord_offsetA,x+j_coord_offsetB,
                                              x+j_coord_offsetC,x+j_coord_offsetD,
                                              &jx0,&jy0,&jz0);

            /* Calculate displacement vector */
            dx10             = _mm_sub_ps(ix1,jx0);
            dy10             = _mm_sub_ps(iy1,jy0);
            dz10             = _mm_sub_ps(iz1,jz0);
            dx20             = _mm_sub_ps(ix2,jx0);
            dy20             = _mm_sub_ps(iy2,jy0);
            dz20             = _mm_sub_ps(iz2,jz0);
            dx30             = _mm_sub_ps(ix3,jx0);
            dy30             = _mm_sub_ps(iy3,jy0);
            dz30             = _mm_sub_ps(iz3,jz0);

            /* Calculate squared distance and things based on it */
            rsq10            = gmx_mm_calc_rsq_ps(dx10,dy10,dz10);
            rsq20            = gmx_mm_calc_rsq_ps(dx20,dy20,dz20);
            rsq30            = gmx_mm_calc_rsq_ps(dx30,dy30,dz30);

            rinv10           = sse41_invsqrt_f(rsq10);
            rinv20           = sse41_invsqrt_f(rsq20);
            rinv30           = sse41_invsqrt_f(rsq30);

            rinvsq10         = _mm_mul_ps(rinv10,rinv10);
            rinvsq20         = _mm_mul_ps(rinv20,rinv20);
            rinvsq30         = _mm_mul_ps(rinv30,rinv30);

            /* Load parameters for j particles */
            jq0              = gmx_mm_load_4real_swizzle_ps(charge+jnrA+0,charge+jnrB+0,
                                                              charge+jnrC+0,charge+jnrD+0);

            fjx0             = _mm_setzero_ps();
            fjy0             = _mm_setzero_ps();
            fjz0             = _mm_setzero_ps();

            /**************************
             * CALCULATE INTERACTIONS *
             **************************/

            /* Compute parameters for interactions between i and j atoms */
            qq10             = _mm_mul_ps(iq1,jq0);

            /* COULOMB ELECTROSTATICS */
            velec            = _mm_mul_ps(qq10,rinv10);
            felec            = _mm_mul_ps(velec,rinvsq10);

            fscal            = felec;

            /* Calculate temporary vectorial force */
            tx               = _mm_mul_ps(fscal,dx10);
            ty               = _mm_mul_ps(fscal,dy10);
            tz               = _mm_mul_ps(fscal,dz10);

            /* Update vectorial force */
            fix1             = _mm_add_ps(fix1,tx);
            fiy1             = _mm_add_ps(fiy1,ty);
            fiz1             = _mm_add_ps(fiz1,tz);

            fjx0             = _mm_add_ps(fjx0,tx);
            fjy0             = _mm_add_ps(fjy0,ty);
            fjz0             = _mm_add_ps(fjz0,tz);

            /**************************
             * CALCULATE INTERACTIONS *
             **************************/

            /* Compute parameters for interactions between i and j atoms */
            qq20             = _mm_mul_ps(iq2,jq0);

            /* COULOMB ELECTROSTATICS */
            velec            = _mm_mul_ps(qq20,rinv20);
            felec            = _mm_mul_ps(velec,rinvsq20);

            fscal            = felec;

            /* Calculate temporary vectorial force */
            tx               = _mm_mul_ps(fscal,dx20);
            ty               = _mm_mul_ps(fscal,dy20);
            tz               = _mm_mul_ps(fscal,dz20);

            /* Update vectorial force */
            fix2             = _mm_add_ps(fix2,tx);
            fiy2             = _mm_add_ps(fiy2,ty);
            fiz2             = _mm_add_ps(fiz2,tz);

            fjx0             = _mm_add_ps(fjx0,tx);
            fjy0             = _mm_add_ps(fjy0,ty);
            fjz0             = _mm_add_ps(fjz0,tz);

            /**************************
             * CALCULATE INTERACTIONS *
             **************************/

            /* Compute parameters for interactions between i and j atoms */
            qq30             = _mm_mul_ps(iq3,jq0);

            /* COULOMB ELECTROSTATICS */
            velec            = _mm_mul_ps(qq30,rinv30);
            felec            = _mm_mul_ps(velec,rinvsq30);

            fscal            = felec;

            /* Calculate temporary vectorial force */
            tx               = _mm_mul_ps(fscal,dx30);
            ty               = _mm_mul_ps(fscal,dy30);
            tz               = _mm_mul_ps(fscal,dz30);

            /* Update vectorial force */
            fix3             = _mm_add_ps(fix3,tx);
            fiy3             = _mm_add_ps(fiy3,ty);
            fiz3             = _mm_add_ps(fiz3,tz);

            fjx0             = _mm_add_ps(fjx0,tx);
            fjy0             = _mm_add_ps(fjy0,ty);
            fjz0             = _mm_add_ps(fjz0,tz);

            fjptrA             = f+j_coord_offsetA;
            fjptrB             = f+j_coord_offsetB;
            fjptrC             = f+j_coord_offsetC;
            fjptrD             = f+j_coord_offsetD;

	    gmx_mm_decrement_1rvec_4ptr_swizzle_ps(fjptrA,fjptrB,fjptrC,fjptrD,fjx0,fjy0,fjz0);

            /* Inner loop uses 81 flops */
        }

        if(jidx<j_index_end)
        {

            /* Get j neighbor index, and coordinate index */
            jnrlistA         = jjnr[jidx];
            jnrlistB         = jjnr[jidx+1];
            jnrlistC         = jjnr[jidx+2];
            jnrlistD         = jjnr[jidx+3];
            /* Sign of each element will be negative for non-real atoms.
             * This mask will be 0xFFFFFFFF for dummy entries and 0x0 for real ones,
             * so use it as val = _mm_andnot_ps(mask,val) to clear dummy entries.
             */
            dummy_mask = gmx_mm_castsi128_ps(_mm_cmplt_epi32(_mm_loadu_si128((const __m128i *)(jjnr+jidx)),_mm_setzero_si128()));
            jnrA       = (jnrlistA>=0) ? jnrlistA : 0;
            jnrB       = (jnrlistB>=0) ? jnrlistB : 0;
            jnrC       = (jnrlistC>=0) ? jnrlistC : 0;
            jnrD       = (jnrlistD>=0) ? jnrlistD : 0;
            j_coord_offsetA  = DIM*jnrA;
            j_coord_offsetB  = DIM*jnrB;
            j_coord_offsetC  = DIM*jnrC;
            j_coord_offsetD  = DIM*jnrD;

            /* load j atom coordinates */
            gmx_mm_load_1rvec_4ptr_swizzle_ps(x+j_coord_offsetA,x+j_coord_offsetB,
                                              x+j_coord_offsetC,x+j_coord_offsetD,
                                              &jx0,&jy0,&jz0);

            /* Calculate displacement vector */
            dx10             = _mm_sub_ps(ix1,jx0);
            dy10             = _mm_sub_ps(iy1,jy0);
            dz10             = _mm_sub_ps(iz1,jz0);
            dx20             = _mm_sub_ps(ix2,jx0);
            dy20             = _mm_sub_ps(iy2,jy0);
            dz20             = _mm_sub_ps(iz2,jz0);
            dx30             = _mm_sub_ps(ix3,jx0);
            dy30             = _mm_sub_ps(iy3,jy0);
            dz30             = _mm_sub_ps(iz3,jz0);

            /* Calculate squared distance and things based on it */
            rsq10            = gmx_mm_calc_rsq_ps(dx10,dy10,dz10);
            rsq20            = gmx_mm_calc_rsq_ps(dx20,dy20,dz20);
            rsq30            = gmx_mm_calc_rsq_ps(dx30,dy30,dz30);

            rinv10           = sse41_invsqrt_f(rsq10);
            rinv20           = sse41_invsqrt_f(rsq20);
            rinv30           = sse41_invsqrt_f(rsq30);

            rinvsq10         = _mm_mul_ps(rinv10,rinv10);
            rinvsq20         = _mm_mul_ps(rinv20,rinv20);
            rinvsq30         = _mm_mul_ps(rinv30,rinv30);

            /* Load parameters for j particles */
            jq0              = gmx_mm_load_4real_swizzle_ps(charge+jnrA+0,charge+jnrB+0,
                                                              charge+jnrC+0,charge+jnrD+0);

            fjx0             = _mm_setzero_ps();
            fjy0             = _mm_setzero_ps();
            fjz0             = _mm_setzero_ps();

            /**************************
             * CALCULATE INTERACTIONS *
             **************************/

            /* Compute parameters for interactions between i and j atoms */
            qq10             = _mm_mul_ps(iq1,jq0);

            /* COULOMB ELECTROSTATICS */
            velec            = _mm_mul_ps(qq10,rinv10);
            felec            = _mm_mul_ps(velec,rinvsq10);

            fscal            = felec;

            fscal            = _mm_andnot_ps(dummy_mask,fscal);

            /* Calculate temporary vectorial force */
            tx               = _mm_mul_ps(fscal,dx10);
            ty               = _mm_mul_ps(fscal,dy10);
            tz               = _mm_mul_ps(fscal,dz10);

            /* Update vectorial force */
            fix1             = _mm_add_ps(fix1,tx);
            fiy1             = _mm_add_ps(fiy1,ty);
            fiz1             = _mm_add_ps(fiz1,tz);

            fjx0             = _mm_add_ps(fjx0,tx);
            fjy0             = _mm_add_ps(fjy0,ty);
            fjz0             = _mm_add_ps(fjz0,tz);

            /**************************
             * CALCULATE INTERACTIONS *
             **************************/

            /* Compute parameters for interactions between i and j atoms */
            qq20             = _mm_mul_ps(iq2,jq0);

            /* COULOMB ELECTROSTATICS */
            velec            = _mm_mul_ps(qq20,rinv20);
            felec            = _mm_mul_ps(velec,rinvsq20);

            fscal            = felec;

            fscal            = _mm_andnot_ps(dummy_mask,fscal);

            /* Calculate temporary vectorial force */
            tx               = _mm_mul_ps(fscal,dx20);
            ty               = _mm_mul_ps(fscal,dy20);
            tz               = _mm_mul_ps(fscal,dz20);

            /* Update vectorial force */
            fix2             = _mm_add_ps(fix2,tx);
            fiy2             = _mm_add_ps(fiy2,ty);
            fiz2             = _mm_add_ps(fiz2,tz);

            fjx0             = _mm_add_ps(fjx0,tx);
            fjy0             = _mm_add_ps(fjy0,ty);
            fjz0             = _mm_add_ps(fjz0,tz);

            /**************************
             * CALCULATE INTERACTIONS *
             **************************/

            /* Compute parameters for interactions between i and j atoms */
            qq30             = _mm_mul_ps(iq3,jq0);

            /* COULOMB ELECTROSTATICS */
            velec            = _mm_mul_ps(qq30,rinv30);
            felec            = _mm_mul_ps(velec,rinvsq30);

            fscal            = felec;

            fscal            = _mm_andnot_ps(dummy_mask,fscal);

            /* Calculate temporary vectorial force */
            tx               = _mm_mul_ps(fscal,dx30);
            ty               = _mm_mul_ps(fscal,dy30);
            tz               = _mm_mul_ps(fscal,dz30);

            /* Update vectorial force */
            fix3             = _mm_add_ps(fix3,tx);
            fiy3             = _mm_add_ps(fiy3,ty);
            fiz3             = _mm_add_ps(fiz3,tz);

            fjx0             = _mm_add_ps(fjx0,tx);
            fjy0             = _mm_add_ps(fjy0,ty);
            fjz0             = _mm_add_ps(fjz0,tz);

            fjptrA             = (jnrlistA>=0) ? f+j_coord_offsetA : scratch;
            fjptrB             = (jnrlistB>=0) ? f+j_coord_offsetB : scratch;
            fjptrC             = (jnrlistC>=0) ? f+j_coord_offsetC : scratch;
            fjptrD             = (jnrlistD>=0) ? f+j_coord_offsetD : scratch;

	    gmx_mm_decrement_1rvec_4ptr_swizzle_ps(fjptrA,fjptrB,fjptrC,fjptrD,fjx0,fjy0,fjz0);

            /* Inner loop uses 81 flops */
        }

        /* End of innermost loop */

        gmx_mm_update_iforce_3atom_swizzle_ps(fix1,fiy1,fiz1,fix2,fiy2,fiz2,fix3,fiy3,fiz3,
                                              f+i_coord_offset+DIM,fshift+i_shift_offset);

        /* Increment number of inner iterations */
        inneriter                  += j_index_end - j_index_start;

        /* Outer loop uses 18 flops */
    }

    /* Increment number of outer iterations */
    outeriter        += nri;

    /* Update outer/inner flops */

    inc_nrnb(nrnb,eNR_NBKERNEL_ELEC_W4_F,outeriter*18 + inneriter*81);
}
