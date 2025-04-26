# Base‑N Trie

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

> A collection of trie (prefix‑tree) implementations for arbitrary bases:
> - **Decimal trie** (base‑10)  


---

## 📖 Table of Contents

1. [What is a Base‑N Trie?](#what-is-a-base-n-trie)  
2. [Features](#features)  
3. [Project Structure](#project-structure)  
4. [Getting Started](#getting-started)  
   - [Prerequisites](#prerequisites)  
   - [Build & Run](#build--run)  
5. [Usage Examples](#usage-examples)  
6. [Configuration & Style](#configuration--style)  
7. [Future Roadmap](#future-roadmap)  
8. [Contributing](#contributing)  
9. [License](#license)  

---

## ❓ What is a Base‑N Trie?

A **trie** (aka prefix tree) is an efficient tree‑like data structure for storing associative arrays where the keys are sequences (strings, numbers, etc.). A **base‑N** trie is simply a trie parameterized by an alphabet of size *N* (for example, 10 digits for decimal, 16 symbols for hexadecimal, or 36+ for alphanumeric).

With a base‑N trie you can:

- **Insert** keys (e.g. `"0039"`, `"FF00"`, `"AB2021"`)  
- **Search** for exact keys  
- **Remove** keys (with clean‑up of unused branches)  
- **Traverse** all stored keys  

---

## ⚙️ Features

- ✅ **Decimal trie** (`src/decimal_trie.c`)   
- 🔄 **Recursive & iterative removal** with automatic cleanup  
- 📚 **Well‑documented public API** in `include/*.h`  
- 🧪 **Unit tests** in `tests/*.c`  
- 🛠 **Configurable via** `.clang-format` & **checks enforced** by `.editorconfig`

---

## 🗂 Project Structure

<!-- TREE START -->
```plaintext
base-n-trie/
├── src/  
│   ├── base_n_trie.c   
├── include/  
│   ├── base_n_trie.h
├── examples/
│   ├── demo_decimal.c    
├── tests/  
│   ├── test_decimal_trie.c
│   ├── basic.c
│   ├── test_config.c
│   ├── test_decimal_trie.c  
├── .gitignore  
├── .editorconfig  
├── .clang-format  
├── STYLE.md  
└── README.md
```  
<!-- TREE END -->
---

## 🚀 Getting Started

### Prerequisites

- A C11‑compatible compiler (e.g. `gcc`, `clang`)  
- [Make](https://www.gnu.org/software/make/) (optional, for convenience)  

### Build & Run

1. **Clone** the repository  
   ```bash
   git clone https://github.com/your‑username/base-n-trie.git
   cd base-n-trie
   ```

2. **Build** with Make (or compile manually, works on Linux & WSL)  
   ```bash
   make
   ```

   _Or manually:_  
   ```bash
   gcc -std=c11 -Wall -Wextra -O2 -Isrc src/base_n_trie.c examples/demo_decimal.c -o demo && ./demo
   ```

3. **Run tests**
     tests/basic.c  
   ```bash
gcc -std=c11 -Wall -Wextra -O2 -Isrc src/base_n_trie.c tests/basic.c -o test && ./test
   ```

---

## 📝 Usage Examples

### Decimal Trie

```c
#include "base_n_trie.h"

int main(void) {
    BaseNTrieTrie *t = trie_create();
   trie_insert(t, "0039");
   trie_insert(t, "1234567");

    if (trie_search(t, "0039")) {
        printf("Found 0039!\n");
    }

    trie_delete(t, "0039");
    trie_destroy(t);
    return 0;
}
```

### Hex Trie

```c
#include "hex_trie.h"

int main(void) {
    HexTrie *h = create_hextrie();
    insert_hex(h, "FF00");
    // ...
    destroy_hextrie(h);
}
```

---

## 📐 Configuration & Style

- See **STYLE.md** for naming, formatting, and documentation conventions  
- Editor‑integrated rules in **.clang-format** and **.editorconfig**  
- 4 spaces indentation, ≤ 80 columns, K&R braces, snake_case APIs  

---

## 🔮 Future Roadmap

- ➕ **Turn decimal implementation into generic base-N core** (our next big chunk)
- ➕ **Hex trie** (base‑16)  
- ➕ **Hybrid alphanumeric trie** (custom bases combining letters & digits)
- ➕ **Autocomplete** / prefix‑search APIs  
- ➕ **Serialization** to/from disk  
- ➕ **Benchmarking** & performance tuning  
- ➕ **Language bindings** (Python, Rust, etc.)  

---

## 🤝 Contributing

1. Fork the repo  
2. Create a feature branch: `git checkout -b feat/my‑new‑trie`  
3. Commit your changes: `git commit -am 'Add fancy feature'`  
4. Push to your branch: `git push origin feat/my‑new‑trie`  
5. Open a Pull Request—discussion welcome!

Please follow the **STYLE.md** guide when adding new code.

---

## 📄 License

This project is released under the [MIT License](LICENSE). Feel free to use, modify, and redistribute!

---

> Built with ❤️ and C99

