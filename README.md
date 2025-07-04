<div id="top">

<div align="center">

<img src="https://raw.githubusercontent.com/KardasLand/AvaSYS-GCS-Revised/refs/heads/master/ui/assets/neostellar-logo.png" width="50%" style="position: relative; top: 0; right: 0;" alt="Neo Stellar"/>

# <code>AvaSYS Ground Control System</code>

<em>Elevate drone control. Revolutionize aerial operations. Soar higher.</em>

<em>Built with the tools and technologies:</em>

<img src="https://img.shields.io/badge/CMake-064F8C.svg?style=default&logo=CMake&logoColor=white" alt="CMake">
<img src="https://img.shields.io/badge/GStreamer-8B0000.svg?style=default&logo=GStreamer&logoColor=white" alt="GStreamer">
<img src="https://img.shields.io/badge/QtQuick-00BFFF.svg?style=default&logo=Qt&logoColor=white" alt="QtQuick">
<img src="https://img.shields.io/badge/Qt6-00BFFF.svg?style=default&logo=Qt&logoColor=white" alt="Qt6">
<img src="https://img.shields.io/badge/C%2B%2B-00599C.svg?style=default&logo=c%2B%2B&logoColor=white" alt="C++">
<img src="https://img.shields.io/badge/OpenCV-5C3EE8.svg?style=default&logo=OpenCV&logoColor=white" alt="OpenCV">
<img src="https://img.shields.io/badge/OpenSSL-721412.svg?style=default&logo=OpenSSL&logoColor=white" alt="OpenSSL">

</div>
<br>

---

## Table of Contents

