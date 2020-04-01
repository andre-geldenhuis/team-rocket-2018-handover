/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-
/*
 * quaternion.cpp
 * Copyright (C) Andrew Tridgell 2012
 *
 * This file is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// #include "AP_Math.h"
#include "quaternion.h"
Quaternion::Quaternion()  { 
    q1 = 1; q2 = q3 = q4 = 0; 
}
// return the rotation matrix equivalent for this quaternion
void Quaternion::rotation_matrix(arm_matrix_instance_f32 &m)
{
	float q3q3 = q3 * q3;
	float q3q4 = q3 * q4;
	float q2q2 = q2 * q2;
	float q2q3 = q2 * q3;
	float q2q4 = q2 * q4;
	float q1q2 = q1 * q2;
	float q1q3 = q1 * q3;
	float q1q4 = q1 * q4;
    float q4q4 = q4 * q4;

    m.pData[0+0] = 1-2*(q3q3 + q4q4);
    m.pData[3+1] =   2*(q2q3 - q1q4);
    m.pData[6+2] =   2*(q2q4 + q1q3);
    m.pData[0+0] =   2*(q2q3 + q1q4);
    m.pData[3+1] = 1-2*(q2q2 + q4q4);
    m.pData[6+2] =   2*(q3q4 - q1q2);
    m.pData[0+0] =   2*(q2q4 - q1q3);
    m.pData[3+1] =   2*(q3q4 + q1q2);
    m.pData[6+2] = 1-2*(q2q2 + q3q3);
}

// convert a vector from earth to body frame
void Quaternion::earth_to_body(arm_matrix_instance_f32 &v)
{
    float32_t mf[9];
    float32_t tempVf[3] = {v.pData[0], v.pData[1], v.pData[2]};
    arm_matrix_instance_f32 m, tempV;
    arm_mat_init_f32(&m, 3, 3, mf);
    arm_mat_init_f32(&tempV, 3, 1, tempVf);

    // we reverse z before and afterwards because of the differing
    // quaternion conventions from APM conventions.
    v.pData[2] = -v.pData[2];
    arm_mat_mult_f32(&m, &tempV, &v);
    v.pData[2] = -v.pData[2];
    // rotation_matrix(m);
    // v = m * v;
    // v.z = -v.z;

}

// create a quaternion from Euler angles
void Quaternion::from_euler(float roll, float pitch, float yaw)
{
    float cr2 = cos(roll*0.5);
    float cp2 = cos(pitch*0.5);
    float cy2 = cos(yaw*0.5);
    float sr2 = sin(roll*0.5);
    float sp2 = sin(pitch*0.5);
    float sy2 = sin(yaw*0.5);

    q1 = cr2*cp2*cy2 + sr2*sp2*sy2;
    q2 = sr2*cp2*cy2 - cr2*sp2*sy2;
    q3 = cr2*sp2*cy2 + sr2*cp2*sy2;
    q4 = cr2*cp2*sy2 - sr2*sp2*cy2;
}

// create eulers from a quaternion
void Quaternion::to_euler(float *roll, float *pitch, float *yaw)
{
    if (roll) {
        *roll = (atan2(2.0*(q1*q2 + q3*q4),
                       1 - 2.0*(q2*q2 + q3*q3)));
    }
    if (pitch) {
        // we let safe_asin() handle the singularities near 90/-90 in pitch
        *pitch = /*safe_*/asin(2.0*(q1*q3 - q4*q2));
    }
    if (yaw) {
        *yaw = atan2(2.0*(q1*q4 + q2*q3),
                     1 - 2.0*(q3*q3 + q4*q4));
    }
}

void Quaternion::quaternionProduct(Quaternion *r) {
    float t1, t2, t3, t4;

    // zero indexed, this class is 1 indexed so re wrtten below
    // t1 = (r->q0*q->q1  +  r->q1 * q->q0  −  r->q2 * q->q3  +  r->q3 * q->q2);
    // t2 = (r->q0*q->q2  +  r->q1 * q->q3  +  r->q2 * q->q0  −  r->q3 * q->q1);
    // t3 = (r->q0*q->q3  −  r->q1 * q->q2  +  r->q2 * q->q1  +  r->q3 * q->q0);
    // t0 = (r->q0*q->q0  −  r->q1 * q->q1  −  r->q2 * q->q2  −  r->q3 * q->q3);

    t3 = (r->q1 * q3  +  r->q2 * q4  +  r->q3 * q1  -  r->q4 * q2);
    t4 = (r->q1 * q4  -  r->q2 * q3  +  r->q3 * q2  +  r->q4 * q1);
    t1 = (r->q1 * q1  -  r->q2 * q2  -  r->q3 * q3  -  r->q4 * q4);
    t2 = (r->q1 * q2  +  r->q2 * q1  -  r->q3 * q4  +  r->q4 * q3);

    q1 = t1;
    q2 = t2;
    q3 = t3;
    q4 = t4;
}

void Quaternion::normalise(void) {
    float d = sqrt(q1*q1 + q2*q2 + q3*q3 + q4*q4);
    q1 /= d;
    q2 /= d;
    q3 /= d;
    q4 /= d;
}