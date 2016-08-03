#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <type_traits>

#include "helper_cuda.h"
#include <cmath>

#define norm1(x, y) (sqrt(x*x + y*y))
#define norm2(x, y) (fabs(x) + fabs(y))
#define norm3(x, y) (atan(y/x))

template<class T>
struct Mat
{
	T 
		*ptr;
	size_t 
		*pitch,
		*rows,
		*cols;

	inline __device__ T Get(size_t row, size_t col)
	{
		return *((T*)((char*)ptr + row * *pitch) + col);
	}

	inline __device__ T Set(size_t row, size_t col, T value)
	{
		*((T*)((char*)ptr + row * *pitch) + col) = value;
	}

	__device__ T eval(Mat<T> ref, size_t i_o, size_t j_o, size_t dh)
	{
		T result = 0;
		size_t i, j, i2, j2;
		size_t row_o, col_o;

		row_o = i_o - dh;
		col_o = j_o - dh;

		for (i = row_o, i2 = 0; i <= i_o + dh; i++, i2++)
		{
			for (j = col_o, j2 = 0; j <= j_o + dh; j++, j2++)
			{
				result += Get(i, j) * ref.Get(i2, j2);
			}
		}

		return result;
	}

	inline T h_Get(size_t row, size_t col)
	{
		return *((T*)((char*)ptr + row * *pitch) + col);
	}

	inline T h_Set(size_t row, size_t col, T value)
	{
		*((T*)((char*)ptr + row * *pitch) + col) = value;
	}

	T h_eval(Mat<T> ref, size_t i_o, size_t j_o, size_t dh)
	{
		T result = 0;
		size_t i, j, i2, j2;
		size_t row_o, col_o;

		row_o = i_o - dh;
		col_o = j_o - dh;

		for (i = row_o, i2 = 0; i <= i_o + dh; i++, i2++)
		{
			for (j = col_o, j2 = 0; j <= j_o + dh; j++, j2++)
			{
				result += h_Get(i, j) * ref.h_Get(i2, j2);
			}
		}

		return result;
	}
};

