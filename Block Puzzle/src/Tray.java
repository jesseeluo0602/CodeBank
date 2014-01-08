import java.awt.Point;
import java.util.Random;
public class Tray {
boolean recursed = false;	
String shiftPrevious;
Block[][] myBlocks;
int rows;
int columns;
Tray(int rows, int columns){
	// Constructor to be used in the TrayReader where the starting and goal Trays will be created.
	this.rows=rows;
	this.columns=columns;
	this.myBlocks=new Block[rows][columns];
	this.shiftPrevious=null;
}
Tray(Block[][] shiftedBlocks,int rows, int columns,String shiftPrevious){
	// Constructor to be used by the newRandTray method to make the next random Tray.
	this.rows=rows;
	this.columns=columns;
	this.myBlocks=shiftedBlocks;
	this.shiftPrevious=shiftPrevious;	
}
void addBlock(String toAdd){
	// Used in conjunction with the Tray( rows , columns ) constructor in TrayReader.
	Block thisBlock = new Block(toAdd);
	String[] corners = toAdd.split(" ");
	int mytopRow = Integer.parseInt(corners[0]);
	int myleftCol = Integer.parseInt(corners[1]);
	for(int i=mytopRow;i<=mytopRow + thisBlock.myHeight();i++){
		for(int j=myleftCol;j<=myleftCol + thisBlock.myWidth();j++){
			this.myBlocks[i][j]=thisBlock;
		}
	}
}


Point topleft (int i, int j) {
	// Given a point on the board, return the top left corner of that block, or if
	// a block does not occupy that point, return null.

	if (this.myBlocks[i][j] == null) {
		return null;
	}

	int x = i;
	int y = j;

	if (x != 0) {
		while (x >= 0 && this.myBlocks[i][j] == this.myBlocks[x][j]) {
			x--;
		}
		x++;
	}

	if (y != 0) {
		while (y >= 0 && this.myBlocks[i][j] == this.myBlocks[i][y]) {
			y--;
		}
		y++;
	}

	//System.out.println("x=" +x +", y="+y);
	return new Point (y, x);
}



boolean hasBlock(int row, int column){
	// Checks myTray to see if there is a block at point (row,column).
	////System.out.println("Row and Column: " +row + " "+ column);
	if(this.myBlocks[row][column]!=null){
		return true;
	}else{
		return false;
	}
}

Tray newRandTray(){
	// Outputs a new Tray that has had one Block shifted with a legal move.
	Random randomGenerator = new Random();
	int randomRow = 0;
	int randomCol = 0;
	if(this.rows>1){
		randomRow = randomGenerator.nextInt(this.rows);
	}
	if(this.columns>1){
		randomCol = randomGenerator.nextInt(this.columns);
	}
	int direction = randomGenerator.nextInt(3);
	Point topLeft = this.topleft(randomRow, randomCol);
		while(this.myBlocks[randomRow][randomCol]==null || !this.myRandomMove(direction, randomRow, randomCol, topLeft)){
			if(this.rows>1){
				randomRow = randomGenerator.nextInt(this.rows);
			}
			if(this.columns>1){
				randomCol = randomGenerator.nextInt(this.columns);
			}
			topLeft = this.topleft(randomRow, randomCol);
			direction = randomGenerator.nextInt(3);
		}
		//up
		if(direction == 0){
			String newshiftPrevious = topLeft.y + " " + topLeft.x + " " + (topLeft.y-1) + " " + topLeft.x;
			String comparisonstring = (topLeft.y-1) + " " + topLeft.x + " " + topLeft.y + " " + topLeft.x;
			if (this.recursed == true && comparisonstring.equals(this.shiftPrevious)){this.recursed=true; return this.newRandTray();}
			return new Tray(shiftUp(topLeft), this.rows, this.columns, newshiftPrevious);
		}
		// right
		if(direction == 1){
			String newshiftPrevious = topLeft.y + " " + topLeft.x + " " + topLeft.y + " " + (topLeft.x+1);
			String comparisonstring = topLeft.y + " " + (topLeft.x+1) + " " + topLeft.y + " " + topLeft.x;
			if (this.recursed == true && comparisonstring.equals(this.shiftPrevious)){this.recursed=true; return this.newRandTray();}
			return new Tray(shiftRight(topLeft),this.rows,this.columns, newshiftPrevious);
		}
		// down
		if(direction == 2){
			String newshiftPrevious = topLeft.y + " " + topLeft.x + " " + (topLeft.y+1) + " " + topLeft.x;
			String comparisonstring = (topLeft.y+1)+ " " + topLeft.x + " " + topLeft.y + " " + topLeft.x;
			if (this.recursed == true && comparisonstring.equals(this.shiftPrevious)){this.recursed=true; return this.newRandTray();}
			return new Tray(shiftDown(topLeft),this.rows,this.columns, newshiftPrevious);
		}
		// left
		if(direction == 3){
			String newshiftPrevious = topLeft.y + " " + topLeft.x + " " + topLeft.y + " " + (topLeft.x-1);
			String comparisonstring = topLeft.y + " " + (topLeft.x-1) + " " + topLeft.y + " " + topLeft.x;
			if (this.recursed == true && comparisonstring.equals(this.shiftPrevious)){this.recursed=true; return this.newRandTray();}
			return new Tray(shiftLeft(topLeft),this.rows,this.columns, newshiftPrevious);
		}
		return null;

	}
boolean myRandomMove(int direction, int randomRow, int randomCol, Point topLeft){
		// up
			if(direction == 0){
				//System.out.println("I'm trying to go up.");
				for(int i = 0; i <= this.myBlocks[randomRow][randomCol].myWidth(); i++){
					if(topLeft.y==0){
						//System.out.println("I'm at the upper boundary.");
						return false;
					}
					if(this.hasBlock(topLeft.y-1, i+topLeft.x)){
						//System.out.println("There is a block above me.");
						return false;
					}
				}
				return true;
			}
			// right
			if(direction == 1){
				//System.out.println("I'm trying to go right.");
				for(int i = 0; i <= this.myBlocks[randomRow][randomCol].myHeight(); i++){
					//System.out.print("topLeft.x ="+topLeft.x+" Block.myWidth="+this.myBlocks[randomRow][randomCol].myWidth()+" this.columns -1="+(this.columns-1));
					if( (topLeft.x+this.myBlocks[randomRow][randomCol].myWidth()==this.columns-1)){
						//System.out.println("I am at the far Right.");
						return false;
					}
					if(this.hasBlock(topLeft.y+i, topLeft.x+this.myBlocks[randomRow][randomCol].myWidth()+1)){
						//System.out.println("I have a block to my right.");
						return false;
					}
				}
				return true;
			}
			// down
			if(direction == 2){
				////System.out.println("I'm trying to go down.");
				for(int i = 0; i <= this.myBlocks[randomRow][randomCol].myWidth(); i++){
					if(topLeft.y + this.myBlocks[randomRow][randomCol].myHeight()+1 == this.rows){
						//System.out.println("I am at the bottom.");
						return false;
					}
					if(this.hasBlock(topLeft.y +this.myBlocks[randomRow][randomCol].myHeight()+1, i+topLeft.x)){
						//System.out.println("I have a block to my left.");
						return false;
					}
				}
				return true;
				//String newshiftPrevious = topLeft.y + " " + topLeft.x + " " + (topLeft.y+1) + " " + topLeft.x;
				//return new Tray(shiftDown(topLeft),this.rows-1,this.columns-1, newshiftPrevious);
			}
			// left
			else{
				////System.out.println("I'm trying to go left.");
				for(int i =0; i <= this.myBlocks[randomRow][randomCol].myHeight();i++){
					if(topLeft.x==0 ){
						//System.out.println("I am to the far left.");
						return false;
					}
					if(this.hasBlock(topLeft.y+i,topLeft.x-1)){
						//System.out.println("I have a block to my left.");
						return false;
					}
				}
				return true;
				//String newshiftPrevious = topLeft.y + " " + topLeft.x + " " + topLeft.y + " " + (topLeft.x-1);
				//return new Tray(shiftLeft(topLeft),this.rows-1,this.columns-1, newshiftPrevious);
			}
}
Block[][] shiftUp(Point topLeft){
	Block[][] copy = this.copy();
	Block myBlock = this.myBlocks[topLeft.y][topLeft.x];
	//Delete bottom row of pointers in the copy and adds top row.
	for(int i = topLeft.x; i<=topLeft.x+myBlock.myWidth(); i++){
		copy[topLeft.y-1][i]=myBlock;
		copy[topLeft.y+myBlock.myHeight()][i]=null;
	}
	return copy;
}
Block[][] shiftRight(Point topLeft){
	Block[][] copy = this.copy();
	Block myBlock = this.myBlocks[topLeft.y][topLeft.x];
	//Delete left column of pointers in the copy and adds right column.
	for(int i = topLeft.y; i<=topLeft.y+myBlock.myHeight(); i++){
		copy[i][topLeft.x+myBlock.myWidth()+1]=myBlock;
		copy[i][topLeft.x]=null;
	}
	return copy;
}
Block[][] shiftDown(Point topLeft){
	Block[][] copy = this.copy();
	Block myBlock = this.myBlocks[topLeft.y][topLeft.x];
	//Delete top row of pointers in the copy and adds bottom row.
	for(int i = topLeft.x; i<=topLeft.x+myBlock.myWidth(); i++){
		copy[topLeft.y+myBlock.myHeight()+1][i]=myBlock;
		copy[topLeft.y][i]=null;
	}
	return copy;
}
Block[][] shiftLeft(Point topLeft){
	Block[][] copy = this.copy();
	Block myBlock = this.myBlocks[topLeft.y][topLeft.x];
	//Delete left column of pointers in the copy and adds right column.
	for(int i = topLeft.y; i<=topLeft.y+myBlock.myHeight(); i++){
		copy[i][topLeft.x-1]=myBlock;
		copy[i][topLeft.x+myBlock.myWidth()]=null;
	}
	return copy;
}
Block[][] copy(){
	Block[][] myCopy = new Block[this.rows][this.columns];
	for(int i=0; i<this.rows;i++){
		for(int j=0; j<this.columns; j++){
			myCopy[i][j] = this.myBlocks[i][j];
		}
	}
	return myCopy;
}

public String toString(){
	String myString="";
	for(int i=0; i<this.rows;i++){
		myString+="\n";
		for(int j=0; j<this.columns; j++){
			if(this.myBlocks[i][j]!=null){
				myString += "X";
			}else{ myString += "_";}
		}
	}

	return myString;
	}

public boolean equals(Tray tray){
	if (this.toString()==tray.toString()){
		return true;
	}
	return false;
}

public int hashCode(){
	return this.toString().hashCode();
}
}
