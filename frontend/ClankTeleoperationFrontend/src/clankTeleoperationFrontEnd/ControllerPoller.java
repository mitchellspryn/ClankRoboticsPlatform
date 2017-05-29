package clankTeleoperationFrontEnd;


import java.util.List;

import net.java.games.input.Component;
import net.java.games.input.Controller;
import net.java.games.input.ControllerEnvironment;

public class ControllerPoller {

	private Thread PollThread;
	private IControllerCommandMapping Controls;
	private NetworkManager NetworkManager;
	private int PollWaitMs = 20;
	
	private boolean BackgroundThreadRunning = false;
	private boolean BackgroundThreadRequestStop = false;
	
	private Component[] previousComponents; //It appears to be impossible to clone a controller directly...
	
	public ControllerPoller(IControllerCommandMapping controls, NetworkManager nm) {
		this.Controls = controls;
		this.NetworkManager = nm;
		this.PollThread = new Thread(new Runnable(){
			@Override
			public void run() {
				RunInBackground();
			}
		});
	}
	
	public void SetPollWaitMs(int ms) {
		this.PollWaitMs = ms;
	}
	
	public int GetPollWaitMs() {
		return this.PollWaitMs;
	}
	
	public void Start() {
		this.BackgroundThreadRequestStop = false;
		this.PollThread.start();
	}
	
	public void Stop() {
		this.BackgroundThreadRequestStop = true;
		while(this.BackgroundThreadRunning) {}; //Block until thread terminates
	}
	
	/*Main background thread*/
	private void RunInBackground() {
		this.BackgroundThreadRunning = true;
		
		/*Grab the initial state of the controller*/
		ControllerEnvironment ce = ControllerEnvironment.getDefaultEnvironment();
		Controller[] controllerList = ce.getControllers();

		/*Also verify that there is only one controller plugged in*/
		/*TODO: Multi-controller support?*/
		if (controllerList.length != 1) {
			System.out.println("Error in Controller Poller: Incompatible number of controllers plugged in.");
			System.out.println("The following devices are detected:");
			for (int i = 0; i < controllerList.length; i++) {
				System.out.println(i + ": " + controllerList[i].getName() + ", " + controllerList[i].getType());
			}
			this.BackgroundThreadRunning = false;
			return;
		}
		
		Controller controller = controllerList[0];
		controller.poll();
		this.previousComponents = controller.getComponents();
		
		while(!this.BackgroundThreadRequestStop) {
			try {
				controller.poll();
				
				Component[] newComponents = controller.getComponents();
				for (int i = 0; i < newComponents.length; i++) {
					if (!newComponents[i].equals(previousComponents[i])) {
						this.Controls.RegisterChangedComponent(newComponents[i]);
						this.previousComponents[i] = newComponents[i];
					}
				}
				
				List<String> updateStrings = this.Controls.getUpdateStrings();
				if (this.NetworkManager.IsConnected()) {
					for (String updateString : updateStrings) {
						this.NetworkManager.writeData(updateString);
						Thread.sleep(1); //TODO: Is this needed?
					}
				}
				
				Thread.sleep(this.PollWaitMs);
			} catch (Exception e) {
				e.printStackTrace(); //TODO: Better error handling code
			}
			
		}
		this.BackgroundThreadRunning = false;
	}

}
