import os
import argparse

def flatten_directory(input_dir, output_file):
    with open(output_file, 'w') as out_file:
        for root, _, files in os.walk(input_dir):
            for name in files:
                file_path = os.path.join(root, name)
                rel_path = os.path.relpath(file_path, input_dir)
                out_file.write(f"--- {rel_path}\n")
                with open(file_path, 'r', errors='ignore') as in_file:
                    out_file.write(in_file.read() + "\n")

def main():
    parser = argparse.ArgumentParser(description="Flatten a directory's files into a single text file.")
    parser.add_argument("folder", help="Path to the input folder")
    parser.add_argument("-o", "--output", default="flattened_output.txt", help="Output file name")
    args = parser.parse_args()

    flatten_directory(args.folder, args.output)

if __name__ == "__main__":
    main()
