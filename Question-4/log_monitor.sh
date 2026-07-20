#!/bin/bash

# Log Monitor - Real-time ERROR tracking
# Question-4: Command Pipelines Demo

LOG_FILE="system.log"
ERROR_REPORT="error_report.txt"

echo "========================================="
echo "  Real-Time Log Monitoring System"
echo "========================================="
echo ""

# Create sample log file
echo "INFO: System started at $(date)" > "$LOG_FILE"
echo "INFO: Loading configuration..." >> "$LOG_FILE"
echo "WARNING: High memory usage detected" >> "$LOG_FILE"
echo "ERROR: Database connection failed" >> "$LOG_FILE"
echo "INFO: Retrying connection..." >> "$LOG_FILE"

echo "✓ Sample log file created: $LOG_FILE"
echo ""

# Command Pipeline Explanation
echo "COMMAND PIPELINE:"
echo "tail -f system.log | grep --line-buffered 'ERROR' | tee error_report.txt"
echo ""

# Show what each component does
echo "COMPONENT BREAKDOWN:"
echo "1. tail -f          → Follow log in real-time"
echo "2. |                → Pipe output to next command"
echo "3. grep ERROR       → Filter only ERROR lines"
echo "4. |                → Pipe filtered output"
echo "5. tee report.txt   → Display AND save to file"
echo ""

# Initialize report
echo "ERROR Report - Generated: $(date)" > "$ERROR_REPORT"
echo "=========================================" >> "$ERROR_REPORT"
echo "" >> "$ERROR_REPORT"

# Extract existing errors
grep 'ERROR' "$LOG_FILE" >> "$ERROR_REPORT" 2>/dev/null

echo "✓ Initial ERROR report created"
echo ""

# Demonstrate real-time monitoring (5 seconds)
echo "Starting real-time monitoring (5 seconds demo)..."
echo ""

# Background log generator
(
  sleep 1
  echo "INFO: Processing request #101" >> "$LOG_FILE"
  sleep 1
  echo "ERROR: Authentication failed for user 'admin'" >> "$LOG_FILE"
  sleep 1
  echo "INFO: Cache cleared" >> "$LOG_FILE"
  sleep 1
  echo "ERROR: Disk space critically low" >> "$LOG_FILE"
  sleep 1
  echo "INFO: Backup completed" >> "$LOG_FILE"
) &

# Monitor with timeout
timeout 5s tail -f "$LOG_FILE" 2>/dev/null | grep --line-buffered 'ERROR' | tee -a "$ERROR_REPORT" &

MONITOR_PID=$!
sleep 6
kill $MONITOR_PID 2>/dev/null

echo ""
echo "✓ Monitoring complete"
echo ""

# Show results
echo "========================================="
echo "  FINAL ERROR REPORT"
echo "========================================="
cat "$ERROR_REPORT"
echo ""

# Statistics
ERROR_COUNT=$(grep -c 'ERROR' "$LOG_FILE")
echo "Total Errors Found: $ERROR_COUNT"
echo ""

echo "✓ Report saved to: $ERROR_REPORT"
