package clankTeleoperationFrontEnd;

import java.util.LinkedList;
import java.util.List;

import net.java.games.input.Component;

public class TeleoperatedControllerCommandMapping implements IControllerCommandMapping {

	private double RightStickLastX = -200; //To trigger movement on first activation
	private double RightStickLastY = -200;
	private double RightStickNewX = -200;
	private double RightStickNewY = -200;
	
	/*TODO: tweak values?*/
	private double MinAnalogStickMovement = 0.02;
	private double DeadZoneValue = 0.05;
	
	@Override
	public void RegisterChangedComponent(Component newValue) {
		if (newValue.getName() == "rx") {           //Right analog stick X value (on range [-1, 1])
			RightStickNewX = newValue.getPollData();
		}
		else if (newValue.getName() == "ry") {      //Right analog stick Y value (on range [-1, 1])
			RightStickNewY = newValue.getPollData();
		}
		return;
	}
	@Override
	public List<String> getUpdateStrings() {

		List<String> returnValue = new LinkedList<String>();
		/*Determine if we need to create a drive command*/
		if (Math.abs(RightStickNewX - RightStickLastX) >= MinAnalogStickMovement || Math.abs(RightStickNewY - RightStickLastY) >= MinAnalogStickMovement) {
			double r = Math.sqrt( (RightStickNewX*RightStickNewX) + (RightStickNewY * RightStickNewY) );
			double theta = Math.atan2(RightStickNewX, RightStickNewY);
			
			if (r > DeadZoneValue) {
				returnValue.add(String.format("Base:base.drive=(%f|%f)\n", r, theta)); //TODO: More elegant packet format. JSON perhaps?
			} else {
				returnValue.add("Base:base.drive=(0.0|0.0)\n");
			}
		}
		return returnValue;
	}

}
