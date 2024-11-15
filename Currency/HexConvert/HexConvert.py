"""
This is a simple Python script for convet hex data `0A` ot `0x0A`.
Function: This is a simple Python script for convet hex data `0A` ot `0x0A`.
Author: Liu Jie
Date: 2024-11-15

usage method:
```bash
python HexConvert.py path_of_file
```
"""
import sys
import configparser

def process_hex_data(file_path):
    """
    Process the hexadecimal data in the specified file, format it as a C array, and write it back to the file.

    Parameters:
    file_path (str): The path to the file to be processed.

    Returns:
    None
    """
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            begin = file.read(1)
            if begin == '{':
                return
    except FileNotFoundError:
        # Handle the case where the file is not found
        print("File not found, please check the path.")
    except Exception as e:
        # Handle other exceptions
        print(f"Error processing file: {e}")


    try:
        # Open the file and read its content, stripping leading and trailing whitespace
        with open(file_path, 'r', encoding='utf-8') as file:
            content = file.read().strip()

        # print(content)

        # Split the content by spaces and add '0x' before each item and a comma after
        hex_list = [f"0x{item}" for item in content.split()]

        # Convert the list to a C array format, adding curly braces
        formatted_content = "{" + ", ".join(hex_list) + "}"

        # Write the formatted content back to the file
        with open(file_path, 'w', encoding='utf-8') as file:
            file.write(formatted_content)

        # Print success message
        print("File processed successfully.")
    except FileNotFoundError:
        # Handle the case where the file is not found
        print("File not found, please check the path.")
    except Exception as e:
        # Handle other exceptions
        print(f"Error processing file: {e}")

if __name__ == "__main__":
    config = configparser.ConfigParser()
    config.read('config.ini')
    path = config.get('path', 'path_of_file')

    process_hex_data(path)

    print('ok')
