/*
MIT License

Copyright (c) 2021 Marcin Borowicz <marcinbor85@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "dev.h"

#include <time.h>
#include <string.h>
#include <stdlib.h>

static size_t null_data_getter(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t **data)
{
        (void)self;
        (void)file;
        (void)data;

        /* do nothing */

        return 0;
}

static void null_data_setter(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t *data, size_t size)
{
        (void)self;
        (void)file;
        (void)data;
        (void)size;

        /* do nothing */
}

static size_t random_data_getter(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t **data)
{
        (void)self;
        (void)file;
        (void)data;

        static int r;
        r = rand();
        *data = (uint8_t*)&r;
        return sizeof(r);
}

static const struct ush_file_descriptor g_dev_desc[] = {
        {
                .name = "null",
                .get_data = null_data_getter,
                .set_data = null_data_setter,
        },
        {
                .name = "random",
                .get_data = random_data_getter,
        }
};

static struct ush_node_object g_dev;

static const struct ush_file_descriptor g_dev_bus_desc[] = {
        {
                .name = "spi",
                .help = "not implemented yet\r\n",
        },
        {
                .name = "uart",
                .help = "not implemented yet\r\n"
        }
};

static struct ush_node_object g_dev_bus;

static uint8_t g_ram_buffer[256];
static uint8_t g_rom_buffer[1024];
static uint8_t g_eeprom_buffer[32];

static size_t ram_data_getter(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t **data)
{
        (void)self;
        (void)file;

        *data = g_ram_buffer;
        return sizeof(g_ram_buffer);
}

static void ram_data_setter(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t *data, size_t size)
{
        (void)self;
        (void)file;

        memcpy(g_ram_buffer, data, size);
}

static size_t eeprom_data_getter(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t **data)
{
        (void)self;
        (void)file;

        *data = g_eeprom_buffer;
        return sizeof(g_eeprom_buffer);
}

static void eeprom_data_setter(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t *data, size_t size)
{
        (void)self;
        (void)file;

        memcpy(g_eeprom_buffer, data, size);
}

static size_t rom_data_getter(struct ush_object *self, struct ush_file_descriptor const *file, uint8_t **data)
{
        (void)self;
        (void)file;

        *data = g_rom_buffer;
        return sizeof(g_rom_buffer);
}

static const struct ush_file_descriptor g_dev_mem_desc[] = {
        {
                .name = "ram",
                .get_data = ram_data_getter,
                .set_data = ram_data_setter,
        },
        {
                .name = "eeprom",
                .get_data = eeprom_data_getter,
                .set_data = eeprom_data_setter,
        },
        {
                .name = "rom",
                .get_data = rom_data_getter,
        },
};

static struct ush_node_object g_dev_mem;

void dev_mount(struct ush_object *ush)
{
        srand(time(NULL));

        for (size_t i = 0; i < sizeof(g_rom_buffer); i++)
                g_rom_buffer[i] = rand();
        
        for (size_t i = 0; i < sizeof(g_eeprom_buffer); i++)
                g_eeprom_buffer[i] = 0xFF;

        ush_node_mount(ush, "/dev", &g_dev, g_dev_desc, sizeof(g_dev_desc) / sizeof(g_dev_desc[0]));
        ush_node_mount(ush, "/dev/bus", &g_dev_bus, g_dev_bus_desc, sizeof(g_dev_bus_desc) / sizeof(g_dev_bus_desc[0]));
        ush_node_mount(ush, "/dev/mem", &g_dev_mem, g_dev_mem_desc, sizeof(g_dev_mem_desc) / sizeof(g_dev_mem_desc[0]));
}
