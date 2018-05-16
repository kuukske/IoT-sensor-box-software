import sys, csv, re
from PyQt5.QtWidgets import QWidget, QPushButton, QLineEdit, QLabel, \
    QApplication, QComboBox, QFileDialog, QMessageBox

class Example(QWidget):

    def __init__(self):
        super().__init__()

        self.initUI()

    def showDialog(self, filename):
        dialog = QMessageBox(self)
        dialog.setIcon(QMessageBox.Information)
        dialog.setText("The configuration file is saved at " + filename)
        dialog.setWindowTitle("Configuration file created!")
        dialog.setStandardButtons(QMessageBox.Ok)
        dialog.exec_()

    def showNotFilledDialog(self):
        nfDialog = QMessageBox(self)
        nfDialog.setIcon(QMessageBox.Warning)
        nfDialog.setText("Not all the boxes have been filled!")
        nfDialog.setInformativeText('Please fill out all the boxes and '
                                    'generate the configuration file again.')
        nfDialog.setStandardButtons(QMessageBox.Ok)
        nfDialog.exec_()

    def showNotConfigFile(self):
        ncDialog = QMessageBox(self)
        ncDialog.setIcon(QMessageBox.Warning)
        ncDialog.setText("The file selected is not correct!")
        ncDialog.setInformativeText('The file that is selected is not a '
                                    'config.csv file. Please select a valid '
                                    'file.')
        ncDialog.setStandardButtons(QMessageBox.Ok)
        ncDialog.exec_()

    def initUI(self):

        self.setGeometry(300, 300, 550, 350)
        self.setWindowTitle('Atos configuration file generator')

        self.label_welcome = QLabel('This Program can be used to '
                                    'generate a configuration file for '
                                    'the translation module \nof the '
                                    'mobile sensor box.', self)
        self.label_welcome.move(10,10)

        self.label_sensorType = QLabel("Sensor Type:", self)
        self.label_sensorType.move(10, 80)
        self.lineEdit_sensorType = QLineEdit(self)
        self.lineEdit_sensorType.move(160, 80)
        self.lineEdit_sensorType.setToolTip('Enter the sensor type. e.g. '
                                            'Temperature, Humidity, shock.')
        self.lineEdit_sensorType.setText("")

        self.label_sensorName = QLabel("Sensor Name:", self)
        self.label_sensorName.move(10, 105)
        self.lineEdit_sensorName = QLineEdit(self)
        self.lineEdit_sensorName.move(160, 105)
        self.lineEdit_sensorName.setToolTip('Enter the sensor name. e.g. '
                                            'DHT11, NTC')
        self.lineEdit_sensorName.setText("")

        self.label_moduleID = QLabel('Translation module ID:', self)
        self.label_moduleID.move(10, 130)
        self.dropdown_moduleID = QComboBox(self)
        self.dropdown_moduleID.addItems([' ', '1', '2', '3', '4', '5', '6',
                                         '7'])
        self.dropdown_moduleID.move(160, 130)
        self.dropdown_moduleID.setToolTip('Select the translation module ID. '
                                          'This has to be a number between 1 '
                                          'and 7.')

        self.label_commType = QLabel('Communication type:', self)
        self.label_commType.move(10, 155)
        self.dropdown_commType = QComboBox(self)
        self.dropdown_commType.addItems([' ', 'SPI', 'I2C', 'Digital',
                                        'Analogue', 'ONE-WIRE', 'DHT'])
        self.dropdown_commType.move(160, 155)
        self.dropdown_commType.setToolTip('Select the communication protocol '
                                          'that the translation module has '
                                          'to use to communicate with the '
                                          'sensor.')

        self.label_range = QLabel("Measurement range: ", self)
        self.textbox_rangeLow = QLineEdit(self)
        self.textbox_rangeHigh = QLineEdit(self)
        self.label_range.move(10, 180)
        self.textbox_rangeLow.move(160, 180)
        self.textbox_rangeHigh.move(300, 180)
        self.textbox_rangeLow.setToolTip('Enter the lowest value of the '
                                         'measurement range.')
        self.textbox_rangeHigh.setToolTip('Enter the highest value of the '
                                          'measurement range.')
        self.textbox_rangeLow.setText("")
        self.textbox_rangeHigh.setText("")

        self.label_measurementUnit = QLabel('Measurement Unit:', self)
        self.label_measurementUnit.move(10, 205)
        self.textbox_measurementUnit = QLineEdit(self)
        self.textbox_measurementUnit.move(160, 205)
        self.textbox_measurementUnit.setToolTip('Enter the unit of '
                                                'measurement of this sensor. '
                                                'e.g. Celsius, Fahrenheit, '
                                                'Meter')
        self.textbox_measurementUnit.setText("")

        self.generateButton = QPushButton('Generate Configuration file', self)
        self.generateButton.move(10, 280)
        self.generateButton.clicked.connect(self.generateFile)

        self.loadButton = QPushButton('Load Configuration file', self)
        self.loadButton.move(210, 280)
        self.loadButton.clicked.connect(self.load)

        self.closeButton = QPushButton('Close', self)
        self.closeButton.move(400, 280)
        self.closeButton.clicked.connect(self.closeScreen)





        self.raise_()
        self.show()

    def generateFile(self):
        if (self.lineEdit_sensorType.text == "") | (
                self.lineEdit_sensorName.text == "") | (
                self.textbox_rangeLow == "") | (self.textbox_rangeHigh == \
                "")| (self.textbox_measurementUnit == "") | (\
                self.dropdown_commType.currentText() == ' ') | \
                (self.dropdown_moduleID.currentText() == ' '):
            self.showNotFilledDialog()
            return

        else:
            path = str(QFileDialog.getExistingDirectory(self, "Select "
                                                              "\Directory"))
            if path:
                filename = path + '/config.csv'

                with open(filename, 'w') as csvfile:
                    writer = csv.writer(csvfile, quotechar='|',
                                        quoting=csv.QUOTE_MINIMAL)

                    writer.writerow([self.lineEdit_sensorType.text()])
                    writer.writerow([self.lineEdit_sensorName.text()])
                    writer.writerow([str(self.dropdown_moduleID.currentText())])
                    writer.writerow([str(self.dropdown_commType.currentText())])
                    writer.writerow([self.textbox_rangeLow.text()])
                    writer.writerow([self.textbox_rangeHigh.text()])
                    writer.writerow([self.textbox_measurementUnit.text()])

                    self.showDialog(filename)

    def closeScreen(self):
        sys.exit()

    def load(self):
        path = str(QFileDialog.getOpenFileName(self, 'OpenFile'))
        if 'config.csv' in path:
            path = path[:-19]
            path = path[2:]
            print(path)
            with open(path) as csvfile:
                reader = csv.reader(csvfile)
                data = []

                for row in reader:
                    data.append(row)

            self.lineEdit_sensorType.setText(data[0][0])
            self.lineEdit_sensorName.setText(data[1][0])
            self.dropdown_moduleID.setCurrentText(str(data[2][0]))
            self.dropdown_commType.setCurrentText(str(data[3][0]))
            self.textbox_rangeLow.setText(data[4][0])
            self.textbox_rangeHigh.setText(data[5][0])
            self.textbox_measurementUnit.setText(data[6][0])



        else:
            self.showNotConfigFile()
            return

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = Example()
    sys.exit(app.exec_())