import os
from dotenv import load_dotenv
import argparse

def number_to_16_bit_binary(integer):
    binary = ""
    if integer == 0:
        return "0000000000000000"
    for i in range(0, 16):
        div = (2 ** (15 - i))
     #   print(div)
        if (integer / div) >= 1:
            binary += "1"
            integer -= div
        else:
            binary += "0"
    return binary


def main():

    load_dotenv()
    
    parser = argparse.ArgumentParser(description="Chatbot")
    parser.add_argument("target_file", type=str, help="Target File")
    args = parser.parse_args()
    file_path = args.target_file
    working_directory = '.'

    c_inst = {
        "0":  "1110101010",
        "1":  "1110111111",
        "-1": "1110111010",
        "D":  "1110001100",
        "A":  "1110110000",
        "M":  "1111110000",
        "!D": "1110001101",
        "!A": "1110110001",
        "!M": "1111110001",
        "-D": "1110001111",
        "-A": "1110110011",
        "-M": "1111110011",
        "D+1": "1110011111",
        "A+1": "1110110111",
        "M+1": "1111110111",
        "D-1": "1110001110",
        "A-1": "1110110010",
        "M-1": "1111110010",
        "D+A": "1110000010",
        "D+M": "1111000010",
        "D-A": "1110010011",
        "D-M": "1111010011",
        "A-D": "1110000111",
        "M-D": "1111000111",
        "D&A": "1110000000",
        "D&M": "1111000000",
        "D|A": "1110010101",
        "D|M": "1111010101",
    }
    d_inst = {
        "000": "000",
        "M": "001",
        "D": "010",
        "MD": "011",
        "A": "100",
        "AM": "101",
        "AD": "110",
        "ADM": "111",
    }
    j_inst = {
        "000": "000",
        "JGT": "001",
        "JEQ": "010",
        "JGE": "011",
        "JLT": "100",
        "JNE": "101",
        "JLE": "110",
        "JMP": "111",
    }
    pd_symbols = {
        "R0": 0,
        "R1": 1,
        "R2": 2,
        "R3": 3,
        "R4": 4,
        "R5": 5,
        "R6": 6,
        "R7": 7,
        "R8": 8,
        "R9": 9,
        "R10": 10,
        "R11": 11,
        "R12": 12,
        "R13": 13,
        "R14": 14,
        "R15": 15,
        "SCREEN": 16384,
        "KBD": 24576,
        "SP": 0,
        "LCL": 1,
        "ARG": 2,
        "THIS": 3,
        "THAT": 4,
    }


    working_directory_abs = os.path.abspath(working_directory)
    target_file = os.path.normpath(os.path.join(working_directory_abs, file_path))
    valid_file_path = os.path.commonpath([working_directory_abs, target_file]) == working_directory_abs
    if not valid_file_path:
        print("Error: Cannot read \"{file_path}\" as it is outside the working directory")
    if not os.path.isfile(target_file):
        print("Error: File not found or is not a regular file: \"{file_path}\"")
    with open(target_file, "r") as f:
        instruction_str = f.read()

    label_count = 0
   # print(instruction_str)
    instructions = instruction_str.split('\n')
 #   print(instructions[0])

 ## FIRST PASS - ADD LABELS TO SYMBOL TABLE
    line_counter = 0
    for i in range(0, len(instructions)):
        new_inst = instructions[i].strip()
     #   print(new_inst)
        if "//" not in new_inst:
            if new_inst[0] == '(' and new_inst[-1] == ')': #ITS A LABEL
          #      print(f"ADDING LABEL {new_inst[1:-1]} to table with a value of {line_counter} ")
                pd_symbols[new_inst[1:-1]] = (line_counter)
            else:
                line_counter += 1
            
    # INITIALIZE OUTPUT
    output = ""
    ## SECOND PASS - 
    line_counter = 0
    for i in range(0, len(instructions)):
        new_inst = instructions[i].strip()
        # CHECK IF A INSTRUCTION - ADD BINARY LINE TO OUTPUT
        if "//" not in new_inst:
            if new_inst[0] == '@':
              #  print(f"A INSTRUCTION - {new_inst}")
                if new_inst[1:].isdigit(): #A INSTRUCTION
                    output += number_to_16_bit_binary(int(new_inst[1:])) + '\n'
                else:
                    if new_inst[1:] in pd_symbols:
              #          print(f"{new_inst[1:]} found in the symbol table")
                        output += number_to_16_bit_binary(pd_symbols[new_inst[1:]]) + '\n'
                    else:
              #          print(f"Adding {new_inst[1:]} to the table with a value of {16+label_count}")
                        pd_symbols[new_inst[1:]] = (16 + label_count)
                        output += number_to_16_bit_binary(16 + label_count) + '\n'
                        label_count += 1
                
                print(f"{line_counter} - {new_inst}")
                line_counter += 1
            else:
                if new_inst[0] == '(' and new_inst[-1] == ')': #L-INSTRUCTION
                    output += number_to_16_bit_binary(pd_symbols[new_inst[1:-1]]) + '\n'
                else:
                    if '=' in new_inst:
                        dest = new_inst[0:new_inst.index('=')]
                        if ';' in new_inst:
                            jmp = new_inst[new_inst.index(';') + 1:]
                            comp = new_inst[new_inst.index('=') + 1: new_inst.index(';')]          
                        else:
                            comp = new_inst[new_inst.index('=') + 1:]
                            jmp = "000"         
                    else:
                        if ';' in new_inst:
                            dest = "000"
                            jmp = new_inst[new_inst.index(';') + 1 :]
                            comp = new_inst[0:new_inst.index(';')]
                    line_counter += 1
                    print(f"{line_counter - 1} - {dest}={comp};{jmp}")
                binary = c_inst[comp] + d_inst[dest] + j_inst[jmp]
                output += binary + '\n'
    for k in pd_symbols:
        print(k,pd_symbols[k])
        print(number_to_16_bit_binary(pd_symbols[k]))
    print("--------START OF BINARY--------")
    print(output)

    with open("output.b", "w") as r:
        r.write(output)

main()
