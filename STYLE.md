# Coding & Naming Style Guide

## 1. File layout
- `src/` → `.c` implementation files  
- `include/` → `.h` public headers  
- `tests/` → unit‑test programs  

## 2. Naming conventions

### 2.1 Types / structs  
Use either `PascalCase` or `snake_case_t` for typedef’d structs.  
```c
typedef struct DecTrieNode_s {
    /* … */
} DecTrieNode;
```

### 2.2 Functions  
All function names are `snake_case`, for example:  
```c
DecTrie *create_dectrie(void);
```

### 2.3 Macros / constants  
All macros and compile‑time constants are `UPPER_SNAKE_CASE`, for example:  
```c
#define DIGIT_COUNT 10
```

## 3. Formatting

### 3.1 Indentation  
- **4 spaces** per indent level  
- **No** tab characters  

### 3.2 Line length  
- Maximum **80** columns  

### 3.3 Braces  
K&R style (opening brace on same line):  
```c
if (cond) {
    /* … */
} else {
    /* … */
}
```

## 4. Documentation

- **Public APIs**: one‑line doc comments in the header above each function.  
- **Implementation details**: short `//` comments inline in the `.c` files for any non‑obvious logic.

