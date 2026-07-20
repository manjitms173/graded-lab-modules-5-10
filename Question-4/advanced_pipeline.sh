#!/bin/bash

# Advanced Pipeline Demo
# Shows various redirection and /dev/null usage

echo "========================================="
echo "  Advanced Pipeline Demonstrations"
echo "========================================="
echo ""

# Demo 1: Suppress unwanted output
echo "DEMO 1: Suppress stderr with /dev/null"
echo "Command: ls /nonexistent 2>/dev/null || echo 'File not found'"
ls /nonexistent 2>/dev/null || echo "✓ Error suppressed, custom message shown"
echo ""

# Demo 2: Multiple filters
echo "DEMO 2: Multiple grep filters"
echo "Command: cat log | grep ERROR | grep -v 'Ignored'"
cat > temp.log << 'LOGEND'
ERROR: Critical failure
INFO: System OK
ERROR: Ignored - minor issue
ERROR: Database down
LOGEND

echo "Original log:"
cat temp.log
echo ""
echo "Filtered (ERROR but not Ignored):"
cat temp.log | grep 'ERROR' | grep -v 'Ignored'
rm temp.log
echo ""

# Demo 3: Tee to multiple files
echo "DEMO 3: Split output to multiple destinations"
echo "Command: echo 'data' | tee file1.txt file2.txt"
echo "Important data" | tee output1.txt output2.txt > /dev/null
echo "✓ Written to: output1.txt, output2.txt"
cat output1.txt
rm output1.txt output2.txt
echo ""

# Demo 4: Complex pipeline
echo "DEMO 4: Complex pipeline with statistics"
cat > sample.log << 'LOGEND'
2024-01-01 10:00:00 INFO: Started
2024-01-01 10:01:00 ERROR: Connection failed
2024-01-01 10:02:00 WARNING: Retry
2024-01-01 10:03:00 ERROR: Timeout
2024-01-01 10:04:00 INFO: Success
2024-01-01 10:05:00 ERROR: Permission denied
LOGEND

echo "Command: cat log | grep ERROR | awk '{print \$4, \$5}' | tee errors.txt"
cat sample.log | grep 'ERROR' | awk '{print $4, $5}' | tee errors.txt
echo ""
echo "✓ Error summary created"
rm sample.log errors.txt
echo ""

echo "========================================="
echo "  Efficiency Improvements"
echo "========================================="
echo ""
echo "1. PIPES (|)"
echo "   - No temporary files needed"
echo "   - Data flows directly between commands"
echo "   - Memory efficient"
echo ""
echo "2. GREP"
echo "   - Filters data early in pipeline"
echo "   - Reduces data volume"
echo "   - Fast pattern matching"
echo ""
echo "3. TAIL -f"
echo "   - No polling needed"
echo "   - Instant new line detection"
echo "   - Low CPU usage"
echo ""
echo "4. REDIRECTION (>, >>)"
echo "   - Direct file output"
echo "   - No print overhead"
echo "   - Append without reading"
echo ""
echo "5. /dev/null"
echo "   - Discards unwanted output"
echo "   - Prevents clutter"
echo "   - Zero disk usage"
echo ""

