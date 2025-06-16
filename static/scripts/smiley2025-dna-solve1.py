import sys
import marshal
import sys
from z3 import *
s = []
m = {}
nm = {'A': 0, 'T': 1, 'G': 2, 'C': 3}
unlucky = [b'\x8coooooooooooonooolooo,ooo\x9cSooo\x06o\x12o\x1bo\x0bnvo\x13o\x0bmSo\x1bo\x0blvo\x13o\x0bnSo\x1bo\x0bkvo\x13o\x0blSo\x1bo\x0bmvo\x13o\x0bkSo\x13o\x0eo\x0bo<oFj!\xb5n;\xb5n.\xb5n(\xb5n,\xc6n\xb5m\x01\x02\xc6n\xb5l\x1b\x02\x1f\xc6o\x1deooo\x95fS\x1a\x01\x03\x1a\x0c\x04\x16Q\xb5h\x1a\x01\x03\x1a\x0c\x04\x16booo\x9ccoookmcncncncngn', b'\x96uuuuuuuuuuuuruuu}uuu6uuu\x86\x11uuu\x11t\x08u\x11w\x08t\x11v\x08w\x11q\x11p\xf1u\tu1u\xf6t\x08v\tu\tt\tw\x13v1u(n\x08q\x01u\x01t\x01w\xd5v\xd4u\xf6t\xf6t1u(e)w\x08p\x08s\tv\tspulu\x01w\tq\tpluluMuvuIu\x04i\x04g\tv\x14w\x11u&u\\s;\xafq426!\xafq!642\xafq6!24\x16tuuuuuuuuuuuwuuusuuu&uuu\x86ouuu\x1cu\tu(|\x08t\tt\x01u\x01t\xd5w\xd4u\xf6t\xe6w\x04w&u\\u\xdcv\xafv\x06\x00\x18\xafw\x1b\x18\xafs\x03\x14\x19\x00\x10\x06\xdcw\xafw[E\xaft\x16\xdcu\x07xuuu\x8f|I\x00\x1b\x19\x00\x16\x1e\x0cK\xafr\x00\x1b\x19\x00\x16\x1e\x0cnuuu\x86wuuuou\x8fh\x00\x1b\x19\x00\x16\x1e\x0c*G[I\x19\x1a\x16\x14\x19\x06K[I\x11\x1c\x16\x01\x16\x1a\x18\x05K\xdcq\xaf|\x10\x1b\x00\x18\x10\x07\x14\x01\x10\xafs\x06\x1a\x07\x01\x10\x11\x07}uuu\xafq\x1e\x10\x0c\x06\xdcr\xafw\x06D\xafw\x06G\xafw\x06F\xafv\x01\x18\x05\xaft\x06\xaft\x1c\x07yuuu\x07xuuu\x07xuuu\x07{uuu\x07zuuucuuu\x86guuuqwqtqt{t{tmtotw\x8a}w', b"\x8aiiiiiiiiiiiihiiiniiijiii\x9a/iii\x1di\rh\xeah\xe0i\xe1i\xc9h\x1di\rk\xeah\xc9k\rj\rm\xedi\x1dj\xc9m\xc8i\xc8k\xc8hhi.i\xeei\x0fh\rl\ro\xeda\ro\x1dl\xeaj\x14i\x15i\x1dj\xeah\x08j\ri:i@n'\xb3o\x1b\x08\x07\r\x06\x04\xb3`\x0f\x1c\x07\n\x1d\x06\x06\x05\x1a\nkiiiiiiiiiiikiiikiii:iii\x9aaiii\x15i\x15h(i:i@h'\xc0i\xc0k\xb3h\x11\xb3h\x10\x1bliii\x1bliii\x93`U\x1c\x07\x05\x1c\n\x02\x10W\xb3n\x1c\x07\x05\x1c\n\x02\x10Miii\x9akiiiai\x93r\x1c\x07\x05\x1c\n\x02\x106ZGU\x05\x06\n\x08\x05\x1aWGU\x05\x08\x04\x0b\r\x08W\niiiiiiiiiiiiiiiijiiiiiii\x9aCiii\x0ci3h\ri3k\xeei\xeeh\x0fk\rh\rk\xeda3j\xeei\x0fh\rj\rm\xeda3m\xeeimi3l:i@l\x93s\x1c\x07\x05\x1c\n\x02\x106ZGU\x05\x06\n\x08\x05\x1aWG\x1c\x07\x05\x1c\n\x02\x10\nkiiiiiiiiiiimiiiliiiziii\x9a-iii\x1di\xeai\xc9h\x15h\xc8hhi\x1dk\rh\xeah\x14k\xe1h\xc9j\x15k\xc8hhi\x1dm\rk\xeah-i4e\x14j\x15h\x15k\x15jpipi\x15i\rh\x15jpiUi\x18z\ri:i@j'\xb3m(*.=\x80miii\xc0l\xb3l\x1a\x1c\x19\x0c\x1b\xb3a66\x00\x07\x00\x1d66\xb3m\x05\x00\x1a\x1d\xb3n\x1a\x01\x1c\x0f\x0f\x05\x0c\xb3l\x1b\x08\x07\x0e\x0c\xc0m\xb3m\x1a\x0c\x05\x0f\xb3n\x04\x08\x19\x19\x00\x07\x0e\xb3m\x02\x0c\x10\x1a\xb3h\x00\xc0k\xb3`66\n\x05\x08\x1a\x1a66\xb3h\x1b\x1bliii\x1b`iii\x1bciiiOiii\x9aeiiiehahcheh\x7fhm\x96\x93J\x1c\x07\x05\x1c\n\x02\x106ZGU\x05\x06\n\x08\x05\x1aWG\x1c\x07\x05\x1c\n\x02\x10G66\x00\x07\x00\x1d66\nkiiiiiiiiiiiliiiliiiziii\x9a;iii\x1di\rh\xeah\x14k\x1di\rk\xeah\x14j`i\x15k\xc9h\rm\xc8h\x14m\x1dk\xeei\x0fh\rl\ro\xeda\x15j\xc9j\x15m\xc8h\xc9m\xc8i\ri\rn\xeckpi-i\xeah\xeah\x1bA\x1dl\xeai\xc9o\xe1i\xc8h:i\x18`@a'\x1bkiii\xb3n\x01\x08\x1a\x01\x05\x00\x0b=\x80Iiii\nhiiiiiiiiiiikiiimiiiZiii\x9auiii\xe8i\x15i4`\x14h\x15h\x1di\xe1i\xeah\x02k?ihi\x18k\ri:i@h'\xc0h\xb3j\x06\x1b\r\xc0k\xb3kGY\x1bniii\xc0h\xb3j\x02\x0c\x10\x1bliii\x1b`iii\x1bciii[iii\x9amiiik\xe9si\x93P\x1c\x07\x05\x1c\n\x02\x106ZGU\x05\x06\n\x08\x05\x1aWG\x1c\x07\x05\x1c\n\x02\x10G66\x0e\x0c\x1d\x00\x1d\x0c\x0466GU\x05\x06\n\x08\x05\x1aWGU\x0e\x0c\x07\x0c\x11\x19\x1bW\x80hiii\xc0n\xb3c66\x00\x04\x19\x06\x1b\x1d66\xb3`\x1b\x08\x07\r\x0b\x10\x1d\x0c\x1a\xb3j\x08\x05\x05\xb3o\x1a\x01\x08[\\_\xb3o\r\x00\x0e\x0c\x1a\x1d\x1bziii\xb3b66\x0e\x0c\x1d\x00\x1d\x0c\x0466\xc0l\x1bpiii\x1bBiii\xb3m\x01\x05\x00\x0b\xb3m\x1b\x05\x00\x0b\xb3h\x0b\xc0h\x1bwiii\x1bCiii\x1b`iii\x1bciiiDiii\x9agiiiahahkhchAhehk\x94\x93O\x1c\x07\x05\x1c\n\x02\x106ZGU\x05\x06\n\x08\x05\x1aWG\x1c\x07\x05\x1c\n\x02\x10G66\x0e\x0c\x1d\x00\x1d\x0c\x0466\xc0o\xb3a66\x07\x08\x04\x0c66\xb3c66\x04\x06\r\x1c\x05\x0c66\xb3e66\x18\x1c\x08\x05\x07\x08\x04\x0c66\x1b}iii\x1b\\iii\xb3d66\n\x05\x08\x1a\x1a\n\x0c\x05\x0566\x1bliii\xc0h\x1bviii\x1bSiii\x1b`iii\x1bciiiLiii\x9aoiiiaigh}n\x1bciii\xc0o\x1bYiii\xb3m\x1a\x0c\x0c\r\xb3o\x1b\x0c\r\x1c\n\x0c\xb3k\x07\x04\xb3o\x1f\x08\x05\x1c\x0c\x1a\xb3m\r\x00\n\x1d\xc0h\x1bciii\x1bliii\x1b+iii\x1b`iii\x1bciiiHiii\x9aaiiiakwh}hey", b'\x82aaaaaaaaaaaacaaagaaa"aaa\x92]aaa&a
trans = lambda s: sum((nm[c] << 2 * i for i, c in enumerate(s)))
if len(sys.argv)!= 2:
    print(f'Usage: {sys.argv[0]} <dna_file>')
    sys.exit(1)
code = open(sys.argv[1]).read()

flag= b".;,;.{AAAAAAAAAAAAAAAAAAAAAAiAAAouAAAAAaAAAAAAAAAAAAAAA}"
if len(flag)!= 56:
    exit('WRONG!')
if flag[:6]!= b'.;,;.{':
    exit('WRONG!')
if flag[(-1)]!= 125:
    exit('WRONG!')

flag = list(flag[6:(-1)])
for i in range(len(flag)):
    if flag[i] == ord('A'):
        flag[i]= BitVec(f'bv_{i}', 8)

sol = Solver()

for i in range(len(flag)):
    m[640 + i] = flag[i]
pc = 0
while pc < len(code):

    pri, pro = map(trans, [code[pc:pc + 2], code[pc + 2:pc + 12]])
    if pri == 0:
        s.append(pro)
        pc += 12
    elif pri == 1:
        if not s:
            raise Exception('Stack underflow')
        s.pop()
        pc+=2
    elif pri == 2:
        if pro not in m:
            raise Exception(f'Uninitialized memory access at {pro}')
        #print(m[pro])
        s.append(m[pro])
        pc+=12
    elif pri == 3:
        if not s:
            raise Exception('Stack underflow')
        m[pro] = s.pop()
        pc += 12
    elif pri == 4:
        if len(s) < 2:
            raise Exception('Stack underflow')
        a, b = (s.pop(), s.pop())
        s.append(a + b)
        pc+=2
    elif pri == 5:
        if len(s) < 2:
            raise Exception('Stack underflow')
        a, b = (s.pop(), s.pop())
        s.append(b - a)
        pc+=2
    elif pri == 6:
        if len(s) < 2:
            raise Exception('Stack underflow')
        a, b = (s.pop(), s.pop())
        s.append(a * b)
        pc += 2
    elif pri == 7:
        if len(s) < 2:
            raise Exception('Stack underflow')
        a, b = (s.pop(), s.pop())
        if a == 0:
            raise Exception('Division by zero')
        s.append(b % a)
        pc+=2
    elif pri == 8:
        # <class 'int'> <class 'z3.z3.BitVecRef'> 710793
        if len(s) < 2:
            raise Exception('Stack underflow')
        a, b = (s.pop(), s.pop())
        s.append(1 if a == b else 0)
        if type(a) == int and type(b) == int and a == 49:
            if sol.check() == sat:
                model = sol.model()
                print('.;,;,{', end='')
                for i in range(49):
                    try:
                        print(chr(model[flag[i]].as_long()),end='')
                    except (KeyError,IndexError):
                        print(chr(flag[i]), end='')
                print('}')
            else:
                print('unsat')
            exit(0)
        sol.add(a == b)
        pc+=2
    elif pri == 9:
        pc = pro
        print(f"JMP {hex(pro)}")
        pc+=12
    elif pri == 10:
        if not s:
            raise Exception('Stack underflow')
        if s.pop() == 1:
            pc = pro
        else:  # inserted
            pc += 12
        print(f"JMP {hex(pro)} IF POP == 1")
    elif pri == 11:
        if not s:
            raise Exception('Stack underflow')
        if s.pop()!= 1:
            pc = pro
        else:  # inserted
            pc += 12
        print(f"JMP {hex(pro)} IF POP != 1")
    elif pri == 12:
        print("PUTC(POP)")
        if not s:
            raise Exception('Stack underflow')
        print(chr(s.pop()), end='')
        pc+=2
    elif pri == 13:
        if not s:
            raise Exception('Stack underflow')
        key = s.pop()

        print("MARSHAL: ", key)
        def f():
            return
        f.__code__ = marshal.loads(bytes([b ^ key for b in unlucky.pop(0)]))
        f()
        pc+=2
    elif pri== 14:
        if len(s) < 2:
            raise Exception('Stack underflow')
        a, b = (s.pop(), s.pop())
        if a not in nm or b not in nm:
            raise Exception('Invalid')
        nm[a], nm[b] = (nm[b], nm[a])
        pc+=2
    elif pri==15:
        break
        print("HALT")
        pc+=2
    else:
        print("OH SHIT")
        print(pc)
        exit(0)
