# Arduino Uno SMD Setup Guide

This guide covers installing the Arduino IDE, connecting an Arduino Uno
SMD, installing the USB-to-serial driver, and selecting the correct
board and port in Windows. It does **not** include sketches or projects.

## 1. Download and install Arduino IDE

1.  Open the official Arduino software page:
    -   https://www.arduino.cc/en/software/
2.  Download **Arduino IDE 2** for Windows.
3.  Run the installer and complete the installation.
4.  Open Arduino IDE.

## 2. Install the Arduino Uno board package

1.  In Arduino IDE, open **Tools → Board → Boards Manager** (or open
    Boards Manager from the sidebar).
2.  Search for **Arduino AVR Boards**.
3.  Install **Arduino AVR Boards by Arduino**.
    -   In the setup shown in this conversation, version **1.8.8** was
        available.
4.  Close Boards Manager.

## 3. Connect the Arduino

1.  Use the USB A-to-B cable supplied with the board.
2.  Connect the square USB-B end to the Arduino Uno.
3.  Connect the USB-A end directly to a USB port on the computer.
4.  Check that the board's **ON** power LED lights up.

Use a known data-capable USB cable. Some USB cables provide power only
and cannot transfer data.

## 4. Identify the USB serial chip

In this setup, Windows detected the board as:

**FT232R USB UART**

That indicates an FTDI FT232R USB-to-serial interface. The driver
required for this device is the FTDI Virtual COM Port (VCP) driver.

If Windows shows a different USB serial chip, such as CH340 or CP210x,
use the driver for that exact chip instead. Do not install a different
manufacturer's driver just because it is listed here.

## 5. Download and install the FTDI driver

Official FTDI driver page:

-   https://ftdichip.com/drivers/vcp-drivers/

Direct Windows driver installer ZIP:

-   https://ftdichip.com/wp-content/uploads/2025/03/CDM2123620_Setup.zip

Alternative driver files ZIP:

-   https://ftdichip.com/wp-content/uploads/2025/03/CDM-v2.12.36.20-WHQL-Certified.zip

Installation steps:

1.  Download the direct installer ZIP.
2.  Extract the ZIP file.
3.  Open the extracted folder.
4.  Run `CDM2123620_Setup.exe`. If Windows asks for permission, approve
    the installer.
5.  Follow the installer prompts.
6.  Unplug the Arduino USB cable, wait a moment, and reconnect it.

If you prefer Windows Device Manager installation, right-click **FT232R
USB UART → Update driver → Browse my computer for drivers**, then select
the extracted driver folder and continue.

## 6. Confirm the COM port in Windows

1.  Open **Device Manager**.
2.  Expand **Ports (COM & LPT)**.
3.  Look for **USB Serial Port (COMx)**, where `x` is a number, such as
    COM3.
4.  If needed, unplug and reconnect the Arduino to identify which entry
    appears and disappears.

**Important:** Do not select `COM1` just because it appears in the list.
In this setup, COM1 was the computer's built-in communications port, not
the Arduino. Select the new COM port created by the FTDI driver.

If no port appears:

-   Try another USB port directly on the computer.
-   Try another known data-capable USB A-to-B cable.
-   Check Device Manager for `FT232R USB UART`, an unknown USB device,
    or a device with a warning symbol.
-   Confirm that the correct driver installed successfully.

## 7. Select the board and port in Arduino IDE

1.  Connect the Arduino.
2.  In Arduino IDE, choose **Tools → Board → Arduino AVR Boards →
    Arduino Uno**.
3.  Open **Tools → Port**.
4.  Select the Arduino's detected port, for example **COM3**. Use the
    COM number shown on your own computer; it may differ.
5.  The board selection should be **Arduino Uno** (the classic
    ATmega328P Uno), not **Arduino UNO R4**.

## 8. Setup checklist

-   [ ] Arduino IDE 2 installed.
-   [ ] Arduino AVR Boards package installed.
-   [ ] Arduino connected with a USB A-to-B data cable.
-   [ ] Board power LED is on.
-   [ ] FT232R driver installed, if the board uses the FT232R interface.
-   [ ] Arduino appears as **USB Serial Port (COMx)** in Device Manager.
-   [ ] **Arduino Uno** selected in Arduino IDE.
-   [ ] The matching Arduino COM port selected in Arduino IDE.

## Official download links

  ------------------------------------------------------------------------------------------------------------------------
  Item                                Link
  ----------------------------------- ------------------------------------------------------------------------------------
  Arduino IDE                         https://www.arduino.cc/en/software/

  FTDI VCP driver page                https://ftdichip.com/drivers/vcp-drivers/

  FTDI Windows installer ZIP          https://ftdichip.com/wp-content/uploads/2025/03/CDM2123620_Setup.zip

  FTDI driver files ZIP               https://ftdichip.com/wp-content/uploads/2025/03/CDM-v2.12.36.20-WHQL-Certified.zip
  ------------------------------------------------------------------------------------------------------------------------

------------------------------------------------------------------------

**Note:** The FTDI driver links are specifically for boards that use the
FT232R USB interface, as identified in this setup. The Arduino Uno SMD
refers to the surface-mount ATmega328P version; it is configured in the
IDE as **Arduino Uno**.
