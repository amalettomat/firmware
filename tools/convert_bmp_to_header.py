#!/usr/bin/env python3
"""Convert a 16-bit RGB565 BMP to a C header file with correct little-endian byte order.

Usage:
  python3 convert_bmp_to_header.py input.bmp output.h [--name <struct_name>] [--include <path>] [--extern]
"""
import struct
import sys
import os
import re

def read_bmp(filepath):
    with open(filepath, 'rb') as f:
        data = f.read()

    if data[0:2] != b'BM':
        raise ValueError("Not a BMP file")

    px_off = struct.unpack('<I', data[10:14])[0]
    w = struct.unpack('<i', data[18:22])[0]
    h = struct.unpack('<i', data[22:26])[0]
    bpp = struct.unpack('<H', data[28:30])[0]

    if bpp != 16:
        raise ValueError(f"Expected 16-bit BMP, got {bpp}-bit")

    pixel_data = data[px_off:]

    # BMP rows are bottom-up; flip vertically
    row_bytes = w * 2
    rows = [pixel_data[r*row_bytes:(r+1)*row_bytes] for r in range(abs(h))]
    flipped = b''.join(rows[::-1])

    return w, abs(h), flipped

def make_hex_string(data_bytes, width=40):
    """Format byte array as C string literals with \\x hex escapes."""
    lines = []
    line = ''
    for i, b in enumerate(data_bytes):
        line += f'\\x{b:02x}'
        if len(line) >= width or (i + 1) % 20 == 0:
            if line:
                lines.append('   "' + line + '"')
            line = ''
    if line:
        lines.append('   "' + line + '"')
    return '\n'.join(lines)

def make_octal_string(data_bytes, width=38):
    """Format byte array as C string literals with \\ooo octal escapes (GIMP style)."""
    lines = []
    line = ''
    for i, b in enumerate(data_bytes):
        line += f'\\{b:03o}'
        if len(line) >= width:
            if line:
                lines.append('  "' + line + '"')
            line = ''
    if line:
        lines.append('  "' + line + '"')
    return '\n'.join(lines)

def generate_header(w, h, pixel_bytes, name, include_path=None, extern=False, use_octal=False):
    """Generate a C header file content."""
    guard = f'BMP_{name.upper()}_H_'
    lines = []
    lines.append(f'#ifndef {guard}')
    lines.append(f'#define {guard}')
    lines.append('')
    inc = include_path or os.path.join('..', 'BmpImage.h')
    lines.append(f'#include "{inc}"')
    lines.append('')

    if extern:
        lines.append(f'extern const char {name}_data[] PROGMEM =')
    else:
        lines.append(f'const char {name}_data[] PROGMEM =')

    if use_octal:
        lines.append(make_octal_string(pixel_bytes))
    else:
        lines.append(make_hex_string(pixel_bytes))
    lines.append(';')
    lines.append('')

    if extern:
        lines.append(f'extern const struct BmpImage {name} = {{')
    else:
        lines.append(f'const struct BmpImage {name} = {{')
    lines.append(f'  {w}, {h}, 2,')
    lines.append(f'  {name}_data')
    lines.append('};')
    lines.append('')
    lines.append(f'#endif // {guard}')
    return '\n'.join(lines) + '\n'

def main():
    import argparse
    parser = argparse.ArgumentParser(description='Convert 16-bit BMP to C header')
    parser.add_argument('input', help='Input BMP file')
    parser.add_argument('output', help='Output header file')
    parser.add_argument('--name', default=None, help='Struct/data name (default: derived from filename)')
    parser.add_argument('--include', default=None, help='Path for BmpImage.h include')
    parser.add_argument('--extern', action='store_true', help='Use extern declarations')
    parser.add_argument('--octal', action='store_true', help='Use octal escapes (GIMP style)')
    args = parser.parse_args()

    w, h, pixel_bytes = read_bmp(args.input)

    if args.name:
        name = args.name
    else:
        base = os.path.splitext(os.path.basename(args.input))[0]
        name = re.sub(r'[^a-zA-Z0-9_]', '_', base)

    header = generate_header(w, h, pixel_bytes, name, args.include, args.extern, args.octal)

    with open(args.output, 'w') as f:
        f.write(header)

    print(f'Generated {args.output}: {w}x{h}x16, {len(pixel_bytes)} bytes of pixel data')

if __name__ == '__main__':
    main()
