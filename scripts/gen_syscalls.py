#!/usr/bin/env python3
"""
Generate syscall table from syscall definitions
"""

import sys
import os
import re

def parse_syscall_header(filepath):
    """Parse syscall definitions from header file"""
    syscalls = []
    
    try:
        with open(filepath, 'r') as f:
            content = f.read()
            
        # Find syscall definitions
        pattern = r'#define\s+SYSCALL_(\w+)\s+(\d+)'
        matches = re.finditer(pattern, content)
        
        for match in matches:
            syscalls.append({
                'name': match.group(1),
                'number': int(match.group(2))
            })
    except Exception as e:
        print(f"Error parsing {filepath}: {e}", file=sys.stderr)
        return []
    
    return sorted(syscalls, key=lambda x: x['number'])

def generate_table(syscalls):
    """Generate syscall table C code"""
    code = """// Auto-generated syscall table
// Do not edit manually!

#include <sys/syscall.h>

typedef int (*syscall_handler_t)(void);

extern syscall_handler_t syscall_handlers[SYSCALL_COUNT];

syscall_handler_t syscall_handlers[SYSCALL_COUNT] = {
"""
    
    for syscall in syscalls:
        code += f"    [SYSCALL_{syscall['name']}] = sys_{syscall['name'].lower()},\n"
    
    code += ""};
"""
    
    return code

def main():
    if len(sys.argv) < 2:
        print("Usage: gen_syscalls.py <input_header> [output_file]", file=sys.stderr)
        sys.exit(1)
    
    input_file = sys.argv[1]
    output_file = sys.argv[2] if len(sys.argv) > 2 else "syscall_table.c"
    
    print(f"Parsing syscalls from {input_file}...")
    syscalls = parse_syscall_header(input_file)
    
    if not syscalls:
        print("No syscalls found!", file=sys.stderr)
        sys.exit(1)
    
    print(f"Found {len(syscalls)} syscalls")
    
    table = generate_table(syscalls)
    
    with open(output_file, 'w') as f:
        f.write(table)
    
    print(f"Generated: {output_file}")

if __name__ == '__main__':
    main()
