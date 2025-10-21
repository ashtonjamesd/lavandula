<img src="lavandula.png" alt="drawing" width="400"/>

<br/>


**Lavandula** is a lightweight, fast, and intuitive C web framework designed for building modern web applications quickly. It focuses on simplicity, performance, and productivity, providing all the essentials without the bloat of heavier frameworks.

```c
#include "lavandula.h" 

appRoute(home, ctx) {
  return ok("Hello, World", TEXT_PLAIN);
}

int main() {
  App app = createApp();
  get(&app, "/home", home);

  runApp(&app);
}
```

See `/examples` for more.

**![Join the Discord server](https://discord.gg/5H3kkazG)**

[![Lavandula](https://github.com/ashtonjamesd/lavandula/actions/workflows/ci.yaml/badge.svg?branch=main)](https://github.com/ashtonjamesd/lavandula/actions/workflows/ci.yaml)


> ⚠️ **Notice:** Lavandula is still in early development and not ready for production use.  
> Expect frequent breaking changes and incomplete documentation.

<hr/>

## Features

- Controller and routing system
- HTTP endpoint support (GET, POST, etc)
- Controller local/global middleware pipeline
- Minimal dependencies (pure C)
- Quick project scaffolding via the CLI
- Built-in unit testing framework
- Environment variable support
- Built-in logging
- SQLite integration
- Built-in JSON library
- Static file serving
- Request validator library
- 'Hot Restart' mechanism

## In Progress

- CORS policy configuration
- Multithreading
- TLS
- PostgreSL, MySQL integrations, etc
- Rate Limiting
- Session cookies


## Installation

To install Lavandula, follow these setps.

1. Clone the repository

```bash
git clone https://github.com/ashtonjamesd/lavandula.git
cd lavandula
```


2. Run the install script

```bash
./install.sh
```

You should see the following:

```bash
[SUCCESS] 🎉 Lavandula installation completed!

Quick Start:
 lavu new my-project # Create a new project
 cd my-project
 lavu run # Run your project

Documentation:
 GitHub: https://github.com/ashtonjamesd/lavandula
```


3. Finish

You should now be able to run the Lavu CLI tool and begin writing your application. Refer to [this document](doc/api/cli.md) for how to use Lavu.


## Getting Started

1. Create a new project

```
lavu new myProject
```

This will set up an application folder structure and copy the Lavandula `/src` directory into the created folder structure. This allows you to easily compile the framework and your app together.

Output:

```
Setting up Lavandula project 'myProject'...

-> Created myProject/lavandula.yml

...

🎉 Lavandula project 'myProject' setup finished successfully!

Next steps:
  1. cd myProject
  2. lavu run
```


2. Run

```
lavu run
```

Your application will run on http://localhost:3000/.

3. Read the docs
  
   - [Getting Started](doc/getting_started/1_installing.md)
   - [Project Tutorial](doc/tutorial/tutorial.md)


## Contributing

Contributions are welcome. Feel free to submit pull requests or open issues for feature requests or bugs.

Some places to start:
- Possible memory leaks
- Application security
- Outdated and unfinished documentation (API changes warrant a docs update)


## License



Lavandula is registered under the MIT License.
