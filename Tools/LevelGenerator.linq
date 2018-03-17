<Query Kind="Program">
  <Namespace>System.Drawing</Namespace>
</Query>

void Main()
{
	for (int level = 0; level <= 2; level++) 
	{
		GenerateLevelFile(level);
	}
}

void GenerateLevelFile(int level) 
{
	Bitmap bgBmp = LoadLevelBackground(level);
	bgBmp.Dump();
	Bitmap blocksBmp = LoadLevelBlocks(level);
	blocksBmp.Dump();
	IEnumerable<byte> blockData = GenerateBlockData(blocksBmp);
	Color[] indices = ReadIndices(bgBmp);
	IEnumerable<byte> palette0 = GeneratePaletteData(bgBmp, indices, 0);
	IEnumerable<byte> palette1 = GeneratePaletteData(bgBmp, indices, 1);
	IEnumerable<byte> palette2 = GeneratePaletteData(bgBmp, indices, 2);
	IEnumerable<byte> palette3 = GeneratePaletteData(bgBmp, indices, 3);
	IEnumerable<byte> bgData = GenerateBackgroundData(bgBmp, indices);
	WriteFile(level, blockData, palette0, palette1, palette2, palette3, bgData);
}

Bitmap LoadLevelBackground(int level) 
{
	return (Bitmap) Image.FromFile(Path.Combine(Path.GetDirectoryName(Util.CurrentQueryPath), $"..\\Assets\\levels\\bg{level.ToString("D3")}.png"), true);
}

Bitmap LoadLevelBlocks(int level) 
{
	return (Bitmap) Image.FromFile(Path.Combine(Path.GetDirectoryName(Util.CurrentQueryPath), $"..\\Assets\\levels\\{level.ToString("D3")}.png"), true);
}

Color[] ReadIndices(Bitmap bmp) 
{
	Color[] result = new Color[16];
	for (int i = 0; i < 16; i++) 
	{
		result[i] = bmp.GetPixel(i * 4, 0);
	}
	return result;
}

IEnumerable<byte> GenerateBlockData(Bitmap bmp) 
{
	for (int y = 0; y < 32; y++)
	{
		for (int x = 0; x < 40; x++) 
		{
			int argb = bmp.GetPixel(x, y).ToArgb();
			// Solid
			if (argb == Color.Black.ToArgb()) 
			{
				yield return 1;
			}
			// Deadly
			else if (argb == Color.Red.ToArgb()) 
			{
				yield return 2;
			}
			// Open
			else
			{
				yield return 0;
			}
		}
	}
}

IEnumerable<byte> GeneratePaletteData(Bitmap bmp, Color[] indices, int paletteIndex) 
{
	for (int i = 0; i < 16; i++) 
	{
		Color color = bmp.GetPixel(i * 4, 4 + paletteIndex);
		int rgb565 = ColorToRgb565(color);
		yield return (byte)(rgb565 & 0xff);
		yield return (byte)((rgb565 & 0xff00) >> 8);
	}
}

IEnumerable<byte> GenerateBackgroundData(Bitmap bmp, Color[] indices) 
{
	for (int y = 0; y < 128; y++)
	{
		for (int x = 0; x < 160; x += 2) 
		{
			int high = FindIndex(indices, bmp.GetPixel(x, y + 8));
			int low = FindIndex(indices, bmp.GetPixel(x + 1, y + 8));
			// Console.WriteLine($"({x+1}, {y+8})");
			yield return (byte)(((high & 0xF) << 4) | (low & 0xF));
		}
	}
}

int FindIndex(Color[] indices, Color color) 
{
	for (int i = 0; i < indices.Length; i++) 
	{
		if (indices[i].Equals(color)) return i;
	}
	
	throw new Exception($"Color {color} not found.");
}

void WriteFile(int level, IEnumerable<byte> blockData, IEnumerable<byte> palette0, IEnumerable<byte> palette1, IEnumerable<byte> palette2, IEnumerable<byte> palette3, IEnumerable<byte> bgData)
{
	string filename = Path.Combine(Path.GetDirectoryName(Util.CurrentQueryPath), $"..\\binaries\\dn game\\levels\\{level.ToString("D3")}.dnl");
	IEnumerable<byte> header = new byte[] { (byte)'d', (byte)'n', 1 };
	File.WriteAllBytes(filename, header
		.Concat(blockData)
		.Concat(palette0)
		.Concat(palette1)
		.Concat(palette2)
		.Concat(palette3)
		.Concat(bgData)
		.ToArray());
}

int ColorToRgb565(Color color) {
	return ((color.R & 0xF8) << 8) | ((color.G & 0xFC) << 3) | (color.B >> 3);
}