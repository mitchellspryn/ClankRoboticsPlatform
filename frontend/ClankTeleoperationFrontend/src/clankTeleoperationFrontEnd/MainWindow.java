package clankTeleoperationFrontEnd;

import org.eclipse.swt.SWT;
import org.eclipse.swt.browser.Browser;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.graphics.Device;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;

public class MainWindow {

	protected Shell shell;
	
	private static String IpAddress = "192.168.1.120";
	private static int PortNumber = 12345;
	
	private NetworkManager RobotConnectionManager;
	private ControllerPoller CPoller;
	private TeleoperatedControllerCommandMapping ControllerStrategy;
	
	/*Window object variables*/
	public Label connectionStatusLabel;
	public Button connectionButton;
	public Browser browser;
	
	/**
	 * Launch the application.
	 * @param args
	 */
	public static void main(String[] args) {
		try {
			MainWindow window = new MainWindow();
			window.createContents();
			
			window.Initialize();
			
			window.open();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	private void Initialize() {
		/*Attempt to connect to the robot using the default settings*/
		this.RobotConnectionManager = new NetworkManager();
		if (!RobotConnectionManager.openConnection(IpAddress, PortNumber, 5)) {
			this.connectionStatusLabel.setText("Connection status: Disconnected.");
			Device device = Display.getCurrent();
			this.connectionStatusLabel.setForeground(device.getSystemColor(SWT.COLOR_RED));
		} else {
			this.connectionStatusLabel.setText("Connection status: Connected!");
			Device device = Display.getCurrent();
			this.connectionStatusLabel.setForeground(device.getSystemColor(SWT.COLOR_GREEN));
		}
		
		/*Set up and start the controller poller*/
		this.ControllerStrategy = new TeleoperatedControllerCommandMapping();
		this.CPoller = new ControllerPoller(this.ControllerStrategy, this.RobotConnectionManager);
		this.CPoller.Start();
		
	}
	
	/*SWT WindowBuilder functions below here*/
	/**
	 * Open the window.
	 */
	public void open() {
		Display display = Display.getDefault();
		shell.open();
		shell.layout();
		while (!shell.isDisposed()) {
			if (!display.readAndDispatch()) {
				display.sleep();
			}
		}
	}

	/**
	 * Create contents of the window.
	 */
	protected void createContents() {
		shell = new Shell();
		shell.setSize(1000, 800);
		shell.setText("SWT Application");
		shell.setLayout(null);
		
		browser = new Browser(shell, SWT.NONE);
		browser.setBounds(10, 10, 960, 666);
		
		connectionButton = new Button(shell, SWT.NONE);
		connectionButton.addSelectionListener(new SelectionAdapter() {
			@Override
			public void widgetSelected(SelectionEvent e) {
				System.out.println("Not implemented yet.");
			}
		});
		connectionButton.setBounds(10, 682, 158, 29);
		connectionButton.setText("Edit Connection Info");
		
		connectionStatusLabel = new Label(shell, SWT.NONE);
		connectionStatusLabel.setBounds(10, 717, 229, 17);
		connectionStatusLabel.setText("Status: Unset");

	}
}
