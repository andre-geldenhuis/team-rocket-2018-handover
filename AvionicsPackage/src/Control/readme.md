# Control System
the control system is composed of several transfer functions which when strung together map from ADC counts from the IMU device to PWM counts sent to two servos.  

## Step 1
the first step is to convert the buffer we receive from the IMU into actual ADC counts to be utilised, this is done in the MPU9250 libaray with code like this:
`_axcounts = (((int16_t)_buffer[0]) << 8) | _buffer[1];` \\
`_aycounts = (((int16_t)_buffer[2]) << 8) | _buffer[3];` \\
`_azcounts = (((int16_t)_buffer[4]) << 8) | _buffer[5];` \\

## Step 2
the next step is to put the counts into vectors

## Step 3
Once in vectors, the counts must be transformed into usable values.

Values vectors $\vec{v}, \; [\vec{v_a}] = m/s^2, \; [\vec{v_g}] = \text{degrees}/s, \; [\vec{v_h}] = \mu\text{T}$

Count vectors $\vec{c}, \; c \in {\mathbb{Z}^{+}}^3, c_i < 2^{16}$

Scale matrix $S = \begin{bmatrix} s_1 & 0 & 0 \\ 0 & s_2 & 0 \\ 0 & 0 & s_3\end{bmatrix}$

Rotation matrix $R \in \mathbb{R}^{3\times 3}$ to rotate all sensors to share a common frame

bias vector $\vec{b}$

$\vec{v} = S \cdot R \cdot \vec{c} - \vec{b}$

not quite a linear transform because offsets.  it is affine.  we can express an affine transform as a linear transform in homogeneous coordinates


\begin{align}
    \tilde{v} & = \begin{bmatrix} v_1\\ v_2 \\ v_3 \\ 1 \end{bmatrix} \\
    \tilde{c} & = \begin{bmatrix} c_1\\ c_2 \\ c_3 \\ 1 \end{bmatrix} \\
    A & = \begin{bmatrix} S \cdot R & \vec{b} \\ 0^T & 1\end{bmatrix} \in \mathbb{R}^{4\times 4}\\
    \tilde{v} & = A \cdot \tilde{c}\\
\end{align}

## Step 4
once we have values we can construct a quaternion representing the amount of rotation since the last sample by 