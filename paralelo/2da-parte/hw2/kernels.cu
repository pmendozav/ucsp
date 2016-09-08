#pragma once

__device__ void Set(Complex *v, const size_t &i, const  size_t &j, const size_t &offset, float2 &val)
{
	v[__mul24(i, offset) + j] = val;
}
__device__ void Mult(Complex& a, const Complex &b, const float &c = 1)
{
	Complex t = { c * (a.x * b.x - a.y * b.y), c * (a.y * b.x + a.x * b.y) };
	a = t;
}
__global__ void padKernel(
	Complex *d_PaddedKernel,
	const Dim dim,
	const int kernel_dim,
	const int kernel_r)
{
	const int j = __mul24(blockDim.x, blockIdx.x) + threadIdx.x;
	const int i = __mul24(blockDim.y, blockIdx.y) + threadIdx.y;
	int _i, _j;

	if (j < kernel_dim && i < kernel_dim)
	{
		_j = j - kernel_r; if (_j < 0) _j += dim.cols;
		_i = i - kernel_r; if (_i < 0) _i += dim.rows;
		Set(d_PaddedKernel, _i, _j, dim.cols, tex2D(texKernel, (float)j + 0.5f, (float)i + 0.5f));
	}
}
__global__ void padData(
	Complex *d_PaddedData,
	const Dim dim,
	const Dim dim_ref,
	const int kernel_r)
{
	const int j = __mul24(blockDim.x, blockIdx.x) + threadIdx.x;
	const int i = __mul24(blockDim.y, blockIdx.y) + threadIdx.y;
	const int borderW = dim_ref.cols + kernel_r;
	const int borderH = dim_ref.rows + kernel_r;
	int _i, _j;

	if (j < dim.cols && i < dim.rows)
	{
		if (j < dim_ref.cols) _i = j;
		if (i < dim_ref.rows) _j = i;
		if (j >= dim_ref.cols && j < borderW) _i = dim_ref.cols - 1;
		if (i >= dim_ref.rows && i < borderH) _j = dim_ref.rows - 1;
		if (j >= borderW) _i = 0;
		if (i >= borderH) _j = 0;

		Set(d_PaddedData, i, j, dim.cols, tex2D(texData, (float)_i + 0.5f, (float)_j + 0.5f));
	}
}
__global__ void _product(
	Complex *d_PaddedData,
	const Complex *d_PaddedKernel,
	const int N)
{
	const int tid = __mul24(blockDim.x, blockIdx.x) + threadIdx.x;
	const int offset = __mul24(blockDim.x, gridDim.x);
	const float alpha = 1.0f / (float)N;

	for (size_t i = tid; i < N; i += offset)
		Mult(d_PaddedData[i], d_PaddedKernel[i], alpha);
}

void product(Complex *A, Complex *B, Dim dim)
{
	_product << <8, 1024 >> >(A, B, dim.cols * dim.rows);
}