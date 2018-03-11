<Query Kind="Program">
  <Namespace>System.Drawing</Namespace>
</Query>

void Main()
{
	/*
	Bitmap bg = LoadBitmap("bg.png");
	var bgPaletteMap = new Dictionary<int, int>() {
		{ Color.Magenta.ToArgb(), 0 },
		{ Color.Aqua.ToArgb(), 1 },
		{ Color.Yellow.ToArgb(), 2}, 
		{ Color.Red.ToArgb(), 3 },
		{ Color.Lime.ToArgb(), 4},
		{ Color.Blue.ToArgb(), 5},
		{ Color.White.ToArgb(), 6},
		{ Color.Black.ToArgb(), 7}
	};
	
	for (int offsetY = 0; offsetY < bg.Height; offsetY += 4) {
		for (int offsetX = 0; offsetX < bg.Width; offsetX += 4) {
			ProcessBitmap(bg, bgPaletteMap, offsetX, offsetY, 4, 4);	
		}
	}
	*/
	
	ProcessBitmap(LoadBitmap("player.png"), new Dictionary<int, int>() {
		{ Color.White.ToArgb(), 0},
		{ Color.Black.ToArgb(), 6}
	});
	
}

// Define other methods and classes here
Bitmap LoadBitmap(string fileRelativeToAssets) {
	return (Bitmap) Image.FromFile(Path.Combine(Path.GetDirectoryName(Util.CurrentQueryPath), $"..\\Assets\\{fileRelativeToAssets}"), true);
}

void ProcessBitmap(Bitmap bitmap, Dictionary<int, int> paletteMap, int offsetX = 0, int offsetY = 0, int width = 0, int height = 0) {
	// bitmap.Dump();
	
	string result = "";
	if (width == 0) width = bitmap.Width;
	if (height == 0) height = bitmap.Height;
	
	for (int row = offsetY; row < offsetY + height; row++) {
		for (int col = offsetX; col < offsetX + width; col += 2) {
			int val = (paletteMap[bitmap.GetPixel(col, row).ToArgb()] << 4) | (paletteMap[bitmap.GetPixel(col + 1, row).ToArgb()]);
			result += $", 0x{val.ToString("X").PadLeft(2, '0')}";	
		}
		// result += "\n";
	}
	
	result.Dump();
}