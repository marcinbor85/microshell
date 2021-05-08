[![Build Status](https://travis-ci.org/marcinbor85/microshell.svg?branch=main)](https://travis-ci.org/marcinbor85/microshell)
# microshell
Lightweight pure C implementation of virtual shell, compatible with VT100 terminal.
Support root tree, run-time mounting paths, global commands, and much more.
Works out-of-the-box on Arduino-compatible boards.
Dedicated for bare-metal embedded systems.

[Project Homepage](https://microshell.pl/)
[DEMO](https://microshell.pl/demo)

![alt text](https://github.com/marcinbor85/microshell/blob/main/website/screen.png?raw=true "Screenshot")

## Features
* names autocompletation (do You like a TAB-functionality on You favorite bash shell?)
* no dynamic allocations (no memory leaks - no problem)
* hardware independent (works just as well on AVR, PIC, STM32, ESP32 as it on x86 or RPI)
* rich set of examples (available in the Arduino Library Manager)
* pure C source code (works on rich set of compilers)
* backspace key feature (simply works)
* compatible vith VT100 standard (works out of the box with default putty configuration)
* easy to extend (adding more "weird" features it has never been easier)
* buildin commands (must-have, basic support of LS, CAT, PWD, HELP, XXD, ECHO)
* scalable (configuration allows You to exclude unnecessary modules from building)
* translation-ready (do You want Hindi translation? no problem!)
* no internal buffers (support unlimited data stream lengths)
* no static variables (possibility to use multiple independent shells in single system)
* object oriented architecture (pointers attack!)
* support root tree with static virtual files with callbacks (full customization)
* extremely simple to integrate (only 1 simple interface with IO operations)
* asynchronous architecture (static callbacks is not a problem)
* non-blocking api (You just need to call one non-blocking function on main loop)
* unit and functional tests (for greater certainty that nothing will break down)

## Build

Build, test and run DEMO:

```sh
cmake -Bbuild .
cd build
make
make test
./bin/demo
```

## Usage

Define I/O interface:

```c
// non-blocking read interface
static int ush_read(struct ush_object *self, char *ch)
{
    // should be implemented as a FIFO
    if (Serial.available() > 0) {
        *ch = Serial.read();
        return 1;
    }
    return 0;
}

// non-blocking write interface
static int ush_write(struct ush_object *self, char ch)
{
    // should be implemented as a FIFO
    return (Serial.write(ch) == 1);
}

// I/O interface descriptor
static const struct ush_io_interface ush_iface = {
    .read = ush_read,
    .write = ush_write,
};
```

Define shell descriptor and shell instance:

```c
// working buffers allocations (size could be customized)
#define BUF_IN_SIZE    32
#define BUF_OUT_SIZE   32
#define PATH_MAX_SIZE  32

static char ush_in_buf[BUF_IN_SIZE];
static char ush_out_buf[BUF_OUT_SIZE];

// microshell instance handler
static struct ush_object ush;

// microshell descriptor
static const struct ush_descriptor ush_desc = {
    .io = &ush_iface,                           // I/O interface pointer
    .input_buffer = ush_in_buf,                 // working input buffer
    .input_buffer_size = sizeof(ush_in_buf),    // working input buffer size
    .output_buffer = ush_out_buf,               // working output buffer
    .output_buffer_size = sizeof(ush_out_buf),  // working output buffer size
    .path_max_length = PATH_MAX_SIZE,           // path maximum length (stack)
    .hostname = "arduino",                      // hostname (in prompt)
};

// root directory handler
static struct ush_node_object root;
```

Setup and run:

```c
void setup()
{
    // initialize I/O interface
    Serial.begin(115200UL);

    // initialize microshell instance
    ush_init(&ush, &ush_desc);

    // mount root directory (root must be first)
    ush_node_mount(&ush, "/", &root, NULL, 0);

    // mount other directories here
    // ...
}

void loop()
{
    // non-blocking microshell service
    ush_service(&ush);

    // do other non-blocking stuff here
    // ...
}
```

## Contribute

If You want to contribute this project - excellent! Fork it, make bugs, and send a PR :)
If You want to support me in developing this project - please [![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donate_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=EQJAX25PAQKCS)
In case of any question, feel free to send my an <a href="mailto: marcinbor85@microshell.pl">email</a>.
