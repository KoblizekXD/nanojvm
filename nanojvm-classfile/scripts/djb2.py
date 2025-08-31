import sys

def string_hash_bytes(data: bytes) -> int:
    """C-style djb2 hash (uint32_t)."""
    h = 5381
    for b in data:
        h = ((h << 5) + h) + b
        h &= 0xFFFFFFFF
    return h

def hash_file_lines(path: str):
    with open(path, "rb") as f:
        for line in f:
            line = line.rstrip(b"\r\n")
            h = string_hash_bytes(line)

            print(f"{h:#010x}")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <file1> [file2 ...]")
        sys.exit(1)

    for filename in sys.argv[1:]:
        hash_file_lines(filename)
