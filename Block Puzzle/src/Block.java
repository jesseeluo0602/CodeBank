public class Block {
	
	// myHeight and myWidth start at index 0. So a block 1x1 is (0,0).
	private int myHeight;
	private int myWidth;
	
	public Block (String s) {
		String[] corners = s.split(" ");
		// Checks if the line input has four arguments passed in for the top left corner
		// and bottom right corner. If not, throw an error.
		if (corners.length != 4) {
			throw new IllegalArgumentException("Invalid number of arguments: " +
					"Board construction requires FOUR integers to be passed in");
		}
			this.myHeight=Integer.parseInt(corners[2])-Integer.parseInt(corners[0]);
			this.myWidth=Integer.parseInt(corners[3])-Integer.parseInt(corners[1]);
	}
	int myHeight(){
		return this.myHeight;
	}
	int myWidth(){
		return this.myWidth;
	}
}
