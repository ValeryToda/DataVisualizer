# DataVisualizer
Handy tool to visualize, perform data sanity check and export relevant data for the Udacity Self-Driving Simulator

![alt text](https://github.com/ValeryToda/DataVisualizer/blob/master/DataVisualizer.png "User Interface")

### Usage:

* Use the Load button to load a CSV File (format: Center image path, left image path, right image path, steering angle, ...)
* Use the keyboard arrow Up and Down keys to navigate through the loaded images 
* Use the keyboard key Del to delete the images associated (left image, center image, right image) with the selected Image-ID
* Use the Export button to select a folder to save the filepaths and corresponding steering angles associated with the Image-IDs as CSV file (format: left image path, center image path, right image path, steering angle).

### File Description:

* DataVisualizer.app: MacOS executable file (Qt 5.9; macOS Sierra)

Feel free to download, compile and play around with it. Enjoy!

### How to build the project

* Download and install the latest Qt version from [here](https://info.qt.io/download-qt-for-application-development)
* Download the project files
* Open the project with the application Qt Creator and build it.

### Basic Qt Setup to successfully compile the project

![alt text](https://github.com/ValeryToda/DataVisualizer/blob/master/QtInstallationSetup.png, "Qt installation setup")
