# Wireless Museum People Counter

This project is an implementation of a people counter system for museums using wireless sensor networks (WSN). It utilizes Arduino nodes equipped with RFID readers to detect the presence of visitors in different areas of the museum. The data from these nodes is transmitted wirelessly to a master node, which then sends the information to a PC running a PyQt GUI for real-time monitoring.

## Features
**RFID Detection**: Each Arduino node is equipped with an RFID reader to detect the presence of visitors carrying RFID tags.
**Wireless Communication**: Utilizes Aloha wireless communication protocol to transmit data from sensor nodes to the master node.
**Real-time Monitoring**: The PyQt GUI running on a PC provides real-time monitoring of visitor count in different areas of the museum.
**Scalability**: The system can be easily scaled to cover larger museum areas by adding more sensor nodes as needed.
## Hardware Requirements
### Slave Nodes
* Arduino Uno
* RC522 RFID readers for each sensor node
* RFM22 wireless communication modules
### Master Node
* Arduino Uno
* RFM22 wireless communication module
## Software Requirements
* Arduino IDE for programming Arduino boards
* Python for running the PyQt GUI on the PC
* PyQt library for building the GUI
* [RF22](https://github.com/matthijskooijman/RF22/tree/max) and [MFRC522](https://github.com/miguelbalboa/rfid) libraries for Arduino
