
namespace core
{	
	template<typename T>
	p_vector<Image<T>> Split(Image<T> in)
	{
		p_uchar *data;
		p_vector<Image<T>> out;
		p_vector<p_uchar *> buf;
		p_int offset, dim;
		p_int rows, cols;

		rows = in.rows;
		cols = in.cols;
		data = in.data;
			
		switch (in.channels)
		{
		case 1:
			offset = 0;
			dim = 1;
			break;
		case 3:
			offset = 3;
			dim = 3;
			break;
		default: //NO_VALID_CHANNEL or other
			return out; //TODO: throw error
			break;
		}
				
		out.resize(dim);
		buf.resize(dim);
		for (auto i = 0; i < dim; i++)
			buf[i] = new p_uchar[rows * cols];

		for (auto i = 0; i < rows; i++)
		{
			for (auto j = 0; j < cols; j++)
			{
				for (auto d = 0; d < dim; d++)
				{
					buf[d][i*cols +j] = data[i*cols*dim + j*dim + d];
				}
			}
		}
		
		for (auto i = 0; i < dim; i++)
			out[i] = Image<T>(buf[i], rows, cols, 1);

		return out;
	}

	template<typename T>
	void Merge(const p_vector<Image<T>> &src, Image<T> &dst)
	{
		p_uchar *data;
		p_vector<Image<T>> out;
		p_vector<p_uchar *> ptr;
		p_uchar *buf;
		p_int offset, dim;
		p_int rows, cols;

		rows = src[0].rows;
		cols = src[0].cols;
		dim = src.size();
		
		if (dim != 3)
		{
			//TODO: throw error
			return;
		}

		ptr.resize(dim);
		for (auto i = 0; i < dim; i++)
			ptr[i] = (p_uchar *)src[i].data;

		buf = new p_uchar[rows * cols * dim];

		for (auto i = 0; i < rows; i++)
		{
			for (auto j = 0; j < cols; j++)
			{
				for (auto d = 0; d < dim; d++)
				{
					//buf[d][i*cols + j] = data[i*cols*dim + j*dim + d];
					buf[i*cols*dim + j*dim + d] = ptr[d][i*cols + j];
				}
			}
		}

		dst = Image<T>(buf, rows, cols, dim);
	}
};












