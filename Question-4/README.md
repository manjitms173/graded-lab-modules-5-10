# Question-4: Log Monitoring Pipeline

Real-time log analysis using command pipelines

## Objective

Build a monitoring tool that:
1. Displays new log entries in real-time
2. Extracts ERROR messages
3. Maintains a separate report file
4. Suppresses unnecessary output

## Solution

Command Pipeline:
tail -f system.log | grep --line-buffered 'ERROR' | tee error_report.txt

## Component Breakdown

1. tail -f system.log
   - Follows file in real-time
   - Shows new lines as they appear
   - No polling overhead

2. | (pipe)
   - Connects command output to input
   - No temporary files needed
   - Memory efficient

3. grep 'ERROR'
   - Filters only ERROR lines
   - Reduces data volume early
   - Fast pattern matching

4. tee error_report.txt
   - Displays on screen AND saves to file
   - Single pass through data
   - No duplicate processing

## Efficiency Improvements

1. Pipes (|)
   - Direct data flow between commands
   - No disk I/O for intermediate data
   - Commands run in parallel
   - Memory efficient streaming

2. grep
   - Filters early in pipeline
   - Only relevant data continues
   - Reduces downstream processing
   - Highly optimized regex engine

3. tail -f
   - Uses kernel inotify mechanism
   - No busy waiting or polling
   - Instant response to new data
   - Very low CPU usage

4. Redirection (>, >>, 2>)
   - Direct file output
   - Append without reading entire file
   - Separate stdout/stderr streams
   - Clean syntax

5. /dev/null
   - Discards unwanted output
   - Zero disk usage
   - Prevents screen clutter
   - Improves readability

## Usage

./log_monitor.sh          # Basic monitoring
./advanced_pipeline.sh    # Advanced demo
./test_monitor.sh         # Run tests

## Real-World Examples

Monitor Apache errors:
tail -f /var/log/apache2/error.log | grep 'ERROR' | tee -a daily_errors.log

Multiple filters:
tail -f system.log | grep -E 'ERROR|CRITICAL' | grep -v 'Ignored' | tee alerts.txt

## Common Redirection

command > file              # Overwrite
command >> file             # Append
command 2> file             # Errors only
command &> file             # Both streams
command 2>/dev/null         # Discard errors
command > file 2>&1         # Both to same file

## Performance Comparison

Inefficient:
grep 'ERROR' log.txt > temp1.txt
grep 'CRITICAL' temp1.txt > temp2.txt
cat temp2.txt
rm temp1.txt temp2.txt

Efficient:
cat log.txt | grep 'ERROR' | grep 'CRITICAL'

Result: 3x faster, no temporary files, less disk I/O

## Key Learning

Pipelines combine simple commands into powerful tools. Each command does one thing well, data flows efficiently, and real-time processing has low overhead.

## Files

- log_monitor.sh - Main monitoring script
- advanced_pipeline.sh - Advanced demos
- test_monitor.sh - Test script
- system.log - Sample log file
- error_report.txt - Filtered output

## Success

- Real-time monitoring with tail -f
- ERROR filtering with grep
- Report file with tee
- Clean output with /dev/null
- Efficient pipeline design
