# GSM Based Temperature Monitoring System-


The objective of the project is to design a system that monitors the temperature of a room and sends an
SMS in one of two cases:<\br>
● The temperature exceeds a level (Temperature Threshold).<\br>
● The temperature rise rate exceeds a rate specified by the user (Temperature Rate Threshold).<\br>
The system allows the user to specify both the Temperature Threshold and Temperature Rate Threshold.<\br>
The resolution with which Temperature Threshold can be specified is at most 2℃ or less. The resolution
with which the Temperature Threshold Rate is set to be 5℃/min or less. The range in which Temperature
Rate threshold exists is about [35℃ - 90℃] or more.<\br>
The main Function of the project is built on how to combine individual components that have a specified
job together with the atmega32 to get the achieve its overall goal.<\br>
The combined system should satisfy these requirements:<\br>
1- Allow the user to specify the limits.<\br>
2- Monitor the temperature continuously.<\br>
3- Show current temperature.<\br>
4- Send an SMS to the user when the temperature is out of range or if the rate exceeds limits.<\br>
