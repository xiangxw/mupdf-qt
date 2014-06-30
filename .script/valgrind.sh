#!/bin/bash
# This script use valgrind to check memory leaks.

set -e

CMDS=("./bin/countpages_c pdf_reference_1-7.pdf" \
      "./bin/exportpage_c pdf_reference_1-7.pdf 1" \
      "./bin/qimage_c pdf_reference_1-7.pdf" \
      "./bin/showinfo_c pdf_reference_1-7.pdf" \
      "./bin/showtext_c pdf_reference_1-7.pdf 1" \
      "./bin/outline_c pdf_reference_1-7.pdf" \
      "./bin/test_document pdf_reference_1-7.pdf" \
      "./bin/test_outline pdf_reference_1-7.pdf" \
      "./bin/test_page pdf_reference_1-7.pdf" \
      "./bin/test_text pdf_reference_1-7.pdf 1"
      )

for (( i = 0; i < ${#CMDS[*]}; i++ )); do
    ${CMDS[$i]}
    valgrind --error-exitcode=1 --leak-check=full --gen-suppressions=all --suppressions=../.script/qt.supp ${CMDS[$i]}
done
