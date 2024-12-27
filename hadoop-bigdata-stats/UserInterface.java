import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.Scanner;

public class UserInterface {

	public static void main (String[] args) {
		
		Scanner input = new Scanner(System.in);
		
		System.out.println("\n1.Category\n2.Brand\n3.Event\nSelect the key type: ");
		int keyType = input.nextInt();
		
		System.out.println("\n1.Mean\n2.Min-Max\n3.Range\nSelect a function: ");
		int mode = input.nextInt();
		
		if (mode == 1) {
			runCommand("hadoop jar /home/emre/BigDataProject/mean.jar Mean " + keyType + " /user/input /user/output");
		} else if (mode == 2) {
			runCommand("hadoop jar /home/emre/BigDataProject/minmax.jar MinMax " + keyType + " /user/input /user/output");
		} else {
			runCommand("hadoop jar /home/emre/BigDataProject/range.jar Range " + keyType + " /user/input /user/output");
		}
		
		input.close();
		
	}
	
	public static void runCommand(String command) {
		String s;
        Process p;
        try {
            p = Runtime.getRuntime().exec(command);
            BufferedReader br = new BufferedReader(
                new InputStreamReader(p.getInputStream()));
            while ((s = br.readLine()) != null)
                System.out.println("-> " + s);
            p.waitFor();
            System.out.println ("exit: " + p.exitValue() + "\nGetting results.\n");
            p.destroy();
            printOutput("hadoop fs -cat /user/output/part-r-00000");
        } catch (Exception e) {}
	}
	
	public static void printOutput(String command) {
		String s;
        Process p;
        try {
            p = Runtime.getRuntime().exec(command);
            BufferedReader br = new BufferedReader(
                new InputStreamReader(p.getInputStream()));
            while ((s = br.readLine()) != null)
            	System.out.println("->" + s);
            p.waitFor();
            System.out.println ("exit: " + p.exitValue());
            p.destroy();
        } catch (Exception e) {}
	}
	
}

