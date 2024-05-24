import random
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5 import uic
import threading
import time

MAX_PEOPLE = 20

def parse_line(line):
    data = line.split(", ")
    room = data[0].split(" = ")[1]
    tag_id = data[1].split(" = ")[1].rstrip("\n")
    return room, tag_id

class MyGUI(QMainWindow):
    update_labels_signal = pyqtSignal(int, list)

    def colorFinder(self, room):
        div = self.room_counters[room] / MAX_PEOPLE

        if div < 0.25:
            return "152, 255, 153"
        elif div < 0.5:
            return "255, 255, 153"
        elif div < 0.75:
            return "255, 215, 102"
        else:
            return "255, 128, 128"
        
    def __init__(self):
        super(MyGUI, self).__init__()
        uic.loadUi('gui.ui', self)

        self.setFixedSize(873, 512)

        self.room_counters = [0, 0, 0, 0, 0, 0, 0, 0]
        self.tags = {}
        self.total_sum = 0

        self.update_labels_signal.connect(self.update_labels)
        self.lineEdit = self.findChild(QLineEdit, 'lineEdit')
        self.lineEdit.returnPressed.connect(self.on_return_pressed)

        self.show()

        threading.Thread(target=self.read_serial, daemon=True).start()

    def on_return_pressed(self):
        tag_id = self.lineEdit.text()
        if tag_id in self.tags:
            room = int(self.tags[tag_id])
            if room == 0:
                self.label_12.setText("Entrance")
            elif room == 7:
                self.label_12.setText("Exit")
            else:
                self.label_12.setText(f"Room {room}")
        else:
            self.label_12.setText("Tag not found")
        self.lineEdit.clear()

    def read_serial(self):
        with open("data/data.txt", "r") as file:
            for line in file:
                if line.startswith("room"):
                    rand_delay = random.randint(0, 2000)
                    room, tag_id = parse_line(line)
                    self.update_people(room)
                    self.update_tags(int(room), tag_id)
                    time.sleep(rand_delay / 1000)

    def update_people(self, room):
        self.update_counters(int(room))
        self.update_labels_signal.emit(self.total_sum, self.room_counters[:])

    def update_tags(self, room, tag_id):
        if room == 8:
            if tag_id in self.tags:
                del self.tags[tag_id]
            return
        if tag_id in self.tags:
            self.tags[tag_id] = room
        else:
            self.tags[tag_id] = room

    def update_counters(self, room):
        if room == 8:
            self.room_counters[7] -= 1
            self.total_sum -= 1
            return
        
        self.room_counters[room] += 1

        if room == 0:
            self.total_sum += 1
        else:
            self.room_counters[room-1] -= 1

    def update_labels(self):
        self.label_total.setText(str(self.total_sum))
        for i in range(8):
            self.tableWidget.setItem(0, i, QTableWidgetItem(str(self.room_counters[i])))
            getattr(self, f"count_{i}").setText(str(self.room_counters[i]))
            getattr(self, f"frame{i}").setStyleSheet(f"background-color: rgb({self.colorFinder(i)})")
    

def main():
    app = QApplication([])
    window = MyGUI()
    app.exec_()

if __name__ == '__main__':
    main()