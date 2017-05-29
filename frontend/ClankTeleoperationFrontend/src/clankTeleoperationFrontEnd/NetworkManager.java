package clankTeleoperationFrontEnd;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.Socket;


public class NetworkManager {
	private Socket RobotCommunicationSocket;
	private DataOutputStream RobotOutgoingCommunicationStream;
	private DataInputStream RobotIncomingCommunicationStream;
	private boolean IsConnected = false;
	
	public boolean openConnection(String ipAddress, int portNumber, int numberOfRetries) {

		/*Ping-pong check*/
		for (int i = 0; i < numberOfRetries; i++) {			
			try {
				Common.DebugPrintln(String.format("Attempting to connect to %s:%d", portNumber, numberOfRetries));
				Common.DebugPrint(String.format("Attempt %d of %d...", i, numberOfRetries));
				this.RobotCommunicationSocket = new Socket(ipAddress, portNumber);
				this.RobotOutgoingCommunicationStream = new DataOutputStream(RobotCommunicationSocket.getOutputStream());
				this.RobotIncomingCommunicationStream = new DataInputStream(RobotCommunicationSocket.getInputStream());
				Thread.sleep(100);
			
			
				this.RobotOutgoingCommunicationStream.writeUTF("Ping\n");
				Thread.sleep(1000);
				String result = this.RobotIncomingCommunicationStream.readUTF();
				if (result == "Pong") {
					Common.DebugPrint("Success!");
					this.IsConnected = true;
					return true;
				}
			} catch (Exception e) {
				System.out.println("Error establishing communications: ");
				System.out.println(e);
				System.out.println("Stack trace:");
				e.printStackTrace();
			}
		}
		
		/*We've exhausted the retries*/
		Common.DebugPrint("Failed to connect.");
		
		this.RobotCommunicationSocket = null;
		this.RobotOutgoingCommunicationStream = null;
		this.RobotIncomingCommunicationStream = null;
		this.IsConnected = false;
		return false;
	}
	
	public boolean IsConnected() {
		return this.IsConnected;
	}
	
	public boolean writeData(String data) {
		/*TODO: Implement*/
		return true;
	}
	
}
