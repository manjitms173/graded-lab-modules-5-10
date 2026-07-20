# Question 1: Duplicate Submission Detector

## Overview
Automated shell script to detect and manage duplicate student submissions using MD5 checksums.

**Author:** [Your Name]
**Date:** July 20, 2026

---

## Problem Statement
Detect duplicate student submissions and backup only unique files.

---

## Solution

### Features
- MD5-based duplicate detection
- Automatic backup of unique files
- Comprehensive reporting
- Error logging with color-coded output

### Technology
- Bash shell scripting
- MD5 hashing algorithm
- Associative arrays

---

## Usage

Run the script:

chmod +x duplicate_detector.sh
./duplicate_detector.sh

View results:

ls -lh backup_unique_submissions/
cat submission_report.txt

---

## Test Results

Total Submissions: 10 files
Unique Files: 6 files
Duplicates: 4 files
Duplicate Rate: 40%

---

## Learning Outcomes

- Bash scripting
- Hash-based duplicate detection
- File operations
- Error handling
- Report generation

---

Status: Complete

