# LC-3 Virtual Machine in C

A simple virtual machine written in C, implementing the **LC-3** architecture.
Based on the tutorial: [Write your Own Virtual Machine](https://www.jmeiners.com/lc3-vm/).

---

## ✨ Overview
This project is an educational LC-3 Virtual Machine that demonstrates how to simulate a small CPU, memory, and instruction set The main goal is to learn computer architecture basics and system-level programming in C.

---

## 📂 Project Structure
- **Lc3.c** : Main source code (fetch-decode-execute loop, registers, memory, traps, etc.)  
- **README.md** : Project documentation 
- **LICENSE** : MIT License

---

## 🔧 Requirements
- C compiler (`gcc` or `clang`)  
- Command line environment (Linux / Unix / macOS / Windows with MinGW or WSL)

---

## 🚀 Build & Run
### Build:
```sh
gcc -std=c11 -O2 Lc3.c -o mvc
