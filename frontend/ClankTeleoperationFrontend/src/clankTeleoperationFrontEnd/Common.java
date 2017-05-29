package clankTeleoperationFrontEnd;

public class Common {
	public static boolean DebugPrint = true;
	
	public static void DebugPrint(String s) {
		if (DebugPrint) {
			System.out.print(s);
		}
	}
	
	public static void DebugPrintln(String s) {
		if (DebugPrint) {
			System.out.println(s);
		}
	}


}