template<class T>
static inline Mat<T> createDevMat2d(size_t rows, size_t cols, T *src)
{
	Mat<T>
		dst;
	size_t
		pitch,
		width,
		height;

	width = cols * sizeof(T);
	height = rows;

	checkCudaErrors(cudaMallocPitch(&(dst.ptr), &pitch, width, height));
	checkCudaErrors(cudaMalloc(&(dst.pitch), sizeof(size_t)));
	checkCudaErrors(cudaMalloc(&(dst.rows), sizeof(size_t)));
	checkCudaErrors(cudaMalloc(&(dst.cols), sizeof(size_t)));

	checkCudaErrors(cudaMemcpy2D(dst.ptr, pitch, src, width, width, height, cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMemcpy(dst.pitch, &pitch, sizeof(size_t), cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMemcpy(dst.rows, &rows, sizeof(size_t), cudaMemcpyHostToDevice));
	checkCudaErrors(cudaMemcpy(dst.cols, &cols, sizeof(size_t), cudaMemcpyHostToDevice));

	return dst;
}

template<class T>
static inline Mat<T> createHostMat2d(size_t rows, size_t cols, T *src)
{
	Mat<T>
		dst;
	size_t
		pitch,
		width,
		height;

	width = cols * sizeof(T);
	height = rows;

	dst.ptr = src;
	dst.pitch = new size_t[1]{width};
	dst.rows = new size_t[1]{rows};
	dst.cols = new size_t[1]{cols};

	return dst;
}

template<class T>
__global__ void conv2d2(Mat<T> img, Mat<T> kernel, T *result)
{
	size_t dh;
	size_t rows, cols;
	size_t tid_x, tid_y, offset_x, offset_y;

	rows = *(img.rows);
	cols = *(img.cols);
	dh = floor(static_cast<T>(*(kernel.rows) / 2));

	offset_x = gridDim.x * blockDim.x;
	offset_y = gridDim.y * blockDim.y;

	for (tid_x = threadIdx.x + blockIdx.x * blockDim.x; tid_x < rows; tid_x += offset_x)
		for (tid_y = threadIdx.y + blockIdx.y * blockDim.y; tid_y < cols; tid_y += offset_y)
			result[tid_x * cols + tid_y] = img.eval(kernel, tid_x, tid_y, dh);
}

template<class T>
static void cpu_conv2d2(Mat<T> img, Mat<T> kernel, T *result)
{
	size_t dh;
	size_t rows, cols;
	size_t tid_x, tid_y;

	rows = *(img.rows);
	cols = *(img.cols);
	dh = floor(static_cast<T>(*(kernel.rows) / 2));

	for (tid_x = dh; tid_x < rows - dh; tid_x++)
		for (tid_y = dh; tid_y < cols - dh; tid_y++)
			result[tid_x * cols + tid_y] = img.h_eval(kernel, tid_x, tid_y, dh);
}

template<class T>
__global__ void sobelFilter(Mat<T> img, Mat<T> Gx, Mat<T> Gy, T *result)
{
	size_t dh;
	size_t rows, cols;
	size_t tid_x, tid_y, offset_x, offset_y;
	T x, y;

	rows = *(img.rows);
	cols = *(img.cols);
	dh = floor(static_cast<T>(*(Gx.rows) / 2));

	offset_x = gridDim.x * blockDim.x;
	offset_y = gridDim.y * blockDim.y;

	for (tid_x = threadIdx.x + blockIdx.x * blockDim.x; tid_x < rows; tid_x += offset_x)
		for (tid_y = threadIdx.y + blockIdx.y * blockDim.y; tid_y < cols; tid_y += offset_y)
		{
			x = img.eval(Gx, tid_x, tid_y, dh);
			y = img.eval(Gy, tid_x, tid_y, dh);
			result[tid_x * cols + tid_y] = static_cast<T>(norm2(x, y));
		}
}

template<class T>
static void cpu_sobelFilter(Mat<T> img, Mat<T> Gx, Mat<T> Gy, T *result)
{
	size_t dh;
	size_t rows, cols;
	size_t tid_x, tid_y;
	T x, y;

	rows = *(img.rows);
	cols = *(img.cols);
	dh = floor(static_cast<T>(*(Gx.rows) / 2));

	for (tid_x = dh; tid_x < rows - dh; tid_x++)
		for (tid_y = dh; tid_y < cols - dh; tid_y++)
		{
			x = img.h_eval(Gx, tid_x, tid_y, dh);
			y = img.h_eval(Gy, tid_x, tid_y, dh);
			result[tid_x * cols + tid_y] = static_cast<T>(norm2(x, y));
		}
}

template<class T>
static T *Mat2Pointer(cv::Mat img)
{
	T *ptr = new T[img.rows * img.cols];
	for (int i = 0; i < img.rows; i++)
		memcpy(&(ptr[i*img.cols]), img.ptr<T>(i, 0), img.cols * sizeof(T));
	return ptr;
}

template<class T>
static cv::Mat Pointer2Mat(T *ptr, size_t rows, size_t cols)
{
	cv::Mat img;

	if (std::is_same<T, float>::value)
		img = cv::Mat::zeros(rows, cols, CV_32FC1);
	else
		img = cv::Mat::zeros(rows, cols, CV_8UC1);

	for (int i = 0; i < img.rows; i++)
		memcpy(img.ptr<T>(i, 0), &(ptr[i*img.cols]), img.cols * sizeof(T));
	return img;
}

typedef float type_ref;

template __global__ void conv2d2<type_ref>(Mat<type_ref> img, Mat<type_ref> kernel, type_ref *result);
template __global__ void sobelFilter<type_ref>(Mat<type_ref> img, Mat<type_ref> Gx, Mat<type_ref> Gy, type_ref *result);

static void test_gpu()
{
	//cudaEvent_t start, stop;
	cv::Mat
		in;
	size_t
		rows,
		cols;
	type_ref
		*src_img,
		*src_kernel_g_x,
		*src_kernel_g_y;
	Mat<type_ref>
		d_img,
		d_kernel_g_x,
		d_kernel_g_y;
	type_ref
		*d_result,
		*h_result;
	dim3
		Blocks,
		Threads;
	float
		t_gx = 0;

	//cudaEventCreate(&start);
	//cudaEventCreate(&stop);

	Blocks = dim3(4, 4);
	Threads = dim3(5, 5);

	in = cv::imread("modelos.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	in.convertTo(in, CV_32FC1);

	rows = in.rows;
	cols = in.cols;
	src_img = Mat2Pointer<type_ref>(in);

	d_img = createDevMat2d(rows, cols, src_img);

	h_result = new type_ref[rows * cols];
	checkCudaErrors(cudaMalloc((void **)&d_result, rows * cols * sizeof(type_ref)));

	//Gx
	printf("Testing Gx-filter..\n");
	src_kernel_g_x = new type_ref[3 * 3]{
		-1, 0, 1,
			-2, 0, 2,
			-1, 0, 1};

	d_kernel_g_x = createDevMat2d(3, 3, src_kernel_g_x);
	//cudaEventRecord(start);
	conv2d2<type_ref> << <Blocks, Threads >> >(d_img, d_kernel_g_x, d_result);
	//checkCudaErrors(cudaDeviceSynchronize());
	//cudaEventRecord(stop);
	//cudaEventSynchronize(stop);
	//cudaEventElapsedTime(&t_gx, start, stop);
	checkCudaErrors(cudaMemcpy(h_result, d_result, rows * cols * sizeof(type_ref), cudaMemcpyDeviceToHost));

	in = Pointer2Mat<type_ref>(h_result, in.rows, in.cols);
	in.convertTo(in, CV_8UC1);
	imwrite("Gx.tif", in);

	//Gy
	printf("Testing Gy-filter..\n");
	src_kernel_g_y = new type_ref[3 * 3]{
		1, 2, 1,
			0, 0, 0,
			-1, -2, -1};

	d_kernel_g_y = createDevMat2d(3, 3, src_kernel_g_y);
	conv2d2<type_ref> << <Blocks, Threads >> >(d_img, d_kernel_g_y, d_result);
	checkCudaErrors(cudaDeviceSynchronize());
	checkCudaErrors(cudaMemcpy(h_result, d_result, rows * cols * sizeof(type_ref), cudaMemcpyDeviceToHost));

	in = Pointer2Mat<type_ref>(h_result, in.rows, in.cols);
	in.convertTo(in, CV_8UC1);
	imwrite("Gy.tif", in);

	//Sobel Filter
	printf("Testing gradient..\n");
	sobelFilter<type_ref> << <Blocks, Threads >> >(d_img, d_kernel_g_x, d_kernel_g_y, d_result);
	checkCudaErrors(cudaDeviceSynchronize());
	checkCudaErrors(cudaMemcpy(h_result, d_result, rows * cols * sizeof(type_ref), cudaMemcpyDeviceToHost));

	in = Pointer2Mat<type_ref>(h_result, in.rows, in.cols);
	in.convertTo(in, CV_8UC1);
	imwrite("SobelFilter.tif", in);
}

static void test_cpu()
{
	cv::Mat
		in;
	size_t
		rows,
		cols;
	type_ref
		*src_img,
		*src_kernel_g_x,
		*src_kernel_g_y;
	Mat<type_ref>
		h_img,
		kernel_g_x,
		kernel_g_y;
	type_ref
		*d_result,
		*h_result;
	dim3
		Blocks,
		Threads(5, 5);

	Blocks = dim3(4, 4);
	Threads = dim3(5, 5);

	in = cv::imread("modelos.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	in.convertTo(in, CV_32FC1);

	rows = in.rows;
	cols = in.cols;
	src_img = Mat2Pointer<type_ref>(in);

	h_img = createHostMat2d(rows, cols, src_img);

	h_result = new type_ref[rows * cols];

	src_kernel_g_x = new type_ref[3 * 3]{
		-1, 0, 1,
			-2, 0, 2,
			-1, 0, 1};
	src_kernel_g_y = new type_ref[3 * 3]{
		1, 2, 1,
			0, 0, 0,
			-1, -2, -1};

	//Gx
	printf("Testing Gx-filter..\n");
	kernel_g_x = createHostMat2d(3, 3, src_kernel_g_x);
	cpu_conv2d2<type_ref>(h_img, kernel_g_x, h_result);
	
	in = Pointer2Mat<type_ref>(h_result, in.rows, in.cols);
	in.convertTo(in, CV_8UC1);
	imwrite("Gx_cpu.tif", in);
	//return;
	//Gy
	printf("Testing Gy-filter..\n");
	kernel_g_y = createHostMat2d(3, 3, src_kernel_g_y);
	cpu_conv2d2<type_ref>(h_img, kernel_g_y, h_result);

	in = Pointer2Mat<type_ref>(h_result, in.rows, in.cols);
	in.convertTo(in, CV_8UC1);
	imwrite("Gy_cpu.tif", in);

	//Sobel Filter
	printf("Testing gradient..\n");
	cpu_sobelFilter<type_ref>(h_img, kernel_g_x, kernel_g_y, h_result);

	in = Pointer2Mat<type_ref>(h_result, in.rows, in.cols);
	in.convertTo(in, CV_8UC1);
	imwrite("SobelFilter_cpu.tif", in);
}


int main()
{
	printf("******Testing cpu..\n");
	test_cpu();
	printf("******Testing gpu..\n");
	test_gpu();

	return 0;
}
