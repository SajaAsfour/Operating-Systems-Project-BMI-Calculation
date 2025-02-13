# 🖥️ Operating Systems Project - BMI Calculation

## 📌 Overview
This repository contains my implementation of **Project 1 for the Operating Systems (ENCS3390) course at Birzeit University**. The project focuses on comparing **three different approaches** to calculating the average BMI (Body Mass Index) for a given dataset:
1. **Naïve Approach:** A single-process implementation.
2. **Multiprocessing Approach:** Utilizes multiple child processes for parallel computation.
3. **Multithreading Approach:** Uses POSIX threads (Pthreads) to process data concurrently.

The goal is to **analyze execution time and performance** across these three methods and evaluate their efficiency based on **Amdahl’s Law**.

## 🛠 Features
- ✅ **Naïve Approach:** Sequential processing of BMI calculations.
- ✅ **Multiprocessing:** Distributes workload across multiple child processes using **fork()**.
- ✅ **Inter-Process Communication (IPC):** Uses pipes to aggregate results from child processes.
- ✅ **Multithreading:** Implements **joinable threads** using Pthreads.
- ✅ **Performance Evaluation:**
  - Measures execution time for each approach.
  - Analyzes the impact of **different numbers of threads and processes**.
  - Applies **Amdahl’s Law** to determine the optimal parallelization strategy.

## 📂 Contents
- 📜 **saja1210737.c:** C source code implementing all three approaches.
- 📄 **Project Report (ReportProject1.pdf & ENCS3390_Project1.docx):**
  - Detailed discussion on implementation.
  - Execution time comparisons.
  - Analysis based on Amdahl’s Law.
  - Execution time results for different thread/process configurations.

## 📌 Requirements
- GCC Compiler
- Linux environment with at least **4 CPU cores**
- Basic knowledge of **C programming, processes, and threads**

## 👩‍💻 Author
**Saja Asfour**
- 🎓 Computer Engineering Student at Birzeit University
- 🏠 GitHub: [SajaAsfour](https://github.com/SajaAsfour)

## 📜 License
This repository is for educational purposes. Feel free to use and reference the work, but please give proper credit. 😊

