
# Windows Development Repository

Welcome to the Windows Development Repository. This repository contains various examples and projects demonstrating Windows development using Win32 API and COM in C and C++.

## Table of Contents

1. [Win32](#win32)
    - [Windowing](#windowing)
    - [Multithreading](#multithreading)
    - [Dynamic Link Library (DLL)](#dynamic-link-library-dll)
    - [Bitmap Loading](#bitmap-loading)
2. [COM](#com)
    - [ClassFactory](#classfactory)
    - [QI Properties](#qi-properties)
    - [Containment](#containment)
    - [Aggregation](#aggregation)
    - [Automation](#automation)
    - [Exe Server](#exe-server)
    - [ProxyStub](#proxystub)
3. [SineWave Simulator](#sinewave-simulator)
4. [Getting Started](#getting-started)
5. [Contributing](#contributing)
6. [License](#license)
7. [Acknowledgments](#acknowledgments)

## Win32

### Windowing

**Description:**  
This section includes various examples of creating and managing windows using the Win32 API. Examples cover basic window creation, handling messages, adding icons, displaying text, creating multi-color windows, and simple animations.

**Examples:**
- **First Window:** Basic window creation and message loop.
  - `windowing/first_window.c`
- **Messages:** Handling different types of messages.
  - `windowing/messages.c`
- **Icon:** Adding icons to a window.
  - `windowing/icon.c`
- **Hello World:** Displaying "Hello World" in a window.
  - `windowing/hello_world.c`
- **Multi-Color Window:** Creating a window with multiple colors.
  - `windowing/multi_color_window.c`
- **Animation:** Simple window animation.
  - `windowing/animation.c`

### Multithreading

**Description:**  
Examples demonstrating the use of multithreading in Win32 applications. These examples show how to create and manage multiple threads to perform concurrent tasks.

**Examples:**
- **Creating Threads:** Basic thread creation and management.
  - `multithreading/multithreading.c`

### Dynamic Link Library (DLL)

**Description:**  
This section covers the creation and usage of Dynamic Link Libraries (DLLs) in Win32 applications. It includes examples of using `__declspec` and `.def` files for implicit and explicit linking methods.

**Examples:**
- **Using Declspec:** Creating and using DLLs with `__declspec`.
  - `dll/dll_declspect.c`
- **Using Deffile:** Creating and using DLLs with `.def` files.
  - `dll/dll_deffile.c`
- **Implicit Linking:** Using implicit linking to load DLLs.
  - `dll/dll_implicit.c`
- **Explicit Linking:** Using explicit linking to load DLLs.
  - `dll/dll_explicit.c`

### Bitmap Loading

**Description:**  
Examples demonstrating how to load and display bitmap images in a Win32 application.

**Examples:**
- **Loading Bitmaps:** Loading and displaying bitmap images.
  - `bitmap_loading/bitmap_loading.c`

## COM

### ClassFactory

**Description:**  
Example demonstrating the implementation of the `ClassFactory` design pattern in COM.

**Example:**
- `com/classfactory.cpp`

### QI Properties

**Description:**  
Example demonstrating QueryInterface (QI) properties in COM.

**Example:**
- `com/qi_properties.cpp`

### Containment

**Description:**  
Example demonstrating COM containment using inner and outer servers.

**Example:**
- `com/containment.cpp`

### Aggregation

**Description:**  
Example demonstrating COM aggregation using inner and outer servers.

**Example:**
- `com/aggregation.cpp`

### Automation

**Description:**  
Examples demonstrating COM automation with 8 different clients: C, C++, Python, Java, JavaScript, Ruby, .NET, and Perl.

**Examples:**
- **C Client:** `automation/c_client.c`
- **C++ Client:** `automation/cpp_client.cpp`
- **Python Client:** `automation/python_client.py`
- **Java Client:** `automation/java_client.java`
- **JavaScript Client:** `automation/javascript_client.js`
- **Ruby Client:** `automation/ruby_client.rb`
- **.NET Client:** `automation/dotnet_client.cs`
- **Perl Client:** `automation/perl_client.pl`

### Exe Server

**Description:**  
Example demonstrating the implementation of an EXE server in COM.

**Example:**
- `com/exe_server.cpp`

### ProxyStub

**Description:**  
Example demonstrating the use of ProxyStub in COM.

**Example:**
- `com/proxystub.cpp`

## SineWave Simulator

**Description:**  
Project made using Win32 and COM in C++. This project shows a sine wave and its properties changing UI components.

**Example:**
- `sinewave_simulator/sinewave_simulator.cpp`

## Getting Started

### Prerequisites

- Windows operating system
- Visual Studio or any compatible C/C++ compiler

### Installing

1. Clone the repository to your local machine:
    ```bash
    git clone https://github.com/yourusername/windows-development.git
    ```
2. Open the desired project in Visual Studio.
3. Build and run the project.

## Contributing

Contributions are welcome! Feel free to open an issue or submit a pull request with your improvements and new examples.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Thank you Pradnya Ma'am Teacher at Astromedicomp for teaching this amazing course
- Microsoft documentation and tutorials
- Various online resources and forums

