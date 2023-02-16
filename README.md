# Arduino Spaceship
IMPORT: Using GROVE kit sensor kit, so no digital or analogue port specifications.

Interactive spaceship using accelerometer and OLED screen 


<img width="131" alt="Screen Shot 2023-02-16 at 3 00 30 PM" src="https://user-images.githubusercontent.com/78057184/219473935-fac16e23-721b-4597-b6bc-9c6e533bccb4.png">

By moving the accelerometer, it ahould move the spaceship forward, backwards, to the left, or right depending on the direction of your tilt. 
In fact, we can do that because this device measures the acceleration in directions x, y and z. The acceleration on a flat surface would then be 0 for y and x,
but 9.81m/s^2 in the z direction because of gravity, so depending on how you tilt the screen, the gravitational force will impact each direction specificaly.
