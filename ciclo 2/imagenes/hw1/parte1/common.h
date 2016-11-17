const int NO_VALID_CHANNEL = 0;

namespace core
{
	//grayscale and rgb (bgr format)
	template<typename T>
	class Image
	{
	//private:

	public:
		T *data;
		p_int rows;
		p_int cols;
		p_int channels;
		p_int depth;

		Image();
		Image(p_int rows, p_int cols, p_int channels = 1, p_int depth = 8);
		Image(p_uchar *data, p_int rows, p_int cols, p_int channels, p_int depth = 8);
		Image(Bitmap &bitmap);
	};

	template<typename T>
	Image<T>::Image()
	{
		rows = 0;
		cols = 0;
		depth = 0;
		channels = NO_VALID_CHANNEL;
		data = NULL;
	}

	template<typename T>
	Image<T>::Image(p_int _rows, p_int _cols, p_int _channels, p_int _depth) :
		rows(_rows), cols(_cols), channels(_channels), depth(_depth)
	{
		data = (T *)(new p_uchar[rows * cols * channels]);
	}

	template<typename T>
	Image<T>::Image(p_uchar *_data, p_int _rows, p_int _cols, p_int _channels, p_int _depth) :
		rows(_rows), cols(_cols), channels(_channels), depth(_depth)
	{
		data = (T *)_data;
	}

	template<typename T>
	Image<T>::Image(Bitmap &bitmap)
	{
		data = (T *)bitmap.GetRawData();
		rows = bitmap.Rows();
		cols = bitmap.Cols();
		depth = bitmap.Depth();

		switch (depth)
		{
		case 8:
			channels = 1;
			break;
		case 24:
			channels = 3;
			break;
		default:
			channels = 0;
			break;
		}
	}
}

