package clankTeleoperationFrontEnd;

import java.util.List;

import net.java.games.input.Component;

public interface IControllerCommandMapping {
	public void RegisterChangedComponent(Component newValue);
	public List<String> getUpdateStrings();
}