- [Table of Contents](#table-of-contents)
- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
    - [Project Index](#project-index)
- [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [Installation](#installation)
    - [Usage](#usage)
- [Roadmap](#roadmap)
- [Contributing](#contributing)
- [License](#license)
- [Acknowledgments](#acknowledgments)

---

## Overview

AvaSYS2 is a comprehensive ground control system for managing and monitoring drones in real-time. It combines robust communication protocols with an intuitive user interface, streamlining drone operations.

Continuation of the [AvaSYS](https://github.com/NeoStellar/AvaSys-GCS) project, this version introduces significant improvements and new features, including:

**Why AvaSYS2?**

This project simplifies complex drone management tasks while providing powerful features for both developers and operators. The core features include:

- **🚁 MAVLink Integration:** Seamless communication with drones using industry-standard protocols.
- **🗺️ Real-time Tracking:** Monitor vehicle telemetry and position data on an interactive map.
- **🎥 Video Streaming:** Integrate live video feeds from drones using GStreamer and OpenCV.
- **🌐 Teknofest Integration:** Connect and transmit data to the Teknofest server effortlessly.

---

## Features

|      | Component       | Details                                                                                                                         |
| :--- | :-------------- |:--------------------------------------------------------------------------------------------------------------------------------|
| ⚙️  | **Architecture**  | <ul><li>QML-based UI architecture</li><li>C++ backend integration</li><li>Modular design with separate QML components</li></ul> |
| 🔌 | **Integrations**  | <ul><li>OpenCV integration</li><li>OpenSSL support</li><li>MAVLink protocol integration</li><li>Qt6 framework</li></ul>         |
| 📦 | **Dependencies**  | <ul><li>Qt6 & Qt Quick</li><li>OpenCV</li><li>OpenSSL</li><li>MAVLink</li><li>PkgConfig</li></ul>                               |

---

## Project Structure

```sh
└── /
    ├── CMakeLists.txt
    ├── src
    │   ├── core
    │   ├── main.cpp
    │   ├── managers
    │   └── network
    └── ui
        ├── Main.qml
        ├── assets
        ├── components
        └── panels
```

### Project Index

<details open>
	<summary><b><code>/</code></b></summary>
	<!-- __root__ Submodule -->
	<details>
		<summary><b>__root__</b></summary>
		<blockquote>
			<div class='directory-path' style='padding: 8px 0; color: #666;'>
				<code><b>⦿ __root__</b></code>
			<table style='width: 100%; border-collapse: collapse;'>
			<thead>
				<tr style='background-color: #f8f9fa;'>
					<th style='width: 30%; text-align: left; padding: 8px;'>File Name</th>
					<th style='text-align: left; padding: 8px;'>Summary</th>
				</tr>
			</thead>
				<tr style='border-bottom: 1px solid #eee;'>
					<td style='padding: 8px;'><b><a href='/CMakeLists.txt'>CMakeLists.txt</a></b></td>
					<td style='padding: 8px;'>- Configures the AvaSYS2 project build environment using CMake<br>- Defines project structure, dependencies, and compilation settings<br>- Specifies required libraries including Qt6, GStreamer, MAVLink, OpenSSL, and OpenCV<br>- Sets up the main executable, QML modules, and resources<br>- Establishes linking and include directories for external libraries<br>- Configures installation parameters for the final application bundle.</td>
				</tr>
			</table>
		</blockquote>
	</details>
	<!-- ui Submodule -->
	<details>
		<summary><b>ui</b></summary>
		<blockquote>
			<div class='directory-path' style='padding: 8px 0; color: #666;'>
				<code><b>⦿ ui</b></code>
			<table style='width: 100%; border-collapse: collapse;'>
			<thead>
				<tr style='background-color: #f8f9fa;'>
					<th style='width: 30%; text-align: left; padding: 8px;'>File Name</th>
					<th style='text-align: left; padding: 8px;'>Summary</th>
				</tr>
			</thead>
				<tr style='border-bottom: 1px solid #eee;'>
					<td style='padding: 8px;'><b><a href='/ui/Main.qml'>Main.qml</a></b></td>
					<td style='padding: 8px;'>- Defines the main application window for the AvaSYS Ground Control System 2.0<br>- Sets up the overall layout, including a header with the application title, a central MainPanel component, and a BottomPanel component<br>- Establishes the basic structure and appearance of the user interface, serving as the entry point for the applications visual elements and user interactions.</td>
				</tr>
			</table>
			<!-- panels Submodule -->
			<details>
				<summary><b>panels</b></summary>
				<blockquote>
					<div class='directory-path' style='padding: 8px 0; color: #666;'>
						<code><b>⦿ ui.panels</b></code>
					<table style='width: 100%; border-collapse: collapse;'>
					<thead>
						<tr style='background-color: #f8f9fa;'>
							<th style='width: 30%; text-align: left; padding: 8px;'>File Name</th>
							<th style='text-align: left; padding: 8px;'>Summary</th>
						</tr>
					</thead>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='/ui/panels/BottomPanel.qml'>BottomPanel.qml</a></b></td>
							<td style='padding: 8px;'>- BottomPanel.qml defines the layout and functionality of the bottom information panel in the user interface<br>- It displays crucial vehicle data such as latitude, longitude, altitude, speed, roll, heading, voltage, current, and battery status<br>- The panel uses both real-time vehicle data and mock data when no vehicle is connected, ensuring continuous display of information<br>- Gauges and text elements present the data in an organized and visually appealing manner.</td>
						</tr>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='/ui/panels/MainPanel.qml'>MainPanel.qml</a></b></td>
							<td style='padding: 8px;'>- MainPanel.qml defines the primary user interface for a vehicle tracking application<br>- Implements a map view displaying vehicle locations, with interactive markers for selection<br>- Utilizes QtQuick and QtLocation modules to render the map and vehicle positions<br>- Includes a simple list view for debugging purposes<br>- Configures map plugin settings for OpenStreetMap with custom tile provider integration.</td>
						</tr>
					</table>
				</blockquote>
			</details>
			<!-- components Submodule -->
			<details>
				<summary><b>components</b></summary>
				<blockquote>
					<div class='directory-path' style='padding: 8px 0; color: #666;'>
						<code><b>⦿ ui.components</b></code>
					<table style='width: 100%; border-collapse: collapse;'>
					<thead>
						<tr style='background-color: #f8f9fa;'>
							<th style='width: 30%; text-align: left; padding: 8px;'>File Name</th>
							<th style='text-align: left; padding: 8px;'>Summary</th>
						</tr>
					</thead>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='/ui/components/CompassGauge.qml'>CompassGauge.qml</a></b></td>
							<td style='padding: 8px;'>- CompassGauge component creates a circular compass display for heading information<br>- It renders a gauge with cardinal directions, a rotating arrow pointer, and a central value display<br>- The component accepts a heading value, title, and unit as properties<br>- It utilizes QtQuick Canvas for drawing the gauge elements and provides smooth animations for value changes, enhancing the user interface with a visually appealing and functional compass representation.</td>
						</tr>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='/ui/components/StyledGauge.qml'>StyledGauge.qml</a></b></td>
							<td style='padding: 8px;'>- StyledGauge component creates a customizable circular gauge for displaying numerical values<br>- It features a dynamic needle, color-coded arcs, and labeled scale<br>- The gauge adapts to different value ranges, updates smoothly, and includes a central display for the current value and unit<br>- This reusable UI element enhances data visualization across the application, providing an intuitive and visually appealing way to represent metrics.</td>
						</tr>
					</table>
				</blockquote>
			</details>
		</blockquote>
	</details>
	<!-- src Submodule -->
	<details>
		<summary><b>src</b></summary>
		<blockquote>
			<div class='directory-path' style='padding: 8px 0; color: #666;'>
				<code><b>⦿ src</b></code>
			<table style='width: 100%; border-collapse: collapse;'>
			<thead>
				<tr style='background-color: #f8f9fa;'>
					<th style='width: 30%; text-align: left; padding: 8px;'>File Name</th>
					<th style='text-align: left; padding: 8px;'>Summary</th>
				</tr>
			</thead>
				<tr style='border-bottom: 1px solid #eee;'>
					<td style='padding: 8px;'><b><a href='/src/main.cpp'>main.cpp</a></b></td>
					<td style='padding: 8px;'>- Initializes the main application, sets up the QML engine, and registers essential components for the AvaSYS2 project<br>- Establishes connections between C++ and QML, configures environment variables, and initializes core services like Mavlink and Teknofest server<br>- Serves as the entry point, orchestrating the startup sequence and launching the graphical user interface for the drone control system.</td>
				</tr>
			</table>
			<!-- managers Submodule -->
			<details>
				<summary><b>managers</b></summary>
				<blockquote>
					<div class='directory-path' style='padding: 8px 0; color: #666;'>
						<code><b>⦿ src.managers</b></code>
					<table style='width: 100%; border-collapse: collapse;'>
					<thead>
						<tr style='background-color: #f8f9fa;'>
							<th style='width: 30%; text-align: left; padding: 8px;'>File Name</th>
							<th style='text-align: left; padding: 8px;'>Summary</th>
						</tr>
					</thead>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='/src/managers/TeknofestClient.h'>TeknofestClient.h</a></b></td>
							<td style='padding: 8px;'>- TeknofestClient manages communication with the Teknofest server, handling authentication, telemetry data fetching, and transmission<br>- It encapsulates server properties, login functionality, and telemetry operations<br>- The class provides methods for starting and stopping telemetry updates, transmitting vehicle data, and emits signals for login status and received telemetry<br>- It utilizes QNetworkAccessManager for network operations and includes structures for storing authentication and QR code properties.</td>
						</tr>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='/src/managers/VehicleManager.cpp'>VehicleManager.cpp</a></b></td>
							<td style='padding: 8px;'>- VehicleManager handles the creation, management, and updating of vehicle objects within the application<br>- It maintains a list of vehicles from different data sources (MAVLink and Teknofest API), provides methods for selecting vehicles, and facilitates the transmission of telemetry data<br>- The class also manages the lifecycle of vehicle objects, including creation, updating, and removal, while exposing the vehicle list to QML for display and interaction.</td>
						</tr>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='/src/managers/MavlinkManager.h'>MavlinkManager.h</a></b></td>
							<td style='padding: 8px;'>- MavlinkManager handles communication with drones using the MAVLink protocol over UDP<br>- It provides methods for connecting, disconnecting, and sending commands to drones, such as arming, takeoff, and return-to-launch<br>- The class manages multiple vehicle connections, tracks their network endpoints, and emits signals with updated vehicle information<br>- This component is crucial for integrating drone control and telemetry into the larger application architecture.</td>
						</tr>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='/src/managers/VehicleManager.h'>VehicleManager.h</a></b></td>
							<td style='padding: 8px;'>- VehicleManager handles the creation, management, and updating of vehicle objects within the application<br>- It maintains a collection of vehicles, provides access to them for QML, and manages the selection of a main vehicle<br>- The class interfaces with both MAVLink and Teknofest data sources, updating vehicle information and facilitating telemetry transmission<br>- It serves as a central hub for vehicle-related operations and data management in the project.</td>
						</tr>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='/src/managers/TeknofestClient.cpp'>TeknofestClient.cpp</a></b></td>
							<td style='padding: 8px;'>- TeknofestClient manages communication with the Teknofest server, handling login and telemetry operations<br>- It utilizes an HttpClient for network requests and processes server responses<br>- The class facilitates user authentication, sends vehicle telemetry data, and receives updated telemetry information<br>- It emits signals for login status and received telemetry, allowing other components to react to server interactions and data updates.</td>
						</tr>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='/src/managers/MavlinkManager.cpp'>MavlinkManager.cpp</a></b></td>
							<td style='padding: 8px;'>- MavlinkManager handles MAVLink protocol communication for the drone control system<br>- It establishes UDP connections, processes incoming messages, and sends commands to vehicles<br>- The class manages vehicle state updates, including position, attitude, and battery information<br>- It provides methods for arming, disarming, takeoff, and return-to-launch operations, serving as a crucial interface between the ground control station and connected drones.</td>
						</tr>
					</table>
				</blockquote>
			</details>
			<!-- network Submodule -->
			<details>
				<summary><b>network</b></summary>
				<blockquote>
					<div class='directory-path' style='padding: 8px 0; color: #666;'>
						<code><b>⦿ src.network</b></code>
					<table style='width: 100%; border-collapse: collapse;'>
					<thead>
						<tr style='background-color: #f8f9fa;'>
							<th style='width: 30%; text-align: left; padding: 8px;'>File Name</th>
							<th style='text-align: left; padding: 8px;'>Summary</th>
						</tr>
					</thead>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='/src/network/HttpClient.h'>HttpClient.h</a></b></td>
							<td style='padding: 8px;'>- HttpClient.h defines a network communication interface for the application<br>- It handles login and telemetry data transmission through HTTP requests<br>- The class provides methods for sending login credentials and telemetry information, and emits signals upon completion of these operations<br>- This component facilitates secure data exchange between the client application and server, enabling user authentication and real-time data reporting functionality.</td>
						</tr>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='/src/network/HttpClient.cpp'>HttpClient.cpp</a></b></td>
							<td style='padding: 8px;'>- HttpClient.cpp implements a network client for handling HTTP requests in the project<br>- It manages login authentication, session management, and telemetry data transmission<br>- The class provides methods for sending login requests, processing responses, and transmitting telemetry information<br>- It utilizes Qts networking capabilities to handle connections, cookies, and JSON data formatting<br>- The client emits signals to notify other components of request outcomes, facilitating asynchronous communication within the application.</td>
						</tr>
					</table>
				</blockquote>
			</details>
			<!-- core Submodule -->
			<details>
				<summary><b>core</b></summary>
				<blockquote>
					<div class='directory-path' style='padding: 8px 0; color: #666;'>
						<code><b>⦿ src.core</b></code>
					<table style='width: 100%; border-collapse: collapse;'>
					<thead>
						<tr style='background-color: #f8f9fa;'>
							<th style='width: 30%; text-align: left; padding: 8px;'>File Name</th>
							<th style='text-align: left; padding: 8px;'>Summary</th>
						</tr>
					</thead>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='/src/core/VideoItem.h'>VideoItem.h</a></b></td>
							<td style='padding: 8px;'>- VideoItem.h defines a crucial component for video processing and display within the application<br>- It integrates GStreamer and OpenCV functionalities, enabling video streaming and frame manipulation<br>- The class inherits from QQuickPaintedItem, allowing seamless integration with Qts UI framework<br>- Key features include starting the GStreamer pipeline, updating frames, and painting video content, forming a bridge between backend video processing and frontend display capabilities.</td>
						</tr>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='/src/core/AppContext.cpp'>AppContext.cpp</a></b></td>
							<td style='padding: 8px;'>- AppContext serves as the central hub for managing core components of the application<br>- It implements the Singleton pattern, providing global access to VehicleManager, MavlinkManager, and TeknofestClient instances<br>- Initializes Mavlink connections and Teknofest server properties, and establishes communication channels between components through signal-slot connections<br>- Facilitates the coordination of vehicle updates, telemetry transmission, and login processes across the application.</td>
						</tr>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='/src/core/Vehicle.h'>Vehicle.h</a></b></td>
							<td style='padding: 8px;'>- Defines the Vehicle class, representing a central entity in the projects architecture<br>- Encapsulates essential vehicle attributes and behaviors, including position, flight data, battery status, and operational modes<br>- Facilitates communication between the core system and user interface by exposing properties and signals for real-time updates<br>- Supports different data sources and provides methods for updating vehicle state, enabling seamless integration with various components of the application.</td>
						</tr>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='/src/core/VideoItem.cpp'>VideoItem.cpp</a></b></td>
							<td style='padding: 8px;'>- VideoItem.cpp implements a video streaming component for a Qt-based application<br>- It handles UDP video reception, decoding, and rendering using GStreamer and OpenCV<br>- The class sets up a pipeline to receive H.264 video, converts it to a displayable format, and paints the frames onto a QQuickPaintedItem<br>- This enables real-time video display within the applications user interface.</td>
						</tr>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='/src/core/Vehicle.cpp'>Vehicle.cpp</a></b></td>
							<td style='padding: 8px;'>- Defines the Vehicle class, representing individual vehicles in the system<br>- Implements properties and methods for managing vehicle attributes such as ID, position, altitude, speed, battery status, and flight mode<br>- Provides functionality to update and retrieve vehicle information, supporting different data sources like local MAVLink and Teknofest API<br>- Facilitates vehicle tracking and status monitoring within the larger application architecture.</td>
						</tr>
						<tr style='border-bottom: 1px solid #eee;'>
							<td style='padding: 8px;'><b><a href='/src/core/AppContext.h'>AppContext.h</a></b></td>
							<td style='padding: 8px;'>- AppContext serves as a central hub for managing core components of the application<br>- It provides access to VehicleManager, MavlinkManager, and TeknofestClient through a singleton pattern<br>- The class facilitates initialization of Mavlink communication and Teknofest server connection, while exposing key managers as properties for easy access throughout the application<br>- AppContext acts as a unifying element, enabling seamless interaction between different parts of the system.</td>
						</tr>
					</table>
				</blockquote>
			</details>
		</blockquote>
	</details>
</details>

---

## Getting Started

### Prerequisites

This project requires the following dependencies:

- **Programming Language:** C++
- **Package Manager:** Cmake

### Installation

Build  from the source and intsall dependencies:

1. **Clone the repository:**

    ```sh
   git clone https://github.com/KardasLand/AvaSYS-GCS-Revised
    ```

2. **Navigate to the project directory:**

    ```sh
    cd AvaSYS-GCS-Revised
    ```

3. **Install the dependencies:**
	**Using [cmake](https://cmake.org/):**

	```sh
	cmake . && make
	```

### Usage

Run the project with:
```sh
export THUNDERFOREST_API_KEY=<your_api_key>
export TEKNOFEST_API_URL=<your_api_url>
./cmake-build-debug/appAvaSYS2
```

---

## Roadmap

- [X] **`Task 1`**: <strike>Teknofest Integration.</strike>
- [ ] **`Task 2`**: Implement video streaming using GStreamer and OpenCV.
- [ ] **`Task 3`**: Implement support for multiple vehicles.
- [ ] **`Task 4`**: Enhance the user interface with additional features and components.
- [ ] **`Task 5`**: Implement notifications for critical events.
- [ ] **`Task 6`**: Implement mission buttons for vehicle control.

---

## Contributing

- **💬 [Join the Discussions](https://github.com/KardasLand/AvaSYS-GCS-Revised/discussions)**: Share your insights, provide feedback, or ask questions.
- **🐛 [Report Issues](https://github.com/KardasLand/AvaSYS-GCS-Revised/issues)**: Submit bugs found or log feature requests for the `AvaSYS-GCS-Revised` project.
- **💡 [Submit Pull Requests](https://github.com/KardasLand/AvaSYS-GCS-Revised/blob/main/CONTRIBUTING.md)**: Review open PRs, and submit your own PRs.

<details closed>
<summary>Contributing Guidelines</summary>

1. **Fork the Repository**: Start by forking the project repository to your github account.
2. **Clone Locally**: Clone the forked repository to your local machine using a git client.
   ```sh
   git clone https://github.com/KardasLand/AvaSYS-GCS-Revised
   ```
3. **Create a New Branch**: Always work on a new branch, giving it a descriptive name.
   ```sh
   git checkout -b new-feature-x
   ```
4. **Make Your Changes**: Develop and test your changes locally.
5. **Commit Your Changes**: Commit with a clear message describing your updates.
   ```sh
   git commit -m 'Implemented new feature x.'
   ```
6. **Push to github**: Push the changes to your forked repository.
   ```sh
   git push origin new-feature-x
   ```
7. **Submit a Pull Request**: Create a PR against the original project repository. Clearly describe the changes and their motivations.
8. **Review**: Once your PR is reviewed and approved, it will be merged into the main branch. Congratulations on your contribution!
</details>

<details closed>
<summary>Contributor Graph</summary>
<br>
<p align="left">
   <a href="https://github.com{/KardasLand/AvaSYS-GCS-Revised/}graphs/contributors">
      <img src="https://contrib.rocks/image?repo=KardasLand/AvaSYS-GCS-Revised">
   </a>
</p>
</details>

---

## License

 The project is protected under the Apache 2.0 license. For more details, please refer to the [LICENSE](LICENSE).

---

## Acknowledgments

- Thanks to the [Teknofest](https://www.teknofest.org/) for providing the API and server manual.

<div align="right">

[![][back-to-top]](#top)

</div>


[back-to-top]: https://img.shields.io/badge/-BACK_TO_TOP-151515?style=flat-square


---
