<Query Kind="Program">
  <Namespace>System.Drawing</Namespace>
</Query>

void Main()
{
	Bitmap level = LoadLevel(1);
	level.Dump();
	PrintLevel(level);
	/*
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
	/*
	ProcessBitmap(LoadBitmap("player.png"), new Dictionary<int, int>() {
		{ Color.White.ToArgb(), 0},
		{ Color.Black.ToArgb(), 8}
	});
	*/
}

// Define other methods and classes here
Bitmap LoadLevel(int level) {
	return (Bitmap) Image.FromFile(Path.Combine(Path.GetDirectoryName(Util.CurrentQueryPath), $"..\\Assets\\levels\\{level.ToString("D3")}.png"), true);
}

void PrintLevel(Bitmap bitmap) {
	string result = "const byte tiles[LEVEL_ROWS][LEVEL_COLS] = {\n";

	for (int row = 0; row < bitmap.Height; row++) {
		if (row != 0) {
			result += ",\n";
		}
		
		for (int col = 0; col < bitmap.Width; col++) {
			if (col == 0) result += "  { ";
			else result += ", ";
			
			int color = bitmap.GetPixel(col, row).ToArgb();
			if (color == Color.Black.ToArgb()) {
				result += "1";
			}
			else if (color == Color.Red.ToArgb()) {
				result += "2";
			}
			else {
				result += "0";
			}
		}
		result += " }";
	}
	result += "\n};";
	
	result.Dump();
}