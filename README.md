# LinuxShell: A Minimal Linux Command Shell

LinuxShell is a lightweight command-line shell implementation in C++. It supports executing single commands, piping multiple commands, and managing input/output redirection, mimicking basic Unix/Linux shell behavior.

## **Features**

- **Single Command Execution:** Run commands like `ls`, `pwd`, `echo Hello`.
- **Command Piping:** Use pipes like `ls | grep cpp | sort`.
- **Error Handling:** Basic error messages for unknown commands or failed processes.
- **Process Management:** Supports child process creation and waiting.

## **Getting Started**

### **Prerequisites**

- A Linux-based operating system (WSL, Ubuntu, etc.).
- A C++ compiler such as `g++`.

### **Installation**

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/raheelnasir/minimal-linux-command-shell.git
   cd minimal-linux-command-shell
   ```

2. **Compile the Code:**
   ```bash
   g++ -o linux-shell main.cpp
   ```

3. **Run the Shell:**
   ```bash
   ./linux-shell
   ```

### **Usage**

- Run standard Unix/Linux commands:
  ```bash
  LinuxShell> ls
  LinuxShell> pwd
  LinuxShell> echo "Hello, World!"
  ```

- Use multiple commands with pipes:
  ```bash
  LinuxShell> ls | grep cpp | sort
  ```

- Exit the shell:
  ```bash
  LinuxShell> quit
  ```

### **Example Output:**
```
LinuxShell> echo "Hello, LinuxShell!"
Hello, LinuxShell!
LinuxShell> ls | grep main
main.cpp
LinuxShell> quit
Exiting shell...
```

## **Project Structure**

```
LinuxShell/
├── command-shell.cpp         # Main source code
└── README.md        # Project documentation
```

## **License**

This project is licensed under the MIT License. See the `LICENSE` file for details.

## **Acknowledgments**

- Unix/Linux system programming resources.
- C++ system call tutorials and references.


