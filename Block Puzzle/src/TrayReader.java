import java.util.Scanner;
public class TrayReader {
	
		private Tray startingTray;
		private Tray goalTray;
		private String debugOption;
		 
		// The first command-line argument, if specified, is the debugging option
	    // The second command-line argument(or first if no debugging) is the name of
	    // a file from which to read the starting Tray.
	    // The third command-line argument (or second if no debugging) is the name of
	    // a file from which to read the goal Tray.
	    
	    TrayReader (String [ ] args) {
	        if (args.length > 3){
	        	System.err.println ("*** bad Tray File: Cannot take more then 3 arguments.");
	            System.exit (1);
	        }
	        if (args.length < 2 ){
	        	System.err.println ("*** bad Tray File: Need at least 2 arguments.");
	            System.exit (1);
	        }
	        // if there are all three arguments. This implies there is a debugging argument at the front.
	    	if (args.length == 3) {
	    		this.debugOption=args[0];
	    		this.makeStartTray(args[1]);
	    		this.makeGoalTray(args[2]);
	    	}
	    	// if there is no debugging command at the beginning.
	    	if (args.length == 2){
	    		this.makeStartTray(args[0]);
	    		this.makeGoalTray(args[1]);
	    	}
	    }
	    String debugOption(){return this.debugOption;}
	    Tray goalTray(){return this.goalTray;}
	    Tray startingTray(){return this.startingTray;}
	    void makeStartTray(String arg){
	    	// Creates the first Tray state based on the .txt file name passed in.
	    	InputSource boardStart = new InputSource(arg);
	    	String curStartLine = boardStart.readLine();
	    	Scanner startLineScanner=new Scanner(curStartLine);
	    	int myLength = Integer.parseInt(startLineScanner.next());
	    	int myWidth = Integer.parseInt(startLineScanner.next());
	    	startLineScanner.close();
	    	startingTray=new Tray(myLength, myWidth);
	    	curStartLine=boardStart.readLine();
	    	while(curStartLine!=null){
	    		this.startingTray.addBlock(curStartLine);
	    		curStartLine=boardStart.readLine();
	    	}
	    }
	    void makeGoalTray(String arg){
	    	// Creates the goal Tray state based on the .txt file name passed in.
	    	InputSource boardGoal = new InputSource(arg);
	    	goalTray=new Tray(startingTray.rows,startingTray.columns);
	    	String curGoalLine = boardGoal.readLine();
	    	while(curGoalLine!=null){
	    		goalTray.addBlock(curGoalLine);
	    		curGoalLine = boardGoal.readLine();
	    	}
	    }

	}


