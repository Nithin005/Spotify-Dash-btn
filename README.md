# Spotify Dash Button

A ESP8266/ESP32 Dash Button (Similar to Amazon Dash) for Spotify Music Controls.

### Features

- [x] Add the Currently playing song to the Liked Songs
- [x] uses HTTPS Requests

### Features Yet to be Implemented

- [ ] Deep Sleep Function / Soft latch circuit (to automatically power off the microcontroller after finishing running the code)
- [ ] More Spotify Controls
- [ ] 3D Printable Enclosure

# Instructions

The Code is in [src/main.ino](https://github.com/Nithin005/Spotify-Dash-btn/blob/master/src/main.ino) . Add your Spotify Credentials and Wifi Access point Info in it And Upload the Code Using Arduino IDE or Platform IO.

Each time the ESP Turns ON, it adds the Currently playing song to the Liked Songs Playlist.



