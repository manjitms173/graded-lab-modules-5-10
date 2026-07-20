#!/bin/bash

echo "Testing Log Monitor..."
echo ""

# Run main monitor
./log_monitor.sh > outputs/monitor_output.txt 2>&1

echo "✓ Test complete"
echo ""
echo "Generated files:"
ls -lh system.log error_report.txt outputs/monitor_output.txt
