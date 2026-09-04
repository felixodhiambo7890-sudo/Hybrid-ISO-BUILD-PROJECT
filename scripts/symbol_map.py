#!/usr/bin/env python3
"""
Generate symbol map for debugging and stack traces
"""

import subprocess
import sys
import re

def extract_symbols(elf_file):
    """Extract symbols from ELF file"""
    try:
        result = subprocess.run(
            ['x86_64-elf-objdump', '-t', elf_file],
            capture_output=True,
            text=True
        )
        
        symbols = {}
        for line in result.stdout.split('\n'):
            parts = line.split()
            if len(parts) >= 6 and parts[0] != 'SYMBOL':
                try:
                    addr = parts[0]
                    name = parts[-1]
                    if addr != '....':
                        symbols[int(addr, 16)] = name
                except (ValueError, IndexError):
                    continue
        
        return symbols
    except Exception as e:
        print(f"Error extracting symbols: {e}", file=sys.stderr)
        return {}

def generate_map(symbols, output_file):
    """Generate symbol map file"""
    with open(output_file, 'w') as f:
        f.write("# Kernel Symbol Map\n")
        f.write("# Address          Symbol\n")
        f.write("#\n")
        
        for addr in sorted(symbols.keys()):
            f.write(f"0x{addr:016x}  {symbols[addr]}\n")
    
    print(f"Generated symbol map: {output_file}")

def main():
    if len(sys.argv) < 2:
        print("Usage: symbol_map.py <kernel.elf> [output_file]", file=sys.stderr)
        sys.exit(1)
    
    elf_file = sys.argv[1]
    output_file = sys.argv[2] if len(sys.argv) > 2 else "kernel.syms"
    
    print(f"Extracting symbols from {elf_file}...")
    symbols = extract_symbols(elf_file)
    
    if not symbols:
        print("No symbols found!", file=sys.stderr)
        sys.exit(1)
    
    print(f"Found {len(symbols)} symbols")
    generate_map(symbols, output_file)

if __name__ == '__main__':
    main()
