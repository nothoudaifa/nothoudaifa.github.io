import sys
trans = lambda s: sum((nm[c] << 2 * i for i, c in enumerate(s)))
if len(sys.argv)!= 2:
    print(f'Usage: {sys.argv[0]} <dna_file>')
    sys.exit(1)
code = open(sys.argv[1]).read()

s = []
m = {}
nm = {'A': 0, 'T': 1, 'G': 2, 'C': 3}
pc = 0
while pc < len(code):

    pri, pro = map(trans, [code[pc:pc + 2], code[pc + 2:pc + 12]])
    print(hex(pc)+' ', end='')
    if pri == 0:
        print(f'PUSH {hex(pro)}')
        pc += 12
    elif pri == 1:
        print("POP NOTHING")
        pc+=2
    elif pri == 2:
        print(f"PUSH [{hex(pro)}]")
        pc+=12
    elif pri == 3:
        print(f"[{hex(pro)}] = POP")
        pc += 12
    elif pri == 4:
        print("ADD")
        pc+=2
    elif pri == 5:
        print("SUB")
        pc+=2
    elif pri == 6:
        print(f"MULT")
        pc += 2
    elif pri == 7:
        print("MOD")
        pc+=2
    elif pri == 8:
        print("CMPEQ")
        pc+=2
    elif pri == 9:
        print(f"JMP {hex(pro)}")
        pc+=12
    elif pri == 10:
        print(f"JMP {hex(pro)} IF POP == 1")
        pc+=12
    elif pri == 11:
        print(f"JMP {hex(pro)} IF POP != 1")
        pc+=12
    elif pri == 12:
        print("PUTC(POP)")
        pc+=2
    elif pri == 13:
        print("CALL MARSHAL")
        pc+=2
    elif pri== 14:
        print("SWITCH")
        pc+=2
    elif pri==15:
        print("HALT")
        pc+=2
    else:
        print("OH SHIT")
        print(pc)
        exit(0)
