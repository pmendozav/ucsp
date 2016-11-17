/*
*Headers: FILEHEADER (WIN2XBITMAPHEADER), INFOHEADER (WIN3XBITMAPHEADER), 
*PALETTEHEADER (WIN3XPALETTEELEMENT) were extracted from:
*http://www.fileformat.info/format/bmp/egff.htm and
*https://msdn.microsoft.com/en-us/library/windows/desktop/dd183381(v=vs.85).aspx
*/

typedef struct _CIEXYZ
{
	LONG X;
	LONG Y;
	LONG Z;
};

typedef struct _CIEXYZTRIPLE
{
	_CIEXYZ  ciexyzRed;
	_CIEXYZ  ciexyzGreen;
	_CIEXYZ  ciexyzBlue;
} ;


#pragma pack(push, 1)
struct FILEHEADER
{
	p_word FileType;  //File type, always 4D42h ("BM")
	p_dword FileSize;  //Size of the file in bytes
	p_word Reserved1;  //Always 0
	p_word Reserved2;  //Always 0
	p_dword BitmapOffset;  //Starting position of image data in bytes
};
#pragma pack(pop)

#pragma pack(push, 1)
struct INFOHEADER
{
	//v3
	p_dword Size;  //Size of this header in bytes
	p_long Width;  //Image width in pixels
	p_long Height;  //Image height in pixels
	p_word Planes; //Number of color planes
	p_word BitsPerPixel; //Number of bits per pixel
	p_dword Compression;//Compression methods used
	p_dword SizeOfBitmap;  //Size of bitmap in bytes
	p_long XPelsPerMeter;  //Horizontal resolution in pixels per meter
	p_long YPelsPerMeter;  //Vertical resolution in pixels per meter
	p_dword ColorsUsed;  //Number of colors in the image
	p_dword ColorsImportant;  //Minimum number of important colors

	//v4
	p_dword        RedMask;
	p_dword        GreenMask;
	p_dword        BlueMask;
	p_dword        AlphaMask;
	p_dword        CSType;
	_CIEXYZTRIPLE  Endpoints;
	p_dword        GammaRed;
	p_dword        GammaGreen;
	p_dword        GammaBlue;

	//v5
	p_dword        Intent;
	p_dword        ProfileData;
	p_dword        ProfileSize;
	p_dword        Reserved;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct PALETTEHEADER
{
	BYTE Blue;      //Blue component
	BYTE Green;     //Green component
	BYTE Red;       //Red component
	BYTE Reserved;  //Padding (always 0)
};
#pragma pack(pop)

typedef FILEHEADER fileheader;
typedef INFOHEADER infoheader;
typedef PALETTEHEADER paletteheader;

const p_word BMP_FORMAT = 0x4D42;
const p_int BITS_IN_BYTE = 8;
const p_int FILEHEADER_SIZE = 14;
const p_int INFOHEADER_SIZE = 40;

class Bitmap
{
private:
	p_long bytes_h_info;
	infoheader h_info;
	fileheader h_file;
	paletteheader *h_palette;
	p_long palette_size;
	
	p_word Type();
	p_dword OffBits();
	p_dword ImageSize();
	p_word BitsPerPixel();
	p_dword SizeInBytes();
public:
	p_byte *data;

	void Load(p_char *filename);
	void Save(p_char *filename);
	void PrintHeaders();
	
	p_long Rows();
	p_long Cols();
	p_long Depth();

