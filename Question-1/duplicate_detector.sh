#!/bin/bash

#=============================================================================
# Script Name: duplicate_detector.sh
# Purpose: Detect duplicate submissions, backup unique files, and generate report
# Author: [Your Name]
# Date: January 20, 2026
#=============================================================================

# Color codes for terminal output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Initialize counters
total_files=0
duplicate_files=0
unique_files=0
backed_up_files=0
errors=0

# Define directories and files
SUBMISSION_DIR="submissions"
BACKUP_DIR="backup_unique_submissions"
REPORT_FILE="submission_report.txt"
ERROR_LOG="error_log.txt"

# Clear previous error log
> "$ERROR_LOG"

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  Duplicate Submission Detector${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# Check if submission directory exists
if [ ! -d "$SUBMISSION_DIR" ]; then
    echo -e "${RED}Error: Submission directory not found!${NC}" | tee -a "$ERROR_LOG"
    exit 1
fi

# Create backup directory
mkdir -p "$BACKUP_DIR" 2>>"$ERROR_LOG"
if [ $? -ne 0 ]; then
    echo -e "${RED}Error: Failed to create backup directory${NC}" | tee -a "$ERROR_LOG"
    ((errors++))
fi

# Count total files
total_files=$(find "$SUBMISSION_DIR" -type f | wc -l)
echo -e "${YELLOW}Total files found: $total_files${NC}"
echo ""

# Create temporary file to store checksums
TEMP_CHECKSUM="temp_checksums.txt"
> "$TEMP_CHECKSUM"

echo -e "${BLUE}Analyzing submissions...${NC}"
echo ""

# Generate MD5 checksums for all files
find "$SUBMISSION_DIR" -type f -exec md5sum {} \; > "$TEMP_CHECKSUM" 2>>"$ERROR_LOG"

# Declare associative array to track checksums
declare -A checksum_map

# Process each file
while IFS= read -r line; do
    checksum=$(echo "$line" | awk '{print $1}')
    filepath=$(echo "$line" | awk '{print $2}')
    filename=$(basename "$filepath")
    
    if [ -z "${checksum_map[$checksum]}" ]; then
        # First occurrence - this is unique
        checksum_map[$checksum]="$filepath"
        ((unique_files++))
        
        # Copy to backup directory
        cp "$filepath" "$BACKUP_DIR/" 2>>"$ERROR_LOG"
        if [ $? -eq 0 ]; then
            ((backed_up_files++))
            echo -e "${GREEN}✓ Backed up: $filename${NC}"
        else
            echo -e "${RED}✗ Failed to backup: $filename${NC}" | tee -a "$ERROR_LOG"
            ((errors++))
        fi
    else
        # Duplicate found
        ((duplicate_files++))
        original="${checksum_map[$checksum]}"
        echo -e "${YELLOW}⚠ Duplicate: $filename (matches $(basename "$original"))${NC}"
    fi
done < "$TEMP_CHECKSUM"

# Clean up temporary file
rm -f "$TEMP_CHECKSUM"

echo ""
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  Processing Complete${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# Generate detailed report
{
    echo "=========================================="
    echo "  SUBMISSION ANALYSIS REPORT"
    echo "=========================================="
    echo ""
    echo "Report Generated: $(date '+%Y-%m-%d %H:%M:%S')"
    echo "Submission Directory: $SUBMISSION_DIR"
    echo "Backup Directory: $BACKUP_DIR"
    echo ""
    echo "=========================================="
    echo "  SUMMARY STATISTICS"
    echo "=========================================="
    echo ""
    echo "Total Files Processed:        $total_files"
    echo "Unique Submissions Found:     $unique_files"
    echo "Duplicate Submissions Found:  $duplicate_files"
    echo "Files Backed Up:              $backed_up_files"
    echo "Errors Encountered:           $errors"
    echo ""
    echo "=========================================="
    echo "  DUPLICATE ANALYSIS"
    echo "=========================================="
    echo ""
    
    if [ $duplicate_files -gt 0 ]; then
        duplicate_percentage=$(awk "BEGIN {printf \"%.2f\", ($duplicate_files/$total_files)*100}")
        echo "Duplicate Rate: ${duplicate_percentage}%"
        echo ""
        echo "Action Taken: Duplicates were identified but not backed up."
        echo "Only unique submissions were copied to backup directory."
    else
        echo "No duplicates found. All submissions are unique."
    fi
    
    echo ""
    echo "=========================================="
    echo "  FILE LISTING - BACKED UP FILES"
    echo "=========================================="
    echo ""
    ls -lh "$BACKUP_DIR" 2>/dev/null | tail -n +2
    echo ""
    echo "=========================================="
    echo "  BACKUP VERIFICATION"
    echo "=========================================="
    echo ""
    echo "Backup Directory Size: $(du -sh "$BACKUP_DIR" 2>/dev/null | awk '{print $1}')"
    echo "Number of Files in Backup: $(ls -1 "$BACKUP_DIR" 2>/dev/null | wc -l)"
    echo ""
    
    if [ -s "$ERROR_LOG" ]; then
        echo "=========================================="
        echo "  ERRORS ENCOUNTERED"
        echo "=========================================="
        echo ""
        echo "Please check '$ERROR_LOG' for detailed error messages."
    else
        echo "No errors encountered during processing."
    fi
    
    echo ""
    echo "=========================================="
    echo "  END OF REPORT"
    echo "=========================================="
    
} > "$REPORT_FILE"

# Display report on screen
cat "$REPORT_FILE"

# Final status
echo ""
if [ $errors -eq 0 ] && [ $backed_up_files -eq $unique_files ]; then
    echo -e "${GREEN}✓ Process completed successfully!${NC}"
else
    echo -e "${YELLOW}⚠ Process completed with warnings. Check error log.${NC}"
fi

echo ""
echo -e "Report saved to: ${BLUE}$REPORT_FILE${NC}"
echo -e "Error log saved to: ${BLUE}$ERROR_LOG${NC}"
echo -e "Backup location: ${BLUE}$BACKUP_DIR${NC}"
echo ""
