# tiger-compiler
![image](https://github.com/jihandong/tiger-compiler/blob/main/cover.jpg)

## description

C implemention base on tiger book.

Name style: every type and function has prefixs which point out modules they belong to, now we have:
- AST_: Abstract Syntax Tree. Astnode structures and constructors.
- ESC_: Escape. To find escaped variables.
- FRM_: Frame. (To be finished ...)
- SMT_: Semantic.
- SYM_: Symbol. Symbol-Table structures, constructors and some methods.
- TMP_: Temp. (To be finished ...)
- TR_: Translate. (To be finished ...)
- TY_: Type. Type structures and constructors.
- UTL: Utility. Tool functions, such as alloc/free and error-message.

To use tiger-compiler, just make it on UNIX os.

```
make
./a.out test/queens.tig
```

## Status

Current Progress.
- lexer OK, can print all tokens.
- parser OK, can print ast.
- Type check OK... mostly.
- working on frame now.
