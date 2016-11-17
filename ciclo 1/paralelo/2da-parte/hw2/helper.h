#pragma once

#ifndef Complex
typedef float2 Complex;
#endif

texture<Complex, 2, cudaReadModeElementType> texKernel;
texture<Complex, 2, cudaReadModeElementType> texData;

struct Dim
{
	int rows;
	int cols;
	Dim(){};
	Dim(const int &rows, const int &cols) : cols(cols), rows(rows){};
};

struct Config
{
	cufftHandle
		fftplan;
	Complex
		*d_kernel,
		*d_data,
		*h_kernel,
		*h_data,
		*h_res_cpu,
		*h_res_gpu;
	cudaArray
		*a_kernel,
		*a_data;		
};

inline int Ceil(const int &a, const int &b){
	return static_cast<int>(ceil(static_cast<float>(a) / static_cast<float>(b)));
}

inline int GetkB(const int &a, const  int &b){
	return (a % b == 0) ? a : (a / b + 1) * b;
}

int calculateFFTsize(int dataSize){
	int 
		hiBit;
	unsigned int 
		lowPOT, 
		hiPOT;

	dataSize = GetkB(dataSize, 16);

	for (hiBit = 31; hiBit >= 0; hiBit--)
		if (dataSize & (1U << hiBit)) break;

	lowPOT = 1U << hiBit;
	if (lowPOT == dataSize) return dataSize;

	hiPOT = 1U << (hiBit + 1);
	if (hiPOT <= 1024)
		return hiPOT;
	else
		return GetkB(dataSize, 512);
}
Dim setFFTdim(const Dim &imgDim, const int &padding)
{
	return{ calculateFFTsize(imgDim.rows + padding), calculateFFTsize(imgDim.cols + padding) };
}

bool check(const Complex *h_res_cpu, const Complex *h_res_gpu, const int &rows, const int &cols, const int &fft_cols)
{
	double 
		sum_delta2,
		sum_ref2;
	Complex 
		cpu, 
		gpu;

	sum_delta2 = sum_ref2 = 0;
	for (size_t i = 0; i < rows; i++)
		for (size_t j = 0; j < cols; j++)
		{
			cpu = h_res_cpu[i * cols + j];
			gpu = h_res_gpu[i * fft_cols + j];
			sum_delta2 += (cpu.x - gpu.x) * (cpu.x - gpu.x) + (cpu.y - gpu.y) * (cpu.y - gpu.y);
			sum_ref2 += cpu.x * cpu.x + cpu.y * cpu.y;
		}

	return (sqrt(sum_delta2 / sum_ref2) < 1e-4);
}

void Shutdown(Config &cfg)
{
	cudaUnbindTexture(texData);
	cudaUnbindTexture(texKernel);
	cufftDestroy(cfg.fftplan);
	cudaFree(cfg.d_data);
	cudaFree(cfg.d_kernel);
	cudaFreeArray(cfg.a_data);
	cudaFreeArray(cfg.a_kernel);

	delete 
		cfg.h_kernel, 
		cfg.h_data,
		cfg.h_res_cpu,
		cfg.h_res_gpu;
}

inline int SizeInBytes(const Dim &dim, const int &size_one)
{
	return dim.cols * dim.rows * size_one;
}
inline int SizeInBytes(const int &size, const int &size_one)
{
	return size * size_one;
}

template <typename T>
void h_Set(Complex *v, const int i, const int j, const  int offset, T &val)
{
	v[i * offset + j].x = static_cast<float>(val);
	v[i * offset + j].y = static_cast<float>(0);
}
void save(const std::string &fname, const Complex *img, const Dim &dim_in, const  Dim &dim_out)
{
	cv::Mat tmp = cv::Mat(dim_in.rows, dim_in.cols, CV_8UC1);
	for (size_t i = 0; i < (dim_in.rows); i++){
		for (size_t j = 0; j < (dim_in.cols); j++){
			tmp.at<uchar>(i, j) = (uchar)(img[i * dim_out.cols + j].x);
		}
	}
	cv::imwrite(fname, tmp);
}

void cpu_product(Complex& a, const Complex &b, const Complex &c){
	Complex tmp;
	tmp.x = a.x + b.x * c.x - b.y * c.y;
	tmp.y = a.y + b.x * c.y + b.y * c.x;
	a = tmp;
}

void cpu_conv2d(
	Complex *h_Result,
	const Complex *h_data,
	const Complex *h_kernel,
	const Dim &data_dim,
	const int &kernel_dim)
{
	int rows, cols;
	int _i, _j;
	int r;
	Complex sum;

	r = static_cast<int>(floor(kernel_dim));
	rows = data_dim.rows;
	cols = data_dim.cols;

	for (size_t i = 0; i < rows; i++)
		for (size_t j = 0; j < cols; j++)
		{
			sum = { 0, 0 };
			for (size_t ki = -(kernel_dim - r - 1); ki <= r; ki++)
				for (size_t kj = -(kernel_dim - r - 1); kj <= r; kj++)
				{
					_i = j + kj;
					_j = i + ki;
					if (_i < 0) _i = 0;
					if (_j < 0) _j = 0;
					if (_i >= cols) _i = cols - 1;
					if (_j >= rows) _j = rows - 1;

					cpu_product(sum, h_data[_j * cols + _i], h_kernel[(r - ki) * kernel_dim + (r - kj)]);
				}
			h_Result[i * cols + j] = sum;
		}
}