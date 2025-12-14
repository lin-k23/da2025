#include <stdio.h>
#include <vector>
#include <algorithm>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
using namespace std;

// ======================= 稀疏矩阵 Triplet =======================
struct MyTriplet
{
    int r, c;
    double v;
    MyTriplet(int r_, int c_, double v_) : r(r_), c(c_), v(v_) {}
};

// ======================= CSR 稀疏矩阵 =======================
struct MySparseMatrix
{
    int n;
    vector<int> row_ptr, col;
    vector<double> val;

    MySparseMatrix(int n_ = 0) : n(n_) {}

    void setFromTriplets(const vector<MyTriplet> &tri_src)
    {
        vector<MyTriplet> tri = tri_src;

        // 显式类型比较器
        sort(tri.begin(), tri.end(), [](const MyTriplet &a, const MyTriplet &b)
             { return (a.r < b.r) || (a.r == b.r && a.c < b.c); });

        row_ptr.assign(n + 1, 0);

        // 不用 auto
        for (size_t i = 0; i < tri.size(); i++)
        {
            row_ptr[tri[i].r + 1]++;
        }

        for (int i = 0; i < n; i++)
            row_ptr[i + 1] += row_ptr[i];

        int nnz = tri.size();
        col.resize(nnz);
        val.resize(nnz);

        vector<int> cur = row_ptr;

        for (size_t i = 0; i < tri.size(); i++)
        {
            const MyTriplet &t = tri[i];
            int pos = cur[t.r]++;
            col[pos] = t.c;
            val[pos] = t.v;
        }
    }

    vector<double> mul(const vector<double> &x) const
    {
        vector<double> y(n, 0.0);
        for (int i = 0; i < n; i++)
        {
            double s = 0;
            for (int k = row_ptr[i]; k < row_ptr[i + 1]; k++)
                s += val[k] * x[col[k]];
            y[i] = s;
        }
        return y;
    }
};

// ======================= CG 迭代求解器 =======================
struct MyCGSolver
{
    const MySparseMatrix *A = nullptr;
    int maxIter = 2000;
    double tol = 1e-12;

    void compute(const MySparseMatrix &A_) { A = &A_; }

    vector<double> solve(const vector<double> &b) const
    {
        int n = A->n;

        vector<double> x(n, 0.0);
        vector<double> r = b;
        vector<double> p = b;

        auto dot = [&](const vector<double> &a, const vector<double> &b)
        {
            double s = 0;
            for (int i = 0; i < n; i++)
                s += a[i] * b[i];
            return s;
        };

        double rsold = dot(r, r);

        for (int it = 0; it < maxIter; it++)
        {
            vector<double> Ap = A->mul(p);
            double pAp = dot(p, Ap);
            double alpha = rsold / (pAp == 0 ? 1e-30 : pAp);

            for (int i = 0; i < n; i++)
            {
                x[i] += alpha * p[i];
                r[i] -= alpha * Ap[i];
            }
            double rsnew = dot(r, r);

            if (sqrt(rsnew) < tol)
                break;

            double beta = rsnew / rsold;
            for (int i = 0; i < n; i++)
                p[i] = r[i] + beta * p[i];

            rsold = rsnew;
        }
        return x;
    }
};

MySparseMatrix buildMatrix(int N, double mu_x, double mu_y, int type)
{
    int M = N - 1;
    int size = M * M;
    vector<MyTriplet> triplets;

    double sign = (type == 0) ? -1.0 : 1.0;

    double diag = 1.0 - sign * 2.0 * (mu_x + mu_y);

    double val_diag, val_off_x, val_off_y;
    if (type == 0)
    { // Matrix A
        val_diag = 1.0 + 2.0 * mu_x + 2.0 * mu_y;
        val_off_x = -mu_x;
        val_off_y = -mu_y;
    }
    else
    { // Matrix B
        val_diag = 1.0 - 2.0 * mu_x - 2.0 * mu_y;
        val_off_x = mu_x;
        val_off_y = mu_y;
    }

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            int row = i * M + j;
            triplets.emplace_back(row, row, val_diag);

            if (i > 0)
                triplets.emplace_back(row, row - M, val_off_x);
            if (i < M - 1)
                triplets.emplace_back(row, row + M, val_off_x);

            if (j > 0)
                triplets.emplace_back(row, row - 1, val_off_y);
            if (j < M - 1)
                triplets.emplace_back(row, row + 1, val_off_y);
        }
    }

    MySparseMatrix Mat(size);
    Mat.setFromTriplets(triplets);
    return Mat;
}

double func_source(double x, double y, double t)
{
    double term1 = -M_PI * (x * x + y * y) * sin(M_PI * t);
    double term2 = -2.0 * cos(M_PI * t);
    return term1 + term2;
}

double func_boundary(double x, double y, double t)
{
    return cos(M_PI * t) * (x * x + y * y);
}

double func_initial(double x, double y)
{
    return x * x + y * y;
}

