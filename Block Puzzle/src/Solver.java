import java.util.LinkedList;
import java.util.HashSet;


public class Solver {
	private String debug;
	private Tray myStart;
	private Tray myGoal;
	private LinkedList<String> SolutionPath;
	Solver(String debug, Tray myStart,Tray myGoal){
		this.debug=debug;
		this.myStart=myStart;
		this.myGoal=myGoal;
		this.SolutionPath=new LinkedList<String>();
	}
	Solver(Tray myStart, Tray myGoal){
		this.debug=null;
		this.myStart=myStart;
		this.myGoal=myGoal;
		this.SolutionPath=new LinkedList<String>();
	}
	String debug(){
		return this.debug;
	}
	Tray myStart(){
		return this.myStart;
	}
	Tray myGoal(){
		return this.myGoal;
	}
	LinkedList<String> SolutionPath(){
		return this.SolutionPath;
	}
	static boolean isDone(Tray currTray, Tray goalTray) {
			// Iterates through each block in the goal board and checks the current board if the blocks
			// match. If every goal block is in the right location in the current board, return true.
			Block[][] check = goalTray.myBlocks;
			
			// Iterates through each block in the goal board.
			for (int i = 0; i < goalTray.rows; i++) {
				for (int j = 0; j < goalTray.columns; j++) {
						//System.out.println("Point"+i+", "+j+": "+ check[i][j]);
						if (check[i][j] != null) {
							//System.out.println(currTray.rows + " " + currTray.columns);
							if (currTray.hasBlock(i,j)) {
								
								// Checks if the blocks are not equal and returns false.
								if (//currTray.topleft(i,j).equals(goalTray.myBlocks.topleft(i, j)) ||
									currTray.myBlocks[i][j].myHeight() != check[i][j].myHeight() ||
									currTray.myBlocks[i][j].myWidth() != check[i][j].myWidth()) {
									return false;
								}
							} else{
								return false;
							}
							// If the current Board does not contain the goal block, return false.
							
						}
					}
				}
			// If everything passes, return true.
			return true;
		}
	
	void findSolution(){
		// Creates a solution and prints to System.out the solution path, or exits if impossible.
		// Timing is the key to our random algorithm. We assume that there is a relatively short 
		// solution to the Tray; one that can be found close to the starting Tray. Therefore,
		// we generate random Trays starting from myStart. If we cannot find a solution in 80
		// seconds we can say with a high statistical certainty that the board is impossible to
		// solve.
		outerloop:
		while(true){
			Tray currentTray=this.myStart;
			HashSet<Tray> previousTrays=new HashSet<Tray>();
			previousTrays.add(currentTray);
			long startTime = System.currentTimeMillis();
			while (System.currentTimeMillis() - startTime < 500) {
				SolutionPath.add(currentTray.shiftPrevious);
				if (isDone(currentTray, this.myGoal)) {
					break outerloop;
				}
				Tray RandomTray=currentTray.newRandTray();
				while (previousTrays.contains(RandomTray)){
					RandomTray=currentTray.newRandTray();
					System.out.println("ping1");
				}
				previousTrays.add(RandomTray);
				currentTray = RandomTray;

			}
			SolutionPath = new LinkedList<String>();
		}
		for (int i = 0; i < SolutionPath.size(); i++) {
			System.out.println("is this null?");
			System.out.println(SolutionPath.get(i));
		}
		}
	
	
	
	public static void main (String [ ] args) {
		Solver mySolver=null;
		TrayReader toSolve = new TrayReader(args);
		if(args.length==2){
			mySolver = new Solver(toSolve.startingTray(),toSolve.goalTray());
		}
		else if(args.length==3){
			mySolver = new Solver(toSolve.debugOption(),toSolve.startingTray(),toSolve.goalTray());
		}else{
			System.err.println("Incorrect command line entries.");
		}
		mySolver.findSolution();
	}

}

