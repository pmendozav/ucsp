namespace core
{
	template<typename T>
	void Hist(Image<T> *in, p_vector<p_double> &out, p_bool normalized = false, p_bool acum = false)
	{
		p_uchar *ptr;
		p_int rows, cols, depth, offset, val, N, nbins;

		ptr = (p_uchar *)in->data;
		rows = in->rows;
		cols = in->cols;
		depth = in->depth;

		N = rows * cols;

		if (depth == 8)
			nbins = 256;
		else
		{
			nbins = 0;
			//TODO: throw error
		}

		out.resize(nbins);

		//
		for (auto i = 0; i < rows; i++)
		{
			for (auto j = 0; j < cols; j++)
			{
				val = ptr[i*cols + j];
				out[val] += 1;
			}
		}

		if (normalized)
		{
			for (auto i = 0; i < nbins; i++)
			{
				out[i] /= N;
				//out[i] = (out[i]/N)*255;
			}
		}

		if (acum)
		{
			for (auto i = 1; i < nbins; i++)
			{
				out[i] += out[i - 1];
			}
		}
	}

	template<typename T>
	void Equ(Image<T> *img, const p_vector<p_double> &T)
	{
		p_uchar *ptr, r;
		p_int rows, cols, depth, offset, pos, N, nbins;

		ptr = (p_uchar *)img->data;
		rows = img->rows;
		cols = img->cols;
		depth = img->depth;

		if (depth == 8)
			N = 255;
		else
		{
			N = 0;
			//TODO: throw error
		}

		for (auto i = 0; i < rows; i++)
		{
			for (auto j = 0; j < cols; j++)
			{
				pos = i*cols + j;
				ptr[pos] = round(N * T[ptr[pos]]);
			}
		}
	}
}