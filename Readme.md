# Project Overview

I will be working on the blue gecko which will be working in two modes:
1.	Beacon Mode: This mode will activate the blue gecko in beacon mode and will continuously advertise to connect to a device.
2.	Security Mode: This mode will activate the proximity sensor which will work in security mode to detect intrusion.

Beacon Mode: This mode will be started when the authentication will be completed when the fingerprint is approved for the first time, and the system is started. After the authentication first time, server will start beaconing mode of the blue gecko. In the Beacon mode it will be transiting continuously to connect to any device. Once connected to a client, the app will open the map stored for the floor on the client’s application. This can be the first step for the indoor navigation. In this mode, the proximity sensor won’t be active.

Security Mode: This mode will be started when the authentication will be completed for the second time, this means the system will be closed.  In security mode, the blue gecko will be sleeping, the motion sensor will start and if any motion is detected then a LED will glow indicating intruder’s presence.

Link for the Individual Report: https://drive.google.com/drive/u/1/folders/1W3Kh64A_Q4Nl3I2ewa-m0OtcLxVkeYUI
