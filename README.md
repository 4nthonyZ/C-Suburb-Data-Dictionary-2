# Suburb Data Dictionary — COMP20003 Project

This project implements a dictionary system in **C**, used to **lookup** and **delete** records of Australian suburbs from CSV files. It was developed for the COMP20003 course at the University of Melbourne.

## 🛠 Features

- **Stage 1 (dict1)**:  
  Search suburb records by *Official Name Suburb* and print matching results.  
  Records printed to file, with result counts printed to `stdout`.

- **Stage 2 (dict2)**:  
  Delete records by suburb name, then output the updated dataset in CSV format.  
  Total number of deletions printed to `stdout`.

## 💡 Implementation Highlights

- Data is stored in a **linked list of structs**.
- Lookup and deletion both operate based on the **second column (Official Name Suburb)**.
- Maintains original data order.
- Supports modular extension for future stages (Assignment 2).

## 📁 Dataset Overview

Five datasets of increasing size were provided for testing:

| File Name        | Number of Suburbs | Notes                                      |
|------------------|-------------------|--------------------------------------------|
| `dataset_1.csv`   | 1                 | Basic sanity check                          |
| `dataset_15.csv`  | 15                | Small-scale debugging                       |
| `dataset_100.csv` | 100               | Duplicates removed in A2, 51 suburbs omitted |
| `dataset_1000.csv`| 1000              | Full-scale performance/memory test          |

Duplicates removed in `dataset_100.csv` include suburbs like **Carlton**, **Windsor**, **Richmond**, and **Kensington**.

## 🚀 Usage

### Compile:
```bash
make -B dict1
make -B dict2
