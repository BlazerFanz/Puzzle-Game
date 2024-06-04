# egnr103-final

In this game you want to get through all the levels before running out of time. for each level you have 60 seconds and you much turn on all the lights by figuring out what buttons or what sensors to activate. 

User inputs will be buttons, switch, light, temperature and moisture (also maybe accelerometer)
Buttons will be interrupts which will set a flag if it’s pressed or held depending on what's needed. The interrupt would check to see if its raw value is FALLING
Switch will also be an interrupt to see if it's been flicked or not. This means checking LOW vs. HIGH
Light will be set to be at least a certain amount so having an if statement which detects that.
Temperature and moisture will be the same thing, making sure it's a certain amount which will then allow progression so just doing this through an if statement.
If I do an accelerometer, it would be detecting motion and I could pair this with boolean logic when if its moving and if x button is LOW while direction is changing, then y would happen
The speaker will be used with a map function to make it so the lower the time the higher the frequency 
Y = map(x, 60, 1, 200, 500)
All of these would change how many lights are on, the more sensors complete then more lights will turn on
