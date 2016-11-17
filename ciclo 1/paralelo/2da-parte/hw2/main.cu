#pragma once
#ifndef USE_RAND_DATA
#define USE_RAND_DATA
#endif

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <cufft.h>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdio.h>

#include "helper.h"
#include "kernels.cu"
#include "helper_timer.h"

int main()
{
	StopWatchInterface
		*hTimer;
	clock_t 
		t_init, 
		t_end;
	Config 
		cfg;
	std::string 
		buf,
		fname;
	cudaChannelFormatDesc
		formatdesc;
	float 
		*input_kernel;
	cv::Mat 
		img;
	int
		fft_bytes,
		kernel_bytes,
		data_bytes,
		kernel_dim,
		kernel_size,
		kernel_r,
		padding;
	Dim
		threadsByBlockDim,
		imgDim,
		fftDim;
	dim3
		threadBlock,
		BlockGrid;
	
	//initialize variables
	hTimer = NULL;
	sdkCreateTimer(&hTimer);

	threadsByBlockDim = Dim(16, 16);
	
#ifndef USE_RAND_DATA
	kernel_dim = 3;
	kernel_size = kernel_dim * kernel_dim;
	kernel_r = floor(kernel_dim);
	padding = kernel_dim - 1;

	//fname = "C:\\Users\\Public\\Pictures\\Sample Pictures\\Tulips.jpg";
	fname = "C:\\Users\\Public\\Pictures\\Sample Pictures\\Desert.jpg";
	input_kernel = new float[kernel_size] {
		-1, -2, -1,
			0, 0, 0,
			1, 2, 1
	};

	img = cv::imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
	imgDim.cols = img.cols;
	imgDim.rows = img.rows;
	////
#else
	int _rows = 1000;
	int _cols = 1000;
	kernel_dim = 11;
	kernel_size = kernel_dim * kernel_dim;
	kernel_r = floor(kernel_dim);
	padding = kernel_dim - 1;

	input_kernel = new float[kernel_size];

	imgDim.cols = _rows;
	imgDim.rows = _cols;
#endif

	fftDim = setFFTdim(imgDim, padding);
	
	fft_bytes	 = SizeInBytes(fftDim, sizeof(Complex));
	kernel_bytes = SizeInBytes(kernel_size, sizeof(Complex));
	data_bytes	 = SizeInBytes(imgDim, sizeof(Complex));

	formatdesc = cudaCreateChannelDesc<Complex>();
	cufftPlan2d(&cfg.fftplan, fftDim.rows, fftDim.cols, CUFFT_C2C);

	cfg.h_kernel = new Complex[kernel_bytes];
	cfg.h_data = new Complex[data_bytes];
	cfg.h_res_cpu = new Complex[data_bytes];
	cfg.h_res_gpu = new Complex[fft_bytes];
	cudaMallocArray(&cfg.a_kernel, &formatdesc, kernel_dim, kernel_dim);
	cudaMallocArray(&cfg.a_data, &formatdesc, imgDim.cols, imgDim.rows);
	cudaMalloc((void **)&cfg.d_kernel, fft_bytes);
	cudaMalloc((void **)&cfg.d_data, fft_bytes);

#ifndef USE_RAND_DATA
	for (size_t i = 0; i < kernel_size; i++)
		h_Set(cfg.h_kernel, i, 0, 1, input_kernel[i]);
	for (size_t i = 0; i < imgDim.rows; i++)
		for (size_t j = 0; j < imgDim.cols; j++)
			h_Set(cfg.h_data, i, j, imgDim.cols, img.at<uchar>(i, j));
#else
	float val = 1;
	for (size_t i = 0; i < kernel_size; i++)
	{
		val = 1.f;
		h_Set(cfg.h_kernel, i, 0, 1, val);
	}
		
	for (size_t i = 0; i < imgDim.rows; i++)
		for (size_t j = 0; j < imgDim.cols; j++)
		{
			val = (float)rand() / (float)RAND_MAX;
			h_Set(cfg.h_data, i, j, imgDim.cols, val);
		}
#endif

	//assigning memory at gpu
	cudaMemset(cfg.d_kernel, 0, fft_bytes);
	cudaMemset(cfg.d_data, 0, fft_bytes);
	cudaMemcpyToArray(cfg.a_kernel, 0, 0, cfg.h_kernel, kernel_bytes, cudaMemcpyHostToDevice);
	cudaMemcpyToArray(cfg.a_data, 0, 0, cfg.h_data, data_bytes, cudaMemcpyHostToDevice);
	cudaBindTextureToArray(texKernel, cfg.a_kernel);
	cudaBindTextureToArray(texData, cfg.a_data);

	//preprocessing data
	threadBlock = dim3(threadsByBlockDim.rows, threadsByBlockDim.cols);
	
	BlockGrid = dim3(Ceil(kernel_dim, threadBlock.x), Ceil(kernel_dim, threadBlock.y));
	padKernel << <BlockGrid, threadBlock >> >(
		cfg.d_kernel,
		fftDim,
		kernel_dim,
		kernel_r);

	BlockGrid = dim3(Ceil(fftDim.cols, threadBlock.x), Ceil(fftDim.rows, threadBlock.y));
	padData << <BlockGrid, threadBlock >> >(
		cfg.d_data,
		fftDim,
		imgDim,
		kernel_r);
	
	//convolution using convolution theorem
	std::cout << "Running with cufft.." << std::endl;
	sdkResetTimer(&hTimer);
	cudaDeviceSynchronize();
	sdkStartTimer(&hTimer);
	cufftExecC2C(cfg.fftplan, (cufftComplex *)cfg.d_kernel, (cufftComplex *)cfg.d_kernel, CUFFT_FORWARD);
	cufftExecC2C(cfg.fftplan, (cufftComplex *)cfg.d_data, (cufftComplex *)cfg.d_data, CUFFT_FORWARD);
	product(cfg.d_data, cfg.d_kernel, fftDim);
	cufftExecC2C(cfg.fftplan, (cufftComplex *)cfg.d_data, (cufftComplex *)cfg.d_data, CUFFT_INVERSE);
	cudaDeviceSynchronize();
	sdkStopTimer(&hTimer);
	
	cudaMemcpy(cfg.h_res_gpu, cfg.d_data, fft_bytes, cudaMemcpyDeviceToHost);
	
	std::cout << "Running with cpu::conv2d.." << std::endl;
	t_init = clock();
	cpu_conv2d(cfg.h_res_cpu, cfg.h_data, cfg.h_kernel, imgDim, kernel_dim);
	t_end = clock();

	//saving
#ifndef RAND_DATA
	std::cout << "Saving results.." << std::endl;
	save("result.jpg", cfg.h_res_gpu, imgDim, fftDim);
#endif

	//comparing with conv2d
	std::cout << "Checking.." << std::endl;
	buf = check(cfg.h_res_cpu, cfg.h_res_gpu, imgDim.rows, imgDim.cols, fftDim.cols) ? ":)" : "-.-";
	std::cout << "..Test: " << buf.c_str() << std::endl;
	
	//release memory
	Shutdown(cfg);

	std::cout << "GPU: " << sdkGetTimerValue(&hTimer) << "ms" << std::endl;
	std::cout << "CPU: " << ((double)(t_end - t_init) / CLOCKS_PER_SEC) * 1e3 << "ms" << std::endl;
	
	//system("pause");
}
