

$$
\begin{bmatrix}
1 & x & x^2 \\
1 & y & y^2 \\
1 & z & z^2 \\
\end{bmatrix}
$$

$$
\begin{array}{c|lcr}
n & \text{Left} & \text{Center} & \text{Right} \\
\hline
1 & 0.24 & 1 & 125 \\
2 & -1 & 189 & -8 \\
3 & -20 & 2000 & 1+10i
\end{array}
$$

$$
\begin{align}
D &= \begin{vmatrix}
1 & 1 & 1 \\
x_1 & x_2 & x_3 \\
x_1^2 & x_2^2 & x_3^2 \\
\end{vmatrix} \\
&= \begin{vmatrix}
1 & 0 & 0 \\
x_1 & x_2-x_1 & x_3-x_1 \\
x_1^2 & x_2^2-x_1^2 & x_3^2-x_1^2 \\
\end{vmatrix}\\
&= (x_2-x_1)(x_3-x_1)\begin{vmatrix}
1 & 0 & 0 \\
x_1 & 1 & 1 \\
x_1^2 & x_2+x_1 & x_3+x_1 \\
\end{vmatrix} \\
&= (x_2-x_1)(x_3-x_1)\begin{vmatrix}
1 & 0 & 0 \\
x_1 & 1 & 0 \\
x_1^2 & x_2+x_1 & x_3-x_2 \\
\end{vmatrix} \\
&= (x_2-x_1)(x_3-x_1)(x_3-x_2)
\end{align}
$$