	p_byte *GetRawData();
};

void Bitmap::Load(p_char *filename)
{
	std::ifstream file;

	file.open(filename, std::fstream::binary);

	file.read((p_char *)&h_file, sizeof(fileheader));
	
	file.read((p_char *)&bytes_h_info, sizeof(p_dword));
	file.seekg((p_dword)file.tellg() - sizeof(p_dword));
	
	file.read((p_char *)&h_info, bytes_h_info);

	switch (h_info.BitsPerPixel)
	{
	case 1:
		palette_size = 2;
		break;
	//case 2:
	//	palette_size = ?;
	//	break;
	case 4:
		palette_size = 16;
		break;
	case 8:
		palette_size = 256;
		break;
	//case 16:
	//	palette_size = ?;
	//	break;
	//case 24:
	//	palette_size = 0;
	//	break;
	default:
		palette_size = 0;
		break;
	}

	if (palette_size != 0)
	{
		h_palette = new paletteheader[palette_size];
		file.read((p_char *)h_palette, palette_size * sizeof(paletteheader));
	}

	data = new p_byte[SizeInBytes()];
	file.read((p_char *)data, SizeInBytes());

	file.close();
}

void Bitmap::Save(p_char *filename)
{
	fileheader fheader;
	infoheader iheader;
	std::ofstream file;

	if (!data) return;

	file.open(filename, std::fstream::binary);
	if (!file.is_open()) return;

	//fheader
	fheader.FileType = MAKEWORD('B', 'M');
	fheader.FileSize = FILEHEADER_SIZE;
	fheader.Reserved1 = 0;
	fheader.Reserved2 = 0;
	fheader.BitmapOffset = h_file.BitmapOffset;

	//iheader
	iheader.Size = INFOHEADER_SIZE;
	iheader.Width = h_info.Width;
	iheader.Height = h_info.Height;
	iheader.Planes = 1;
	iheader.BitsPerPixel = h_info.BitsPerPixel;
	iheader.Compression = 0;
	iheader.SizeOfBitmap = 0;
	iheader.XPelsPerMeter = h_info.XPelsPerMeter;
	iheader.YPelsPerMeter = h_info.YPelsPerMeter;
	iheader.ColorsUsed = h_info.ColorsUsed;
	iheader.ColorsImportant = 0;

	file.write((p_char *)&(fheader), sizeof(fileheader));
	file.write((p_char *)&iheader, bytes_h_info);

	//palette header
	if (!h_palette == NULL)
		file.write((p_char *)h_palette, palette_size * sizeof(paletteheader));

	//data
	file.write((p_char *)data, SizeInBytes());

	file.close();
}

p_byte *Bitmap::GetRawData()
{
	return data;
}

p_long Bitmap::Depth()
{
	return h_info.BitsPerPixel;
}

void Bitmap::PrintHeaders()
{
	std::cout << "File Header" << std::endl;
	std::cout << "FileType = " << h_file.FileType << std::endl;
	std::cout << "FileSize = " << h_file.FileSize << std::endl;
	std::cout << "Reserved1 = " << h_file.Reserved1 << std::endl;
	std::cout << "Reserved2 = " << h_file.Reserved2 << std::endl;
	std::cout << "BitmapOffset = " << h_file.BitmapOffset << std::endl;
	
	std::cout << "Information Header" << std::endl;
	std::cout << "Size = " << h_info.Size << std::endl;
	std::cout << "Width = " << h_info.Width << std::endl;
	std::cout << "Height = " << h_info.Height << std::endl;
	std::cout << "Planes = " << h_info.Planes << std::endl;
	std::cout << "BitsPerPixel = " << h_info.BitsPerPixel << std::endl;
	std::cout << "Compression = " << h_info.Compression << std::endl;
	std::cout << "SizeOfBitmap = " << h_info.SizeOfBitmap << std::endl;
	std::cout << "XPelsPerMeter = "	<< h_info.XPelsPerMeter	<< std::endl;
	std::cout << "YPelsPerMeter = "	<< h_info.YPelsPerMeter	<< std::endl;
	std::cout << "ColorsUsed = " << h_info.ColorsUsed << std::endl;
	std::cout << "ColorsImportant = " << h_info.ColorsImportant << std::endl;
}

p_word Bitmap::Type()
{
	return h_file.FileType;
}

p_dword Bitmap::OffBits()
{
	return h_file.BitmapOffset;
}

p_dword Bitmap::ImageSize()
{
	return h_info.SizeOfBitmap;
}

p_long Bitmap::Rows()
{
	return h_info.Height;
}

p_long Bitmap::Cols()
{
	return h_info.Width;
}
p_word Bitmap::BitsPerPixel()
{
	return h_info.BitsPerPixel;
}
p_dword Bitmap::SizeInBytes()
{
	if (h_info.SizeOfBitmap == 0)
		return Rows() * Cols() * BitsPerPixel() / BITS_IN_BYTE;
	else
		return h_info.SizeOfBitmap;
}