vector<double> calc_load_vector(
    const MySparseMatrix &B,
    const vector<double> &U_prev,
    int N, double dx, double dy,
    double xmin, double xmax, double ymin, double ymax,
    double dt, double t_n, double mu_x, double mu_y)
{
    int M = N - 1;
    double t_next = t_n + dt;
    double t_half = t_n + dt / 2.0;

    // 1. b_explicit = B * U^n
    vector<double> b = B.mul(U_prev);

    for (int i = 0; i < M; i++)
    {
        for (int j = 0; j < M; j++)
        {
            int idx = i * M + j; // 这里的 i 对应 x 索引，j 对应 y 索引

            // 修正：使用 xmin, ymin 和 dx, dy 计算真实的物理坐标
            double real_x = xmin + (i + 1) * dx;
            double real_y = ymin + (j + 1) * dy;

            // 2. dt * f(x, y, t_half)
            double src = dt * func_source(real_x, real_y, t_half);
            b[idx] += src;

            // 3. Boundary Effects
            // 修正：使用真实的边界坐标 xmin, xmax, ymin, ymax

            // 左边界 (i=0 -> index 1 in physics, neighbor is index 0 -> xmin)
            if (i == 0)
            {
                double g_val = func_boundary(xmin, real_y, t_n) + func_boundary(xmin, real_y, t_next);
                b[idx] += mu_x * g_val;
            }
            // 右边界
            if (i == M - 1)
            {
                double g_val = func_boundary(xmax, real_y, t_n) + func_boundary(xmax, real_y, t_next);
                b[idx] += mu_x * g_val;
            }
            // 下边界
            if (j == 0)
            {
                double g_val = func_boundary(real_x, ymin, t_n) + func_boundary(real_x, ymin, t_next);
                b[idx] += mu_y * g_val;
            }
            // 上边界
            if (j == M - 1)
            {
                double g_val = func_boundary(real_x, ymax, t_n) + func_boundary(real_x, ymax, t_next);
                b[idx] += mu_y * g_val;
            }
        }
    }
    return b;
}
int main()
{
    // u(x,y,t) parameters

    double c, T, xmin, xmax, ymin, ymax;
    int N, Nt;

    // point to evaluate, use bilinear interpolation
    double x, y;
    scanf("%lf %lf %d %d %lf %lf %lf %lf %lf %lf", &c, &T, &N, &Nt, &xmin, &xmax, &ymin, &ymax, &x, &y);
    // check input correct
    // printf("c=%.6f T=%.6f N=%d Nt=%d xmin=%.6f xmax=%.6f ymin=%.6f ymax=%.6f x=%.6f y=%.6f\n", c, T, N, Nt, xmin, xmax, ymin, ymax, x, y);

    // compute dx, dy, dt
    double dx = (xmax - xmin) / N;
    double dy = (ymax - ymin) / N;
    double dt = T / Nt;

    // compute indices
    int i = (x - xmin) / dx;
    int j = (y - ymin) / dy;

    // constants matrix A
    double ux = c * dt / (2 * dx * dx);
    double uy = c * dt / (2 * dy * dy);

    MySparseMatrix A = buildMatrix(N, ux, uy, 0);
    MySparseMatrix B = buildMatrix(N, ux, uy, 1);

    // check matrix A
    // for (int row = 0; row < A.n; row++)
    // {
    //     for (int k = A.row_ptr[row]; k < A.row_ptr[row + 1]; k++)
    //     {
    //         printf("A(%d,%d)=%.6f ", row, A.col[k], A.val[k]);
    //     }
    //     printf("\n");
    // }

    // check matrix B
    // for (int row = 0; row < B.n; row++)
    // {
    //     for (int k = B.row_ptr[row]; k < B.row_ptr[row + 1]; k++)
    //     {
    //         printf("B(%d,%d)=%.6f ", row, B.col[k], B.val[k]);
    //     }
    //     printf("\n");
    // }

    MyCGSolver solver;
    solver.compute(A);
    solver.tol = 1e-8;

    int M = N - 1;
    std::vector<double> U(M * M);

    // initial condition U^0
    for (int r = 0; r < M; r++)
    {
        for (int c_idx = 0; c_idx < M; c_idx++)
        {
            double real_x = xmin + (r + 1) * dx;
            double real_y = ymin + (c_idx + 1) * dy;
            U[r * M + c_idx] = func_initial(real_x, real_y);
        }
    }

    // time-stepping loop
    double current_time = 0.0;

    for (int t = 0; t < Nt; t++)
    {
        std::vector<double> b = calc_load_vector(B, U, N, dx, dy, xmin, xmax, ymin, ymax, dt, current_time, ux, uy);
        std::vector<double> U_new = solver.solve(b);
        U = U_new;
        current_time += dt;
    }

    // bilinear interpolation to get u(x,y,T)
    auto get_node_val = [&](int grid_i, int grid_j, double time) -> double
    {
        // edge nodes
        if (grid_i == 0)
            return func_boundary(xmin, ymin + grid_j * dy, time);
        if (grid_i == N)
            return func_boundary(xmax, ymin + grid_j * dy, time);
        if (grid_j == 0)
            return func_boundary(xmin + grid_i * dx, ymin, time);
        if (grid_j == N)
            return func_boundary(xmin + grid_i * dx, ymax, time);

        int vec_idx = (grid_i - 1) * M + (grid_j - 1);
        return U[vec_idx];
    };

    // compute weights for bilinear interpolation
    // u(x,y,T) = wx1*wy1*u(i,j) + wx2*wy1*u(i+1,j) + wx1*wy2*u(i,j+1) + wx2*wy2*u(i+1,j+1)
    double x1 = xmin + i * dx;
    double y1 = ymin + j * dy;
    double wx2 = (x - x1) / dx;
    double wy2 = (y - y1) / dy;
    double wx1 = 1.0f - wx2;
    double wy1 = 1.0f - wy2;

    // u(i,j), u(i+1,j), u(i,j+1), u(i+1,j+1)
    double u_00 = get_node_val(i, j, current_time);
    double u_10 = get_node_val(i + 1, j, current_time);
    double u_01 = get_node_val(i, j + 1, current_time);
    double u_11 = get_node_val(i + 1, j + 1, current_time);

    double result = wx1 * wy1 * u_00 +
                    wx2 * wy1 * u_10 +
                    wx1 * wy2 * u_01 +
                    wx2 * wy2 * u_11;

    printf("%.8f\n", result);
}